// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/logic/local/police/PolicemanLogic.h"
#include "em5/logic/local/EntityLogicHelper.h"
#include "em5/action/ActionPriority.h"
#include "em5/action/move/MoveAction.h"
#include "em5/action/base/EnterVehicleAction.h"
#include "em5/action/base/StopGuidePersonAction.h"
#include "em5/action/base/WaitForEnterVehicleAction.h"
#include "em5/component/vehicle/VehicleComponent.h"
#include "em5/command/base/PlacePersonCommand.h"
#include "em5/command/police/SpeakToPersonCommand.h"
#include "em5/command/component/CommandableComponent.h"
#include "em5/command/CommandContext.h"
#include "em5/component/person/PersonComponent.h"
#include "em5/component/movement/PersonMagnetComponent.h"
#ifndef ENDUSER
	#include "em5/debug/GameDebugGroup.h"
#endif
#include "em5/specs/LogicSpecsGroup.h"
#include "em5/specs/ActionRangeSpecsGroup.h"
#include "em5/game/targetpoint/EnterVehicleTargetPointProvider.h"
#include "em5/game/units/OrderInfo.h"
#include "em5/game/units/UnitHelper.h"
#include "em5/game/component/GameComponent.h"
#include "em5/map/MapHelper.h"
#include "em5/map/EntityHelper.h"
#include "em5/settings/GameSettingsGroup.h"
#include "em5/plugin/Jobs.h"
#include "em5/plugin/Messages.h"

#include <qsf_game/base/DistanceHelper.h>
#include <qsf_game/command/CommandManager.h>
#include <qsf_game/command/CommandSystem.h>
#include <qsf_game/QsfGameHelper.h>

#include <qsf_ai/navigation/goal/ReachObjectGoal.h>
#include <qsf_ai/navigation/goal/ReachSinglePointGoal.h>

#ifndef ENDUSER
	#include <qsf/debug/DebugDrawManager.h>
	#include <qsf/debug/request/TextDebugDrawRequest.h>
#endif
#include <qsf/logic/action/ActionComponent.h>
#include <qsf/map/query/ComponentMapQuery.h>
#include <qsf/QsfHelper.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 PolicemanLogic::GAMELOGIC_TYPE_ID = qsf::StringHash("em5::PolicemanLogic");


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	const uint32 PolicemanLogic::ACTION_PRIORITY = action::AUTOMATISM_STD;


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	bool PolicemanLogic::areGangstersAround(qsf::Entity& entity)
	{
		// Tutorial is special - there's only 1 real gangster (and accidental gangsters which shouldn't be in the map but we're not going to hunt them now)
		if (entity.getMap().getCoreEntity().getOrCreateComponentSafe<GameComponent>().isInTutorial())
		{
			return false;
		}

		// Any gangster persons in a certain circle
		const float GANGSTER_SEARCH_RADIUS = 150.f;
		std::vector<PersonComponent*> personComponentVector;
		if (!qsf::ComponentMapQuery(entity.getMap()).getInstancesInCircle(EntityHelper(entity).getPosition(), GANGSTER_SEARCH_RADIUS, personComponentVector))
			return false;

		for (PersonComponent* personComponent : personComponentVector)
		{
			// A person is counted as gangster when
			// - It is marked as gangster
			// - It is handcuffed but not inside a container (Only handcuffed persons can be placed in police vehicles)
			if (personComponent->getGangsterFlag() || (personComponent->getHandcuffedFlag() && nullptr == EntityHelper(personComponent->getEntity()).getContainerEntity()))
			{
				return true;
			}
		}

		return false;
	}


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	PolicemanLogic::PolicemanLogic(uint32 gameLogicId) :
		CarryPersonLogic(gameLogicId),
		mIsAutomatismActive(false),
		mAutomatismTargetVehicle(qsf::getUninitialized<uint64>())
	{
		// Nothing here
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::GameLogic methods                 ]
	//[-------------------------------------------------------]
	bool PolicemanLogic::onStartup()
	{
		// Nothing to do right now
		// -> For performance reasons, do updates not in every tick, but in fixed time intervals
		qsf::jobs::JobConfiguration jobConfiguration;
		jobConfiguration.mTimeBetweenCalls = qsf::Time::fromSeconds(1.0f);
		mSimulationJobProxy.registerAt(Jobs::SIMULATION_LOGIC, boost::bind(&PolicemanLogic::updateSimulation, this, _1), jobConfiguration);

		// Catch if the user commanded the unit
		// Using pre instead of post command as automatism itself uses a command to start
		mPostCommandProxy.registerAt(qsf::MessageConfiguration(Messages::EM5_PRE_COMMAND_EXECUTE), boost::bind(&PolicemanLogic::onPreCommandExecute, this, _1));

		// If the police man is injured, free the gangster
		mOnInjuredProxy.registerAt(qsf::MessageConfiguration(Messages::EM5_COMPONENT_GETSINJURED_PERSON, getEntityId()), boost::bind(&PolicemanLogic::onPolicemanInjured, this, _1));
		mOnContaminatedProxy.registerAt(qsf::MessageConfiguration(Messages::EM5_COMPONENT_HEALTH_PERSON_CONTAMINATED, getEntityId()), boost::bind(&PolicemanLogic::onPolicemanInjured, this, _1));

		mMessageProxy.registerAt(qsf::MessageConfiguration(Messages::EM5_ACTION_PERSON_PLACED_IN_POLICE, getEntityId()), boost::bind(&PolicemanLogic::onPersonPlacedInVehicle, this, _1));

		return true;
	}

	void PolicemanLogic::onShutdown()
	{
		mSimulationJobProxy.unregister();
		mPostCommandProxy.unregister();
		mOnInjuredProxy.unregister();
		mOnContaminatedProxy.unregister();
		mMessageProxy.unregister();
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void PolicemanLogic::updateSimulation(const qsf::JobArguments& jobArguments)
	{
		if (!MapHelper::isUnitAutomatismActive())
		{
			// No automatism active
			mIsAutomatismActive = false;
			return;
		}

		if (mIsAutomatismActive)
		{
			// Check if target for place person is still valid
			if (qsf::isInitialized(getCarryPerson()))
			{
				qsf::Entity* targetVehicleEntity = getEntity()->getMap().getEntityById(mAutomatismTargetVehicle);
				if (targetVehicleEntity == nullptr)
				{
					abortAutomatism();
				}
				else
				{
					qsf::MovableComponent* movableComponent = targetVehicleEntity->getComponent<qsf::MovableComponent>();
					if (movableComponent != nullptr && movableComponent->isMoving())
						abortAutomatism();
				}
			}
		}

		// Access the caller's action plan
		const qsf::ActionComponent& actionComponent = getEntity()->getOrCreateComponentSafe<qsf::ActionComponent>();

		// Check if the unit is currently busy
		// -> Overwrite idle actions and low-priority automatisms
		if (actionComponent.getCurrentPriority() >= action::AUTOMATISM_STD)
		{
			// Reset wait time
			mTimeWaiting = qsf::Time::ZERO;
		}
		else
		{
			mIsAutomatismActive = false;

			// Wait some time until getting active
			mTimeWaiting += jobArguments.getTimePassed();

			// Waited long enough now?
			if (mTimeWaiting >= qsf::Time::fromSeconds(LogicSpecsGroup::getInstanceSafe().getPolicemanMinimumWaitingTime()))
			{
				// Perform the automatism now
				mIsAutomatismActive = performAutomatism(qsf::getUninitialized<uint64>());

				// Reset wait time
				mTimeWaiting = qsf::Time::ZERO;
			}
		}
	}

	bool PolicemanLogic::performAutomatism(uint64 ignoreTarget)
	{
		GameSettingsGroup::AutomatismValue automatismValue = GameSettingsGroup::getInstanceSafe().getAutomatismValue();
		if (automatismValue >= GameSettingsGroup::AUTOMATISM_ALL)
		{
			// Check if the policeman is already carrying a person
			if (qsf::isInitialized(getCarryPerson()))
			{
				return placePersonAutomatism(ignoreTarget);
			}
			else
			{
				return speakToPersonAutomatism();
			}
		}

		return false;
	}

	void PolicemanLogic::abortAutomatism(bool tryToRecover)
	{
		qsf::ActionComponent& actionComponent = getEntity()->getOrCreateComponentSafe<qsf::ActionComponent>();
		actionComponent.clearPlan();

		if (tryToRecover)
		{
			// does the unit still have an arrested?
			// (most likely case is that the vehicle drives away because we just placed the arrested and we are done)
			if (qsf::isUninitialized(getCarryPerson()))
			{
				mIsAutomatismActive = false;
				return;
			}

			// try to find another target
			mIsAutomatismActive = performAutomatism(mAutomatismTargetVehicle);

			// no other target?
			if (!mIsAutomatismActive)
			{
				// send back to original position
				qsf::logic::TargetPoint moveTargetConfig;
				moveTargetConfig.mPosition = mAutomatismStartPos;
				actionComponent.pushAction<MoveAction>(action::AUTOMATISM_STD).init(new qsf::ai::ReachSinglePointGoal(moveTargetConfig));
			}
		}
		else
		{
			mIsAutomatismActive = false;
		}
	}

	bool PolicemanLogic::placePersonAutomatism(uint64 ignoreTarget)
	{
		// TODO(mk) This code is the same as the "ParamedicLogic::carryPersonAutomatism()"

		// Get the transform component for the position
		const qsf::TransformComponent* transformComponent = getEntity()->getComponent<qsf::TransformComponent>();
		if (nullptr == transformComponent)
			return false;

		const glm::vec3& ownerPosition = transformComponent->getPosition();

		// Search for vehicle components in the radius
		std::vector<VehicleComponent*> vehicleComponentVector;
		qsf::ComponentMapQuery(getMap()).getInstancesInCircle(transformComponent->getPosition(), LogicSpecsGroup::getInstanceSafe().getPoliceSearchVehicleRadius(), vehicleComponentVector);

		// Sort the list for the distance
		qsf::game::IsLeftComponentCloserToTargetPosition compareObject(mAutomatismStartPos);
		std::sort(vehicleComponentVector.begin(), vehicleComponentVector.end(), compareObject);

		for (VehicleComponent* vehicle : vehicleComponentVector)
		{
			uint64 vehicleId = vehicle->getEntityId();
			if (vehicleId != ignoreTarget && EntityLogicHelper::checkAndExecuteAutomatism<PlacePersonCommand>(getEntity(), &vehicle->getEntity()))
			{
				mAutomatismStartPos = ownerPosition;
				mAutomatismTargetVehicle = vehicleId;

				#ifndef ENDUSER
					if (GameDebugGroup::getInstanceSafe().getShowAutomatismDebug())
					{
						QSF_DEBUGDRAW.requestDraw(qsf::TextDebugDrawRequest("auto:PlacePersonCommand", ownerPosition, qsf::Color4::YELLOW), qsf::DebugDrawLifetimeData(qsf::Time::fromSeconds(5.0f)));
					}
				#endif
				return true;
			}
		}

		return false;
	}

	bool PolicemanLogic::speakToPersonAutomatism()
	{
		if (UnitHelper(getEntitySafe()).isPoliceSEKPerson())
		{
			// SEK units haven't the ability to speak with persons but share the same police Logic. Avoid this automatism for them manually
			return false;
		}

		qsf::Entity* entity = getEntity();
		const qsf::TransformComponent& transformComponent = entity->getOrCreateComponentSafe<qsf::TransformComponent>();

		const glm::vec3 ownerPosition = transformComponent.getPosition();

		// Can we speak to person around?
		SpeakToPersonCommand* speakToPersonCommand = static_cast<SpeakToPersonCommand*>(QSFGAME_COMMAND.getCommandManager().getById<SpeakToPersonCommand>(SpeakToPersonCommand::PLUGINABLE_ID));
		if (nullptr != speakToPersonCommand && speakToPersonCommand->checkCaller(*entity))
		{
			CommandContext commandContext;
			commandContext.mCaller = entity;

			// Get persons in area
			std::vector<PersonComponent*> personComponents;
			qsf::ComponentMapQuery(getMap()).getInstancesInCircle(ownerPosition, LogicSpecsGroup::getInstanceSafe().getPoliceSearchVehicleRadius(), personComponents);
			for (PersonComponent* personComponent : personComponents)
			{
				commandContext.mTargetEntity = &personComponent->getEntity();
				if (speakToPersonCommand->checkContext(commandContext))
				{
					// Do command
					if (EntityLogicHelper::checkAndExecuteAutomatism<SpeakToPersonCommand>(entity, commandContext.mTargetEntity))
					{
						#ifndef ENDUSER
							if (GameDebugGroup::getInstanceSafe().getShowAutomatismDebug())
							{
								QSF_DEBUGDRAW.requestDraw(qsf::TextDebugDrawRequest("auto:SpeakToPersonCommand", ownerPosition, qsf::Color4::YELLOW), qsf::DebugDrawLifetimeData(qsf::Time::fromSeconds(5.0f)));
							}
						#endif
						return true;
					}
				}
			}
		}
		// No target for automatism
		return false;
	}

	void PolicemanLogic::onPersonPlacedInVehicle(const qsf::MessageParameters& parameters)
	{
		// Sometimes the unit wants to enter the vehicle together with the target

		if (getEntity() == nullptr)
			return;

		EntityHelper entityHelper(*getEntity());

		// get the vehicle
		uint64 vehicleId = parameters.getFilter(2);
		qsf::Entity* vehicleEntity = getEntity()->getMap().getEntityById(vehicleId);
		if (vehicleEntity == nullptr)
			return;

		// get it's order info
		OrderInfo* targetVehicleOrderInfo = EntityHelper(*vehicleEntity).getOrderInfo();
		if (targetVehicleOrderInfo == nullptr)
			return;

		// don't enter vehicles which still have free place to put passengers when there are more gangsters around
		if (entityHelper.checkVehicleHasFreeSeatsForPassengerPerson(*vehicleEntity, *targetVehicleOrderInfo) && areGangstersAround(*getEntity()))
			return;

		// do we have a free space?
		OrderInfo* unitOrderInfo = entityHelper.getOrderInfo();
		if (unitOrderInfo == nullptr)
			return;

		int freeSeats = entityHelper.countVehicleFreeSeatsForSquadPerson(*vehicleEntity, *targetVehicleOrderInfo, unitOrderInfo->getName());
		if (freeSeats > 0)
		{
			// This unit should enter
			qsf::ActionComponent& actionComponent = getEntity()->getOrCreateComponentSafe<qsf::ActionComponent>();
			actionComponent.pushAction<MoveAction>(action::AUTOMATISM_STD).init(new qsf::ai::ReachObjectGoal(*getEntity(), *vehicleEntity, EnterVehicleTargetPointProvider::TARGET_POINT_ID));
			actionComponent.pushAction<EnterVehicleAction>(action::BLOCKING).init(*vehicleEntity);

			if (freeSeats > 1)
			{
				// Add extra filter to support derived classes
				uint32 unitType = (getEntity()->getComponent<CommandableComponent>() != nullptr ? getEntity()->getComponent<CommandableComponent>()->getUnitType() : CommandableComponent::UNITTAG_POLICE_POLICE);

				qsf::ActionComponent& vehicleActionComponent = vehicleEntity->getOrCreateComponentSafe<qsf::ActionComponent>();

				// Fill the other seats with idling police guys
				for (int i = 1; i < freeSeats; ++i)
				{
					// Do we have another idling police unit around here?
					qsf::Entity* idlePolice = findIdlingPolicemanAroundVehicle(*vehicleEntity, unitType);
					if (idlePolice != nullptr)
					{
						qsf::ActionComponent& actionComponentIdler = idlePolice->getOrCreateComponentSafe<qsf::ActionComponent>();
						actionComponentIdler.pushAction<MoveAction>(action::AUTOMATISM_STD).init(new qsf::ai::ReachObjectGoal(*idlePolice, *vehicleEntity, EnterVehicleTargetPointProvider::TARGET_POINT_ID));
						actionComponentIdler.pushAction<EnterVehicleAction>(action::BLOCKING).init(*vehicleEntity);

						// Also wait for idle police persons
						vehicleActionComponent.pushAction<WaitForEnterVehicleAction>(action::Priority::BLOCKING, qsf::action::INSERT_AT_FRONT).init(idlePolice->getId(), qsf::Time::fromSeconds(10.0f));
					}
				}
			}
		}
	}

	qsf::Entity* PolicemanLogic::findIdlingPolicemanAroundVehicle(qsf::Entity& vehicleEntity, uint32 filteredUnitType)
	{
		qsf::TransformComponent* transformComponent = vehicleEntity.getComponent<qsf::TransformComponent>();
		if (transformComponent == nullptr)
			return nullptr;

		float maxDistance = ActionRangeSpecsGroup::getInstanceSafe().getEnterVehicleDistance();

		qsf::ComponentMapQuery mapQuery(getEntity()->getMap());
		std::vector<CommandableComponent*> commandableComponents;
		if (mapQuery.getInstancesInCircle<CommandableComponent>(transformComponent->getPosition(), maxDistance, commandableComponents))
		{
			for (CommandableComponent* commandableComponent : commandableComponents)
			{
				// Only take units by the specific type, policeman search for police man, sekspecicalist for sekspecialist
				if (commandableComponent->getUnitType() != filteredUnitType)
					continue;

				qsf::Entity& entityUnit = commandableComponent->getEntity();
				if (entityUnit.getId() == getEntity()->getId())
					continue;

				// Check if we have an active megaphone
				qsf::Entity* equipmentEntity = EntityHelper(entityUnit).getActiveEquipmentEntity();
				if (nullptr != equipmentEntity)
				{
					PersonMagnetComponent* personMagnetComponent = equipmentEntity->getComponent<PersonMagnetComponent>();
					if (nullptr != personMagnetComponent && personMagnetComponent->isVisualizationEnabled())
					{
						continue;
					}
				}

				qsf::ActionComponent* actionComponentIdler = entityUnit.getComponent<qsf::ActionComponent>();
				if (actionComponentIdler == nullptr)
					continue;

				const qsf::ActionPlan& actionPlan = actionComponentIdler->getPlan();
				if (actionPlan.isEmpty())
					return &entityUnit;
			}
		}

		return nullptr;
	}

	void PolicemanLogic::onPreCommandExecute(const qsf::MessageParameters& parameters)
	{
		// TODO(mk) This flag looks really weird
		// Command for this unit?
		// COMMENT(mz): This flag is because the abort behavior for automatisms is weird by game-design (units walking back to their origin). And we have to ensure this abort automtism is never colliding for example with commands. Any other ideas how to catch all situations when the automatisms runs/is stopped are welcome.
		if (getEntity() != nullptr && parameters.getFilter(2) == getEntity()->getId())
		{
			mIsAutomatismActive = false;
		}
	}

	void PolicemanLogic::onPolicemanInjured(const qsf::MessageParameters& parameters)
	{
		// Free the arrested gangster
		qsf::ActionComponent& actionComponent = getEntity()->getOrCreateComponentSafe<qsf::ActionComponent>();
		actionComponent.pushAction<StopGuidePersonAction>(action::BLOCKING, qsf::action::INSERT_AT_FRONT);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

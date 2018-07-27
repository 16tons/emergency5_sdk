// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/action/base/PlacePersonAction.h"
#include "em5/action/base/EnterVehicleAction.h"
#include "em5/action/base/DisappearAction.h"
#include "em5/action/base/WaitForEnterVehicleAction.h"
#include "em5/action/move/MoveAction.h"
#include "em5/action/base/ExitVehicleAction.h"
#include "em5/base/ContainerCategory.h"
#include "em5/command/move/ReturnToBaseCommand.h"
#include "em5/command/base/PlacePersonCommand.h"
#include "em5/command/CommandContext.h"
#include "em5/command/component/CommandableComponent.h"
#include "em5/component/vehicle/VehicleComponent.h"
#include "em5/game/Game.h"
#include "em5/game/animation/AnimationHelper.h"
#include "em5/game/component/GameComponent.h"
#include "em5/game/targetpoint/EnterVehicleTargetPointProvider.h"
#include "em5/game/units/UnitHelper.h"
#include "em5/health/HealthComponent.h"
#include "em5/logic/local/police/PolicemanLogic.h"
#include "em5/logic/local/ReserveLogic.h"
#include "em5/logic/local/MiniMapIconRegistrationLogic.h"
#include "em5/logic/HintHelper.h"
#include "em5/map/EntityHelper.h"
#include "em5/plugin/Messages.h"
#include "em5/settings/GameSettingsGroup.h"
#include "em5/specs/ActionRangeSpecsGroup.h"
#include "em5/specs/LogicSpecsGroup.h"
#include "em5/EM5Helper.h"

#include <qsf_game/QsfGameHelper.h>
#include <qsf_game/base/DistanceHelper.h>
#include <qsf_game/component/base/BoneToBoneLinkComponent.h>
#include <qsf_game/command/CommandSystem.h>

#include <qsf_ai/navigation/goal/ReachObjectGoal.h>

#include <qsf/logic/action/ActionComponent.h>
#include <qsf/logic/action/WaitAction.h>
#include <qsf/logic/gamelogic/GameLogicComponent.h>
#include <qsf/map/Entity.h>
#include <qsf/map/Map.h>
#include <qsf/map/query/ComponentMapQuery.h>
#include <qsf/message/MessageSystem.h>
#include <qsf/QsfHelper.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const qsf::NamedIdentifier PlacePersonAction::ACTION_ID = "em5::PlacePersonAction";


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	PlacePersonAction::PlacePersonAction() :
		Action(ACTION_ID),
		mTargetEntityId(qsf::getUninitialized<uint64>()),
		mPlaceGuidedPerson(false),
		mCurrentState(STATE_INIT),
		mPlaceCase(PLACECASE_PATIENT),
		mDoorType(DoorComponent::DOORTYPE_UNDEFINED),
		mDoorAnimationWatchDog(qsf::Time::fromSeconds(10.0f))
	{
		// Nothing here
	}

	PlacePersonAction::~PlacePersonAction()
	{
		// Remove reservation again
		ReserveLogic::deleteOwnReservation(getMap(), mTargetEntityId, getEntityId());
	}

	void PlacePersonAction::init(qsf::Entity& targetEntity, bool placeGuidedPerson)
	{
		mTargetEntityId = targetEntity.getId();
		mPlaceGuidedPerson = placeGuidedPerson;

		// To avoid multiple actors at the same target, we reserve the target here.
		// It is only checked in command for automatism. If the doctor starts treatment, the logic is changed. Only reserve if no other reserve the target (its not bad if a moveTo reservation is missing)
		// Reserve the entity
		if (ReserveLogic::checkReservation(targetEntity, getEntityId()))
		{
			ReserveLogic::createReservation<ReserveMoveToTargetLogic>(targetEntity, getEntityId());
		}
	}

	uint64 PlacePersonAction::getTargetEntityId() const
	{
		return mTargetEntityId;
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Actions methods                   ]
	//[-------------------------------------------------------]
	void PlacePersonAction::serialize(qsf::BinarySerializer& serializer)
	{
		serializer.serialize(mTargetEntityId);
		serializer.serializeAs<uint16>(mCurrentState);
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	bool PlacePersonAction::onStartup()
	{
		// Get the caller and target entity
		qsf::Entity& callerEntity = getEntity();
		qsf::Entity* targetEntity = callerEntity.getMap().getEntityById(mTargetEntityId);
		if (nullptr == targetEntity)
			return false;

		// Build context
		CommandContext commandContext;
		commandContext.mCaller = &callerEntity;
		commandContext.mTargetEntity = targetEntity;

		// Support different variants of the action
		if (mPlaceGuidedPerson)
		{
			// Nothing here
		}
		else
		{
			// Call "checkCaller" && "checkContext" of the commando for validation
			PlacePersonCommand* placePersonCommand = static_cast<PlacePersonCommand*>(QSFGAME_COMMAND.getCommandManager().getById<PlacePersonCommand>(PlacePersonCommand::PLUGINABLE_ID));
			QSF_CHECK(nullptr != placePersonCommand, "PlacePersonAction::onStartup(): Could not find place person command instance", return false);
			if (!(placePersonCommand->checkCallerWithoutPriority(*commandContext.mCaller) && placePersonCommand->checkContext(commandContext)))
				return false;
		}

		if (!checkPlacementStyle(*targetEntity))
			return false;

		{	// Distance check for the closest door
			VehicleComponent* vehicleComponent = targetEntity->getComponent<VehicleComponent>();
			if (nullptr == vehicleComponent)
				return false;

			const glm::vec3& callerPosition = EntityHelper(callerEntity).getPosition();
			mDoorEntity = vehicleComponent->findNearestDoor(mDoorType, callerPosition);
			if (!mDoorEntity.valid())
				return false;

			DoorComponent* doorComponent = mDoorEntity->getComponent<DoorComponent>();
			if (nullptr == doorComponent)
				return false;

			// Make the distance check
			const float currentDistance = qsf::game::DistanceHelper::getDistance(doorComponent->getDoorOffsetWorldSpacePosition(), callerPosition);
			if (currentDistance > ActionRangeSpecsGroup::getInstanceSafe().getEnterVehicleDistance())
				return false;
		}

		// Check for the reserve logic
		//  -> Don't do this for gangsters, as you might want to have multiple policemen accessing the same car
		if (mPlaceCase != PLACECASE_GANGSTER)
		{
			if (!ReserveLogic::checkReservation(*targetEntity, getEntityId()))
				return false;
		}

		return true;
	}

	qsf::action::Result PlacePersonAction::updateAction(const qsf::Clock& clock)
	{
		// Get the caller entity instance
		qsf::Entity& callerEntity = getEntity();

		// Get the target entity instance
		qsf::Entity* targetEntity = getMap().getEntityById(mTargetEntityId);
		QSF_CHECK(nullptr != targetEntity, "The EM5 place person action was unable to retrieve the target entity instance", return qsf::action::RESULT_DONE);

		// Get the vehicle component
		VehicleComponent* vehicleComponent = targetEntity->getComponent<VehicleComponent>();
		if (nullptr == vehicleComponent)
			return qsf::action::RESULT_DONE;

		EntityHelper callerEntityHelper(callerEntity);
		const uint64 carriedPersonId = callerEntityHelper.getCarryTarget(EntityHelper::CarryLoad::ANY);
		if (qsf::isUninitialized(carriedPersonId))
			return qsf::action::RESULT_DONE;

		// Get the carried person
		qsf::Entity* carriedPerson = getMap().getEntityById(carriedPersonId);
		if (nullptr == carriedPerson)
			return qsf::action::RESULT_DONE;

		// Handle the current state
		switch (mCurrentState)
		{
			case STATE_INIT:
			{
				// Change reserve logic from "moveto" to "entervehicle" (has higher priority)
				ReserveLogic::deleteOwnReservation(getMap(), mTargetEntityId, getEntityId());
				ReserveLogic::createReservation<ReserveEnterVehicleLogic>(*targetEntity, getEntityId());

				// Open the door
				if (mPlaceCase == PLACECASE_PATIENT)
					vehicleComponent->openDoor(mDoorType, true);
				else
					mDoorEntity->getComponentSafe<DoorComponent>().openDoor();

				// Immediately go to the next state
				mCurrentState = STATE_OPEN_DOOR;

				// Fall through by design
			}

			case STATE_OPEN_DOOR:
			{
				// Wait for ending of the door-open animation
				if (mPlaceCase == PLACECASE_PATIENT)
				{
					mDoorAnimationWatchDog -= clock.getTimePassed();	// The watchdog value is an emergency to skip the waiting in case something with the doors (can happen with bad interaction timing)
					QSF_WARN_IF(mDoorAnimationWatchDog <= qsf::Time::ZERO, "PlacePersonAction: waiting for opening doors failed, skip the door open animation", QSF_REACT_NONE);
					if (!vehicleComponent->areAllDoorsOfTypeInState(mDoorType, DoorComponent::DOOR_OPEN) && mDoorAnimationWatchDog > qsf::Time::ZERO)
					{
						return qsf::action::RESULT_CONTINUE;
					}
				}
				else if (mDoorEntity->getComponentSafe<DoorComponent>().getDoorState() != DoorComponent::DOOR_OPEN)
				{
					return qsf::action::RESULT_CONTINUE;
				}

				// Set animation for entering vehicle
				// Caller entity
				// TODO(mk) Out commented because the asset is currently broken (not missing, broken...)
				//getComponent().pushAction<PlayAnimationAction>(getPriority(), qsf::action::INSERT_AT_FRONT).init(AnimationHelper(callerEntity).getAnimationPlacePerson(), true);

				// Carried entity
				qsf::ActionComponent& actionComponent = carriedPerson->getOrCreateComponentSafe<qsf::ActionComponent>();
				actionComponent.pushAction<DisappearAction>(getPriority(), qsf::action::INSERT_AT_FRONT).init(qsf::Time::fromSeconds(1.0f), false);	// Disappear ...
				actionComponent.pushAction<qsf::WaitAction>(getPriority(), qsf::action::INSERT_AT_FRONT).init(qsf::Time::fromSeconds(1.0f));		// ... but first wait

				mCurrentState = STATE_ENTER_VEHICLE;

				// This continue is necessary to initialize the animation
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_ENTER_VEHICLE:
			{
				AnimationHelper animationHelper(callerEntity);
				if (animationHelper.isAnimationPlaying(animationHelper.getAnimationPlacePerson()))
				{
					// Wait for ending of the animation
					return qsf::action::RESULT_CONTINUE;
				}

				mCurrentState = STATE_PLACE_PERSON;

				// Fall through by design
			}

			case STATE_PLACE_PERSON:
			{
				// Destroy old link
				carriedPerson->destroyComponent<qsf::game::BoneLinkComponent>();
				carriedPerson->destroyComponent<qsf::game::BoneToBoneLinkComponent>();

				// Add the person to the container
				EntityHelper(*carriedPerson).enterContainer(*targetEntity, container::CONTAINERTYPE_PASSENGER);

				// Close the door
				if (mPlaceCase == PLACECASE_PATIENT)
				{
					if (!mPlaceGuidedPerson)
					{
						// Enter the vehicle now
						callerEntityHelper.enterContainer(*targetEntity, container::CONTAINERTYPE_SQUAD);
					}

					vehicleComponent->openDoor(mDoorType, false);
				}
				else
				{
					mDoorEntity->getComponentSafe<DoorComponent>().closeDoor();
				}

				mCurrentState = STATE_CLOSE_DOOR;

				// Fall through by design
			}

			case STATE_CLOSE_DOOR:
			{
				// Wait for ending of the close door animation
				if (mPlaceCase == PLACECASE_PATIENT)
				{
					if (!vehicleComponent->areAllDoorsOfTypeInState(mDoorType, DoorComponent::DOOR_CLOSED))
					{
						return qsf::action::RESULT_CONTINUE;
					}
				}
				else if (mDoorEntity->getComponentSafe<DoorComponent>().getDoorState() != DoorComponent::DOOR_CLOSED)
				{
					return qsf::action::RESULT_CONTINUE;
				}

				mCurrentState = STATE_DONE;

				// Fall through by design
			}

			case STATE_DONE:
			{
				// The carried is already in the target vehicle, check if the vehicle is now full, and can drive back home
				bool sendVehicleBackToBase = !EntityHelper(*carriedPerson).checkVehicleHasFreeSeats(*targetEntity);
				const qsf::Time maxTimeToEnterVehicle(qsf::Time::fromSeconds(10.0f));

				GameSettingsGroup::AutomatismValue automatismValue = GameSettingsGroup::getInstanceSafe().getAutomatismValue();
				if (automatismValue < GameSettingsGroup::AUTOMATISM_MEDIUM)
				{
					// Settings can disable the "automatically drive back to base automatism"
					sendVehicleBackToBase = false;
				}

				// Check if placed person is a gangster or a patient
				switch (mPlaceCase)
				{
					case PLACECASE_PATIENT:
					{
						// If the placed person is (still) injured, stop their life or health loss now (needed for ME310)
						HealthComponent* healthComponent = carriedPerson->getComponent<HealthComponent>();
						if (nullptr != healthComponent && healthComponent->isInjured())
						{
							healthComponent->setTreatmentActive(true);
						}

						// Emit message "Person placed in ambulance"
						qsf::MessageParameters parameters;
						parameters.setParameter("personId",  carriedPerson->getId());
						parameters.setParameter("vehicleId", mTargetEntityId);
						parameters.setParameter("actorId",   callerEntity.getId());
						QSF_MESSAGE.emitMessage(qsf::MessageConfiguration(Messages::EM5_ACTION_PERSON_PLACED_IN_AMBULANCE, carriedPerson->getId()), parameters);

						if (sendVehicleBackToBase)
						{
							// Vehicle drive back to base
							qsf::ActionComponent& actionComponent = targetEntity->getComponentSafe<qsf::ActionComponent>();
							actionComponent.clearPlan();
							actionComponent.pushAction<WaitForEnterVehicleAction>(action::Priority::BLOCKING, qsf::action::INSERT_AT_FRONT).init(callerEntity.getId(), maxTimeToEnterVehicle);
							ReturnToBaseCommand::pushReturnToBaseAction(*targetEntity, action::Priority::COMMAND_STD);

							showAutoTransportHint();
							lookForIdleMedicsInArea(*targetEntity);
						}
						else if (UnitHelper(*targetEntity).isAmbulanceIMT() && UnitHelper(callerEntity).isAmbulanceParamedics())
						{
							// If paramedics place a person in an non empty IMT
							qsf::ActionComponent& targetActionComponent = targetEntity->getComponentSafe<qsf::ActionComponent>();
							targetActionComponent.pushAction<qsf::WaitAction>(action::Priority::BLOCKING, qsf::action::INSERT_AT_FRONT).init(qsf::Time::fromSeconds(2.5f));
							targetActionComponent.pushAction<ExitVehicleAction>(action::Priority::BLOCKING, qsf::action::APPEND_TO_BACK).init(getEntityId());
						}
						break;
					}

					case PLACECASE_GANGSTER:
					{
						qsf::MessageParameters parameters;
						parameters.setParameter("personId",  carriedPerson->getId());
						parameters.setParameter("vehicleId", mTargetEntityId);
						parameters.setParameter("actorId",   callerEntity.getId());
						QSF_MESSAGE.emitMessage(qsf::MessageConfiguration(Messages::EM5_ACTION_PERSON_PLACED_IN_POLICE, callerEntity.getId(), targetEntity->getId()), parameters);

						if (!sendVehicleBackToBase && automatismValue >= GameSettingsGroup::AUTOMATISM_MEDIUM)
						{
							// Send back already when all gangsters are placed
							OrderInfo* targetVehicleOrderInfo = EntityHelper(*targetEntity).getOrderInfo();
							if (targetVehicleOrderInfo != nullptr)
								sendVehicleBackToBase = !EntityHelper(*targetEntity).checkVehicleHasFreeSeatsForPassengerPerson(*targetEntity, *targetVehicleOrderInfo);

							// Free passenger seats - but there are no more gangster on the map
							if (!sendVehicleBackToBase)
							{
								sendVehicleBackToBase = !PolicemanLogic::areGangstersAround(callerEntity);
							}
						}

						// When the vehicle should return to base is handled in the police vehicle logic
						if (sendVehicleBackToBase)
						{
							// Wait before leaving in case units want to enter as well
							qsf::ActionComponent& targetActionComponent = targetEntity->getComponentSafe<qsf::ActionComponent>();
							targetActionComponent.pushAction<WaitForEnterVehicleAction>(action::Priority::BLOCKING, qsf::action::INSERT_AT_FRONT).init(callerEntity.getId(), maxTimeToEnterVehicle);
							targetActionComponent.pushAction<qsf::WaitAction>().init(qsf::Time::fromSeconds(1.0f));
							ReturnToBaseCommand::pushReturnToBaseAction(*targetEntity, action::Priority::COMMAND_STD);

							showAutoTransportHint();
						}

						break;
					}

					case PLACECASE_CIVILIAN:
					{
						// If the placed person is injured, stop their life or health loss now (needed for ME310)
						HealthComponent* healthComponent = carriedPerson->getComponent<HealthComponent>();
						if (nullptr != healthComponent && healthComponent->isInjured())
						{
							healthComponent->setTreatmentActive(true);
						}
						break;
					}

					case PLACECASE_UNKNOWN:
						// Nothing to do in this case
						break;
				}

				// Delete the carried person from the logic
				//  -> This should always be done in the final state, because after that the action would return RESULT_DONE in the pre-checks already
				callerEntityHelper.clearCarryTarget(EntityHelper::CarryLoad::ANY);
				AnimationHelper(callerEntity).clearAnimation();

				// Fall through by design
			}
		}
		return qsf::action::RESULT_DONE;
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	bool PlacePersonAction::checkPlacementStyle(qsf::Entity& vehicleEntity)
	{
		mPlaceCase = PLACECASE_UNKNOWN;
		mDoorType = DoorComponent::DOORTYPE_PASSENGER;

		// Check if carried person is a gangster, a patient, etc.
		const CommandableComponent* commandableComponent = vehicleEntity.getComponent<CommandableComponent>();
		if (nullptr != commandableComponent)
		{
			if (commandableComponent->canTransportInjured())
			{
				// It's a patient
				mPlaceCase = PLACECASE_PATIENT;
				mDoorType = DoorComponent::DOORTYPE_PASSENGER;
				if (UnitHelper(vehicleEntity).isAmbulanceHelicopter())
				{
					// Special handling for phc
					mDoorType = DoorComponent::DOORTYPE_CODRIVER;
				}
				return true;
			}

			if (commandableComponent->canTransportGangster())
			{
				// It's a gangster
				mPlaceCase = PLACECASE_GANGSTER;
				mDoorType = DoorComponent::DOORTYPE_PASSENGER;
				return true;
			}

			if (commandableComponent->getUnitType() == CommandableComponent::UNITTAG_MISC_EVACBUS)
			{
				// It's a civilian (used only for ME310)
				mPlaceCase = PLACECASE_CIVILIAN;
				mDoorType = DoorComponent::DOORTYPE_PASSENGER;
				return true;
			}
		}

		return false;
	}

	qsf::Entity* PlacePersonAction::findClosestDoor(qsf::Entity& vehicleEntity)
	{
		qsf::Entity* result = nullptr;
		const VehicleComponent* vehicleComponent = vehicleEntity.getComponent<VehicleComponent>();
		if (vehicleComponent != nullptr)
		{
			const qsf::Entity& callerEntity = getEntity();
			const qsf::TransformComponent* transformComponent = callerEntity.getComponent<qsf::TransformComponent>();
			if (transformComponent != nullptr)
			{
				result = vehicleComponent->findNearestDoor(mDoorType, transformComponent->getPosition());
			}
		}

		return result;
	}

	void PlacePersonAction::showAutoTransportHint() const
	{
		qsf::Entity* targetEntity = getMap().getEntityById(mTargetEntityId);
		if (nullptr == targetEntity)
			return;

		CommandableComponent* commandableComponent = targetEntity->getComponent<CommandableComponent>();
		if (nullptr == commandableComponent)
			return;

		if (GameSettingsGroup::getInstanceSafe().getPlayUnitHints())
		{
			std::string hintId;

			// Only play unit hints when active
			if (commandableComponent->isAmbulanceRTW())
			{
				hintId = "ID_HINT_RTW_AUTOTRANSPORT_01";
			}
			else if (commandableComponent->isAmbulanceHeli())
			{
				hintId = "ID_HINT_RHC_AUTOTRANSPORT_01";
			}
			else if (commandableComponent->isPoliceSTW())
			{
				hintId = "ID_HINT_STW_AUTOTRANSPORT_01";
			}
			else if (commandableComponent->isPoliceMTW())
			{
				hintId = "ID_HINT_MTW_AUTOTRANSPORT_01";
			}
			else if (commandableComponent->isPoliceHeli())
			{
				hintId = "ID_HINT_PHC_AUTOTRANSPORT_01";
			}

			if (!hintId.empty())
			{
				HintHelper::showHint(hintId, commandableComponent->getPlayerId());
			}
		}

		EM5_GAME.getMiniMapIconRegistrationLogic().registerHighlightIcon(mTargetEntityId);
	}

	void PlacePersonAction::lookForIdleMedicsInArea(qsf::Entity& vehicleEntity)
	{
		if (!QSF_MAINMAP.getCoreEntity().getOrCreateComponentSafe<GameComponent>().getAutoEnteringOfDoctorActive())
			return;

		qsf::TransformComponent* transformComponent = vehicleEntity.getComponent<qsf::TransformComponent>();
		if (transformComponent == nullptr)
			return;

		// Add check for additional victims (doctor targets) around
		qsf::ComponentMapQuery mapQuery(vehicleEntity.getMap());
		std::vector<HealthComponent*> healthComponents;
		if (mapQuery.getInstancesInCircle<HealthComponent>(transformComponent->getPosition(), LogicSpecsGroup::getInstanceSafe().getDoctorSearchRadius() * 2.0f, healthComponents))
		{
			for (HealthComponent* healthComponent : healthComponents)
			{
				if (!healthComponent->isInjured())
					continue;

				if (healthComponent->isTreatmentActive())
					continue;

				if (EntityHelper(healthComponent->getEntity()).getContainerEntity() != nullptr)
					continue;

				// Found a health component which need a treatment don't move any doctor in the vehicle
				return;
			}
		}

		// Search for the closest doctor around
		qsf::Entity* result = nullptr;
		float maxDistance = LogicSpecsGroup::getInstanceSafe().getDoctorSearchRadius();
		std::vector<CommandableComponent*> commandableComponents;
		if (mapQuery.getInstancesInCircle<CommandableComponent>(transformComponent->getPosition(), maxDistance, commandableComponents))
		{
			for (CommandableComponent* commandableComponent : commandableComponents)
			{
				if (!commandableComponent->isAmbulanceDoctor())
					continue;

				qsf::Entity& entityUnit = commandableComponent->getEntity();
				if (!entityUnit.getOrCreateComponentSafe<qsf::ActionComponent>().getPlan().isEmpty())
					continue;

				// Check for free seats
				if (!EntityHelper(entityUnit).checkVehicleHasFreeSeats(vehicleEntity))
					continue;

				if (result == nullptr)
				{
					result = &entityUnit;
				}
				else
				{
					// Make distance check to the result
					if (qsf::game::DistanceHelper::getDistance(vehicleEntity, *result) > qsf::game::DistanceHelper::getDistance(vehicleEntity, entityUnit))
					{
						// Take closer entity
						result = &entityUnit;
					}
				}
			}
		}

		// Push entering actions to the result
		if (result != nullptr)
		{
			qsf::ActionComponent& actionComponentIdler = result->getOrCreateComponentSafe<qsf::ActionComponent>();

			// All checks are passed, automatically enter the vehicle
			actionComponentIdler.pushAction<MoveAction>(action::AUTOMATISM_STD).init(new qsf::ai::ReachObjectGoal(*result, vehicleEntity, EnterVehicleTargetPointProvider::TARGET_POINT_ID));
			actionComponentIdler.pushAction<EnterVehicleAction>(action::AUTOMATISM_STD).init(vehicleEntity);

			// Let the vehicle wait
			vehicleEntity.getOrCreateComponentSafe<qsf::ActionComponent>().pushAction<WaitForEnterVehicleAction>(action::Priority::BLOCKING, qsf::action::INSERT_AT_FRONT).init(result->getId(), qsf::Time::fromSeconds(5.0f));
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

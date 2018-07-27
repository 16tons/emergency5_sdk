// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/command/ambulance/SendDogForSearchCommand.h"
#include "em5/action/ambulance/SendDogForSearchAction.h"
#include "em5/action/move/MoveAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/component/objects/TargetFoundCounterComponent.h"
#include "em5/game/targetpoint/OnTouchTargetPointProvider.h"
#include "em5/health/HealthComponent.h"
#include "em5/logic/local/ambulance/RescueDogLeaderLogic.h"
#include "em5/logic/local/ReserveLogic.h"
#include "em5/map/EntityHelper.h"
#include "em5/map/CollisionHelper.h"
#include "em5/network/NetworkManager.h"
#include "em5/specs/ActionRangeSpecsGroup.h"
#include "em5/EM5Helper.h"

#include <qsf_game/base/DistanceHelper.h>
#include <qsf_game/component/base/HiddenComponent.h>
#include <qsf_game/command/CommandContext.h>

#include <qsf_ai/navigation/goal/ReachSinglePointGoal.h>
#include <qsf_ai/navigation/goal/ReachObjectGoal.h>

#include <qsf/base/ScratchBuffer.h>
#include <qsf/debug/request/SegmentDebugDrawRequest.h>
#include <qsf/logic/action/ActionComponent.h>
#include <qsf/map/query/ComponentMapQuery.h>
#include <qsf/map/Map.h>
#include <qsf/QsfHelper.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 SendDogForSearchCommand::PLUGINABLE_ID = qsf::StringHash("em5::SendDogForSearchCommand");


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	const uint32 SendDogForSearchCommand::ACTION_PRIORITY = action::BLOCKING;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	SendDogForSearchCommand::SendDogForSearchCommand(qsf::game::CommandManager* commandManager) :
		Command(commandManager, PLUGINABLE_ID)
	{
		initValues();
	}

	SendDogForSearchCommand::SendDogForSearchCommand(qsf::game::CommandManager* commandManager, uint32 pluginableId) :
		Command(commandManager, pluginableId)
	{
		initValues();
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::Command methods                   ]
	//[-------------------------------------------------------]
	bool SendDogForSearchCommand::checkAvailable()
	{
		return true;
	}

	bool SendDogForSearchCommand::checkCaller(qsf::Entity& caller)
	{
		// Caller has action with a too high priority?
		if (!checkCallerActionPriority(caller, ACTION_PRIORITY))
		{
			return false;
		}

		// Check if unit state is valid
		if (!EntityHelper(caller).isSquadPersonInValidState())
		{
			return false;
		}

		if (nullptr == EM5_NETWORK.getMultiplayerClient())
		{
			RescueDogLeaderLogic* rescueDogLeaderLogic = EntityHelper(caller).getGameLogic<RescueDogLeaderLogic>();
			if (nullptr == rescueDogLeaderLogic)
			{
				return false;
			}

			if (!rescueDogLeaderLogic->getIsDogUnitedWithLeader())
			{
				return false;
			}
		}
		else
		{
			if (!RescueDogLeaderLogic::checkIsRescueDogUnitedWithLeader(caller))
			{
				return false;
			}
		}

		return true;
	}

	bool SendDogForSearchCommand::checkContext(const qsf::game::CommandContext& context)
	{
		// Self-executed command check
		if (!context.mAllowSelfExecution)
		{
			return false;
		}

		return true;
	}

	void SendDogForSearchCommand::execute(const qsf::game::CommandContext& context)
	{
		// Access the caller's action plan
		qsf::ActionComponent& actionComponent = getActionComponent(*context.mCaller);

		// Check if the same command with the same target is already in the action plan, return in this case without clearing the action plan
		const SendDogForSearchAction* sendDogForSearchAction = actionComponent.getAction<SendDogForSearchAction>();
		if (nullptr != sendDogForSearchAction)
		{
			// Don't push the same command twice
			return;
		}

		// Clear plan before the actions are pushed
		actionComponent.clearPlan();

		bool anyFound = false;
		qsf::Entity* targetEntity = getNextTarget(*context.mCaller, anyFound);

		// The "SendDogForSearchAction" also handles the case that target entity is a null pointer
		actionComponent.pushAction<SendDogForSearchAction>(ACTION_PRIORITY).init(targetEntity, SendDogForSearchAction::UseCase::RESCUE_DOG_SEARCH_VICTIM, &anyFound);

		if (nullptr != targetEntity)
		{
			// Create reservation to avoid multiple targets
			qsf::Entity* rescueDog = RescueDogLeaderLogic::getRescueDogEntityFromLeader(*context.mCaller);
			if (nullptr == rescueDog)
			{
				return; // Error
			}

			ReserveLogic::createReservation<ReserveRescueDogLogic>(*targetEntity, rescueDog->getId());

			#ifndef RETAIL
				// Debug output
				qsf::DebugDrawProxy ddp;
				ddp.registerAt(QSF_DEBUGDRAW);
				ddp.addRequest(qsf::SegmentDebugDrawRequest(qsf::Segment::fromTwoPoints(EntityHelper(*context.mCaller).getPosition(), EntityHelper(*targetEntity).getPosition())));
				ddp.setLifetimeInSeconds(5.0f);
				ddp.detachRequests();
			#endif
		}
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	qsf::Entity* SendDogForSearchCommand::getNextTarget(qsf::Entity& callerEntity, bool& anyFound)
	{
		// Try to find a blocked hidden person
		qsf::EntityVectorScratchBuffer hiddenBlockedPersons;
		getAllHiddenBlockedPersons(hiddenBlockedPersons, callerEntity);

		qsf::EntityVectorScratchBuffer blockedInVehicle;
		getAllBlockedInVehicles(blockedInVehicle, callerEntity);

		#ifndef ENDUSER
			QSF_LOG_PRINTS(DEBUG, "Rescue dog targets: " << hiddenBlockedPersons.size() << " hidden blocked; " << blockedInVehicle.size() << " blocked in vehicle");
		#endif

		anyFound = (hiddenBlockedPersons.size() + blockedInVehicle.size() > 0);

		// Get closest target, preferably a hidden blocked person
		const glm::vec3& position = EntityHelper(callerEntity).getPosition();
		const float searchRange = ActionRangeSpecsGroup::getInstanceSafe().getRescueDogSearchDistance();

		qsf::Entity* result = getClosestInsideRange(hiddenBlockedPersons, position, searchRange);
		if (nullptr == result)
		{
			result = getClosestInsideRange(blockedInVehicle, position, searchRange);
		}

		return result;
	}


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	void SendDogForSearchCommand::initValues()
	{
		// Priority is at default value

		// Lowest button priority, so the button is always at the right (with hotkey 'e')
		mButtonPriority = -50;

		mIconSettings.mShowAsGuiButton = true;
		mIconSettings.mButtonIconPath = "send_dog";
		// Set name of the mouse cursor
		mIconSettings.mMouseIconName = "send_dog_for_search";
		mIconSettings.mTooltip = "EM5_Command_31";

		// Set the help page name
		mHelpPageName = "TUTORIAL_HELPTEXT_INTERACTION_RESCUEDOG_SEND";
	}

	void SendDogForSearchCommand::getAllHiddenBlockedPersons(std::vector<qsf::Entity*>& resultList, qsf::Entity& callerEntity)
	{
		qsf::Map& map = callerEntity.getMap();
		const auto& hiddenComponentList = qsf::ComponentMapQuery(map).getAllInstances<qsf::game::HiddenComponent>();

		// Try to find a blocked hidden person
		for (const qsf::game::HiddenComponent* chosenHiddenComponent : hiddenComponentList)
		{
			if (chosenHiddenComponent->getHiddenType() != qsf::game::HiddenComponent::HIDDENTYPE_BLOCKED)
			{
				continue;
			}

			qsf::Entity& blockedEntity = chosenHiddenComponent->getEntity();

			const EntityHelper entityHelper(blockedEntity);

			if (!entityHelper.isInMapBoundaries())
			{
				continue;
			}

			if (!entityHelper.isEntityPerson())
			{
				continue;
			}

			if (!ReserveLogic::checkReservation(blockedEntity, callerEntity.getId(), ReserveLogic::LOW))
			{
				continue;
			}

			// Already found?
			if (nullptr != blockedEntity.getComponent<TargetFoundCounterComponent>())
			{
				continue;
			}

			// Found a potential target
			resultList.push_back(&blockedEntity);
		}
	}

	void SendDogForSearchCommand::getAllBlockedInVehicles(std::vector<qsf::Entity*>& resultList, qsf::Entity& callerEntity)
	{
		qsf::Map& map = callerEntity.getMap();
		const auto& healthComponentList = qsf::ComponentMapQuery(map).getAllInstances<HealthComponent>();

		// Try to find persons, trapped in an civil vehicle
		for (const HealthComponent* chosenHealthComponent : healthComponentList)
		{
			if (!chosenHealthComponent->isInjured())
			{
				continue;
			}

			qsf::Entity& blockedEntity = chosenHealthComponent->getEntity();

			const EntityHelper entityHelper(blockedEntity);

			if (!entityHelper.isInMapBoundaries())
			{
				continue;
			}

			// Check the vehicle the person is inside
			qsf::Entity* civilVehicle = entityHelper.getContainerEntity();
			if (nullptr == civilVehicle)
			{
				continue;
			}

			if (!EntityHelper(*civilVehicle).isCivilRoadVehicle())
			{
				continue;
			}

			if (!ReserveLogic::checkReservation(blockedEntity, callerEntity.getId(), ReserveLogic::LOW))
			{
				continue;
			}

			// Already found?
			if (nullptr != blockedEntity.getComponent<TargetFoundCounterComponent>())
			{
				continue;
			}

			// Found a potential target
			resultList.push_back(&blockedEntity);
		}
	}

	qsf::Entity* SendDogForSearchCommand::getClosestInsideRange(const std::vector<qsf::Entity*>& entityList, const glm::vec3& position, float searchRange)
	{
		// Find the nearest entity
		qsf::Entity* nearestEntity = nullptr;
		float maxDistance2 = searchRange * searchRange;
		for (qsf::Entity* targetEntity : entityList)
		{
			const float currentDistance2 = glm::distance2(EntityHelper(*targetEntity).getPosition(), position);
			if (currentDistance2 < maxDistance2)
			{
				maxDistance2 = currentDistance2;
				nearestEntity = targetEntity;
			}
		}
		return nearestEntity;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

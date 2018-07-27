// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/command/police/SendPoliceDogForSearchCommand.h"
#include "em5/command/ambulance/SendDogForSearchCommand.h"
#include "em5/action/ambulance/SendDogForSearchAction.h"
#include "em5/action/police/ArrestGangsterAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/component/objects/PoliceDogTargetComponent.h"
#include "em5/logic/local/ambulance/RescueDogLeaderLogic.h"
#include "em5/logic/local/ReserveLogic.h"
#include "em5/map/EntityHelper.h"
#include "em5/network/NetworkManager.h"
#include "em5/specs/ActionRangeSpecsGroup.h"
#include "em5/EM5Helper.h"

#include <qsf_game/command/CommandContext.h>

#include <qsf/base/ScratchBuffer.h>
#include <qsf/logic/action/ActionComponent.h>
#include <qsf/logic/gamelogic/GameLogicComponent.h>
#include <qsf/map/query/ComponentMapQuery.h>
#include <qsf/map/Map.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 SendPoliceDogForSearchCommand::PLUGINABLE_ID = qsf::StringHash("em5::SendPoliceDogForSearchCommand");


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	const uint32 SendPoliceDogForSearchCommand::ACTION_PRIORITY = action::BLOCKING;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	SendPoliceDogForSearchCommand::SendPoliceDogForSearchCommand(qsf::game::CommandManager* commandManager) :
		SendDogForSearchCommand(commandManager, PLUGINABLE_ID)
	{
		// Lowest button priority, so the button is always at the right (with hotkey 'e')
		// -> Identical to "em5::SendDogForSearchCommand"
		mButtonPriority = -50;

		mIconSettings.mShowAsGuiButton = true;
		mIconSettings.mButtonIconPath = "send_policedog";

		// Set name of the mouse cursor
		mIconSettings.mMouseIconName = "send_policedog_for_search";
		mIconSettings.mTooltip = "EM5_Command_83";

		// Set the help page name
		mHelpPageName = "TUTORIAL_HELPTEXT_INTERACTION_POLICEDOG_SEARCH";
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::Command methods                   ]
	//[-------------------------------------------------------]
	bool SendPoliceDogForSearchCommand::checkAvailable()
	{
		return true;
	}

	bool SendPoliceDogForSearchCommand::checkCaller(qsf::Entity& caller)
	{
		// Caller has action with a too high priority?
		if (!checkCallerActionPriority(caller, ACTION_PRIORITY))
		{
			return false;
		}

		// Check if unit state is valid
		EntityHelper entityHelper(caller);
		if (!entityHelper.isSquadPersonInValidState())
		{
			return false;
		}

		// Not already carrying someone?
		if (entityHelper.isCarryingAnotherPerson())
			return false;

		if (nullptr != getActionComponent(caller).getAction<ArrestGangsterAction>())
			return false;	// Finish arresting

		if (nullptr == EM5_NETWORK.getMultiplayerClient())
		{
			qsf::GameLogicComponent* gameLogicComponent = caller.getComponent<qsf::GameLogicComponent>();
			if (nullptr == gameLogicComponent)
			{
				return false;
			}

			RescueDogLeaderLogic* rescueDogLeaderLogic = gameLogicComponent->getGameLogic<RescueDogLeaderLogic>();
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

	bool SendPoliceDogForSearchCommand::checkContext(const qsf::game::CommandContext& context)
	{
		// Self-executed command check
		if (!context.mAllowSelfExecution)
		{
			if (nullptr == context.mTargetEntity)
				return false;

			// Check if the target is really inside the vehicle
			PoliceDogTargetComponent* policeDogTargetComponent = context.mTargetEntity->getComponent<PoliceDogTargetComponent>();
			if (nullptr == policeDogTargetComponent)
				return false;
		}
		// else right click on the caller or button pressed

		return true;
	}

	void SendPoliceDogForSearchCommand::execute(const qsf::game::CommandContext& context)
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

		qsf::Entity* targetEntity = nullptr;
		if (nullptr != context.mTargetEntity && context.mCaller != context.mTargetEntity)
		{
			targetEntity = context.mTargetEntity;
		}
		else
		{
			// Search for target, result can be nullptr
			bool anyFound = false;		// Not used for police dog
			targetEntity = getNextTarget(*context.mCaller, anyFound);
		}

		// The "SendDogForSearchAction" also handles the case that target entity is a null pointer
		actionComponent.pushAction<SendDogForSearchAction>(ACTION_PRIORITY).init(targetEntity, SendDogForSearchAction::UseCase::POLICE_DOG_SEARCH_OBJECT);

		if (nullptr != targetEntity)
		{
			// Create reservation to avoid multiple targets
			qsf::Entity* rescueDog = RescueDogLeaderLogic::getRescueDogEntityFromLeader(*context.mCaller);
			if (nullptr == rescueDog)
			{
				return; // Error
			}

			ReserveLogic::createReservation<ReserveRescueDogLogic>(*targetEntity, rescueDog->getId());
		}
	}


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	qsf::Entity* SendPoliceDogForSearchCommand::getNextTarget(qsf::Entity& callerEntity, bool& anyFound)
	{
		const float searchRange = ActionRangeSpecsGroup::getInstanceSafe().getRescueDogSearchDistance();

		// Try to find a police dog target
		qsf::EntityVectorScratchBuffer policeDogTargets;
		getPoliceDogTargets(policeDogTargets, callerEntity, searchRange);

		if (policeDogTargets.empty())
		{
			anyFound = false;
			return nullptr;
		}
		else
		{
			// Note that it's possible to have "anyFound" set, but return a null pointer - namely when none of the targets is in range
			// Attention: "anyFound" is only set to true if there's a target in range, but not if one is outside range
			//  -> That's a difference to the rescue dog
			//  -> It was done as a lazy implementation here, as we don't need this information for the police dog (yet?)
			anyFound = true;
		}

		// Return nearest, can be nullptr
		return getClosestInsideRange(policeDogTargets, EntityHelper(callerEntity).getPosition(), searchRange);
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void SendPoliceDogForSearchCommand::getPoliceDogTargets(std::vector<qsf::Entity*>& resultList, qsf::Entity& callerEntity, float searchRange)
	{
		std::vector<PoliceDogTargetComponent*> policeDogTargetComponents;
		glm::vec3 callerPosition = EntityHelper(callerEntity).getPosition();
		qsf::Map& map = callerEntity.getMap();
		qsf::ComponentMapQuery(map).getInstancesInCircle(callerPosition, searchRange, policeDogTargetComponents);

		// Try to find a police dog target
		for (const PoliceDogTargetComponent* chosenPoliceDogTargetComponent : policeDogTargetComponents)
		{
			qsf::Entity& targetEntity = chosenPoliceDogTargetComponent->getEntity();

			if (!ReserveLogic::checkReservation(targetEntity, callerEntity.getId(), ReserveLogic::LOW))
			{
				continue;
			}

			// Found a potential target
			resultList.push_back(&targetEntity);
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

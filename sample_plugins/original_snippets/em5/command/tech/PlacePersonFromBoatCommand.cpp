// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/command/tech/PlacePersonFromBoatCommand.h"
#include "em5/action/ActionPriority.h"
#include "em5/action/move/MoveAction.h"
#include "em5/action/tech/PlacePersonFromBoatAction.h"
#include "em5/component/vehicle/BoatComponent.h"
#include "em5/component/objects/BoatDropPointComponent.h"
#include "em5/game/targetpoint/BoatDropTargetPointProvider.h"
#include "em5/map/EntityHelper.h"
#include "em5/health/HealthComponent.h"

#include <qsf_game/command/CommandContext.h>

#include <qsf_ai/navigation/goal/ReachObjectGoal.h>

#include <qsf/component/link/LinkComponent.h>
#include <qsf/logic/action/ActionComponent.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 PlacePersonFromBoatCommand::PLUGINABLE_ID = qsf::StringHash("em5::PlacePersonFromBoatCommand");


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	const uint32 PlacePersonFromBoatCommand::ACTION_PRIORITY = action::COMMAND_STD;
	const uint32 PlacePersonFromBoatCommand::ACTION_PRIORITY_AUTOMATISM = action::AUTOMATISM_STD;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	PlacePersonFromBoatCommand::PlacePersonFromBoatCommand(qsf::game::CommandManager* commandManager) :
		Command(commandManager, PLUGINABLE_ID)
	{
		// Priority is at default value

		// Set name of the mouse cursor
		mIconSettings.mMouseIconName = "place_person_from_boat";
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::Command methods                   ]
	//[-------------------------------------------------------]
	bool PlacePersonFromBoatCommand::checkAvailable()
	{
		return true;
	}

	bool PlacePersonFromBoatCommand::checkCallerWithoutPriority(qsf::Entity& caller)
	{
		if (!EntityHelper(caller).isBoat())
			return false;

		if (EntityHelper(caller).isContainerEmpty())
			return false;

		return true;
	}

	bool PlacePersonFromBoatCommand::checkCaller(qsf::Entity& caller)
	{
		// Caller has action with a too high priority?
		if (!checkCallerActionPriorityNoBlocking(caller, action::BLOCKING))
			return false;

		if (!checkCallerWithoutPriority(caller))
			return false;

		// Check if the boat is not on a transporter
		qsf::LinkComponent* linkComponent = caller.getComponent<qsf::LinkComponent>();
		if (nullptr != linkComponent && qsf::isInitialized(linkComponent->getParentId()))
			return false;

		return true;
	}

	bool PlacePersonFromBoatCommand::checkContext(const qsf::game::CommandContext& context)
	{
		// Validate target
		qsf::Entity* targetEntity = context.mTargetEntity;
		if (nullptr == targetEntity)
			return false;

		// Is the target an unload spot?
		const BoatDropPointComponent* boatDropPointComponent = targetEntity->getComponent<BoatDropPointComponent>();
		if (nullptr == boatDropPointComponent)
			return false;

		// Check if succeeded
		return true;
	}

	void PlacePersonFromBoatCommand::execute(const qsf::game::CommandContext& context)
	{
		// Access the caller's action plan
		qsf::ActionComponent& actionComponent = getActionComponent(*context.mCaller);

		// Check if the same command with the same target is already in the action plan, return in this case without clearing the action plan
		const PlacePersonFromBoatAction* layDownPersonFromBoatAction = actionComponent.getAction<PlacePersonFromBoatAction>();
		if (nullptr != layDownPersonFromBoatAction && layDownPersonFromBoatAction->getTargetId() == context.mTargetEntity->getId())
		{
			// Don't push the same command twice
			return;
		}

		// Clear plan before the actions are pushed
		actionComponent.clearPlan();

		actionComponent.pushAction<MoveAction>(ACTION_PRIORITY).init(new qsf::ai::ReachObjectGoal(*context.mCaller, *context.mTargetEntity, BoatDropTargetPointProvider::TARGET_POINT_ID));
		actionComponent.pushAction<PlacePersonFromBoatAction>(action::BLOCKING).init(*context.mTargetEntity);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

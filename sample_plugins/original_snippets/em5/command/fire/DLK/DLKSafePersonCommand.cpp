// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/command/fire/DLK/DLKSafePersonCommand.h"
#include "em5/command/CommandContext.h"
#include "em5/action/fire/DLK/DLKSavePersonAction.h"
#include "em5/action/move/MoveAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/ai/MovementModes.h"
#include "em5/component/building/BuildingComponent.h"
#include "em5/component/objects/CallForRescueComponent.h"
#include "em5/component/person/PersonComponent.h"
#include "em5/game/targetpoint/DlkRescuePersonTargetPointProvider.h"
#include "em5/logic/local/ReserveLogic.h"
#include "em5/logic/local/firefighters/DLKLogic.h"
#include "em5/map/EntityHelper.h"
#include "em5/plugin/Messages.h"
#include "em5/specs/SpecsGroupManager.h"
#include "em5/EM5Helper.h"

#include <qsf_ai/navigation/goal/ReachObjectGoal.h>

#include <qsf/component/move/MovableComponent.h>
#include <qsf/logic/action/ActionComponent.h>
#include <qsf/message/MessageSystem.h>
#include <qsf/map/query/GroundMapQuery.h>
#include <qsf/map/Entity.h>
#include <qsf/map/Map.h>
#include <qsf/QsfHelper.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Local definitions                                     ]
	//[-------------------------------------------------------]
	const uint32 ACTION_PRIORITY = action::COMMAND_STD;
	const uint32 ACTION_PRIORITY_AUTOMATISM = action::AUTOMATISM_STD;


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 DLKSafePersonCommand::PLUGINABLE_ID = qsf::StringHash("em5::DLKSafePersonCommand");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	DLKSafePersonCommand::DLKSafePersonCommand(qsf::game::CommandManager* commandManager) :
		Command(commandManager, PLUGINABLE_ID)
	{
		mIconSettings.mMouseIconName = "dlk_rescue_person";
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::Command methods                   ]
	//[-------------------------------------------------------]
	bool DLKSafePersonCommand::checkAvailable()
	{
		return true;
	}

	bool DLKSafePersonCommand::checkCaller(qsf::Entity& caller)
	{
		return checkCallerActionPriority(caller, ACTION_PRIORITY);
	}

	bool DLKSafePersonCommand::checkContext(const qsf::game::CommandContext& context)
	{
		// Target entity needed
		qsf::Entity* targetEntity = context.mTargetEntity;
		if (nullptr == targetEntity)
			return false;

		// Check if target is hidden
		const EntityHelper entityHelper(*targetEntity);
		if (entityHelper.isEntityHidden())
			return false;

		// Check if the target is reserved
		if (!ReserveLogic::checkReservation(*targetEntity, context.mCaller->getId()))
			return false;

		// Rescue an event-specific tagged person rescue a waving person from a building balcony
		qsf::Entity* personEntity = targetEntity;

		// Check if we have a building as target or the target is inside a building
		BuildingComponent* buildingComponent = DLKLogic::getPlaceOfActionBuilding(*targetEntity);
		if (nullptr != buildingComponent)
		{
			// Is there someone to rescue
			personEntity = buildingComponent->getPersonWavingOutOfWindow();
			if (nullptr == personEntity)
				return false;
		}
		else
		{
			// Rescue only persons with dlk tags
			CallForRescueComponent* callForRescueComponent = personEntity->getComponent<CallForRescueComponent>();
			if (nullptr == callForRescueComponent || !callForRescueComponent->getCanRescuedByDLK())
			{
				return false;
			}
		}

		// Check if personEntity is a person
		const PersonComponent* personComponent = personEntity->getComponent<PersonComponent>();
		if (nullptr == personComponent)
			return false;

		// Height is not checked, the game should never create unreachable targets and if that happens we should save them anyway

		return true;
	}

	void DLKSafePersonCommand::execute(const qsf::game::CommandContext& context)
	{
		// Access the caller's action plan
		qsf::ActionComponent& actionComponent = getActionComponent(*context.mCaller);

		// Check if the same command with the same target is already in the action plan, return in this case without clearing the action plan
		const DLKSavePersonAction* dlkSavePersonAction = actionComponent.getAction<DLKSavePersonAction>();
		if (nullptr != dlkSavePersonAction && dlkSavePersonAction->getTargetEntityId() == context.mTargetEntity->getId())
		{
			// Don't push the same command twice
			return;
		}

		// Clear plan before the actions are pushed
		actionComponent.clearPlan();

		const uint32 usedActionPriority = (static_cast<const CommandContext&>(context).mIsAutomatism ? ACTION_PRIORITY_AUTOMATISM : ACTION_PRIORITY);

		QSF_MESSAGE.emitMessage(qsf::MessageConfiguration(Messages::EM5_ON_COMMAND_EXECUTE, PLUGINABLE_ID, context.mCaller->getId(), context.mTargetEntity->getId()));

		actionComponent.pushAction<MoveAction>(usedActionPriority).init(new qsf::ai::ReachObjectGoal(*context.mCaller, *context.mTargetEntity, DlkRescuePersonTargetPointProvider::TARGET_POINT_ID), MovementModes::MOVEMENT_MODE_PLAYER_VEHICLE_ON_EMERGENCY_OPERATIONS);
		actionComponent.pushAction<DLKSavePersonAction>(action::BLOCKING, qsf::action::APPEND_TO_BACK).init(context.mTargetEntity->getId());
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

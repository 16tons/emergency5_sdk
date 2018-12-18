// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/command/police/ArrestGangsterCommand.h"
#include "em5/action/police/ArrestGangsterAction.h"
#include "em5/action/move/MoveAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/base/EquipmentAssets.h"
#include "em5/component/person/PersonComponent.h"
#include "em5/game/targetpoint/AttackPersonTargetPointProvider.h"
#include "em5/map/EntityHelper.h"
#include "em5/plugin/Messages.h"

#include <qsf_game/command/CommandContext.h>

#include <qsf_ai/navigation/goal/ReachObjectGoal.h>

#include <qsf/map/Map.h>
#include <qsf/logic/action/ActionComponent.h>
#include <qsf/QsfHelper.h>
#include <qsf/message/MessageConfiguration.h>
#include <qsf/message/MessageSystem.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 ArrestGangsterCommand::PLUGINABLE_ID = qsf::StringHash("em5::ArrestGangsterCommand");


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	const uint32 ArrestGangsterCommand::ACTION_PRIORITY = action::COMMAND_STD;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	ArrestGangsterCommand::ArrestGangsterCommand(qsf::game::CommandManager* commandManager) :
		Command(commandManager, PLUGINABLE_ID)
	{
		// Priority is at default value

		// Set name of the mouse cursor
		mIconSettings.mMouseIconName = "arrest_gangster";
	}

	bool ArrestGangsterCommand::checkCallerWithoutPriority(qsf::Entity& caller)
	{
		// Check if unit state is valid
		EntityHelper entityHelper(caller);
		if (!entityHelper.isSquadPersonInValidState())
			return false;

		// Not already carrying someone?
		if (entityHelper.isCarryingAnotherPerson())
			return false;

		// Check if equipment matches
		if (!entityHelper.checkIsActiveEquipment(equipment::POLICE_HANDCUFFS_OPEN) && !entityHelper.checkIsActiveEquipment(equipment::POLICE_HANDCUFFS) &&
			!entityHelper.checkIsActiveEquipment(equipment::POLICE_SWORD))
			return false;

		return true;
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::Command methods                   ]
	//[-------------------------------------------------------]
	bool ArrestGangsterCommand::checkAvailable()
	{
		return true;
	}

	bool ArrestGangsterCommand::checkCaller(qsf::Entity& caller)
	{
		// Caller has action with a too high priority?
		if (!checkCallerActionPriority(caller, ACTION_PRIORITY))
			return false;

		if (!checkCallerWithoutPriority(caller))
			return false;

		return true;
	}

	bool ArrestGangsterCommand::checkContext(const qsf::game::CommandContext& context)
	{
		// We need a target entity
		if (nullptr == context.mTargetEntity)
			return false;

		// Get the person and game logic component
		PersonComponent* targetPersonComponent = context.mTargetEntity->getComponent<PersonComponent>();
		if (nullptr == targetPersonComponent)
			return false;

		// Target person has to be a gangster
		if (!targetPersonComponent->getGangsterFlag())
			return false;

		// Check for healthy gangster (injured gangsters need to be treated instead of arrested)
		if (EntityHelper(*context.mTargetEntity).isPersonInjured())
		{
			return false;
		}

		return true;
	}

	void ArrestGangsterCommand::execute(const qsf::game::CommandContext& context)
	{
		// Access the caller's action plan
		qsf::ActionComponent& actionComponent = getActionComponent(*context.mCaller);

		// Check if the same command with the same target is already in the action plan, return in this case without clearing the action plan
		const ArrestGangsterAction* arrestGangsterAction = actionComponent.getAction<ArrestGangsterAction>();
		if (nullptr != arrestGangsterAction)
		{
			if (arrestGangsterAction->getTargetEntityId() == context.mTargetEntity->getId())
			{
				// Don't push the same command twice
				return;
			}
		}

		QSF_MESSAGE.emitMessage(qsf::MessageConfiguration(Messages::EM5_ON_COMMAND_EXECUTE, PLUGINABLE_ID, context.mCaller->getId(), context.mTargetEntity->getId()));

		// Clear plan before the actions are pushed
		actionComponent.clearPlan();

		actionComponent.pushAction<MoveAction>(ACTION_PRIORITY).init(new qsf::ai::ReachObjectGoal(*context.mCaller, *context.mTargetEntity, AttackPersonTargetPointProvider::TARGET_POINT_ID));
		actionComponent.pushAction<ArrestGangsterAction>(action::BLOCKING).init(*context.mTargetEntity);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

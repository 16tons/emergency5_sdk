// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/command/police/historic/GuardsmanArrestGangsterCommand.h"
#include "em5/action/fire/FireHoseCheckAction.h"
#include "em5/action/police/ArrestGangsterAction.h"
#include "em5/action/move/MoveAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/component/person/PersonComponent.h"
#include "em5/game/targetpoint/AttackPersonTargetPointProvider.h"
#include "em5/map/EntityHelper.h"
#include "em5/base/EquipmentAssets.h"

#include <qsf_ai/navigation/goal/ReachObjectGoal.h>

#include <qsf_game/command/CommandContext.h>
#include <qsf/logic/action/ActionComponent.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 GuardsmanArrestGangsterCommand::PLUGINABLE_ID = qsf::StringHash("em5::GuardsmanArrestGangsterCommand");


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	const uint32 GuardsmanArrestGangsterCommand::ACTION_PRIORITY = action::COMMAND_LOW;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	GuardsmanArrestGangsterCommand::GuardsmanArrestGangsterCommand(qsf::game::CommandManager* commandManager) :
		Command(commandManager, PLUGINABLE_ID)
	{
		// Priority is at default value

		// This command is triggered from a HUD element, we need an icon
		mIconSettings.mButtonIconPath = "guard_medieval_handcuffs";
		mIconSettings.mTooltip = "EM5_Command_36";
		mIconSettings.mMouseIconName = "arrest_gangster_medieval";
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::Command methods                   ]
	//[-------------------------------------------------------]
	bool GuardsmanArrestGangsterCommand::checkAvailable()
	{
		return true;
	}

	bool GuardsmanArrestGangsterCommand::checkCaller(qsf::Entity& caller)
	{
		// Caller has action with a too high priority?
		if (!checkCallerActionPriority(caller, ACTION_PRIORITY))
			return false;

		// Check if unit state is valid
		EntityHelper entityHelper(caller);
		if (!entityHelper.isSquadPersonInValidState())
			return false;

		if (entityHelper.isCarryingAnotherPerson())
			return false;

		return true;
	}

	bool GuardsmanArrestGangsterCommand::checkContext(const qsf::game::CommandContext& context)
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

		if (targetPersonComponent->getPersonFlags(PersonComponent::FLAG_INVOLVED_IN_FIGHT))
			return false;

		// Check for healthy gangster (injured gangsters need to be treated instead of arrested)
		if (EntityHelper(*context.mTargetEntity).isPersonInjured())
		{
			return false;
		}

		return true;
	}

	void GuardsmanArrestGangsterCommand::execute(const qsf::game::CommandContext& context)
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

		// Clear plan before the actions are pushed
		actionComponent.clearPlan();

		actionComponent.pushAction<FireHoseCheckAction>(ACTION_PRIORITY).init(glm::vec3(), true);
		actionComponent.pushAction<MoveAction>(ACTION_PRIORITY).init(new qsf::ai::ReachObjectGoal(*context.mCaller, *context.mTargetEntity, AttackPersonTargetPointProvider::TARGET_POINT_ID));
		actionComponent.pushAction<ArrestGangsterAction>(action::BLOCKING).init(*context.mTargetEntity);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

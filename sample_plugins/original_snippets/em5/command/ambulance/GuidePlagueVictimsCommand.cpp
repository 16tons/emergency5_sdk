// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/command/ambulance/GuidePlagueVictimsCommand.h"
#include "em5/action/move/MoveAction.h"
#include "em5/action/fire/PickupContaminatedPersonAction.h"
#include "em5/action/fire/FireHoseCheckAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/command/CommandContext.h"
#include "em5/game/targetpoint/LeadPersonTargetPointProvider.h"
#include "em5/health/HealthComponent.h"
#include "em5/logic/local/ReserveLogic.h"
#include "em5/map/EntityHelper.h"
#include "em5/plugin/Messages.h"

#include <qsf_ai/navigation/goal/ReachObjectGoal.h>

#include <qsf/input/InputSystem.h>
#include <qsf/input/device/KeyboardDevice.h>
#include <qsf/link/link/prototype/ContainerLink.h>
#include <qsf/logic/action/ActionComponent.h>
#include <qsf/logic/action/TurnToAction.h>
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
	const uint32 GuidePlagueVictimsCommand::PLUGINABLE_ID = qsf::StringHash("em5::GuidePlagueVictimsCommand");


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	const uint32 GuidePlagueVictimsCommand::ACTION_PRIORITY = action::COMMAND_LOW;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	GuidePlagueVictimsCommand::GuidePlagueVictimsCommand(qsf::game::CommandManager* commandManager) :
		Command(commandManager, PLUGINABLE_ID)
	{
		// Priority is at default value

		// Set name of the mouse cursor
		mIconSettings.mMouseIconName = "lead_contaminated_person";
	}

	bool GuidePlagueVictimsCommand::checkCallerWithoutPriority(qsf::Entity& caller)
	{
		// Check if unit state is valid
		EntityHelper entityHelper(caller);
		if (!entityHelper.isSquadPersonInValidState())
			return false;

		if (entityHelper.isCarryingAnotherPerson())
			return false;

		return true;
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::Command methods                   ]
	//[-------------------------------------------------------]
	bool GuidePlagueVictimsCommand::checkAvailable()
	{
		return true;
	}

	bool GuidePlagueVictimsCommand::checkCaller(qsf::Entity& caller)
	{
		// Caller has action with a too high priority?
		if (!checkCallerActionPriority(caller, action::COMMAND_STD))
			return false;

		if (!checkCallerWithoutPriority(caller))
			return false;

		return true;
	}

	bool GuidePlagueVictimsCommand::checkContext(const qsf::game::CommandContext& context)
	{
		// Validate target
		qsf::Entity* targetEntity = context.mTargetEntity;
		if (nullptr == targetEntity)
			return false;

		// Target persons
		HealthComponent* healthComponent = targetEntity->getComponent<HealthComponent>();
		if (nullptr == healthComponent)
			return false;

		// There are two use-cases:
		//  a) Plague victims in EM2016 campaign
		//  b) Or in general when a freeplay event says yes (for persons in EM20 "Nuclear power plant" mission)

		if (healthComponent->getContaminationType() == Injury::CONTAMINATION_PLAGUE)
		{
			// Only after quick test happened
			if (healthComponent->getCanDoContaminationQuickTest())
				return false;
		}
		else
		{
			bool response = false;
			QSF_MESSAGE.emitMessageWithResponse(qsf::MessageConfiguration(Messages::EM5_COMMAND_GUIDE_PERSON_CHECK_TARGET, context.mCaller->getId(), targetEntity->getId()), response);
			if (!response)
			{
				// No valid target
				return false;
			}
		}

		// Only standing persons
		const Injury* targetInjury = healthComponent->getInjury();
		if (nullptr != targetInjury)
		{
			if (!targetInjury->isAffectingHealth())
			{
				return false;
			}
		}

		// Is target reserved
		const CommandContext& em5CommandContext = static_cast<const CommandContext&>(context);
		if (em5CommandContext.mIsAutomatism)
		{
			if (!ReserveLogic::checkReservation(*targetEntity, context.mCaller->getId(), ReserveLogic::MOVETO))
				return false;

			// Don't lead unit automatically in dangerous situations
			if (healthComponent->isReceivingFireDamage())
				return false;
		}

		// Check succeeded
		return true;
	}

	void GuidePlagueVictimsCommand::execute(const qsf::game::CommandContext& context)
	{
		// Access the caller's action plan
		qsf::ActionComponent& actionComponent = getActionComponent(*context.mCaller);

		// Check if the same command with the same target is already in the action plan, return in this case without clearing the action plan
		const PickupContaminatedPersonAction* pickupContaminatedPersonAction = actionComponent.getAction<PickupContaminatedPersonAction>();
		if (nullptr != pickupContaminatedPersonAction && pickupContaminatedPersonAction->getTargetEntityId() == context.mTargetEntity->getId())
		{
			// Don't push the same command twice
			return;
		}

		if (isNoQueueingCommand(*context.mCaller))
		{
			// Clear plan before the actions are pushed
			actionComponent.clearPlan();
		}

		actionComponent.pushAction<FireHoseCheckAction>(ACTION_PRIORITY).init(*context.mTargetEntity, true);
		actionComponent.pushAction<MoveAction>(ACTION_PRIORITY).init(new qsf::ai::ReachObjectGoal(*context.mCaller, *context.mTargetEntity, LeadPersonTargetPointProvider::TARGET_POINT_ID));
		actionComponent.pushAction<qsf::TurnToAction>(ACTION_PRIORITY).init(context.mTargetEntity->getId());
		actionComponent.pushAction<PickupContaminatedPersonAction>(ACTION_PRIORITY).init(context.mTargetEntity->getId(), false);

		// Stop the target's current movement
		getActionComponent(*context.mTargetEntity).clearPlan();
		context.mTargetEntity->destroyComponentById("em5::MoveToDespawnComponent");
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	bool GuidePlagueVictimsCommand::isNoQueueingCommand(qsf::Entity& firefighterEntity)
	{
		if (QSF_INPUT.getKeyboard().anyShiftPressed())
		{
			return false;
		}

		return true;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

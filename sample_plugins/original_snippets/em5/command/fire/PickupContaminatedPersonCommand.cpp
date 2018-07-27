// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/command/fire/PickupContaminatedPersonCommand.h"
#include "em5/action/ActionPriority.h"
#include "em5/action/move/MoveAction.h"
#include "em5/action/fire/FireHoseCheckAction.h"
#include "em5/action/fire/PickupContaminatedPersonAction.h"
#include "em5/game/targetpoint/LeadPersonTargetPointProvider.h"
#include "em5/game/targetpoint/PullPersonTargetPointProvider.h"
#include "em5/game/units/UnitHelper.h"
#include "em5/health/HealthComponent.h"
#include "em5/map/EntityHelper.h"

#include <qsf_game/command/CommandContext.h>

#include <qsf_ai/navigation/goal/ReachObjectGoal.h>

#include <qsf/logic/action/ActionComponent.h>
#include <qsf/logic/action/TurnToAction.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 PickupContaminatedPersonCommand::PLUGINABLE_ID = qsf::StringHash("em5::PickupContaminatedPersonCommand");


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	const uint32 PickupContaminatedPersonCommand::ACTION_PRIORITY = action::COMMAND_STD;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	PickupContaminatedPersonCommand::PickupContaminatedPersonCommand(qsf::game::CommandManager* commandManager) :
		Command(commandManager, PLUGINABLE_ID)
	{
		// Priority is at default value

		// Set name of the mouse cursor
		mIconSettings.mMouseIconName = "decontaminate_person";
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::Command methods                   ]
	//[-------------------------------------------------------]
	bool PickupContaminatedPersonCommand::checkAvailable()
	{
		return true;
	}

	bool PickupContaminatedPersonCommand::checkCaller(qsf::Entity& caller)
	{
		// Caller has action with a too high priority?
		if (!checkCallerActionPriority(caller, ACTION_PRIORITY))
			return false;

		// Check if unit state is valid
		EntityHelper entityHelper(caller);
		if (!entityHelper.isSquadPersonInValidState())
			return false;

		// Unit need an abc suit to do this
		if (!UnitHelper(caller).isFireFighterPersonWithABCSuitActive())
			return false;

		// Must not pull or carry anything
		uint64 carriedId = entityHelper.getCarryTarget(EntityHelper::CarryLoad::ANY);
		if (qsf::isInitialized(carriedId))
			return false;

		return true;
	}

	bool PickupContaminatedPersonCommand::checkContext(const qsf::game::CommandContext& context)
	{
		// TODO(mh): make here a check if the person is reserved

		// Target entity needed
		if (nullptr == context.mTargetEntity)
			return false;

		// Check if the person is contaminated
		const HealthComponent* healthComponent = context.mTargetEntity->getComponent<HealthComponent>();
		if (nullptr == healthComponent)
			return false;

		// Only when the person is not death and contaminated
		if (healthComponent->getState() == HealthComponent::STATE_DEAD)
			return false;

		if (!healthComponent->isContaminated())
			return false;

		if (healthComponent->getContaminationType() == Injury::CONTAMINATION_PLAGUE)
			return false;

		if (EntityHelper(*context.mTargetEntity).isCarriedByOtherPerson())
			return false;

		return true;
	}

	void PickupContaminatedPersonCommand::execute(const qsf::game::CommandContext& context)
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

		// Clear plan before the actions are pushed
		actionComponent.clearPlan();

		actionComponent.pushAction<FireHoseCheckAction>(ACTION_PRIORITY).init(*context.mTargetEntity, true);

		// Find targetpoint
		uint32 targetPointId = LeadPersonTargetPointProvider::TARGET_POINT_ID;
		const HealthComponent* healthComponent = context.mTargetEntity->getComponent<HealthComponent>();
		if (nullptr != healthComponent)
		{
			const HealthComponent::State state = healthComponent->getState();
			if (state == HealthComponent::STATE_INJURED || state == HealthComponent::STATE_DEATHFIGHT)
			{
				targetPointId = PullPersonTargetPointProvider::TARGET_POINT_ID;
			}
		}

		actionComponent.pushAction<MoveAction>(ACTION_PRIORITY).init(new qsf::ai::ReachObjectGoal(*context.mCaller, *context.mTargetEntity, targetPointId));
		actionComponent.pushAction<qsf::TurnToAction>(ACTION_PRIORITY).init(context.mTargetEntity->getId());
		actionComponent.pushAction<PickupContaminatedPersonAction>(ACTION_PRIORITY).init(context.mTargetEntity->getId());
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

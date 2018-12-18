// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/command/police/UseStunGrenadeCommand.h"
#include "em5/action/police/UseStunGrenadeAction.h"
#include "em5/action/move/MoveAction.h"
#include "em5/action/move/TurnToAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/base/EquipmentAssets.h"
#include "em5/game/targetpoint/ThrowStunGrenadeTargetPointProvider.h"
#include "em5/map/EntityHelper.h"
#include "em5/specs/ActionRangeSpecsGroup.h"
#include "em5/EM5Helper.h"

#include <qsf_game/command/CommandContext.h>
#include <qsf_game/base/DistanceHelper.h>

#include <qsf_ai/navigation/goal/AchieveLineOfSightGoal.h>

#include <qsf/physics/collision/BulletCollisionComponent.h>
#include <qsf/logic/action/ActionComponent.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 UseStunGrenadeCommand::PLUGINABLE_ID = qsf::StringHash("em5::UseStunGrenadeCommand");


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	const uint32 UseStunGrenadeCommand::ACTION_PRIORITY = action::COMMAND_STD;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	UseStunGrenadeCommand::UseStunGrenadeCommand(qsf::game::CommandManager* commandManager) :
		Command(commandManager, PLUGINABLE_ID)
	{
		// Priority is at default value

		// Set name of the mouse cursor
		mIconSettings.mMouseIconName = "use_stun_grenade";
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::Command methods                   ]
	//[-------------------------------------------------------]
	bool UseStunGrenadeCommand::checkAvailable()
	{
		return true;
	}

	bool UseStunGrenadeCommand::checkCaller(qsf::Entity& caller)
	{
		// Caller has action with a too high priority?
		if (!checkCallerActionPriority(caller, ACTION_PRIORITY))
			return false;

		// Check if unit state is valid
		EntityHelper entityHelper(caller);
		if (!entityHelper.isSquadPersonInValidState())
			return false;

		// Check if equipment matches
		if (!entityHelper.checkIsActiveEquipment(equipment::POLICE_STUN_GRENADE))
			return false;

		return true;
	}

	bool UseStunGrenadeCommand::checkContext(const qsf::game::CommandContext& context)
	{
		// We need a target entity
		if (nullptr == context.mTargetEntity)
			return false;

		EntityHelper entityHelper(*context.mTargetEntity);

		// Gangsters are OK, but only when not injured
		if (entityHelper.isGangsterPerson() && !entityHelper.isPersonInjured())
			return true;

		// Gangster vehicles are OK
		if (entityHelper.isGangsterRoadVehicle() && (!isMoving(*context.mTargetEntity) || isInVehicleFollowRange(context)))
			return true;

		return false;
	}

	void UseStunGrenadeCommand::execute(const qsf::game::CommandContext& context)
	{
		// Access the caller's action plan
		qsf::ActionComponent& actionComponent = getActionComponent(*context.mCaller);

		// Check if the same command with the same target is already in the action plan, return in this case without clearing the action plan
		const UseStunGrenadeAction* useStunGrenadeAction = actionComponent.getAction<UseStunGrenadeAction>();
		if (nullptr != useStunGrenadeAction)
		{
			if (useStunGrenadeAction->getTargetEntityId() == context.mTargetEntity->getId())
			{
				// Don't push the same command twice
				return;
			}
		}

		// Clear plan before the actions are pushed
		actionComponent.clearPlan();

		// Check how far we are from the target
		if (!isInThrowRange(context))
		{
			// Move to the target if we're too far away for throwing (TODO(mz): maybe AI can handle such distances already?)

			// TODO(mz), AchieveLineOfSightGoal which does not work yet and might need a change in TargetPointProvider once it does
			short traceLineCollisionFlags = qsf::BulletCollisionComponent::mDefaultCollisionMask;
			actionComponent.pushAction<MoveAction>(ACTION_PRIORITY).init(new qsf::ai::AchieveLineOfSightGoal(*context.mCaller, *context.mTargetEntity, ThrowStunGrenadeTargetPointProvider::TARGET_POINT_ID, traceLineCollisionFlags));
		}

		actionComponent.pushAction<TurnToAction>(ACTION_PRIORITY).init(context.mTargetEntity->getId());
		actionComponent.pushAction<UseStunGrenadeAction>(ACTION_PRIORITY).init(context.mTargetEntity->getId());
	}


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	bool UseStunGrenadeCommand::isInThrowRange(const qsf::game::CommandContext& context) const
	{
		const float stunGrenadeThrowDistance = ActionRangeSpecsGroup::getInstanceSafe().getStunGrenadeThrowDistance();
		const float currentDistanceToTarget = qsf::game::DistanceHelper::get2dDistance(*context.mCaller, *context.mTargetEntity);
		return (currentDistanceToTarget < stunGrenadeThrowDistance);
	}

	bool UseStunGrenadeCommand::isInVehicleFollowRange(const qsf::game::CommandContext& context) const
	{
		// Check how far we are from the target
		const float stunGrenadeNoFollowDistance = ActionRangeSpecsGroup::getInstanceSafe().getStunGrenadeNoFollowDistance();
		const float currentDistanceToTarget = qsf::game::DistanceHelper::get2dDistance(*context.mCaller, *context.mTargetEntity);
		return (currentDistanceToTarget < stunGrenadeNoFollowDistance);
	}

	bool UseStunGrenadeCommand::isMoving(const qsf::Entity& entity) const
	{
		const qsf::MovableComponent* movableComponent = entity.getComponent<qsf::MovableComponent>();
		return (movableComponent == nullptr) ? false : (movableComponent->getSpeed() > 0.0f);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

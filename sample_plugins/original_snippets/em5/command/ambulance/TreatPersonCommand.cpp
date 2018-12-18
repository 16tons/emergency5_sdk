// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/command/ambulance/TreatPersonCommand.h"
#include "em5/action/ambulance/TreatPersonAction.h"
#include "em5/action/equipment/GetEquipmentAction.h"
#include "em5/action/base/EnterVehicleAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/action/move/MoveAction.h"
#include "em5/command/CommandContext.h"
#include "em5/game/targetpoint/TreatPersonTargetPointProvider.h"
#include "em5/health/HealthComponent.h"
#include "em5/health/HealthHelper.h"
#include "em5/logic/local/ReserveLogic.h"
#include "em5/map/CollisionHelper.h"
#include "em5/map/EntityHelper.h"

#include <qsf_game/component/base/HiddenComponent.h>

#include <qsf_ai/navigation/goal/ReachObjectGoal.h>
#include <qsf_ai/navigation/em4Router/RouterComponent.h>
#include <qsf_ai/navigation/em4Router/em2012/RouteFinder/RouteFinder.h>
#include <qsf_ai/navigation/em4Router/wrapper/EM3Singletons.h>
#include <qsf_ai/navigation/em4Router/wrapper/actor/ActorFactory.h>
#include <qsf_ai/navigation/em4Router/wrapper/actor/EGenericObject.h>

#include <qsf/component/link/LinkComponent.h>
#include <qsf/logic/action/ActionComponent.h>
#include <qsf/input/device/KeyboardDevice.h>
#include <qsf/input/InputSystem.h>
#include <qsf/QsfHelper.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 TreatPersonCommand::PLUGINABLE_ID = qsf::StringHash("em5::TreatPersonCommand");


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	const uint32 TreatPersonCommand::ACTION_PRIORITY = action::COMMAND_STD;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	TreatPersonCommand::TreatPersonCommand(qsf::game::CommandManager* commandManager) :
		Command(commandManager, PLUGINABLE_ID)
	{
		// Priority is at default value

		// Doesn't start a special command mode (triggered by right-click)
		// Set name of the mouse cursor
		mIconSettings.mMouseIconName = "treat_person";
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::Command methods                   ]
	//[-------------------------------------------------------]
	bool TreatPersonCommand::checkAvailable()
	{
		return true;
	}

	bool TreatPersonCommand::checkCaller(qsf::Entity& caller)
	{
		// Caller has action with a too high priority?
		if (!checkCallerActionPriority(caller, ACTION_PRIORITY))
		{
			if (isNoQueueingCommand(caller))
			{
				return false;
			}
		}

		// Check if unit state is valid
		if (!EntityHelper(caller).isSquadPersonInValidState())
			return false;

		if (EntityHelper(caller).isCarryingAnotherPerson())
			return false;

		return true;
	}

	bool TreatPersonCommand::checkContext(const qsf::game::CommandContext& context)
	{
		// Target entity needed
		qsf::Entity* targetEntity = context.mTargetEntity;
		if (nullptr == targetEntity)
			return false;

		const EntityHelper entityHelper(*targetEntity);

		// Is target person?
		if (!entityHelper.isEntityPerson())
			return false;

		// Is target outside?
		if (nullptr != entityHelper.getContainerEntity())
			return false;

		// Check if the target is already linked to some parent
		qsf::LinkComponent* linkComponent = context.mTargetEntity->getComponent<qsf::LinkComponent>();
		if (nullptr != linkComponent && linkComponent->hasParent())
			return false;

		// Is target visible?
		if (qsf::game::HiddenComponent::isHidden(*targetEntity))
			return false;

		// Is target buried?
		if (entityHelper.isPersonBuried())
			return false;

		if (!HealthHelper(*targetEntity).canTreatedByDoctor())
			return false;

		// Different handling for automatism
		const CommandContext& em5CommandContext = static_cast<const CommandContext&>(context);
		if (em5CommandContext.mIsAutomatism)
		{
			// Is target reserved
			if (!ReserveLogic::checkReservation(*targetEntity, context.mCaller->getId(), ReserveLogic::MOVETO))
				return false;

			// Don't lead unit automatically in dangerous situations
			HealthComponent& healthComponent = targetEntity->getOrCreateComponentSafe<HealthComponent>();
			if (healthComponent.isReceivingFireDamage())
				return false;
		}

		// Check if target is potential reachable
		CollisionHelper collisionHelper(targetEntity->getMap());
		if (collisionHelper.isInBlockedArea(*context.mCaller, *targetEntity))
			return false;

		if (collisionHelper.isInWater(*context.mTargetEntity))
			return false;

		return true;
	}

	void TreatPersonCommand::execute(const qsf::game::CommandContext& context)
	{
		// Access the caller's action plan
		qsf::ActionComponent& actionComponent = getActionComponent(*context.mCaller);

		// Check if the same command with the same target is already in the action plan, return in this case without clearing the action plan
		const TreatPersonAction* treatPersonAction = actionComponent.getAction<TreatPersonAction>();
		if (nullptr != treatPersonAction)
		{
			if (treatPersonAction->getTargetEntityId() == context.mTargetEntity->getId())
			{
				// Don't push the same command twice
				return;
			}
		}

		if (isNoQueueingCommand(*context.mCaller))
		{
			// Clear plan before the actions are pushed
			actionComponent.clearPlan();
		}

		actionComponent.pushAction<MoveAction>(ACTION_PRIORITY).init(new qsf::ai::ReachObjectGoal(*context.mCaller, *context.mTargetEntity, TreatPersonTargetPointProvider::TARGET_POINT_ID));
		actionComponent.pushAction<TreatPersonAction>(ACTION_PRIORITY).init(*context.mTargetEntity);
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	bool TreatPersonCommand::isNoQueueingCommand(qsf::Entity& doctorEntity)
	{
		qsf::ActionComponent& actionComponent = getActionComponent(doctorEntity);

		// Check the actions on his plan, but not entering vehicle
		if (nullptr != actionComponent.getAction<GetEquipmentAction>() && nullptr == actionComponent.getAction<EnterVehicleAction>())
		{
			return false;
		}

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

// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/command/base/eventspecific/UnloadBusCommand.h"
#include "em5/action/move/MoveAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/action/SignalAction.h"
#include "em5/command/base/ExitVehicleCommand.h"
#include "em5/command/component/CommandableComponent.h"
#include "em5/component/vehicle/VehicleComponent.h"
#include "em5/component/person/PersonComponent.h"
#include "em5/logic/local/ReserveLogic.h"
#include "em5/map/EntityHelper.h"

#include <qsf_game/command/CommandContext.h>
#include <qsf_game/component/event/EventTagHelper.h>

#include <qsf_ai/navigation/goal/ReachSinglePointGoal.h>

#include <qsf/base/ScratchBuffer.h>
#include <qsf/component/base/TransformComponent.h>
#include <qsf/component/move/MovableComponent.h>
#include <qsf/link/link/prototype/ContainerLink.h>
#include <qsf/logic/action/ActionComponent.h>
#include <qsf/map/Entity.h>
#include <qsf/map/query/ComponentMapQuery.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 UnloadBusCommand::PLUGINABLE_ID = qsf::StringHash("em5::UnloadBusCommand");

	const uint32 UnloadBusCommand::MESSAGE_READY_FOR_UNLOAD = qsf::StringHash("em5::UnloadBusCommand_ReadyForUnload");


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	const uint32 UnloadBusCommand::ACTION_PRIORITY = action::BLOCKING;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	UnloadBusCommand::UnloadBusCommand(qsf::game::CommandManager* commandManager) :
		Command(commandManager, PLUGINABLE_ID)
	{
		// Priority is at default value

		// Second lowest button priority (ReturnToBase is lower), so the button is always the second place at right (with hotkey 'r')
		mButtonPriority = -40;

		mIconSettings.mShowAsGuiButton = true;
		mIconSettings.mButtonIconPath = "exit_vehicle";
		// Set name of the mouse cursor
		mIconSettings.mMouseIconName = "exit_vehicle";
		mIconSettings.mTooltip = "EM5_Command_UnloadBus";

		// Set the help page name
		mHelpPageName = "TUTORIAL_HELPTEXT_EXIT";
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::Command methods                   ]
	//[-------------------------------------------------------]
	bool UnloadBusCommand::checkAvailable()
	{
		return true;
	}

	bool UnloadBusCommand::checkCaller(qsf::Entity& caller)
	{
		// Caller has action with a too high priority?
		if (!checkCallerActionPriorityNoBlocking(caller, ACTION_PRIORITY))
			return false;

		// Caller (= target) needs to have a vehicle component
		const VehicleComponent* vehicleComponent = caller.getComponent<VehicleComponent>();
		if (nullptr == vehicleComponent)
			return false;

		// Anyone inside?
		qsf::EntityVectorScratchBuffer entitiesToExit;
		ExitVehicleCommand::getEntitiesToExitUnfiltered(caller, entitiesToExit);
		if (entitiesToExit.empty())
			return false;

		if (!ReserveLogic::checkReservation(caller, caller.getId()))
			return false;

		// Done
		return true;
	}

	bool UnloadBusCommand::checkContext(const qsf::game::CommandContext& context)
	{
		// Allow self-executed command only
		if (!context.mAllowSelfExecution)
			return false;

		return true;
	}

	void UnloadBusCommand::execute(const qsf::game::CommandContext& context)
	{
		qsf::Entity& caller = *context.mCaller;
		const uint32 unitTypeId = caller.getComponentSafe<CommandableComponent>().getUnitType();

		// Search for the target position
		qsf::Entity* targetEntity = nullptr;
		{
			qsf::game::EventTagHelper eventTagHelper("em5::UnloadBusCommand");
			if (unitTypeId == CommandableComponent::UNITTAG_MISC_AMBULANCEBUS)
			{
				eventTagHelper.tryAcquireTaggedEntity(targetEntity, "AMBULANCEBUS_UNLOAD");
				QSF_CHECK(nullptr != targetEntity, "No target entity 'AMBULANCEBUS_UNLOAD' found", return);
			}
			else if (unitTypeId == CommandableComponent::UNITTAG_MISC_EVACBUS)
			{
				qsf::EntityVectorScratchBuffer targetCandidates;
				eventTagHelper.tryAcquireTaggedEntities(targetCandidates, "EVACBUS_UNLOAD");
				QSF_CHECK(!targetCandidates.empty(), "No target entity 'EVACBUS_UNLOAD' found", return);

				// Get index of this vehicle, by comparing to entity IDs of other evac busses (there should be 2 busses at most)
				//  -> Looks overly complicated, but this way no two busses use the same unload point
				size_t myIndex = 0;
				for (CommandableComponent* commandableComponent : qsf::ComponentMapQuery(caller.getMap()).getAllInstances<CommandableComponent>())
				{
					if (commandableComponent->getUnitType() == unitTypeId && commandableComponent->getEntityId() < caller.getId())
					{
						++myIndex;
					}
				}

				// Choose one of the targets
				if (myIndex >= targetCandidates.size())
				{
					myIndex = 0;
				}
				targetEntity = targetCandidates[myIndex];
			}
			else
			{
				// Other units not supported
				return;
			}
		}

		// Access the caller's action plan
		qsf::ActionComponent& actionComponent = getActionComponent(caller);
		actionComponent.clearPlan();

		// Move to target point
		const qsf::Transform& targetTransform = targetEntity->getComponentSafe<qsf::TransformComponent>().getTransform();

		qsf::logic::TargetPoint moveTargetConfig;
		moveTargetConfig.mPosition = targetTransform.getPosition();
		moveTargetConfig.mOrientation = targetTransform.getRotation();
		moveTargetConfig.mTolerance = 3.0f;
		actionComponent.pushAction<MoveAction>(action::COMMAND_STD).init(new qsf::ai::ReachSinglePointGoal(moveTargetConfig));

		// The rest is done by the event logic itself, we just signal that the target point is (possibly) reached now
		const qsf::MessageConfiguration messageConfiguration(MESSAGE_READY_FOR_UNLOAD, caller.getId(), targetEntity->getId(), unitTypeId);
		actionComponent.pushAction<SignalAction>(action::COMMAND_STD).init(messageConfiguration, qsf::MessageParameters(), false);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

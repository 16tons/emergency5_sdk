// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/command/tech/LoadBoatToTransporterCommand.h"
#include "em5/command/move/ReturnToBaseCommand.h"
#include "em5/command/CommandContext.h"
#include "em5/command/component/CommandableComponent.h"
#include "em5/action/ActionPriority.h"
#include "em5/action/move/MoveAction.h"
#include "em5/action/tech/UnloadBoatFromTransporterAction.h"
#include "em5/action/tech/LoadBoatToTransporterAction.h"
#include "em5/action/tech/PlacePersonFromBoatAction.h"
#include "em5/action/SignalAction.h"
#include "em5/action/WaitForSignalAction.h"
#include "em5/component/vehicle/BoatComponent.h"
#include "em5/component/objects/BoatDropPointComponent.h"
#include "em5/component/vehicle/RoadVehicleComponent.h"
#include "em5/game/targetpoint/UnloadBoatFromTransporterTargetPointProvider.h"
#include "em5/game/targetpoint/BoatDropTargetPointProvider.h"
#include "em5/plugin/Messages.h"
#include "em5/map/EntityHelper.h"

#include <qsf_ai/navigation/goal/ReachObjectGoal.h>

#include <qsf/map/Map.h>
#include <qsf/logic/action/ActionComponent.h>
#include <qsf/component/base/TransformComponent.h>
#include <qsf/map/query/ComponentMapQuery.h>
#include <qsf/logic/gamelogic/GameLogicComponent.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 LoadBoatToTransporterCommand::PLUGINABLE_ID = qsf::StringHash("em5::LoadBoatToTransporterCommand");


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	const uint32 LoadBoatToTransporterCommand::ACTION_PRIORITY = action::COMMAND_STD;
	const uint32 LoadBoatToTransporterCommand::ACTION_PRIORITY_AUTOMATISM = action::AUTOMATISM_STD;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	LoadBoatToTransporterCommand::LoadBoatToTransporterCommand(qsf::game::CommandManager* commandManager) :
		Command(commandManager, PLUGINABLE_ID)
	{
		// Priority is at default value

		// Set name of the mouse cursor
		mIconSettings.mMouseIconName = "load_boat_to_transporter";
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::Command methods                   ]
	//[-------------------------------------------------------]
	bool LoadBoatToTransporterCommand::checkAvailable()
	{
		return true;
	}

	bool LoadBoatToTransporterCommand::checkCaller(qsf::Entity& caller)
	{
		RoadVehicleComponent* roadVehicleComponent = caller.getComponent<RoadVehicleComponent>();

		if (nullptr == roadVehicleComponent)
			return false;

		// Check if we have no boat in special entity
		if (qsf::isInitialized(roadVehicleComponent->getSpecialEntity()))
			return false;

		// Caller has action with a too high priority?
		if (!checkCallerActionPriority(caller, ACTION_PRIORITY))
			return false;

		if (!EntityHelper(caller).isSquadRoadVehicle())
			return false;

		return true;
	}

	bool LoadBoatToTransporterCommand::checkContext(const qsf::game::CommandContext& context)
	{
		if (nullptr == context.mTargetEntity)
			return false;

		// Is the target a motorboat?
		if (!EntityHelper(*context.mTargetEntity).isBoat())
			return false;

		return true;
	}

	void LoadBoatToTransporterCommand::execute(const qsf::game::CommandContext& context)
	{
		qsf::Entity* dropPointEntity = searchDropPoint(*context.mCaller, *context.mTargetEntity);

		if (nullptr == dropPointEntity)
		{
			//TODO(tl): Check what we should do when we don't find a drop point
			return;
		}

		// Access the caller's action plan
		qsf::ActionComponent& transporterActionComponent = getActionComponent(*context.mCaller);
		qsf::ActionComponent& boatActionComponent = getActionComponent(*context.mTargetEntity);

		// Check if the same command with the same target is already in the action plan, return in this case without clearing the action plan
		const LoadBoatToTransporterAction* loadBoatToTransporterAction = transporterActionComponent.getAction<LoadBoatToTransporterAction>();
		if (nullptr != loadBoatToTransporterAction && loadBoatToTransporterAction->getTargetId() == context.mTargetEntity->getId())
		{
			// Don't push the same command twice
			return;
		}

		qsf::MessageConfiguration message(Messages::EM5_ACTION_SIGNAL, getId());

		transporterActionComponent.clearPlan();
		transporterActionComponent.pushAction<MoveAction>(ACTION_PRIORITY).init(new qsf::ai::ReachObjectGoal(*context.mCaller, *dropPointEntity, UnloadBoatFromTransporterTargetPointProvider::TARGET_POINT_ID));
		transporterActionComponent.pushAction<WaitForSignalAction>(ACTION_PRIORITY).init(message);
		transporterActionComponent.pushAction<LoadBoatToTransporterAction>(action::BLOCKING).init(*context.mTargetEntity);

		boatActionComponent.clearPlan();
		boatActionComponent.pushAction<MoveAction>(ACTION_PRIORITY).init(new qsf::ai::ReachObjectGoal(*context.mTargetEntity, *dropPointEntity, BoatDropTargetPointProvider::TARGET_POINT_ID));
		boatActionComponent.pushAction<PlacePersonFromBoatAction>(action::BLOCKING).init(*dropPointEntity);
		boatActionComponent.pushAction<SignalAction>(action::BLOCKING).init(message);
		// Boat will be blocked by LoadBoatToTransporterAction
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	qsf::Entity* LoadBoatToTransporterCommand::searchDropPoint(const qsf::Entity& transporter, const qsf::Entity& boat)
	{
		const BoatComponent* boatComponent = boat.getComponent<BoatComponent>();
		if (nullptr == boatComponent)
			return nullptr;

		const qsf::TransformComponent* transporterTansformComponent = transporter.getComponent<qsf::TransformComponent>();
		if (nullptr == transporterTansformComponent)
			return nullptr;

		// Get LakeId from boat and search only for dropPoints with the same LakeId
		uint64 lakeId = boatComponent->getLakeId();

		BoatDropPointComponent* bestDropPoint = nullptr;
		float bestSquaredDistance = 0.0f;

		for (BoatDropPointComponent* dropPoint : qsf::ComponentMapQuery(transporter.getMap()).getAllInstances<BoatDropPointComponent>())
		{
			if (dropPoint->getLakeId() == lakeId)
			{
				const qsf::TransformComponent* dropPointTransformComponent = dropPoint->getEntity().getComponent<qsf::TransformComponent>();
				if (nullptr != dropPointTransformComponent)
				{
					const float squaredDistance = glm::distance2(transporterTansformComponent->getPosition(), dropPointTransformComponent->getPosition());

					if (nullptr == bestDropPoint || squaredDistance < bestSquaredDistance)
					{
						bestDropPoint = dropPoint;
						bestSquaredDistance = squaredDistance;
					}
				}
			}
		}
		return (nullptr != bestDropPoint) ? &bestDropPoint->getEntity() : nullptr;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/command/police/BarrierTape/PlaceBarrierTapePole1Command.h"
#include "em5/action/ActionPriority.h"
#include "em5/base/GameplayAssets.h"
#include "em5/command/CommandContext.h"
#include "em5/component/objects/BarrierTapeComponent.h"
#include "em5/map/MapHelper.h"
#include "em5/specs/LogicSpecsGroup.h"

#include <qsf/component/base/TransformComponent.h>
#include <qsf/component/link/LinkComponent.h>
#include <qsf/component/move/MovableComponent.h>
#include <qsf/logic/action/ActionComponent.h>
#include <qsf/map/Map.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 PlaceBarrierTapePole1Command::PLUGINABLE_ID = qsf::StringHash("em5::PlaceBarrierTapePole1Command");


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	const uint32 PlaceBarrierTapePole1Command::ACTION_PRIORITY = action::COMMAND_STD;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	PlaceBarrierTapePole1Command::PlaceBarrierTapePole1Command(qsf::game::CommandManager* commandManager) :
		Command(commandManager, PLUGINABLE_ID)
	{
		// No mouse cursor, no button - command is pushed implicitly by the command mode
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::Command methods                   ]
	//[-------------------------------------------------------]
	bool PlaceBarrierTapePole1Command::checkAvailable()
	{
		// This command will only be pushed explicitly and should never be chosen by some automatism
		return false;
	}

	bool PlaceBarrierTapePole1Command::checkCaller(qsf::Entity& caller)
	{
		// This command will only be pushed explicitly and should thus always have a valid caller
		return true;
	}

	bool PlaceBarrierTapePole1Command::checkContext(const qsf::game::CommandContext& context)
	{
		// This command will only be pushed explicitly and should thus always have a valid context
		return true;
	}

	void PlaceBarrierTapePole1Command::execute(const qsf::game::CommandContext& context)
	{
		// This command is only meant to store the position for the first pole into the caller on the host, so the PlaceBarrierTapePole2Command can then access this information needed to push the actual actions
		// For this to work we have to repeat, what already happened on the client:

		// Sanity check
		QSF_CHECK(nullptr != context.mCaller, "Place barrier tape pole 1 command was called without a caller", QSF_REACT_THROW);

		// TODO(ca) Using entities as temporary datastore is both, expensive and errorprone - we should try to avoid this whenever possible
		// Create an empty local temporary entity
		qsf::Entity* barrierTapeEntity = MapHelper(context.mCaller->getMap()).createObjectByLocalPrefabAssetId(assets::PREFAB_BARRIER_TAPE_PLACEMENT);
		QSF_CHECK(nullptr != barrierTapeEntity, "Failed to create barrier tape entity", QSF_REACT_THROW);

		const qsf::TransformComponent& callerTransformComponent = context.mCaller->getComponentSafe<qsf::TransformComponent>();
		barrierTapeEntity->getOrCreateComponentSafe<qsf::MovableComponent>().warpToPositionAndRotation(callerTransformComponent.getPosition(), callerTransformComponent.getRotation());

		// Link the temporary entity to the caller entity (the policeman), so it won't get lost or leaked
		qsf::LinkComponent& linkComponent = barrierTapeEntity->getOrCreateComponentSafe<qsf::LinkComponent>();
		linkComponent.linkToParent(*context.mCaller, qsf::LinkComponent::HARD_LINK);

		const float transparency = LogicSpecsGroup::getInstanceSafe().getPoliceBarrierTransparency();

		// Initialize barrier tape component
		BarrierTapeComponent& barrierTapeComponent = barrierTapeEntity->getOrCreateComponentSafe<BarrierTapeComponent>();
		barrierTapeComponent.setFinishedBuilding(false);
		barrierTapeComponent.createPole(0);
		barrierTapeComponent.setPoleTransparency(0, transparency);
		barrierTapeComponent.movePole(0, context.getTargetPosition());
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

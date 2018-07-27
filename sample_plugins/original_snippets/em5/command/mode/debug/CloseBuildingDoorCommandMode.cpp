// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/command/mode/debug/CloseBuildingDoorCommandMode.h"
#include "em5/command/CommandContext.h"
#include "em5/map/EntityHelper.h"
#include "em5/component/building/BuildingComponent.h"
#include "em5/component/vehicle/VehicleComponent.h"

#include <qsf/debug/request/TextDebugDrawRequest.h>
#include <qsf/map/Entity.h>
#include <qsf/QsfHelper.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 CloseBuildingDoorCommandMode::PLUGINABLE_ID = qsf::StringHash("em5::CloseBuildingDoorCommandMode");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	CloseBuildingDoorCommandMode::CloseBuildingDoorCommandMode() :
		DebugCommandMode(PLUGINABLE_ID)
	{
		// Nothing here
	}

	CloseBuildingDoorCommandMode::~CloseBuildingDoorCommandMode()
	{
		// Nothing here
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::game::CommandMode methods         ]
	//[-------------------------------------------------------]
	void CloseBuildingDoorCommandMode::startup()
	{
		// Nothing here
	}

	void CloseBuildingDoorCommandMode::shutdown()
	{
		mDebugDrawProxy.unregister();
	}

	bool CloseBuildingDoorCommandMode::executeCommandContext(qsf::game::CommandContext& commandContext, const EntityIdSet& callerIds)
	{
		// Is there a target entity selected at all?
		if (nullptr != commandContext.mTargetEntity)
		{
			qsf::Entity* targetEntity = commandContext.mTargetEntity;
			const EntityHelper entityHelper(*targetEntity);

			BuildingComponent* buildingComponent = targetEntity->getComponent<BuildingComponent>();
			if (nullptr != buildingComponent)
			{
				buildingComponent->setIsDoorBlocked(!buildingComponent->getIsDoorBlocked());
			}
		}

		// Always return false to keep the command mode alive
		return false;
	}

	void CloseBuildingDoorCommandMode::updateCommandMode(const qsf::Clock& clock)
	{
		// Debug output at the player's mouse cursor
		mDebugDrawProxy.registerAt(QSF_DEBUGDRAW);
		mDebugDrawProxy.addRequest(qsf::TextDebugDrawRequest("Close Door", getMousePosition() + glm::vec2(16, 8)));
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

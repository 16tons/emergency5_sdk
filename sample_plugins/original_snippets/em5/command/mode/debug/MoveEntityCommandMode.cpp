// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/command/mode/debug/MoveEntityCommandMode.h"
#include "em5/command/CommandContext.h"

#include <qsf/map/Map.h>
#include <qsf/map/Entity.h>
#include <qsf/component/base/TransformComponent.h>
#include <qsf/component/move/MovableComponent.h>
#include <qsf/debug/request/TextDebugDrawRequest.h>
#include <qsf/QsfHelper.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 MoveEntityCommandMode::PLUGINABLE_ID = qsf::StringHash("em5::MoveEntityCommandMode");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	MoveEntityCommandMode::MoveEntityCommandMode() :
		DebugCommandMode(PLUGINABLE_ID)
	{
		// Nothing here
	}

	MoveEntityCommandMode::~MoveEntityCommandMode()
	{
		// Nothing here
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::game::CommandMode methods         ]
	//[-------------------------------------------------------]
	void MoveEntityCommandMode::startup()
	{
		// Nothing here
	}

	void MoveEntityCommandMode::shutdown()
	{
		mDebugDrawProxy.unregister();
	}

	bool MoveEntityCommandMode::executeCommandContext(qsf::game::CommandContext& commandContext, const EntityIdSet& callerIds)
	{
		if (callerIds.empty())
			return false;

		// Is there a target position?
		if (commandContext.mTargetPosition.is_initialized())
		{
			qsf::Entity* entity = QSF_MAINMAP.getEntityById(*callerIds.begin());

			qsf::MovableComponent* movableComponent = entity->getComponent<qsf::MovableComponent>();
			if (nullptr != movableComponent)
			{
				movableComponent->warpToPositionAndRotation(*commandContext.mTargetPosition, movableComponent->getRotation());
			}
			else
			{
				qsf::TransformComponent* transformComponent = entity->getComponent<qsf::TransformComponent>();
				if (nullptr != transformComponent)
				{
					transformComponent->setPosition(*commandContext.mTargetPosition);
				}
			}
		}

		// Always return false to keep the command mode alive
		return false;
	}

	void MoveEntityCommandMode::updateCommandMode(const qsf::Clock& clock)
	{
		// Debug output at the player's mouse cursor
		mDebugDrawProxy.registerAt(QSF_DEBUGDRAW);
		mDebugDrawProxy.addRequest(qsf::TextDebugDrawRequest("Move", getMousePosition() + glm::vec2(16, 8)));
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

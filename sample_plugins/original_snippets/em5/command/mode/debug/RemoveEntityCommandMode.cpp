// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/command/mode/debug/RemoveEntityCommandMode.h"
#include "em5/command/CommandContext.h"
#include "em5/map/MapHelper.h"

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
	const uint32 RemoveEntityCommandMode::PLUGINABLE_ID = qsf::StringHash("em5::RemoveEntityCommandMode");


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	bool RemoveEntityCommandMode::execute(qsf::game::CommandContext& commandContext)
	{
		// Is there a target entity selected at all?
		if (nullptr != commandContext.mTargetEntity)
		{
			MapHelper(commandContext.mTargetEntity->getMap()).destroyEntity(*commandContext.mTargetEntity);
		}

		// Always return false to keep the command mode alive
		return false;
	}


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	RemoveEntityCommandMode::RemoveEntityCommandMode() :
		DebugCommandMode(PLUGINABLE_ID)
	{
		// Nothing here
	}

	RemoveEntityCommandMode::~RemoveEntityCommandMode()
	{
		// Nothing here
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::game::CommandMode methods         ]
	//[-------------------------------------------------------]
	void RemoveEntityCommandMode::startup()
	{
		// Nothing here
	}

	void RemoveEntityCommandMode::shutdown()
	{
		mDebugDrawProxy.unregister();
	}

	bool RemoveEntityCommandMode::executeCommandContext(qsf::game::CommandContext& commandContext, const EntityIdSet&)
	{
		return execute(commandContext);
	}

	void RemoveEntityCommandMode::updateCommandMode(const qsf::Clock& clock)
	{
		// Debug output at the player's mouse cursor
		mDebugDrawProxy.registerAt(QSF_DEBUGDRAW);
		mDebugDrawProxy.addRequest(qsf::TextDebugDrawRequest("Remove", getMousePosition() + glm::vec2(16, 8)));
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

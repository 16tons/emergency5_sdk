// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/command/mode/debug/StopBurningCommandMode.h"
#include "em5/command/CommandContext.h"
#include "em5/map/EntityHelper.h"
#include "em5/fire/component/FireComponent.h"
#include "em5/fire/component/ComplexFireComponent.h"

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
	const uint32 StopBurningCommandMode::PLUGINABLE_ID = qsf::StringHash("em5::StopBurningCommandMode");


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	bool StopBurningCommandMode::execute(qsf::game::CommandContext& commandContext)
	{
		// Is there a target entity selected at all?
		if (nullptr != commandContext.mTargetEntity)
		{
			EntityHelper(*commandContext.mTargetEntity).cleanupFire();
		}

		// Always return false to keep the command mode alive
		return false;
	}


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	StopBurningCommandMode::StopBurningCommandMode() :
		DebugCommandMode(PLUGINABLE_ID)
	{
		// Nothing here
	}

	StopBurningCommandMode::~StopBurningCommandMode()
	{
		// Nothing here
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::game::CommandMode methods         ]
	//[-------------------------------------------------------]
	void StopBurningCommandMode::startup()
	{
		// Nothing here
	}

	void StopBurningCommandMode::shutdown()
	{
		mDebugDrawProxy.unregister();
	}

	bool StopBurningCommandMode::executeCommandContext(qsf::game::CommandContext& commandContext, const EntityIdSet&)
	{
		return execute(commandContext);
	}

	void StopBurningCommandMode::updateCommandMode(const qsf::Clock& clock)
	{
		// Debug output at the player's mouse cursor
		mDebugDrawProxy.registerAt(QSF_DEBUGDRAW);
		mDebugDrawProxy.addRequest(qsf::TextDebugDrawRequest("Stop Burning", getMousePosition() + glm::vec2(16, 8)));
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

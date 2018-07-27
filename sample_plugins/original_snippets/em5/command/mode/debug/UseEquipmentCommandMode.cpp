// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/command/mode/debug/UseEquipmentCommandMode.h"
#include "em5/action/equipment/UseEquipmentAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/command/CommandContext.h"
#include "em5/map/EntityHelper.h"

#include <qsf/logic/action/ActionComponent.h>
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
	const uint32 UseEquipmentCommandMode::PLUGINABLE_ID = qsf::StringHash("em5::UseEquipmentCommandMode");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	UseEquipmentCommandMode::UseEquipmentCommandMode() :
		DebugCommandMode(PLUGINABLE_ID)
	{
		// Nothing here
	}

	UseEquipmentCommandMode::~UseEquipmentCommandMode()
	{
		// Nothing here
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::game::CommandMode methods         ]
	//[-------------------------------------------------------]
	void UseEquipmentCommandMode::startup()
	{
		// Nothing here
	}

	void UseEquipmentCommandMode::shutdown()
	{
		mDebugDrawProxy.unregister();
	}

	bool UseEquipmentCommandMode::executeCommandContext(qsf::game::CommandContext& commandContext, const EntityIdSet& callerIds)
	{
		// Is there a target entity selected at all?
		if (nullptr != commandContext.mTargetEntity)
		{
			qsf::Entity* targetEntity = commandContext.mTargetEntity;
			const EntityHelper entityHelper(*targetEntity);

			qsf::ActionComponent* actionComponent = targetEntity->getComponent<qsf::ActionComponent>();
			if (nullptr != actionComponent)
			{
				actionComponent->clearPlan();
				actionComponent->pushAction<UseEquipmentAction>(action::COMMAND_LOW).init(UseEquipmentAction::EQUIPMENT_MOBILE);
			}

		}

		// Always return false to keep the command mode alive
		return false;
	}

	void UseEquipmentCommandMode::updateCommandMode(const qsf::Clock& clock)
	{
		// Debug output at the player's mouse cursor
		mDebugDrawProxy.registerAt(QSF_DEBUGDRAW);
		mDebugDrawProxy.addRequest(qsf::TextDebugDrawRequest("Use Equipment", getMousePosition()+ glm::vec2(16, 8)));
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

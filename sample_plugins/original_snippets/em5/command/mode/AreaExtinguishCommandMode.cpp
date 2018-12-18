// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/command/mode/AreaExtinguishCommandMode.h"
#include "em5/command/fire/ExtinguishTargetAreaCommand.h"
#include "em5/command/CommandContext.h"
#include "em5/action/ActionPriority.h"
#include "em5/application/Application.h"
#include "em5/component/effect/HighlightComponent.h"
#include "em5/gui/EmergencyGui.h"
#include "em5/game/groundmap/GroundMaps.h"
#include "em5/specs/FireSpecsGroup.h"
#include "em5/EM5Helper.h"

#include <qsf_game/command/evaluation/CommandEvaluationHelper.h>
#include <qsf_game/command/CommandSystem.h>
#include <qsf_game/QsfGameHelper.h>

#include <qsf/component/base/TransformComponent.h>
#include <qsf/debug/request/TextDebugDrawRequest.h>
#include <qsf/map/query/RayMapQuery.h>
#include <qsf/map/query/GroundMapQuery.h>
#include <qsf/map/Map.h>
#include <qsf/map/Entity.h>
#include <qsf/gui/GuiContext.h>
#include <qsf/QsfHelper.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 AreaExtinguishCommandMode::PLUGINABLE_ID = qsf::StringHash("em5::AreaExtinguishCommandMode");


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	const uint32 AreaExtinguishCommandMode::ACTION_PRIORITY = action::COMMAND_STD;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	AreaExtinguishCommandMode::AreaExtinguishCommandMode() :
		qsf::game::CommandMode(PLUGINABLE_ID),
		mCommandoCar(nullptr),
		mMap(nullptr)
	{
		// Nothing here
	}

	AreaExtinguishCommandMode::~AreaExtinguishCommandMode()
	{
		// Nothing here
	}

	void AreaExtinguishCommandMode::initialize(qsf::Entity* callerEntity)
	{
		if (nullptr != callerEntity)
		{
			mCommandoCar = callerEntity;
			mMap = &callerEntity->getMap();
		}
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::game::CommandMode methods         ]
	//[-------------------------------------------------------]
	void AreaExtinguishCommandMode::startup()
	{
		// Create new entity for highlighting
		mHighlightEntity = mMap->createEntity();
		HighlightComponent& highlightComponent = mHighlightEntity->getOrCreateComponentSafe<HighlightComponent>();

		const float extinguishRadius = FireSpecsGroup::getInstanceSafe().getRadiusCommandoCarExtinguish();
		highlightComponent.setGroundHeight(extinguishRadius * 1.5f);
		highlightComponent.setGroundSpotLightRadius(extinguishRadius);
		highlightComponent.setSpotLightNearClipDistance(extinguishRadius * 0.85f);
	}

	void AreaExtinguishCommandMode::shutdown()
	{
		if (mHighlightEntity.valid())
		{
			mMap->destroyEntityById(mHighlightEntity->getId());
			mHighlightEntity = nullptr;
		}

		mDebugDrawProxy.unregister();
	}

	bool AreaExtinguishCommandMode::executeCommandContext(qsf::game::CommandContext& commandContext, const EntityIdSet& callerIds)
	{
		QSF_ASSERT(nullptr != mMap, "em5::AreaExtinguishCommandMode: Map pointer is not initialized", QSF_REACT_NONE);

		if (!commandContext.hasTargetPosition())
		{
			return false;
		}

		// The command mode will be executed on client side in multiplayer mode, so we need to push the actual actions within another command object to the host for things to work as expected
		ExtinguishTargetAreaCommand& commandObject = QSFGAME_COMMAND.getCommandManager().getSafe<ExtinguishTargetAreaCommand>();
		const qsf::game::CommandEvaluationHelper::EntityArray callers = { mCommandoCar };
		commandContext.mTargetPosition = getPositionUnderMouse();
		// TODO(ca) Review: Is this the correct way to push a command into the usual command processing?
		qsf::game::CommandEvaluationHelper::executeCommand(commandObject, callers, commandContext);

		// End command mode
		return true;
	}

	void AreaExtinguishCommandMode::updateCommandMode(const qsf::Clock& clock)
	{
		if (mHighlightEntity.valid())
		{
			qsf::TransformComponent& transformComponent = mHighlightEntity->getOrCreateComponentSafe<qsf::TransformComponent>();
			transformComponent.setPosition(getPositionUnderMouse());
		}

#ifndef ENDUSER
		// Debug output at the player's mouse cursor
		mDebugDrawProxy.registerAt(QSF_DEBUGDRAW);
		mDebugDrawProxy.addRequest(qsf::TextDebugDrawRequest("AreaExtinguishCommandMode ", EM5_GUI.getGuiContext().getMouseCursorPosition() + glm::vec2(16, 8)));
#endif
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	glm::vec3 AreaExtinguishCommandMode::getPositionUnderMouse()
	{
		const glm::vec2 normalizedMousePosition = EM5_GUI.getGuiContext().getNormalizedMouseCursorPosition();

		// Check the first hit in the world...
		qsf::RayMapQueryResponse rayMapQueryResponse(qsf::RayMapQueryResponse::POSITION_RESPONSE);
		qsf::RayMapQuery(*mMap).getFirstHitByRenderWindowNormalizedPosition(*EM5_APPLICATION.getRenderWindow(), normalizedMousePosition.x, normalizedMousePosition.y, rayMapQueryResponse);

		// and then take the ground position (to avoid jumping all the time)
		float groundHeight = 0.0f;
		qsf::GroundMapQuery(*mMap, GroundMaps::FILTER_DEFAULT).getHeightAt(rayMapQueryResponse.position, groundHeight);

		rayMapQueryResponse.position.y = groundHeight;
		return rayMapQueryResponse.position;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

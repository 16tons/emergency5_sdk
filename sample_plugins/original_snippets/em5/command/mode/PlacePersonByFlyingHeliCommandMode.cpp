// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/command/mode/PlacePersonByFlyingHeliCommandMode.h"
#include "em5/action/ActionPriority.h"
#include "em5/action/move/MoveAction.h"
#include "em5/action/tech/PlacePersonByFlyingHeliAction.h"
#include "em5/ai/MovementModes.h"
#include "em5/application/Application.h"
#include "em5/audio/AudioFeedbackHelper.h"
#include "em5/command/CommandContext.h"
#include "em5/command/mode/DefaultCommandMode.h"
#include "em5/command/tech/PerformPlacePersonByFlyingHeliCommand.h"
#include "em5/component/vehicle/HelicopterComponent.h"
#include "em5/component/vehicle/parts/HelicopterHatchComponent.h"
#include "em5/gui/EmergencyGui.h"
#include "em5/map/CollisionHelper.h"
#include "em5/settings/GameSettingsGroup.h"
#include "em5/EM5Helper.h"

#include <qsf_ai/navigation/goal/ReachSinglePointGoal.h>

#include <qsf/component/base/TransformComponent.h>
#include <qsf/debug/request/TextDebugDrawRequest.h>
#include <qsf/logic/action/ActionComponent.h>
#include <qsf/map/query/GroundMapQuery.h>
#include <qsf/map/query/RayMapQuery.h>
#include <qsf/map/Map.h>
#include <qsf/map/Entity.h>
#include <qsf/math/Ray.h>
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
	const uint32 PlacePersonByFlyingHeliCommandMode::PLUGINABLE_ID = qsf::StringHash("em5::PlacePersonByFlyingHeliCommandMode");


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	const uint32 PlacePersonByFlyingHeliCommandMode::ACTION_PRIORITY = action::COMMAND_STD;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	PlacePersonByFlyingHeliCommandMode::PlacePersonByFlyingHeliCommandMode() :
		qsf::game::CommandMode(PLUGINABLE_ID),
		mCaller(nullptr),
		mMap(nullptr)
	{
		// Nothing here
	}

	PlacePersonByFlyingHeliCommandMode::~PlacePersonByFlyingHeliCommandMode()
	{
		// Nothing here
	}

	void PlacePersonByFlyingHeliCommandMode::initialize(qsf::Entity& caller)
	{
		mCaller = &caller;
		mMap = &caller.getMap();
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::game::CommandMode methods         ]
	//[-------------------------------------------------------]
	void PlacePersonByFlyingHeliCommandMode::startup()
	{
		// Nothing here
	}

	void PlacePersonByFlyingHeliCommandMode::shutdown()
	{
		mDebugDrawProxy.unregister();
	}

	bool PlacePersonByFlyingHeliCommandMode::executeCommandContext(qsf::game::CommandContext& commandContext, const EntityIdSet& callerIds)
	{
		QSF_ASSERT(nullptr != mMap, "em5::PlacePersonByFlyingHeliCommandMode: Map pointer is not initialized", QSF_REACT_NONE);

		if (!commandContext.hasTargetPosition())
			return false;

		const glm::vec3& targetPosition = commandContext.getTargetPosition();
		bool canPushAction = true;

		for (uint64 entityId : callerIds)
		{
			qsf::Entity* selectedEntity = mMap->getEntityById(entityId);
			if (nullptr == selectedEntity)
				continue;

			// TODO(mk) Here is a BUG in the multiple selection. We have to check the position for landing for every callers and push different move positions. Use a kind of "MoveCommand::computeMultiSelectionTargetPositionForOrdering()"
			// Check for collisions
			if (hasCollisionOnTargetPosition(selectedEntity, targetPosition))
				canPushAction = false;

			if (canPushAction)
			{
				canPushAction = false;

				PerformPlacePersonByFlyingHeliCommand::pushCommandToEntity(*selectedEntity, commandContext);
			}
			else
			{
				// In case of multi selection, keep modus active for the next entity
				return false;
			}
		}

		// End command mode
		return true;
	}

	void PlacePersonByFlyingHeliCommandMode::updateCommandMode(const qsf::Clock& clock)
	{
		EmergencyGui& emergencyGui = EM5_APPLICATION.getGui();

		// Do nothing, when the mouse cursor is over an HUD element
		if (GameSettingsGroup::getInstanceSafe().getMouseOverHudElement())
		{
			emergencyGui.resetMouseCursor();
			mDebugDrawProxy.unregister();
			return;
		}

		// Position 3D
		qsf::RayMapQueryResponse rayMapQueryResponse(qsf::RayMapQueryResponse::POSITION_RESPONSE);
		glm::vec2 normalizedMousePosition = EM5_GUI.getGuiContext().getNormalizedMouseCursorPosition();
		boost::optional<qsf::Ray> ray = qsf::RayMapQuery::getRayByRenderWindowNormalizedPosition(*EM5_APPLICATION.getRenderWindow(), normalizedMousePosition.x, normalizedMousePosition.y);
		if (ray.is_initialized())
		{
			qsf::RayMapQuery::QueryParameters parameters;
			parameters.mQueryIncludeFlags = qsf::RayMapQuery::INCLUDE_TERRAIN;

			qsf::RayMapQuery(QSF_MAINMAP).getFirstHitAlongRay(*ray, rayMapQueryResponse, parameters);
			if (nullptr != rayMapQueryResponse.component)
			{
				// Correct height
				qsf::GroundMapQuery(QSF_MAINMAP).getHeightAtXZPosition(rayMapQueryResponse.position.x, rayMapQueryResponse.position.z, rayMapQueryResponse.position.y);
			}
		}

		// Check is position under mouse is collision free
		if (hasCollisionOnTargetPosition(mCaller, rayMapQueryResponse.position))
		{
			emergencyGui.setMouseCursor("place_person_by_flying_heli_no");
		}
		else
		{
			emergencyGui.setMouseCursor("place_person_by_flying_heli");
		}

#ifndef RETAIL
		// Debug output at the player's mouse cursor
		mDebugDrawProxy.registerAt(QSF_DEBUGDRAW);
		mDebugDrawProxy.addRequest(qsf::TextDebugDrawRequest("PlacePersonByFlyingHelicopter ", EM5_GUI.getGuiContext().getMouseCursorPosition() + glm::vec2(16, 8)));
#endif
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	bool PlacePersonByFlyingHeliCommandMode::hasCollisionOnTargetPosition(const qsf::Entity* entity, const glm::vec3& position)
	{
		if (nullptr != entity)
		{
			CollisionHelper collisionHelper(*mMap);

			if (collisionHelper.isInWater(position, false, 0.01f))
				return true;

			HelicopterComponent* helicopterComponent = entity->getComponent<HelicopterComponent>();
			if (helicopterComponent)
			{
				qsf::Entity* specialEntity = mMap->getEntityById(helicopterComponent->getSpecialEntity());
				if (nullptr != specialEntity)
				{
					HelicopterHatchComponent* helicopterHatchComponent = specialEntity->getComponent<HelicopterHatchComponent>();
					if (helicopterHatchComponent)
					{
						qsf::Entity* targetEntity = helicopterHatchComponent->getTargetFromStretcher();
						if (nullptr != targetEntity)
						{
							if (collisionHelper.hasCollisionEM3(*targetEntity, position, 1.2f)) // Safety should be bigger as in the place person action
								return true;
						}

						// Double check, as target entity might not have a collision at all
						if (collisionHelper.hasCollisionEM3(glm::vec3(1.2f, 1.2f, 1.2f), qsf::Transform(position)))
							return true;
					}
				}
			}
		}

		return false;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

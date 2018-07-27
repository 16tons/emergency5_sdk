// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/command/mode/debug/CollisionSensorCommandMode.h"
#include "em5/application/Application.h"
#include "em5/gui/EmergencyGui.h"
#include "em5/map/MapHelper.h"
#include "em5/EM5Helper.h"

#include <qsf_ai/navigation/em4Router/em2012/RouteFinder/RouteFinder.h>

#include <qsf/component/base/TransformComponent.h>
#include <qsf/physics/debug/DebugBulletBoxCollisionSensorComponent.h>
#include <qsf/map/query/GroundMapQuery.h>
#include <qsf/map/query/RayMapQuery.h>
#include <qsf/math/Ray.h>
#include <qsf/gui/GuiContext.h>
#include <qsf/debug/request/TextDebugDrawRequest.h>
#include <qsf/QsfHelper.h>

#include <boost/format.hpp>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 CollisionSensorCommandMode::PLUGINABLE_ID = qsf::StringHash("em5::CollisionSensorCommandMode");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	CollisionSensorCommandMode::CollisionSensorCommandMode() :
		DebugCommandMode(PLUGINABLE_ID)
	{
		// Nothing here
	}

	CollisionSensorCommandMode::~CollisionSensorCommandMode()
	{
		// Nothing here
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::game::CommandMode methods         ]
	//[-------------------------------------------------------]
	void CollisionSensorCommandMode::startup()
	{
		// Nothing here
	}

	void CollisionSensorCommandMode::shutdown()
	{
		if (mSensorEntity.valid())
		{
			QSF_MAINMAP.destroyEntityById(mSensorEntity->getId());
		}
	}

	bool CollisionSensorCommandMode::executeCommandContext(qsf::game::CommandContext& commandContext, const EntityIdSet& callerIds)
	{
		// Always return false to keep the command mode alive
		return false;
	}

	void CollisionSensorCommandMode::updateCommandMode(const qsf::Clock& clock)
	{
		mDebugDrawProxy.unregister();

		// Where to place the collision sensor?
		const glm::vec2 normalizedMousePosition = getNormalizedMousePosition();
		boost::optional<qsf::Ray> ray = qsf::RayMapQuery::getRayByRenderWindowNormalizedPosition(*EM5_APPLICATION.getRenderWindow(), normalizedMousePosition.x, normalizedMousePosition.y);
		if (ray.is_initialized())
		{
			qsf::RayMapQuery::QueryParameters parameters;
			parameters.mQueryIncludeFlags = qsf::RayMapQuery::INCLUDE_TERRAIN;

			qsf::RayMapQueryResponse rayMapQueryResponse(qsf::RayMapQueryResponse::POSITION_RESPONSE);
			qsf::Map& map = QSF_MAINMAP;
			qsf::RayMapQuery(map).getFirstHitAlongRay(*ray, rayMapQueryResponse, parameters);
			if (nullptr != rayMapQueryResponse.component)
			{
				// Collect the ray position
				glm::vec3& position = rayMapQueryResponse.position;
				std::string text = (boost::format("position = %.2f, %.2f, %.2f") % position.x % position.y % position.z).str();

				// Correct height
				qsf::GroundMapQuery(map).getHeightAtXZPosition(position.x, position.z, position.y);

				if (!mSensorEntity.valid())
				{
					// Create a new visualization entity
					qsf::Entity* entity = map.createEntity();
					entity->getOrCreateComponentSafe<qsf::TransformComponent>().setScale(glm::vec3(2.0f, 2.0f, 2.0f));
					entity->createComponent<qsf::DebugBulletBoxCollisionSensorComponent>();

					mSensorEntity = entity;
				}

				{
					// Print out mouse position
					if (qsf::ai::ERouteFinder::Instance()->IsInWater(position))
					{
						text += " (water)";
					}

					mDebugDrawProxy.registerAt(QSF_DEBUGDRAW);
					mDebugDrawProxy.addRequest(qsf::TextDebugDrawRequest(text, EM5_GUI.getGuiContext().getMouseCursorPosition() + glm::vec2(16, 8)));
				}

				// Update position
				mSensorEntity->getComponentSafe<qsf::TransformComponent>().setPosition(position);
			}
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

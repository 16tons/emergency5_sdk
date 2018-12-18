// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/component/vehicle/RoadVehiclePlacementComponent.h"
#include "em5/component/vehicle/RoadVehicleComponent.h"

#include <qsf/map/Map.h>
#include <qsf/map/Entity.h>
#include <qsf/component/base/TransformComponent.h>
#include <qsf/component/link/LinkComponent.h>
#include <qsf/math/Math.h>
#include <qsf/math/Plane.h>
#include <qsf/debug/DebugDrawProxy.h>
#include <qsf/debug/request/TriangleDebugDrawRequest.h>
#include <qsf/QsfHelper.h>

#include <glm/gtx/norm.hpp>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 RoadVehiclePlacementComponent::COMPONENT_ID = qsf::StringHash("em5::RoadVehiclePlacementComponent");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	RoadVehiclePlacementComponent::RoadVehiclePlacementComponent(qsf::Prototype* prototype) :
		qsf::PlacementComponent(prototype),
		mLocalWheelPositionInitialized(false)
	{
		// Nothing to do in here
	}

	RoadVehiclePlacementComponent::~RoadVehiclePlacementComponent()
	{
		// Nothing to do in here
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::PlacementComponent methods        ]
	//[-------------------------------------------------------]
	bool RoadVehiclePlacementComponent::applyPlacement(glm::vec3& position, glm::quat& rotation)
	{
		if (!mLocalWheelPositionInitialized)
		{
			if (!initialize())
				return false;

			// Initialization done
			mLocalWheelPositionInitialized = true;
		}

		// Get wheel world positions
		glm::vec3 wheelWorldPosition[3];
		float maxWheelY = -1e10f;

		for (uint32 i = 0; i < 3; ++i)
		{
			// Convert from local to world space
			wheelWorldPosition[i] = rotation * mLocalWheelPosition[i] + position;

			// Set the position to the ground
			dropToGround(wheelWorldPosition[i]);

			maxWheelY = std::max(maxWheelY, wheelWorldPosition[i].y);
		}

		// Check for flat placement
		{
			float maxHeight = 0.0f;
			for (uint32 i = 0; i < 3; ++i)
			{
				maxHeight += glm::distance2(mLocalWheelPosition[i], mLocalWheelPosition[(i+1)%3]);
			}
			maxHeight = sqrtf(maxHeight) * 0.4f;

			bool flatPlacement = false;
			for (uint32 i = 0; i < 3; ++i)
			{
				if (maxWheelY - wheelWorldPosition[i].y > maxHeight)
				{
					flatPlacement = true;
					break;
				}
			}

			if (flatPlacement)
			{
				for (uint32 i = 0; i < 3; ++i)
				{
					wheelWorldPosition[i].y = maxWheelY;
				}
			}
		}

		{
			// Construct a plane through the three points
			const qsf::Plane plane(wheelWorldPosition[0], wheelWorldPosition[1], wheelWorldPosition[2]);

			// Get new position
			const glm::vec3& planeNormal = plane.getNormal();
			if (fabsf(planeNormal.y) > 0.001f)
			{
				// This is basically the plane equation "ax + by + cz + d = 0", get the value for "y"
				position.y = -(planeNormal.x * position.x + planeNormal.z * position.z + plane.getDistance()) / planeNormal.y;
			}

			// Get new rotation
			rotation = qsf::Math::getRotationByUpVector(planeNormal, rotation);
		}

		// Done
		return true;
	}

	bool RoadVehiclePlacementComponent::applyPlacement(glm::vec3& position, const glm::quat& rotation)
	{
		if (!mLocalWheelPositionInitialized)
		{
			if (!initialize())
				return false;

			// Initialization done
			mLocalWheelPositionInitialized = true;
		}

		// Get wheel world positions
		glm::vec3 wheelWorldPosition[3];
		for (uint32 i = 0; i < 3; ++i)
		{
			// Convert from local to world space
			wheelWorldPosition[i] = rotation * mLocalWheelPosition[i] + position;

			// Set the position to the ground
			dropToGround(wheelWorldPosition[i]);
		}

		{
			// Construct a plane through the three points
			const qsf::Plane plane(wheelWorldPosition[0], wheelWorldPosition[1], wheelWorldPosition[2]);

			// Get new position
			const glm::vec3& planeNormal = plane.getNormal();
			if (fabsf(planeNormal.y) > 0.001f)
			{
				// This is basically the plane equation "ax + by + cz + d = 0", get the value for "y"
				position.y = -(planeNormal.x * position.x + planeNormal.z * position.z + plane.getDistance()) / planeNormal.y;
			}

#if 0
			// Debug output
			qsf::DebugDrawProxy ddp;
			ddp.registerAt(QSF_DEBUGDRAW);
			ddp.addRequest(qsf::TriangleDebugDrawRequest(wheelWorldPosition[0], wheelWorldPosition[1], wheelWorldPosition[2], qsf::Color4::RED, 0.3f));
			ddp.setLifetimeInSeconds(5);
			ddp.detachRequests();
#endif
		}

		// Done
		return true;
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	bool RoadVehiclePlacementComponent::initialize()
	{
		// Get the road vehicle component
		RoadVehicleComponent* roadVehicleComponent = getEntity().getComponent<RoadVehicleComponent>();
		QSF_CHECK(nullptr != roadVehicleComponent, "Using 'RoadVehiclePlacementComponent' without a 'RoadVehicleComponent' in entity with ID " << getEntityId(), return false);

		uint32 wheelCountByIndex[3] = { 0, 0, 0 };

		const qsf::Map& map = getEntity().getMap();
		for (uint64 wheelId : roadVehicleComponent->VehicleWheelsArray.getVariable())
		{
			// Get the wheel entity
			const qsf::Entity* wheelEntity = map.getEntityById(wheelId);
			if (nullptr != wheelEntity)
			{
				// Get the wheel and link component of the wheel
				const WheelComponent* wheelComponent = wheelEntity->getComponent<WheelComponent>();
				const qsf::LinkComponent* wheelLink = wheelEntity->getComponent<qsf::LinkComponent>();

				if (nullptr != wheelComponent && nullptr != wheelLink)
				{
					int wheelIndex = 0;

					if (wheelComponent->getWheelType() == WheelComponent::WHEELTYPE_FRONT_LEFT || wheelComponent->getWheelType() == WheelComponent::WHEELTYPE_FRONT_RIGHT)
					{
						// Front wheel: We don't differentiate between left and right
						wheelIndex = 0;
					}
					else
					{
						// Rear wheel: Don't use wheel type for deciding whether it's left or right (sadly this information can be wrong)
						wheelIndex = (wheelLink->getLocalPosition().x < 0.0f) ? 1 : 2;
					}

					// Sum up local wheel positions to calculate centers
					mLocalWheelPosition[wheelIndex] += wheelLink->getLocalPosition();
					++wheelCountByIndex[wheelIndex];
				}
			}
		}

		// Get center points of local wheel positions
		for (uint32 i = 0; i < 3; ++i)
		{
			if (wheelCountByIndex[i] > 0)
			{
				mLocalWheelPosition[i] /= static_cast<float>(wheelCountByIndex[i]);
			}
			else
			{
				// Set default
				mLocalWheelPosition[i] = (i == 0) ? glm::vec3(0.0f, 0.0f, -1.0f) : (i == 1) ? glm::vec3(-0.5f, 0.0f, +1.0f) : glm::vec3(+0.5f, 0.0f, +1.0f);
			}
		}

		// Sanity check: Do wheels form a valid plane?
		//  -> This is needed e.g. for the SEK robot
		const qsf::Plane plane(mLocalWheelPosition[0], mLocalWheelPosition[1], mLocalWheelPosition[2]);
		if (plane.getNormal().y < 0.9f)
		{
			// Set default
			mLocalWheelPosition[0] = glm::vec3( 0.0f, 0.0f, -0.4f);
			mLocalWheelPosition[1] = glm::vec3(-0.2f, 0.0f, +0.4f);
			mLocalWheelPosition[2] = glm::vec3(+0.2f, 0.0f, +0.4f);
		}

		// Success
		return true;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

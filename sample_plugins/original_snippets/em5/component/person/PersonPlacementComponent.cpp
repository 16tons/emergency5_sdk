// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/component/person/PersonPlacementComponent.h"
#include "em5/game/groundmap/GroundMaps.h"
#include "em5/logic/local/firefighters/DiverLogic.h"
#include "em5/map/EntityHelper.h"
#include "em5/plugin/Messages.h"

//#include <qsf/physics/collision/BulletBoxCollisionComponent.h>	// TODO(mk) This code here is right, but currently the bullet box collision is not updated, the result here is the same as before.
#include <qsf/math/CoordinateSystem.h>
#include <qsf/math/EulerAngles.h>
#include <qsf/map/query/GroundMapQuery.h>
#include <qsf/map/Entity.h>
#include <qsf/renderer/mesh/MeshComponent.h>
#include <qsf/QsfHelper.h>

#include <glm/gtx/quaternion.hpp>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 PersonPlacementComponent::COMPONENT_ID = qsf::StringHash("em5::PersonPlacementComponent");


	//[-------------------------------------------------------]
	//[ Local definitions                                     ]
	//[-------------------------------------------------------]
	const float DIFFERENCE_TO_START_SWIMMING = 1.4f;	// Same as the difference in pivots for diver animations


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	bool PersonPlacementComponent::mustBeSwimming(const glm::vec3& position, float swimmingMinimumWaterHeight)
	{
		glm::vec3 positionTerrain(position);
		glm::vec3 positionWater(position);

		// Check both ground maps (unless some are excluded)
		const bool hasTerrain = dropToGround(positionTerrain, GroundMaps::FILTER_NO_LIQUIDS);
		const bool hasWater = dropToGround(positionWater, GroundMaps::FILTER_DEFAULT);

		if (hasTerrain && hasWater)
		{
			// Start swimming when the water surface is too far above the terrain for walking
			return (positionWater.y - positionTerrain.y > swimmingMinimumWaterHeight);
		}

		return false;
	}

	bool PersonPlacementComponent::mustBeSwimming(const glm::vec3& position)
	{
		return mustBeSwimming(position, DIFFERENCE_TO_START_SWIMMING);
	}


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	PersonPlacementComponent::PersonPlacementComponent(qsf::Prototype* prototype) :
		PlacementComponent(prototype),
		mPersonType(PERSONTYPE_UNINITIALIZED),
		mIsCollapsed(false)
	{
		// Nothing to do in here
	}

	PersonPlacementComponent::~PersonPlacementComponent()
	{
		// Nothing to do in here
	}

	void PersonPlacementComponent::setCollapsedFlag(bool active)
	{
		mIsCollapsed = active;
	}

	bool PersonPlacementComponent::getCollapsedFlag() const
	{
		return mIsCollapsed;
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::PlacementComponent methods        ]
	//[-------------------------------------------------------]
	bool PersonPlacementComponent::applyPlacement(glm::vec3& position, glm::quat& rotation)
	{
		return applyPlacementInternal(position, rotation, true);
	}

	bool PersonPlacementComponent::applyPlacement(glm::vec3& position, const glm::quat& rotation)
	{
		// Ugly const_cast
		return applyPlacementInternal(position, const_cast<glm::quat&>(rotation), false);
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Component methods              ]
	//[-------------------------------------------------------]
	void PersonPlacementComponent::onSetSimulating(bool simulating)
	{
		// Nothing here yet
	}


	//[-------------------------------------------------------]
	//[ Protected static methods                              ]
	//[-------------------------------------------------------]
	bool PersonPlacementComponent::dropToGround(glm::vec3& position, uint32 groundMapFilterMask)
	{
		// Try to find the ground at the given position
		float height = position.y;

		qsf::GroundMapQuery groundMapQuery(QSF_MAINMAP, groundMapFilterMask);
		if (!groundMapQuery.getHeightAtXZPosition(position.x, position.z, height))
		{
			// Ground was not found, so no change at all
			return false;
		}

		// Change the position y coordinate
		position.y = height;
		return true;
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	bool PersonPlacementComponent::applyPlacementInternal(glm::vec3& position, glm::quat& rotation, bool needsUpdatedRotation)
	{
		// Initialization on first placement update
		if (mPersonType == PERSONTYPE_UNINITIALIZED)
		{
			mPersonType = PERSONTYPE_DEFAULT;

			// Check for diver
			DiverLogic* diverLogic = EntityHelper(getEntity()).getGameLogic<DiverLogic>();
			if (nullptr != diverLogic)
			{
				mPersonType = PERSONTYPE_DIVER;
				mDiverLogic = diverLogic;
			}
		}

		const float oldHeight = position.y;

		glm::vec3 positionTerrain(position);
		glm::vec3 positionWater(position);

		// Check both ground maps
		const bool hasTerrain = dropToGround(positionTerrain, GroundMaps::FILTER_NO_LIQUIDS);
		const bool hasWater = dropToGround(positionWater, GroundMaps::FILTER_DEFAULT);

		if (hasTerrain && hasWater)
		{
			if (mPersonType == PERSONTYPE_DIVER && mDiverLogic.valid())
			{
				// If water surface is below terrain, ignore it
				if (positionWater.y < positionTerrain.y)
				{
					position.y = positionTerrain.y;
				}
				else
				{
					// If water surface is below terrain, ignore it
					positionWater.y = std::max(positionTerrain.y, positionWater.y);
					position.y = glm::mix(positionTerrain.y, positionWater.y, mDiverLogic->getSwimmingOffset());
				}
			}
			else
			{
				// Start swimming when the water surface is too far above the terrain for walking
				if (positionWater.y - positionTerrain.y < DIFFERENCE_TO_START_SWIMMING)
				{
					position.y = positionTerrain.y;
				}
				else
				{
					position.y = positionWater.y;
				}
			}
		}
		else if (hasTerrain)
		{
			position.y = positionTerrain.y;
		}
		else if (hasWater)
		{
			position.y = positionWater.y;
		}
		else
		{
			// Error case, no terrain under the person
			return false;
		}

		if (mIsCollapsed)
		{
			// Check the center of the bounding box instead the pivot of the entity
#if 1
			qsf::MeshComponent* meshComponent = getPrototype().getComponent<qsf::MeshComponent>();
			if (nullptr != meshComponent)
			{
				glm::vec3 localMinCorner;
				glm::vec3 localMaxCorner;
				meshComponent->getOgreEntityBoundingBox(localMinCorner, localMaxCorner);

				glm::vec3 center = (localMinCorner + localMaxCorner) / 2.0f;
				center = position + rotation * center;		// Translate from local to world space
				dropToGround(center, GroundMaps::FILTER_DEFAULT);
				position.y = center.y;
			}
#else
			/*	TODO(mk) This code here is right, but currently the bullet box collision is not updated, the result here is the same as before.
			qsf::BulletBoxCollisionComponent* bulletBoxCollisionComponent = getEntity().getComponent<qsf::BulletBoxCollisionComponent>();
			if (nullptr != bulletBoxCollisionComponent)
			{
				const btVector3& boundingBoxCenter = bulletBoxCollisionComponent->getTransform().getOrigin();
				glm::vec3 center = glm::vec3(boundingBoxCenter.x(), boundingBoxCenter.y(), boundingBoxCenter.z());
				dropToGround(center, GroundMaps::FILTER_DEFAULT);
				position.y = center.y;
			}
			*/
#endif
		}

		bool result = (position.y != oldHeight);

		if (needsUpdatedRotation && (rotation * qsf::CoordinateSystem::getUp()).y < 0.99f)
		{
			// Entity is not looking up, so we change this rotation
			const float currentYaw = qsf::EulerAngles::quaternionToEuler(rotation).x;
			rotation = qsf::EulerAngles::eulerToQuaternion(currentYaw, 0, 0);
			result = true;
		}

		return result;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf

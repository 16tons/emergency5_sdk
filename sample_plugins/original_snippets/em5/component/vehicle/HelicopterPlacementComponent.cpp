// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/component/vehicle/HelicopterPlacementComponent.h"
#include "em5/component/vehicle/HelicopterComponent.h"
#include "em5/component/vehicle/parts/HelicopterHatchComponent.h"
#include "em5/game/groundmap/GroundMaps.h"
#include "em5/specs/RoadVehicleSpecsGroup.h"

#include <qsf/map/query/GroundMapQuery.h>
#include <qsf/map/Entity.h>
#include <qsf/map/Map.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 HelicopterPlacementComponent::COMPONENT_ID = qsf::StringHash("em5::HelicopterPlacementComponent");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	HelicopterPlacementComponent::HelicopterPlacementComponent(qsf::Prototype* prototype) :
		PivotOnGroundPlacementComponent(prototype),
		mCurrentHeight(0.0f),
		mHasCurrentHeight(false)
	{
		// Nothing to do in here
	}

	HelicopterPlacementComponent::~HelicopterPlacementComponent()
	{
		// Nothing to do in here
	}

	void HelicopterPlacementComponent::dirtyCurrentHeight()
	{
		mHasCurrentHeight = false;
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::PlacementComponent methods        ]
	//[-------------------------------------------------------]
	bool HelicopterPlacementComponent::applyPlacement(glm::vec3& position, glm::quat& rotation)
	{
		if (mHasCurrentHeight)
		{
			position.y = mCurrentHeight;
		}

		qsf::Map& map = getEntity().getMap();

		float terrainHeight = 0.0f;
		qsf::GroundMapQuery(map, GroundMaps::FILTER_HELICOPTER_GROUND).getHeightAt(position, terrainHeight);
		float optimalFlyHeight = terrainHeight + RoadVehicleSpecsGroup::getInstanceSafe().getHelicopterFlyHeight();
		const float flyHeightChangeSpeed = RoadVehicleSpecsGroup::getInstanceSafe().getHelicopterFlyHeightSpeed() * 0.1f;	// TODO(mk) Use time passed from gameclock. Currently we multiply with 0.1 to simulate the gameclock

		// Check if helicopter must fly higher because he has loaded something
		HelicopterComponent* helicopterComponent = getEntity().getComponent<HelicopterComponent>();
		if (nullptr != helicopterComponent)
		{
			const qsf::Entity* specialEntity = map.getEntityById(helicopterComponent->getSpecialEntity());
			if (nullptr != specialEntity)
			{
				// An injured person increases the fly height
				HelicopterHatchComponent* helicopterHatchComponent = specialEntity->getComponent<HelicopterHatchComponent>();
				if (nullptr != helicopterHatchComponent && helicopterHatchComponent->isTransportingInjuredPerson())
				{
					optimalFlyHeight += 2;
				}
			}
		}

		// Set the helicopter position
		// TODO(fw): It's pretty ugly to do height adaptations here inside the placement
		//  -> the helicopter component could be a better place - at least for regularly updating a "height" variable in here
		bool result = true;
		if (position.y < optimalFlyHeight)
		{
			if (position.y < optimalFlyHeight - 10.0f)
			{
				position.y = optimalFlyHeight - 10.0f;
			}
			else
			{
				position.y += flyHeightChangeSpeed;
				if (position.y > optimalFlyHeight)
				{
					position.y = optimalFlyHeight;
				}
			}
		}
		else if (position.y > optimalFlyHeight)
		{
			position.y -= flyHeightChangeSpeed;
			if (position.y < optimalFlyHeight)
			{
				position.y = optimalFlyHeight;
			}
		}
		else
		{
			result = false;
		}

		mCurrentHeight = position.y;
		mHasCurrentHeight = true;

		return result;
	}

	bool HelicopterPlacementComponent::applyPlacement(glm::vec3& position, const glm::quat& rotation)
	{
		glm::quat dummyRotation = rotation;
		return applyPlacement(position, dummyRotation);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

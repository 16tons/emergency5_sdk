// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/component/polygon/WaterDeepTypePolygonComponent.h"
#include "em5/game/collision/CollisionTypes.h"

#include <qsf/physics/collision/BulletTriangleMeshCollisionComponent.h>
#include <qsf/math/Color4.h>
#include <qsf/map/Entity.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 WaterDeepTypePolygonComponent::COMPONENT_ID	= qsf::StringHash("em5::WaterDeepTypePolygonComponent");
	const uint32 WaterDeepTypePolygonComponent::WATER_DEEP_TYPE = qsf::StringHash("WaterDeepType");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	void WaterDeepTypePolygonComponent::setWaterDeepType(WaterDeepType waterDeepType)
	{
		if (mWaterDeepType != waterDeepType)
		{
			mWaterDeepType = waterDeepType;

			// Update debug visualization
			if (isRunning())
			{
				startDebugDrawing();
			}

			// Promote the property change
			promotePropertyChange(WATER_DEEP_TYPE);
		}
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Component methods                 ]
	//[-------------------------------------------------------]
	bool WaterDeepTypePolygonComponent::onStartup()
	{
		// When the collision component doesn't exists create and set the values depending on the current water deep type
		// This is normally the case when this component is newly created without existing node data
		if (!getEntity().getComponent<qsf::BulletTriangleMeshCollisionComponent>())
		{
			updateCollisionComponentValues();
		}

		return PolygonComponent::onStartup();
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Component methods              ]
	//[-------------------------------------------------------]
	void WaterDeepTypePolygonComponent::onComponentPropertyChange(const qsf::Component& component, uint32 propertyId)
	{
		// Don't react on property changes when not running
		if (!isRunning())
			return;

		// Call base class implementation
		PolygonComponent::onComponentPropertyChange(component, propertyId);

		if (&component == this && propertyId == WATER_DEEP_TYPE)
		{
			// Update the collision values when the water deep type changes
			updateCollisionComponentValues();
		}
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::NodesComponent methods         ]
	//[-------------------------------------------------------]
	const qsf::Color4& WaterDeepTypePolygonComponent::getPolygonDebugColor() const
	{
		switch (mWaterDeepType)
		{
			case SHALLOW_WATER:
			{
				static const qsf::Color4 shallowWaterColor(0.6f, 0.6f, 1.0f, 1.0f);
				return shallowWaterColor;
			}

			case MEDIUM_DEEP_WATER:
			{
				static const qsf::Color4 mediumDepthWaterColor(0.1f, 0.1f, 1.0f, 1.0f);
				return mediumDepthWaterColor;
			}

			case DEEP_WATER:
			{
				static const qsf::Color4 depthWaterColor(0.0f, 0.0f, 0.5f, 1.0f);
				return depthWaterColor;
			}
		}
		return qsf::Color4::BLUE;
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void WaterDeepTypePolygonComponent::updateCollisionComponentValues() const
	{
		qsf::BulletTriangleMeshCollisionComponent* bulletTriangleMeshCollisionComponent = getEntity().getOrCreateComponent<qsf::BulletTriangleMeshCollisionComponent>();

		short collisionMask = 0;

		switch (mWaterDeepType)
		{
			case SHALLOW_WATER:
				collisionMask = collision::SHALLOWWATERFILTER;
				break;

			case MEDIUM_DEEP_WATER:
				collisionMask = collision::MEDIUMWATERFILTER;
				break;

			// Deep water is also default
			case DEEP_WATER:
			default:
				collisionMask = collision::DEEPWATERFILTER;
				break;
		}

		bulletTriangleMeshCollisionComponent->setCollisionFilterGroup(collision::VORONOIFILTER | collision::PROJECTILEFILTER);
		bulletTriangleMeshCollisionComponent->setCollisionMask(collisionMask);

		bulletTriangleMeshCollisionComponent->setPropertyOverrideState("CollisionFilterGroup", Object::PROPERTY_OVERRIDE);
		bulletTriangleMeshCollisionComponent->setPropertyOverrideState("CollisionMask", Object::PROPERTY_OVERRIDE);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

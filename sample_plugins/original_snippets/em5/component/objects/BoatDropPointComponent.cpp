// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/component/objects/BoatDropPointComponent.h"

#include <qsf/debug/request/TextDebugDrawRequest.h>
#include <qsf/debug/request/RectangleDebugDrawRequest.h>
#include <qsf/map/Map.h>
#include <qsf/map/Entity.h>
#include <qsf/map/EntityHelper.h>
#include <qsf/component/base/TransformComponent.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 BoatDropPointComponent::COMPONENT_ID = qsf::StringHash("em5::BoatDropPointComponent");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	BoatDropPointComponent::BoatDropPointComponent(qsf::Prototype* prototype) :
		qsf::Component(prototype),
		mTransporterEntityId(qsf::getUninitialized<uint64>()),
		mLakeId(qsf::getUninitialized<uint64>()),
		mBoatEntityId(qsf::getUninitialized<uint64>()),
		mDropMarkerIndex(0),
		DropMarkerArray(*this, mDropMarkerIds)
	{
		// Nothing to do in here
	}

	BoatDropPointComponent::~BoatDropPointComponent()
	{
		// Nothing to do in here
	}

	void BoatDropPointComponent::highlightPosition(bool highlight)
	{
		if (highlight)
		{
			qsf::Entity* transporterPositionEntity = getEntity().getMap().getEntityById(getTransporterEntityId());
			if (nullptr == transporterPositionEntity)
				return;

			mDebugDrawProxy.registerForComponent(*this);

			const glm::vec3& position = transporterPositionEntity->getComponentSafe<qsf::TransformComponent>().getPosition();

			glm::vec3 corner1;
			glm::vec3 corner2;
			qsf::EntityHelper(*transporterPositionEntity).getAxisAlignedBoundingBox(corner1, corner2);

			mDebugDrawProxy.addRequest(qsf::RectangleDebugDrawRequest(glm::vec3(corner1.x, position.y, corner1.z), glm::vec3(corner2.x, position.y, corner2.z), qsf::Color4::WHITE, 0.5f));
			mDebugDrawProxy.addRequest(qsf::TextDebugDrawRequest("Boat drop point", position));
		}
		else
		{
			mDebugDrawProxy.unregister();
		}
	}

	qsf::TransformComponent* BoatDropPointComponent::getBoatDropTransform() const
	{
		if (qsf::isInitialized(mBoatEntityId))
		{
			qsf::Entity* boatDropEntity = getEntity().getMap().getEntityById(mBoatEntityId);
			if (nullptr != boatDropEntity)
			{
				return boatDropEntity->getComponent<qsf::TransformComponent>();
			}
		}

		return nullptr;
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Component methods                 ]
	//[-------------------------------------------------------]
	void BoatDropPointComponent::serialize(qsf::BinarySerializer& serializer)
	{
		serializer.serialize(mTransporterEntityId);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

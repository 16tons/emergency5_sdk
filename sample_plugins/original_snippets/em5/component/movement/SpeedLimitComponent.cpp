// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/component/movement/SpeedLimitComponent.h"

#include <qsf/component/spatial/SpatialPartitionMemberComponent.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 SpeedLimitComponent::COMPONENT_ID			= qsf::StringHash("em5::SpeedLimitComponent");
	const uint32 SpeedLimitComponent::RADIUS				= qsf::StringHash("Radius");
	const uint32 SpeedLimitComponent::SPEED_LIMIT			= qsf::StringHash("SpeedLimit");

	const float SpeedLimitComponent::DEFAULT_RADIUS			= 10.0f;
	const float SpeedLimitComponent::DEFAULT_SPEED_LIMIT	= 3.0f;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	SpeedLimitComponent::SpeedLimitComponent(qsf::Prototype* prototype):
		qsf::Component(prototype),
		mRadius(DEFAULT_RADIUS),
		mSpeedLimit(DEFAULT_SPEED_LIMIT)
	{
		// Nothing here
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Component methods              ]
	//[-------------------------------------------------------]
	bool SpeedLimitComponent::onStartup()
	{
		// Register in spatial partition system, optimizing range searches for this kind of component
		qsf::SpatialPartitionMemberComponent::registerComponentInPartition(*this);

		// Done
		return true;
	}

	void SpeedLimitComponent::onShutdown()
	{
		// Unregister in spatial partition system
		qsf::SpatialPartitionMemberComponent::unregisterComponentInPartition(*this);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

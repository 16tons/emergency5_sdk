// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline SpatialPartitionManagerComponent::SpatialPartitionManagerComponent(Prototype* prototype) :
		Component(prototype),
		mPartitions(nullptr),
		mPartitionsMemory(nullptr)
	{
		memset(mPartitionIds, 0, sizeof(uint32) * MAX_PARTITIONS);
		memset(mPartitionUsed, 0, sizeof(bool) * MAX_PARTITIONS);
	}

	inline SpatialPartitionManagerComponent::~SpatialPartitionManagerComponent()
	{
		// Nothing here
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf

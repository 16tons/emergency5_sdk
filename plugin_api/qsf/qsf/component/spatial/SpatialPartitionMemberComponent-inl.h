// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline SpatialPartitionMemberComponent::SpatialPartitionMemberComponent(Prototype* prototype) :
		Component(prototype)
	{
		// Nothing here
	}

	inline SpatialPartitionMemberComponent::~SpatialPartitionMemberComponent()
	{
		// Nothing here
	}

	inline const std::pair<glm::vec3, glm::vec3>& SpatialPartitionMemberComponent::getBounds() const
	{
		return mBounds;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf

// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline const AssetProxy& MeshComponent::getMesh() const
	{
		return mMesh;
	}

	inline float MeshComponent::getLodFactor() const
	{
		return mLodFactor;
	}

	inline unsigned short MeshComponent::getMaxLodDetailIndex() const
	{
		return mMaxLodDetailIndex;
	}

	inline unsigned short MeshComponent::getMinLodDetailIndex() const
	{
		return mMinLodDetailIndex;
	}

	inline uint32 MeshComponent::getVisibilityFlags() const
	{
		return mVisibilityFlags;
	}

	inline void MeshComponent::getOgreEntityBoundingBox(glm::vec3& localMinCorner, glm::vec3& localMaxCorner) const
	{
		localMinCorner = mCustomBoundingBoxMinCorner;
		localMaxCorner = mCustomBoundingBoxMaxCorner;
	}

	inline const glm::vec3& MeshComponent::getOgreEntityBoundingBox() const
	{
		return mCustomBoundingBoxLocalExtends;
	}

	inline bool MeshComponent::isCustomBoundingBoxSet() const
	{
		return mIsCustomBoundingBoxSet;
	}

	inline bool MeshComponent::getReuseSkeletonFromParent() const
	{
		return mReuseSkeletonFromParent;
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::RendererComponent methods         ]
	//[-------------------------------------------------------]
	inline Ogre::SceneNode* MeshComponent::getOgreSceneNode() const
	{
		return mOgreSceneNode;
	}

	inline Ogre::v1::Entity* MeshComponent::getOgreEntity() const
	{
		return mOgreEntity;
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::RendererComponent methods      ]
	//[-------------------------------------------------------]
	inline bool MeshComponent::isGridManaged() const
	{
		return true;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf

// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline FlareComponent::FlareComponent(Prototype* prototype) :
		RendererComponent(prototype),
		mBillboardType(POINT),
		mColor(Color4::WHITE),
		mIntensity(1.0f),
		mWidthHeight(1.0f, 1.0f),
		mDirection(0.0, 1.0f, 0.0f),
		mOcclusionScale(0.2f),
		mMaterialAssetProxy("qsf/material/default/flare"),
		mAtlasStackCount(1),
		mAtlasSliceCount(1),
		mOgreSceneNode(nullptr),
		mOgreBillboardFlare(nullptr),
		mOgreBillboardSetFlare(nullptr),
		mOgreBillboardSetQueryArea(nullptr),
		mOgreBillboardSetQueryVisible(nullptr),
		mOgreHardwareOcclusionQueryArea(nullptr),
		mOgreHardwareOcclusionQueryVisible(nullptr),
		mDoOcclusionQuery(false),
		mOcclusionQueriesEnabled(false),
		mVisibilityFactor(1.0f),
		mAngleVisibilityFactor(1.0f),
		mVisibilityAngle(-1.0f),
		mTextureIndex(0),
		mIntensityFactor(1.0f)
	{
		// Nothing to do in here
	}

	inline FlareComponent::BillboardType FlareComponent::getBillboardType() const
	{
		return mBillboardType;
	}

	inline const Color4& FlareComponent::getColor() const
	{
		return mColor;
	}

	inline float FlareComponent::getIntensity() const
	{
		return mIntensity;
	}

	inline const glm::vec2& FlareComponent::getWidthHeight() const
	{
		return mWidthHeight;
	}

	inline const glm::vec3& FlareComponent::getDirection() const
	{
		return mDirection;
	}

	inline float FlareComponent::getOcclusionScale() const
	{
		return mOcclusionScale;
	}

	inline void FlareComponent::setOcclusionQueriesEnabled(bool occlusionQueriesEnabled)
	{
		mOcclusionQueriesEnabled = occlusionQueriesEnabled;
	}

	inline float FlareComponent::getVisibilityAngle() const
	{
		return mVisibilityAngle;
	}

	inline uint32 FlareComponent::getTextureIndex() const
	{
		return mTextureIndex;
	}

	inline const AssetProxy& FlareComponent::getMaterial() const
	{
		return mMaterialAssetProxy;
	}

	inline uint16 FlareComponent::getAtlasNumberOfTextureRows() const
	{
		return mAtlasStackCount;
	}

	inline uint16 FlareComponent::getAtlasNumberOfTexturesPerTextureRow() const
	{
		return mAtlasSliceCount;
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::RendererComponent methods         ]
	//[-------------------------------------------------------]
	inline Ogre::SceneNode* FlareComponent::getOgreSceneNode() const
	{
		return mOgreSceneNode;
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::RendererComponent methods      ]
	//[-------------------------------------------------------]
	inline bool FlareComponent::isGridManaged() const
	{
		return true;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf

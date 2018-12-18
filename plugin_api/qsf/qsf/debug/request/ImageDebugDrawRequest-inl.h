// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline ImageDebugDrawRequest::ImageDebugDrawRequest(const glm::vec2& topLeftPosition, const glm::vec2& size, const std::string& textureLocalAssetName, const Color4& tintColor) :
		mPosition(topLeftPosition),
		mSize(size),
		mUseWorldSpace(false),
		mTintColor(tintColor),
		mTextureLocalAssetName(textureLocalAssetName)
	{
		// Nothing to do in here
	}

	inline ImageDebugDrawRequest::ImageDebugDrawRequest(const glm::vec3& worldPosition, const glm::vec2& size, const std::string& textureLocalAssetName, const Color4& tintColor) :
		mWorldPosition(worldPosition),
		mSize(size),
		mUseWorldSpace(true),
		mTintColor(tintColor),
		mTextureLocalAssetName(textureLocalAssetName)
	{
		// Nothing to do in here
	}

	inline ImageDebugDrawRequest::~ImageDebugDrawRequest()
	{
		// Nothing to do in here
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf

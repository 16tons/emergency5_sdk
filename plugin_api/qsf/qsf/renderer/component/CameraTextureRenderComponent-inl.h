// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline const std::string& CameraTextureRenderComponent::getTextureName() const
	{
		return mTextureName;
	}

	inline uint32 CameraTextureRenderComponent::getRenderWidth() const
	{
		return mRenderWidth;
	}

	inline uint32 CameraTextureRenderComponent::getRenderHeight() const
	{
		return mRenderHeight;
	}

	inline const Time& CameraTextureRenderComponent::getTimeBetweenCalls() const
	{
		return mTimeBetweenCalls;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf

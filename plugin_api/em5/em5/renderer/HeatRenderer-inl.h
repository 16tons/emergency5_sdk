// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	inline Rocket::Core::Texture* HeatRenderer::getHeatTexture() const
	{
		return mRocketTexture;
	}

	inline void HeatRenderer::setRenderingEnabled(bool renderingEnabled)
	{
		mRenderingEnabled = renderingEnabled;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

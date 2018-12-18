// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline bool TextureStreamer::isProgressiveMipLoading() const
	{
		return mIsProgressiveMipLoadingEnabled;
	}

	inline void TextureStreamer::enableProgressiveMipLoading(bool enabled)
	{
		mIsProgressiveMipLoadingEnabled = enabled;
	}

	inline bool TextureStreamer::isFullStreamInByDefaultEnabled() const
	{
		return mIsFullStreamInByDefaultEnabled;
	}

	inline bool TextureStreamer::isAllTextureConsideredForStreamingEnabled() const
	{
		return mConsiderAllTexturesForStreaming;
	}

	inline void TextureStreamer::enableAllTexturesConsideredForStreaming(bool enabled)
	{
		mConsiderAllTexturesForStreaming = enabled;
	}

	inline uint32 TextureStreamer::getTopMipLevelReductionCount() const
	{
		return mTopLevelMipReductionCount;
	}

	inline bool TextureStreamer::isChronologicCacheClearingEnabled() const
	{
		return mEnableChronologicCacheClearing;
	}

	inline const Time& TextureStreamer::getLastTouch() const
	{
		return mLastTouch;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf

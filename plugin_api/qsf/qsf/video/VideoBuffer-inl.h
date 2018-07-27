// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline uint32 VideoBuffer::getFrameIndex(float time)
	{
		std::unique_lock<std::mutex> lock(mMutex);
		return (uint32)(time * mFramesPerSecond + 0.1f);
	}

	inline uint32 VideoBuffer::getWidth() const
	{
		std::unique_lock<std::mutex> lock(mMutex);
		return mWidth;
	}

	inline uint32 VideoBuffer::getHeight() const
	{
		std::unique_lock<std::mutex> lock(mMutex);
		return mHeight;
	}

	inline uint32 VideoBuffer::getWidthUV() const
	{
		std::unique_lock<std::mutex> lock(mMutex);
		return mWidthUV;
	}

	inline uint32 VideoBuffer::getHeightUV() const
	{
		std::unique_lock<std::mutex> lock(mMutex);
		return mHeightUV;
	}

	inline uint32 VideoBuffer::getFrameCount() const
	{
		std::unique_lock<std::mutex> lock(mMutex);
		return mLastBufferedFrame;
	}

	inline float VideoBuffer::getLengthInSec() const
	{
		std::unique_lock<std::mutex> lock(mMutex);
		return mLastBufferedFrame / mFramesPerSecond;
	}

	inline float VideoBuffer::getFramesPerSecond() const
	{
		std::unique_lock<std::mutex> lock(mMutex);
		return mFramesPerSecond;
	}

	inline const float* VideoBuffer::getCropRect() const
	{
		// Not multithreading safe
		return mCropRect;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf

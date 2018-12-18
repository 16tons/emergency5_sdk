// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/base/manager/Manager.h"

#include <boost/container/flat_map.hpp>

#include <mutex>
#include <vector>
#include <map>
#include <memory>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Video buffer class
	*
	*  @remarks
	*    Buffers frames which are (most times) supplied by a codec, which fills the buffer and adds frames.
	*    The User of this buffer (e.g. a display adapter) retrieves frames and marks them as obsolete when they
	*    are used (i.e. displayed).
	*
	*    Implemented as a sparse frame buffer, i.e. in theory, every frame is accessible, but only those
	*    portions supplied by the writer are in-memory (dense buffer parts). The reader can also set frames
	*    as obsolete. The data of those frames will be deleted then. To keep the buffer from growing
	*    indefinitely it is advised to mark frames obsolete as soon as they are no longer of any use!
	*/
	class QSF_API_EXPORT VideoBuffer : public boost::noncopyable
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		/**
		*  Single video frame
		*/
		struct VideoFrame
		{
			VideoFrame() :
				bufferRGBA(nullptr),
				bufferY(nullptr),
				bufferU(nullptr),
				bufferV(nullptr),
				mDirtyBufferRGBA(true)
			{
			}

			~VideoFrame()
			{
				delete [] bufferRGBA;
				delete [] bufferY;
				delete [] bufferU;
				delete [] bufferV;
			}

			uint32* bufferRGBA;		///< On-demand allocated buffer to the RGBA video frame data
			uint8* bufferY;			///< Y buffer. YUV-buffers: These are sized differently so we have an array to access each buffer individually.
			uint8* bufferU;			///< U buffer. YUV-buffers: These are sized differently so we have an array to access each buffer individually.
			uint8* bufferV;			///< V buffer. YUV-buffers: These are sized differently so we have an array to access each buffer individually.
			bool mDirtyBufferRGBA;
		};

		/**
		*  @brief
		*    Holds pointers to separate buffers of YUV planes for a frame
		*/
		struct SplitBufferYUVFrame
		{
			const uint8* YData;
			const uint8* UData;
			const uint8* VData;

			uint32 strideY;
			uint32 strideUV;
		};


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] precachingTime
		*    Precaching time in seconds
		*/
		explicit VideoBuffer(float precachingTime);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~VideoBuffer();


	//[-------------------------------------------------------]
	//[ Public virtual qsf::VideoManager methods              ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Clear out the full buffer, resetting values
		*/
		void clear();
		void clear(uint32 width, uint32 height, uint32 widthUV, uint32 heightUV, float framesPerSecond);

		/**
		*  @brief
		*    Add a YUV frame to the buffer chain
		*/
		void setFrameYUV(uint32 frameIndex, const SplitBufferYUVFrame& yuvFrame);

		/**
		*  @brief
		*    Adds an empty frame to the buffer
		*
		*  @remarks
		*    This just records the frame index as the last recorded frame. When trying to retrieve
		*    this frame, the preceding frame will be returned
		*/
		void setFrameEmpty(uint32 frameIndex);

		/**
		*  @brief
		*    Retrieve a reference to the given frame, if it exists
		*
		*  @return
		*    Valid pointer to a VideoFrame or a nullptr if the frame is not present in the buffer
		*/
		const VideoFrame* getFrame(uint32 frameIndex) const;

		/**
		*  @brief
		*    Calculate the index of a frame based on the video time
		*
		*  @note
		*    - Add 0.1f to compensate floating point errors
		*/
		inline uint32 getFrameIndex(float time);

		/**
		*  @brief
		*    Retrieve the width of a full frame (RGBA/Y planes)
		*/
		inline uint32 getWidth() const;

		/**
		*  @brief
		*    Retrieve the height of a full frame (RGBA/Y planes)
		*/
		inline uint32 getHeight() const;

		/**
		*  @brief
		*    Retrieve the width of a chroma frame (U/V planes)
		*/
		inline uint32 getWidthUV() const;

		/**
		*  @brief
		*    Retrieve the height of a chroma frame (U/V planes)
		*/
		inline uint32 getHeightUV() const;

		/**
		*  @brief
		*    Retrieve the index of frames buffered so far
		*/
		inline uint32 getFrameCount() const;

		/**
		*  @brief
		*    Retrieve the (absolute) length of the buffered frames (i.e. time of the last frame in the buffer)
		*/
		inline float getLengthInSec() const;

		/**
		*  @brief
		*    Retrieve the framerate
		*/
		inline float getFramesPerSecond() const;

		/**
		*  @brief
		*    Set the crop rectangle, in frame size percentages (normalized coordinates)
		*/
		void setCropRect(float left, float top, float right, float bottom);

		/**
		*  @brief
		*    Set the crop rectangle, in pixel coordinates
		*/
		void setCropRect(uint32 left, uint32 top, uint32 width, uint32 height);

		/**
		*  @brief
		*    Retrieve the crop rectangle as normalized coordinates
		*
		*  @return
		*    Reference of a 4-element array of normalized coordinates
		*    in the order LEFT, TOP, RIGHT, BOTTOM
		*
		*  @note
		*    The coordinates of the crop rect can be directly used as texture
		*    coordinates for an output surface to display frames correctly
		*/
		inline const float* getCropRect() const;

		/**
		*  @brief
		*    Delete the data of frames which are now obsolete. The frames
		*    will still be "known" but their data will be erased. This way, the
		*    frame buffer will only keep growing but will not use up humongous amounts of memory
		*/
		void setObsoleteFrames(uint32 lastObsoleteFrameIndex);

		/**
		*  @brief
		*    Convert a single frame from YUV to RGBA, if necessary.
		*    Before accessing RGBA data, make sure to call this method
		*/
		void convertFrameToRGBA(uint32 frameIndex);


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Retrieve an unused video frame
		*
		*  @return
		*    A now no longer unused video frame, null pointer on horrible error
		*/
		VideoFrame* getUnusedVideoFrame();

		/**
		*  @brief
		*    Create a new video frame instance
		*
		*  @return
		*    New video frame instance, you have to destroy the instance in case you no longer need it
		*/
		VideoFrame* createVideoFrame() const;


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
#ifdef RETAIL
		typedef boost::container::flat_map<uint32, VideoFrame*> FrameBufferMap;
#else
		typedef std::map<uint32, VideoFrame*>					FrameBufferMap;		// For easier debugging in Release mode
#endif
		typedef std::vector<VideoFrame*>						FrameBufferList;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		mutable std::mutex	mMutex;				///< Mutex for data synchronization
		float				mPrecachingTime;	///< Precaching time in seconds
		uint32				mLastBufferedFrame;	///< The last frame which has been buffered
		uint32				mFrameCacheSize;
		FrameBufferMap		mFrames;			///< List of video frames, sparse frame buffer, may contain holes
		FrameBufferList		mUnusedFrames;		///< List of currently unused video frame buffers, ready to be recycled

		uint32 mWidth, mHeight;			///< Plane size for RGBA and Y planes
		uint32 mWidthUV, mHeightUV;		///< Plane size for UV planes
		float  mFramesPerSecond;		///< Frames per second
		float  mCropRect[4];			///< Cropping rectangle [LEFT, TOP, RIGHT, BOTTOM] in normalized coordinates


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/video/VideoBuffer-inl.h"

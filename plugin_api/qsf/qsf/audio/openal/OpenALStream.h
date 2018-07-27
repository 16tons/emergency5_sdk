// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/platform/PlatformTypes.h"
#include "qsf/time/Time.h"

#include <OpenAL/al.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace openal
	{
		class OpenALAudioBuffer;
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace openal
	{


		//[-------------------------------------------------------]
		//[ Classes                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Abstract OpenAL audio stream base class
		*/
		class OpenALStream
		{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Destructor
			*/
			inline virtual ~OpenALStream();

			/**
			*  @brief
			*    Return the OpenAL audio source using this stream
			*
			*  @return
			*    The OpenAL audio source using this stream
			*/
			inline ALuint getOpenALSource() const;

			/**
			*  @brief
			*    Return the OpenAL audio buffer this stream is using
			*
			*  @return
			*    The OpenAL audio buffer this stream is using
			*/
			inline const OpenALAudioBuffer& getOpenALAudioBuffer() const;

			/**
			*  @brief
			*    Return whether the stream is looping or not
			*
			*  @return
			*    "true" if stream is looping, else "false"
			*/
			inline bool isLooping() const;

			/**
			*  @brief
			*    Set whether the stream is looping or not
			*
			*  @param[in] looping
			*    If "true" the stream is looping, else "false"
			*/
			inline void setLooping(bool looping);


		//[-------------------------------------------------------]
		//[ Public virtual qsf::openal::OpenALStream methods      ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Initialize the stream for playback
			*
			*  @return
			*    "true" if all went fine, else "false"
			*/
			virtual bool init() = 0;

			/**
			*  @brief
			*    Return whether or not the stream is currently initialized (and therefore running)
			*
			*  @return
			*    "true" if the stream is currently initialized, else "false"
			*/
			virtual bool isInitialized() const = 0;

			/**
			*  @brief
			*    De-initialize the stream (playback stopped)
			*/
			virtual void deInit() = 0;

			/**
			*  @brief
			*    Update the stream
			*
			*  @return
			*    "true" if the stream is still active, else "false" (maybe finished)
			*/
			virtual bool update() = 0;

			/**
			*  @brief
			*    Returns the point-in-time where the currently playing buffer is beginning relatively to
			*    the overall start of the stream
			*/
			virtual Time getActiveBufferBaseTime() const = 0;


		//[-------------------------------------------------------]
		//[ Protected methods                                     ]
		//[-------------------------------------------------------]
		protected:
			/**
			*  @brief
			*    Constructor
			*
			*  @param[in] openALSource
			*    OpenAL audio source using this stream
			*  @param[in] openALAudioBuffer
			*    OpenAL audio buffer this stream is using
			*/
			inline OpenALStream(ALuint openALSource, const OpenALAudioBuffer& openALAudioBuffer);


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			ALuint					 mOpenALSource;			///< OpenAL audio source using this stream
			const OpenALAudioBuffer* mOpenALAudioBuffer;	///< OpenAL audio buffer this stream is using, always valid, do not destroy the instance
			bool					 mLooping;				///< Is the stream looping?


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // openal
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/audio/openal/OpenALStream-inl.h"

// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/base/error/ErrorHandling.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace openal
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		inline OpenALStream::~OpenALStream()
		{
			// Nothing to do in here
		}

		inline ALuint OpenALStream::getOpenALSource() const
		{
			return mOpenALSource;
		}

		inline const OpenALAudioBuffer& OpenALStream::getOpenALAudioBuffer() const
		{
			// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
			QSF_ASSERT(nullptr != mOpenALAudioBuffer, "The OpenAL audio buffer instance is invalid", QSF_REACT_THROW);
			return *mOpenALAudioBuffer;
		}

		inline bool OpenALStream::isLooping() const
		{
			return mLooping;
		}

		inline void OpenALStream::setLooping(bool looping)
		{
			mLooping = looping;
		}


		//[-------------------------------------------------------]
		//[ Protected methods                                     ]
		//[-------------------------------------------------------]
		inline OpenALStream::OpenALStream(ALuint openALSource, const OpenALAudioBuffer& openALAudioBuffer) :
			mOpenALSource(openALSource),
			mOpenALAudioBuffer(&openALAudioBuffer),
			mLooping(false)
		{
			// Nothing to do in here
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // openal
} // qsf

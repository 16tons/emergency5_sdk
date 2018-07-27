// Copyright (C) 2012-2017 Promotion Software GmbH


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
		inline OpenALAudioBuffer::~OpenALAudioBuffer()
		{
			unloadBuffer();
		}

		inline ALuint OpenALAudioBuffer::getOpenALBuffer() const
		{
			return mOpenALBuffer;
		}

		inline std::string OpenALAudioBuffer::getFilename() const
		{
			return mFilename;
		}

		inline const uint8* OpenALAudioBuffer::getData() const
		{
			return mData;
		}

		inline uint32 OpenALAudioBuffer::getDataSize() const
		{
			return mDataSize;
		}


		//[-------------------------------------------------------]
		//[ Public virtual qsf::AudioBuffer methods               ]
		//[-------------------------------------------------------]
		inline bool OpenALAudioBuffer::isLoaded() const
		{
			return (0 != mOpenALBuffer || mStream);
		}

		inline bool OpenALAudioBuffer::isStreamed() const
		{
			return mStream;
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		inline OpenALAudioBuffer::OpenALAudioBuffer(AudioManager& audioManager) :
			AudioBuffer(audioManager),
			mStream(false),
			mOpenALBuffer(0),
			mData(nullptr),
			mDataSize(0),
			mManualStreamSampleFactory(nullptr),
			mHasDuration(false)
		{
			// Nothing here
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // openal
} // qsf

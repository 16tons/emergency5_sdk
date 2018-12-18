// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline const AudioBuffer* AudioSource::getAudioBuffer() const
	{
		return mAudioBuffer;
	}

	inline uint32 AudioSource::getFlags() const
	{
		return mFlags;
	}

	inline void AudioSource::setFlags(uint32 flags)
	{
		mFlags = flags;
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::AudioSource methods               ]
	//[-------------------------------------------------------]
	inline bool AudioSource::load(const AudioBuffer* audioBuffer)
	{
		// Unload the previous audio buffer
		unload();

		mAudioBuffer = audioBuffer;

		// Done
		return true;
	}

	inline void AudioSource::unload()
	{
		mAudioBuffer = nullptr;
	}


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	inline AudioSource::AudioSource(AudioManager& audioManager) :
		AudioResource(audioManager, AudioResource::SOURCE_TYPE),
		mFlags(0),
		mAudioBuffer(nullptr)
	{
		// Nothing to do in here
	}

	inline AudioSource::~AudioSource()
	{
		unload();
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf

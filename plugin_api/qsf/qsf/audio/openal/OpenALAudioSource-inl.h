// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace openal
	{


		//[-------------------------------------------------------]
		//[ Public virtual qsf::AudioSource methods               ]
		//[-------------------------------------------------------]
		inline bool OpenALAudioSource::isSpatial() const
		{
			return mSpatial;
		}

		inline bool OpenALAudioSource::isLooping() const
		{
			return mLooping;
		}

		inline float OpenALAudioSource::getPitch() const
		{
			return mPitch;
		}

		inline float OpenALAudioSource::getReferenceDistance() const
		{
			return mReferenceDistance;
		}

		inline float OpenALAudioSource::getMaximumDistance() const
		{
			return mMaximumDistance;
		}

		inline float OpenALAudioSource::getRolloffFactor() const
		{
			return mRolloffFactor;
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // openal
} // qsf

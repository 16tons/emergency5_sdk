// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace null
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		inline NullAudioSource::~NullAudioSource()
		{
			// Nothing here
		}


		//[-------------------------------------------------------]
		//[ Public virtual qsf::AudioSource methods               ]
		//[-------------------------------------------------------]
		inline bool NullAudioSource::isSpatial() const
		{
			return false;
		}

		inline bool NullAudioSource::isLooping() const
		{
			return false;
		}

		inline float NullAudioSource::getPitch() const
		{
			return 0.0f;
		}

		inline float NullAudioSource::getReferenceDistance() const
		{
			return 0.0f;
		}

		inline float NullAudioSource::getMaximumDistance() const
		{
			return 0.0f;
		}

		inline float NullAudioSource::getRolloffFactor() const
		{
			return 0.0f;
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // null
} // qsf

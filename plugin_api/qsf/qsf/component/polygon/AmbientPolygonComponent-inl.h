// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline AmbientPolygonComponent::AmbientPolygonComponent(Prototype* prototype) :
		PolygonComponent(prototype),
		mAmbientAudioManagement(nullptr),
		mEmitter(nullptr),
		mVolume(1.0f),
		mMaximumVolume(1.0f),
		mCurrentVolume(1.0f),
		mOnTime(Time::fromHours(0.0f)),
		mOffTime(Time::fromHours(0.0f))
	{
		// Nothing to do in here
	}

	inline AmbientPolygonComponent::~AmbientPolygonComponent()
	{
		// Nothing to do in here
	}

	inline const AssetProxy& AmbientPolygonComponent::getAmbientSoundAsset() const
	{
		return mAmbientSoundAssetProxy;
	}

	inline float AmbientPolygonComponent::getVolume() const
	{
		return mVolume;
	}

	inline void AmbientPolygonComponent::setVolume(float newVolume)
	{
		mVolume = newVolume;
	}

	inline float AmbientPolygonComponent::getMaximumVolume() const
	{
		return mMaximumVolume;
	}

	inline void AmbientPolygonComponent::setMaximumVolume(float newMaximumVolume)
	{
		mMaximumVolume = newMaximumVolume;
	}

	inline const Time& AmbientPolygonComponent::getOnTime() const
	{
		return mOnTime;
	}

	inline void AmbientPolygonComponent::setOnTime(const qsf::Time& time)
	{
		mOnTime = time;
	}

	inline const Time& AmbientPolygonComponent::getOffTime() const
	{
		return mOffTime;
	}

	inline void AmbientPolygonComponent::setOffTime(const qsf::Time& time)
	{
		mOffTime = time;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf

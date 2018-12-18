// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline SunComponent::SunComponent(Prototype* prototype) :
		LightComponent(prototype),
		mTime(12.0f, 0.0f),
		mSunriseTime(6.0f, 0.0f),
		mSunsetTime(22.0f, 0.0f),
		mMoonPhase(0.0f),
		mEastDirection(0.0f),
		mAngleOfIncidence(15.0f),
		mOgreLight(nullptr),
		mOgreSceneNode(nullptr)
	{
		// Nothing to do in here
	}

	inline const glm::vec2& SunComponent::getTime() const
	{
		return mTime;
	}

	inline const glm::vec2& SunComponent::getSunriseTime() const
	{
		return mSunriseTime;
	}

	inline const glm::vec2& SunComponent::getSunsetTime() const
	{
		return mSunsetTime;
	}

	inline float SunComponent::getMoonPhase() const
	{
		return mMoonPhase;
	}

	inline float SunComponent::getEastDirection() const
	{
		return mEastDirection;
	}

	inline float SunComponent::getAngleOfIncidence() const
	{
		return mAngleOfIncidence;
	}

	inline SunComponent::~SunComponent()
	{
		// Nothing to do in here
	}

	inline Ogre::Light* SunComponent::getOgreLight() const
	{
		return mOgreLight;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf

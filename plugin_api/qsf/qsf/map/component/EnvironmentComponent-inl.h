// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline EnvironmentComponent::EnvironmentComponent(Prototype* prototype) :
		Component(prototype),
		// Light
		mAmbientColor(0.2f, 0.2f, 0.2f),
		mAmbientTemperature(6600.0f),
		// Fog
		mFogDiffuseColor(0.73f, 0.73f, 0.73f),
		mAtmosphereFogStart(300.0f),
		mAtmosphereFogDensity(0.002f),
		mGroundFogStart(50.0f),
		mGroundFogDensity(0.008f),
		mGroundFogHeightOffset(50.0f),
		mGroundFogHeightDensity(0.008f),
		// Other
		mWindDirection(0.0f)
	{
		// Nothing to do in here
	}

	inline EnvironmentComponent::~EnvironmentComponent()
	{
		// Nothing to do in here
	}

	inline const Color3& EnvironmentComponent::getAmbientColor() const
	{
		return mAmbientColor;
	}

	inline float EnvironmentComponent::getAmbientTemperature() const
	{
		return mAmbientTemperature;
	}

	inline void EnvironmentComponent::setAmbientTemperature(float ambientTemperature)
	{
		assignAndPromoteChange(mAmbientTemperature, ambientTemperature, AMBIENT_TEMPERATURE);
	}

	inline const Color3& EnvironmentComponent::getFogDiffuseColor() const
	{
		return mFogDiffuseColor;
	}

	inline void EnvironmentComponent::setFogDiffuseColor(const Color3& fogDiffuseColor)
	{
		assignAndPromoteChange(mFogDiffuseColor, fogDiffuseColor, FOG_DIFFUSE_COLOR);
	}

	inline float EnvironmentComponent::getAtmosphereFogStart() const
	{
		return mAtmosphereFogStart;
	}

	inline void EnvironmentComponent::setAtmosphereFogStart(float atmosphereFogStart)
	{
		assignAndPromoteChange(mAtmosphereFogStart, atmosphereFogStart, ATMOSPHERE_FOG_START);
	}

	inline float EnvironmentComponent::getAtmosphereFogDensity() const
	{
		return mAtmosphereFogDensity;
	}

	inline void EnvironmentComponent::setAtmosphereFogDensity(float atmosphereFogDensity)
	{
		assignAndPromoteChange(mAtmosphereFogDensity, atmosphereFogDensity, ATMOSPHERE_FOG_DENSITY);
	}

	inline float EnvironmentComponent::getGroundFogStart() const
	{
		return mGroundFogStart;
	}

	inline void EnvironmentComponent::setGroundFogStart(float groundFogStart)
	{
		assignAndPromoteChange(mGroundFogStart, groundFogStart, GROUND_FOG_START);
	}

	inline float EnvironmentComponent::getGroundFogDensity() const
	{
		return mGroundFogDensity;
	}

	inline void EnvironmentComponent::setGroundFogDensity(float groundFogDensity)
	{
		assignAndPromoteChange(mGroundFogDensity, groundFogDensity, GROUND_FOG_DENSITY);
	}

	inline float EnvironmentComponent::getGroundFogHeightOffset() const
	{
		return mGroundFogHeightOffset;
	}

	inline void EnvironmentComponent::setGroundFogHeightOffset(float groundFogHeightOffset)
	{
		assignAndPromoteChange(mGroundFogHeightOffset, groundFogHeightOffset, GROUND_FOG_HEIGHT_OFFSET);
	}

	inline float EnvironmentComponent::getGroundFogHeightDensity() const
	{
		return mGroundFogHeightDensity;
	}

	inline void EnvironmentComponent::setGroundFogHeightDensity(float groundFogHeightDensity)
	{
		assignAndPromoteChange(mGroundFogHeightDensity, groundFogHeightDensity, GROUND_FOG_HEIGHT_DENSITY);
	}

	inline float EnvironmentComponent::getWindDirection() const
	{
		return mWindDirection;
	}

	inline void EnvironmentComponent::setWindDirection(float windDirection)
	{
		assignAndPromoteChange(mWindDirection, windDirection, WIND_DIRECTION);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf

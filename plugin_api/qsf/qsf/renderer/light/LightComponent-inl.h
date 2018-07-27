// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline LightComponent::LightType LightComponent::getLightType() const
	{
		return mLightType;
	}

	inline const Color3& LightComponent::getDiffuseColor() const
	{
		return mDiffuseColor;
	}

	inline void LightComponent::setDiffuseColor(const Color3& diffuseColor)
	{
		// Do not clamp the value
		// -> When using HDR, color values above 1.0 (floating point representation) are valid
		// -> Negative values might have some use for the user
		assignAndPromoteChange(mDiffuseColor, diffuseColor, DIFFUSE_COLOR);
	}

	inline float LightComponent::getDiffuseIntensity() const
	{
		return mDiffuseIntensity;
	}

	inline void LightComponent::setDiffuseIntensity(float diffuseIntensity)
	{
		// Do not clamp the value
		// -> When using HDR, color values above 1.0 (floating point representation) are valid
		// -> Negative values might have some use for the user
		assignAndPromoteChange(mDiffuseIntensity, diffuseIntensity, DIFFUSE_INTENSITY);
	}

	inline float LightComponent::getDiffuseTemperature() const
	{
		return mDiffuseTemperature;
	}

	inline void LightComponent::setDiffuseTemperature(float diffuseTemperature)
	{
		assignAndPromoteChange(mDiffuseTemperature, diffuseTemperature, DIFFUSE_TEMPERATURE);
	}

	inline const AssetProxy& LightComponent::getTexture() const
	{
		return mTexture;
	}

	inline void LightComponent::setTexture(const AssetProxy& texture)
	{
		assignAndPromoteChange(mTexture, texture, TEXTURE);
	}

	inline float LightComponent::getRange() const
	{
		return mRange;
	}

	inline float LightComponent::getFalloffSmooth() const
	{
		return mFalloffSmooth;
	}

	inline void LightComponent::setFalloffSmooth(float falloffSmooth)
	{
		assignAndPromoteChange(mFalloffSmooth, falloffSmooth, FALLOFF_SMOOTH);
	}

	inline float LightComponent::getSpotlightOuterAngle() const
	{
		return mSpotlightOuterAngle;
	}

	inline float LightComponent::getSpotlightInnerAngle() const
	{
		return mSpotlightInnerAngle;
	}

	inline float LightComponent::getSpotlightFalloff() const
	{
		return mSpotlightFalloff;
	}

	inline void LightComponent::setSpotlightFalloff(float spotlightFalloff)
	{
		assignAndPromoteChange(mSpotlightFalloff, spotlightFalloff, SPOTLIGHT_FALLOFF);
	}

	inline float LightComponent::getSpotlightNearClipDistance() const
	{
		return mSpotlightNearClipDistance;
	}

	inline float LightComponent::getMaxSpotlightAngle() const
	{
		return mMaxSpotlightAngle;
	}

	inline void LightComponent::setMaxSpotlightAngle(float maxAngle)
	{
		mMaxSpotlightAngle = maxAngle;
	}

	inline float LightComponent::getShadowFarDistance() const
	{
		return mShadowFarDistance;
	}

	inline void LightComponent::setShadowFarDistance(float shadowFarDistance)
	{
		assignAndPromoteChange(mShadowFarDistance, shadowFarDistance, SHADOW_FAR_DISTANCE);
	}

	inline float LightComponent::getShadowNearClipDistance() const
	{
		return mShadowNearClipDistance;
	}

	inline void LightComponent::setShadowNearClipDistance(float shadowNearClipDistance)
	{
		assignAndPromoteChange(mShadowNearClipDistance, shadowNearClipDistance, SHADOW_NEAR_CLIP_DISTANCE);
	}

	inline float LightComponent::getShadowFarClipDistance() const
	{
		return mShadowFarClipDistance;
	}

	inline void LightComponent::setShadowFarClipDistance(float shadowFarClipDistance)
	{
		assignAndPromoteChange(mShadowFarClipDistance, shadowFarClipDistance, SHADOW_FAR_CLIP_DISTANCE);
	}

	inline const glm::vec3& LightComponent::getWorldSpacePosition() const
	{
		return mWorldSpacePosition;
	}

	inline const glm::quat& LightComponent::getWorldSpaceRotation() const
	{
		return mWorldSpaceRotation;
	}

	inline void LightComponent::setWorldSpaceRotation(const glm::quat& worldSpaceRotation)
	{
		mWorldSpaceRotation = worldSpaceRotation;
	}

	inline float LightComponent::getIntensity() const
	{
		return mIntensity;
	}

	inline void LightComponent::setIntensity(float intensity)
	{
		mIntensity = intensity;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf

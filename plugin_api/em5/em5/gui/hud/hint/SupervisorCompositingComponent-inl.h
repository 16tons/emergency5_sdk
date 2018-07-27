// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline SupervisorCompositingComponent::~SupervisorCompositingComponent()
	{
		// Nothing to do in here
	}

	inline float SupervisorCompositingComponent::getGlobalAnimationSpeed() const
	{
		return mGlobalAnimationSpeed;
	}

	inline void SupervisorCompositingComponent::setGlobalAnimationSpeed(float globalAnimationSpeed)
	{
		assignAndPromoteChange(mGlobalAnimationSpeed, globalAnimationSpeed, GLOBAL_ANIMATION_SPEED);
	}

	inline float SupervisorCompositingComponent::getFireAnimationSpeed() const
	{
		return mFireAnimationSpeed;
	}

	inline void SupervisorCompositingComponent::setFireAnimationSpeed(float fireAnimationSpeed)
	{
		assignAndPromoteChange(mFireAnimationSpeed, fireAnimationSpeed, FIRE_ANIMATION_SPEED);
	}

	inline float SupervisorCompositingComponent::getWaterAnimationSpeed() const
	{
		return mWaterAnimationSpeed;
	}

	inline void SupervisorCompositingComponent::setWaterAnimationSpeed(float waterAnimationSpeed)
	{
		assignAndPromoteChange(mWaterAnimationSpeed, waterAnimationSpeed, WATER_ANIMATION_SPEED);
	}

	inline float SupervisorCompositingComponent::getVegetationAnimationSpeed() const
	{
		return mVegetationAnimationSpeed;
	}

	inline void SupervisorCompositingComponent::setVegetationAnimationSpeed(float vegetationAnimationSpeed)
	{
		assignAndPromoteChange(mVegetationAnimationSpeed, vegetationAnimationSpeed, VEGETATION_ANIMATION_SPEED);
	}

	inline float SupervisorCompositingComponent::getVegetationAnimationFrequency() const
	{
		return mVegetationAnimationFrequency;
	}

	inline void SupervisorCompositingComponent::setVegetationAnimationFrequency(float vegetationAnimationFrequency)
	{
		assignAndPromoteChange(mVegetationAnimationFrequency, vegetationAnimationFrequency, VEGETATION_ANIMATION_FREQUENCY);
	}

	inline float SupervisorCompositingComponent::getVegetationAnimationWeight() const
	{
		return mVegetationAnimationWeight;
	}

	inline void SupervisorCompositingComponent::setVegetationAnimationWeight(float vegetationAnimationWeight)
	{
		assignAndPromoteChange(mVegetationAnimationWeight, vegetationAnimationWeight, VEGETATION_ANIMATION_WEIGHT);
	}

	inline float SupervisorCompositingComponent::getVegetationAnimationBending() const
	{
		return mVegetationAnimationBending;
	}

	inline void SupervisorCompositingComponent::setVegetationAnimationBending(float vegetationAnimationBending)
	{
		assignAndPromoteChange(mVegetationAnimationBending, vegetationAnimationBending, VEGETATION_ANIMATION_BENDING);
	}

	inline float SupervisorCompositingComponent::getVegetationAnimationIndividuality() const
	{
		return mVegetationAnimationIndividuality;
	}

	inline void SupervisorCompositingComponent::setVegetationAnimationIndividuality(float vegetationAnimationIndividuality)
	{
		assignAndPromoteChange(mVegetationAnimationIndividuality, vegetationAnimationIndividuality, VEGETATION_ANIMATION_INDIVIDUALITY);
	}

	inline float SupervisorCompositingComponent::getVegetationAnimationCrosswind() const
	{
		return mVegetationAnimationCrosswind;
	}

	inline void SupervisorCompositingComponent::setVegetationAnimationCrosswind(float vegetationAnimationCrosswind)
	{
		assignAndPromoteChange(mVegetationAnimationCrosswind, vegetationAnimationCrosswind, VEGETATION_ANIMATION_CROSSWIND);
	}

	inline const qsf::AssetProxy& SupervisorCompositingComponent::getGlobalReflectionMap0() const
	{
		return mGlobalReflectionMap0;
	}

	inline void SupervisorCompositingComponent::setGlobalReflectionMap0(const qsf::AssetProxy& globalReflectionMap0)
	{
		assignAndPromoteChange(mGlobalReflectionMap0, globalReflectionMap0, GLOBAL_REFLECTION_MAP_0);
	}

	inline float SupervisorCompositingComponent::getGlobalReflectionMap0Intensity() const
	{
		return mGlobalReflectionMap0Intensity;
	}

	inline void SupervisorCompositingComponent::setGlobalReflectionMap0Intensity(float globalReflectionMap0Intensity)
	{
		assignAndPromoteChange(mGlobalReflectionMap0Intensity, globalReflectionMap0Intensity, GLOBAL_REFLECTION_MAP_0_INTENSITY);
	}

	inline const qsf::AssetProxy& SupervisorCompositingComponent::getGlobalReflectionMap1() const
	{
		return mGlobalReflectionMap1;
	}

	inline void SupervisorCompositingComponent::setGlobalReflectionMap1(const qsf::AssetProxy& globalReflectionMap1)
	{
		assignAndPromoteChange(mGlobalReflectionMap1, globalReflectionMap1, GLOBAL_REFLECTION_MAP_1);
	}

	inline float SupervisorCompositingComponent::getGlobalReflectionMap1Intensity() const
	{
		return mGlobalReflectionMap1Intensity;
	}

	inline void SupervisorCompositingComponent::setGlobalReflectionMap1Intensity(float globalReflectionMap1Intensity)
	{
		assignAndPromoteChange(mGlobalReflectionMap1Intensity, globalReflectionMap1Intensity, GLOBAL_REFLECTION_MAP_1_INTENSITY);
	}

	inline float SupervisorCompositingComponent::getGlobalReflectionMapFactor() const
	{
		return mGlobalReflectionMapFactor;
	}

	inline void SupervisorCompositingComponent::setGlobalReflectionMapFactor(float globalReflectionMapFactor)
	{
		assignAndPromoteChange(mGlobalReflectionMapFactor, globalReflectionMapFactor, GLOBAL_REFLECTION_MAP_FACTOR);
	}

	inline float SupervisorCompositingComponent::getGlobalReflectionSaturation() const
	{
		return mGlobalReflectionSaturation;
	}

	inline void SupervisorCompositingComponent::setGlobalReflectionSaturation(float globalReflectionSaturation)
	{
		assignAndPromoteChange(mGlobalReflectionSaturation, globalReflectionSaturation, GLOBAL_REFLECTION_SATURATION);
	}

	inline bool SupervisorCompositingComponent::getFoggedSky() const
	{
		return mFoggedSky;
	}

	inline void SupervisorCompositingComponent::setFoggedSky(bool foggedSky)
	{
		assignAndPromoteChange(mFoggedSky, foggedSky, FOGGED_SKY);
	}

	inline float SupervisorCompositingComponent::getGlobalSpecularIntensity() const
	{
		return mGlobalSpecularIntensity;
	}

	inline void SupervisorCompositingComponent::setGlobalSpecularIntensity(float globalSpecularIntensity)
	{
		assignAndPromoteChange(mGlobalSpecularIntensity, globalSpecularIntensity, GLOBAL_SPECULAR_INTENSITY);
	}

	inline float SupervisorCompositingComponent::getGlobalGlossinessIntensity() const
	{
		return mGlobalGlossinessIntensity;
	}

	inline void SupervisorCompositingComponent::setGlobalGlossinessIntensity(float globalGlossinessIntensity)
	{
		assignAndPromoteChange(mGlobalGlossinessIntensity, globalGlossinessIntensity, GLOBAL_GLOSSINESS_INTENSITY);
	}

	inline float SupervisorCompositingComponent::getGlobalSsaoIntensity() const
	{
		return mGlobalSsaoIntensity;
	}

	inline void SupervisorCompositingComponent::setGlobalSsaoIntensity(float globalSsaoIntensity)
	{
		assignAndPromoteChange(mGlobalSsaoIntensity, globalSsaoIntensity, GLOBAL_SSAO_INTENSITY);
	}

	inline float SupervisorCompositingComponent::getCloudsIntensity() const
	{
		return mCloudsIntensity;
	}

	inline void SupervisorCompositingComponent::setCloudsIntensity(float cloudsIntensity)
	{
		assignAndPromoteChange(mCloudsIntensity, cloudsIntensity, CLOUDS_INTENSITY);
	}

	inline float SupervisorCompositingComponent::getHdrExposure() const
	{
		return mHdrExposure;
	}

	inline void SupervisorCompositingComponent::setHdrExposure(float hdrExposure)
	{
		assignAndPromoteChange(mHdrExposure, hdrExposure, HDR_EXPOSURE);
	}

	inline float SupervisorCompositingComponent::getHdrKey() const
	{
		return mHdrKey;
	}

	inline void SupervisorCompositingComponent::setHdrKey(float hdrKey)
	{
		assignAndPromoteChange(mHdrKey, hdrKey, HDR_KEY);
	}

	inline const qsf::Color3& SupervisorCompositingComponent::getDepthGradingNearColor() const
	{
		return mDepthGradingNearColor;
	}

	inline void SupervisorCompositingComponent::setDepthGradingNearColor(const qsf::Color3& depthGradingNearColor)
	{
		assignAndPromoteChange(mDepthGradingNearColor, depthGradingNearColor, DEPTH_GRADING_NEAR_COLOR);
	}

	inline const qsf::Color3& SupervisorCompositingComponent::getDepthGradingFarColor() const
	{
		return mDepthGradingFarColor;
	}

	inline void SupervisorCompositingComponent::setDepthGradingFarColor(const qsf::Color3& depthGradingFarColor)
	{
		assignAndPromoteChange(mDepthGradingFarColor, depthGradingFarColor, DEPTH_GRADING_FAR_COLOR);
	}

	inline float SupervisorCompositingComponent::getLensBlendIntensity() const
	{
		return mLensBlendIntensity;
	}

	inline void SupervisorCompositingComponent::setLensBlendIntensity(float lensBlendIntensity)
	{
		assignAndPromoteChange(mLensBlendIntensity, lensBlendIntensity, LENS_BLEND_INTENSITY);
	}

	inline float SupervisorCompositingComponent::getBloomIntensity() const
	{
		return mBloomIntensity;
	}

	inline void SupervisorCompositingComponent::setBloomIntensity(float bloomIntensity)
	{
		assignAndPromoteChange(mBloomIntensity, bloomIntensity, BLOOM_INTENSITY);
	}

	inline float SupervisorCompositingComponent::getVignetteRadius() const
	{
		return mVignetteRadius;
	}

	inline void SupervisorCompositingComponent::setVignetteRadius(float vignetteRadius)
	{
		assignAndPromoteChange(mVignetteRadius, vignetteRadius, VIGNETTE_RADIUS);
	}

	inline float SupervisorCompositingComponent::getVignetteDarkness() const
	{
		return mVignetteDarkness;
	}

	inline void SupervisorCompositingComponent::setVignetteDarkness(float vignetteDarkness)
	{
		assignAndPromoteChange(mVignetteDarkness, vignetteDarkness, VIGNETTE_DARKNESS);
	}

	inline const qsf::AssetProxy& SupervisorCompositingComponent::getColorGradingLut0() const
	{
		return mColorGradingLut0;
	}

	inline void SupervisorCompositingComponent::setColorGradingLut0(const qsf::AssetProxy& colorGradingLut0)
	{
		assignAndPromoteChange(mColorGradingLut0, colorGradingLut0, COLOR_GRADING_LUT_0);
	}

	inline const qsf::AssetProxy& SupervisorCompositingComponent::getColorGradingLut1() const
	{
		return mColorGradingLut1;
	}

	inline void SupervisorCompositingComponent::setColorGradingLut1(const qsf::AssetProxy& colorGradingLut1)
	{
		assignAndPromoteChange(mColorGradingLut1, colorGradingLut1, COLOR_GRADING_LUT_1);
	}

	inline float SupervisorCompositingComponent::getColorGradingLutFactor() const
	{
		return mColorGradingLutFactor;
	}

	inline void SupervisorCompositingComponent::setColorGradingLutFactor(float colorGradingLutFactor)
	{
		assignAndPromoteChange(mColorGradingLutFactor, colorGradingLutFactor, COLOR_GRADING_LUT_FACTOR);
	}

	inline float SupervisorCompositingComponent::getColorGradingBlend() const
	{
		return mColorGradingBlend;
	}

	inline void SupervisorCompositingComponent::setColorGradingBlend(float colorGradingBlend)
	{
		assignAndPromoteChange(mColorGradingBlend, colorGradingBlend, COLOR_GRADING_BLEND);
	}

	inline const qsf::Color3& SupervisorCompositingComponent::getBlendColor() const
	{
		return mBlendColor;
	}

	inline void SupervisorCompositingComponent::setBlendColor(const qsf::Color3& blendColor)
	{
		assignAndPromoteChange(mBlendColor, blendColor, BLEND_COLOR);
	}

	inline float SupervisorCompositingComponent::getBlendIntensity() const
	{
		return mBlendIntensity;
	}

	inline void SupervisorCompositingComponent::setBlendIntensity(float blendIntensity)
	{
		assignAndPromoteChange(mBlendIntensity, blendIntensity, BLEND_INTENSITY);
	}

	// Rain/wet surfaces
	inline bool SupervisorCompositingComponent::isWetSurfacesEnabled() const
	{
		return mWetSurfacesEnabled;
	}

	inline void SupervisorCompositingComponent::setWetSurfacesEnabled(bool wetSurfacesEnabled)
	{
		assignAndPromoteChange(mWetSurfacesEnabled, wetSurfacesEnabled, WET_SURFACES_ENABLED);
	}

	inline float SupervisorCompositingComponent::getWetSurfacesIntensity() const
	{
		return mWetSurfacesIntensity;
	}

	inline void SupervisorCompositingComponent::setWetSurfacesIntensity(float wetSurfacesIntensity)
	{
		assignAndPromoteChange(mWetSurfacesIntensity, wetSurfacesIntensity, WET_SURFACES_INTENSITY);
	}

	inline float SupervisorCompositingComponent::getRainIntensity() const
	{
		return mRainIntensity;
	}

	inline void SupervisorCompositingComponent::setRainIntensity(float rainIntensity)
	{
		assignAndPromoteChange(mRainIntensity, rainIntensity, RAIN_INTENSITY);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace compositing
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		inline DefaultCompositingComponent::~DefaultCompositingComponent()
		{
			// Nothing to do in here
		}

		inline float DefaultCompositingComponent::getGlobalAnimationSpeed() const
		{
			return mGlobalAnimationSpeed;
		}

		inline void DefaultCompositingComponent::setGlobalAnimationSpeed(float globalAnimationSpeed)
		{
			assignAndPromoteChange(mGlobalAnimationSpeed, globalAnimationSpeed, GLOBAL_ANIMATION_SPEED);
		}

		inline float DefaultCompositingComponent::getFireAnimationSpeed() const
		{
			return mFireAnimationSpeed;
		}

		inline void DefaultCompositingComponent::setFireAnimationSpeed(float fireAnimationSpeed)
		{
			assignAndPromoteChange(mFireAnimationSpeed, fireAnimationSpeed, FIRE_ANIMATION_SPEED);
		}

		inline float DefaultCompositingComponent::getWaterAnimationSpeed() const
		{
			return mWaterAnimationSpeed;
		}

		inline void DefaultCompositingComponent::setWaterAnimationSpeed(float waterAnimationSpeed)
		{
			assignAndPromoteChange(mWaterAnimationSpeed, waterAnimationSpeed, WATER_ANIMATION_SPEED);
		}

		inline float DefaultCompositingComponent::getVegetationAnimationSpeed() const
		{
			return mVegetationAnimationSpeed;
		}

		inline void DefaultCompositingComponent::setVegetationAnimationSpeed(float vegetationAnimationSpeed)
		{
			assignAndPromoteChange(mVegetationAnimationSpeed, vegetationAnimationSpeed, VEGETATION_ANIMATION_SPEED);
		}

		inline float DefaultCompositingComponent::getVegetationAnimationFrequency() const
		{
			return mVegetationAnimationFrequency;
		}

		inline void DefaultCompositingComponent::setVegetationAnimationFrequency(float vegetationAnimationFrequency)
		{
			assignAndPromoteChange(mVegetationAnimationFrequency, vegetationAnimationFrequency, VEGETATION_ANIMATION_FREQUENCY);
		}

		inline float DefaultCompositingComponent::getVegetationAnimationWeight() const
		{
			return mVegetationAnimationWeight;
		}

		inline void DefaultCompositingComponent::setVegetationAnimationWeight(float vegetationAnimationWeight)
		{
			assignAndPromoteChange(mVegetationAnimationWeight, vegetationAnimationWeight, VEGETATION_ANIMATION_WEIGHT);
		}

		inline float DefaultCompositingComponent::getVegetationAnimationBending() const
		{
			return mVegetationAnimationBending;
		}

		inline void DefaultCompositingComponent::setVegetationAnimationBending(float vegetationAnimationBending)
		{
			assignAndPromoteChange(mVegetationAnimationBending, vegetationAnimationBending, VEGETATION_ANIMATION_BENDING);
		}

		inline float DefaultCompositingComponent::getVegetationAnimationIndividuality() const
		{
			return mVegetationAnimationIndividuality;
		}

		inline void DefaultCompositingComponent::setVegetationAnimationIndividuality(float vegetationAnimationIndividuality)
		{
			assignAndPromoteChange(mVegetationAnimationIndividuality, vegetationAnimationIndividuality, VEGETATION_ANIMATION_INDIVIDUALITY);
		}

		inline float DefaultCompositingComponent::getVegetationAnimationCrosswind() const
		{
			return mVegetationAnimationCrosswind;
		}

		inline void DefaultCompositingComponent::setVegetationAnimationCrosswind(float vegetationAnimationCrosswind)
		{
			assignAndPromoteChange(mVegetationAnimationCrosswind, vegetationAnimationCrosswind, VEGETATION_ANIMATION_CROSSWIND);
		}

		inline const AssetProxy& DefaultCompositingComponent::getGlobalReflectionMap0() const
		{
			return mGlobalReflectionMap0;
		}

		inline void DefaultCompositingComponent::setGlobalReflectionMap0(const AssetProxy& globalReflectionMap0)
		{
			assignAndPromoteChange(mGlobalReflectionMap0, globalReflectionMap0, GLOBAL_REFLECTION_MAP_0);
		}

		inline float DefaultCompositingComponent::getGlobalReflectionMap0Intensity() const
		{
			return mGlobalReflectionMap0Intensity;
		}

		inline void DefaultCompositingComponent::setGlobalReflectionMap0Intensity(float globalReflectionMap0Intensity)
		{
			assignAndPromoteChange(mGlobalReflectionMap0Intensity, globalReflectionMap0Intensity, GLOBAL_REFLECTION_MAP_0_INTENSITY);
		}

		inline const AssetProxy& DefaultCompositingComponent::getGlobalReflectionMap1() const
		{
			return mGlobalReflectionMap1;
		}

		inline void DefaultCompositingComponent::setGlobalReflectionMap1(const AssetProxy& globalReflectionMap1)
		{
			assignAndPromoteChange(mGlobalReflectionMap1, globalReflectionMap1, GLOBAL_REFLECTION_MAP_1);
		}

		inline float DefaultCompositingComponent::getGlobalReflectionMap1Intensity() const
		{
			return mGlobalReflectionMap1Intensity;
		}

		inline void DefaultCompositingComponent::setGlobalReflectionMap1Intensity(float globalReflectionMap1Intensity)
		{
			assignAndPromoteChange(mGlobalReflectionMap1Intensity, globalReflectionMap1Intensity, GLOBAL_REFLECTION_MAP_1_INTENSITY);
		}

		inline float DefaultCompositingComponent::getGlobalReflectionMapFactor() const
		{
			return mGlobalReflectionMapFactor;
		}

		inline void DefaultCompositingComponent::setGlobalReflectionMapFactor(float globalReflectionMapFactor)
		{
			assignAndPromoteChange(mGlobalReflectionMapFactor, globalReflectionMapFactor, GLOBAL_REFLECTION_MAP_FACTOR);
		}

		inline float DefaultCompositingComponent::getGlobalReflectionSaturation() const
		{
			return mGlobalReflectionSaturation;
		}

		inline void DefaultCompositingComponent::setGlobalReflectionSaturation(float globalReflectionSaturation)
		{
			assignAndPromoteChange(mGlobalReflectionSaturation, globalReflectionSaturation, GLOBAL_REFLECTION_SATURATION);
		}

		inline bool DefaultCompositingComponent::getFoggedSky() const
		{
			return mFoggedSky;
		}

		inline void DefaultCompositingComponent::setFoggedSky(bool foggedSky)
		{
			assignAndPromoteChange(mFoggedSky, foggedSky, FOGGED_SKY);
		}

		inline float DefaultCompositingComponent::getGlobalSpecularIntensity() const
		{
			return mGlobalSpecularIntensity;
		}

		inline void DefaultCompositingComponent::setGlobalSpecularIntensity(float globalSpecularIntensity)
		{
			assignAndPromoteChange(mGlobalSpecularIntensity, globalSpecularIntensity, GLOBAL_SPECULAR_INTENSITY);
		}

		inline float DefaultCompositingComponent::getGlobalGlossinessIntensity() const
		{
			return mGlobalGlossinessIntensity;
		}

		inline void DefaultCompositingComponent::setGlobalGlossinessIntensity(float globalGlossinessIntensity)
		{
			assignAndPromoteChange(mGlobalGlossinessIntensity, globalGlossinessIntensity, GLOBAL_GLOSSINESS_INTENSITY);
		}

		inline float DefaultCompositingComponent::getGlobalSsaoIntensity() const
		{
			return mGlobalSsaoIntensity;
		}

		inline void DefaultCompositingComponent::setGlobalSsaoIntensity(float globalSsaoIntensity)
		{
			assignAndPromoteChange(mGlobalSsaoIntensity, globalSsaoIntensity, GLOBAL_SSAO_INTENSITY);
		}

		inline float DefaultCompositingComponent::getCloudsIntensity() const
		{
			return mCloudsIntensity;
		}

		inline void DefaultCompositingComponent::setCloudsIntensity(float cloudsIntensity)
		{
			assignAndPromoteChange(mCloudsIntensity, cloudsIntensity, CLOUDS_INTENSITY);
		}

		inline float DefaultCompositingComponent::getHdrExposure() const
		{
			return mHdrExposure;
		}

		inline void DefaultCompositingComponent::setHdrExposure(float hdrExposure)
		{
			assignAndPromoteChange(mHdrExposure, hdrExposure, HDR_EXPOSURE);
		}

		inline float DefaultCompositingComponent::getHdrKey() const
		{
			return mHdrKey;
		}

		inline void DefaultCompositingComponent::setHdrKey(float hdrKey)
		{
			assignAndPromoteChange(mHdrKey, hdrKey, HDR_KEY);
		}

		inline const Color3& DefaultCompositingComponent::getDepthGradingNearColor() const
		{
			return mDepthGradingNearColor;
		}

		inline void DefaultCompositingComponent::setDepthGradingNearColor(const Color3& depthGradingNearColor)
		{
			assignAndPromoteChange(mDepthGradingNearColor, depthGradingNearColor, DEPTH_GRADING_NEAR_COLOR);
		}

		inline const Color3& DefaultCompositingComponent::getDepthGradingFarColor() const
		{
			return mDepthGradingFarColor;
		}

		inline void DefaultCompositingComponent::setDepthGradingFarColor(const Color3& depthGradingFarColor)
		{
			assignAndPromoteChange(mDepthGradingFarColor, depthGradingFarColor, DEPTH_GRADING_FAR_COLOR);
		}

		inline float DefaultCompositingComponent::getLensBlendIntensity() const
		{
			return mLensBlendIntensity;
		}

		inline void DefaultCompositingComponent::setLensBlendIntensity(float lensBlendIntensity)
		{
			assignAndPromoteChange(mLensBlendIntensity, lensBlendIntensity, LENS_BLEND_INTENSITY);
		}

		inline float DefaultCompositingComponent::getBloomIntensity() const
		{
			return mBloomIntensity;
		}

		inline void DefaultCompositingComponent::setBloomIntensity(float bloomIntensity)
		{
			assignAndPromoteChange(mBloomIntensity, bloomIntensity, BLOOM_INTENSITY);
		}

		inline float DefaultCompositingComponent::getVignetteRadius() const
		{
			return mVignetteRadius;
		}

		inline void DefaultCompositingComponent::setVignetteRadius(float vignetteRadius)
		{
			assignAndPromoteChange(mVignetteRadius, vignetteRadius, VIGNETTE_RADIUS);
		}

		inline float DefaultCompositingComponent::getVignetteDarkness() const
		{
			return mVignetteDarkness;
		}

		inline void DefaultCompositingComponent::setVignetteDarkness(float vignetteDarkness)
		{
			assignAndPromoteChange(mVignetteDarkness, vignetteDarkness, VIGNETTE_DARKNESS);
		}

		inline const AssetProxy& DefaultCompositingComponent::getColorGradingLut0() const
		{
			return mColorGradingLut0;
		}

		inline void DefaultCompositingComponent::setColorGradingLut0(const AssetProxy& colorGradingLut0)
		{
			assignAndPromoteChange(mColorGradingLut0, colorGradingLut0, COLOR_GRADING_LUT_0);
		}

		inline const AssetProxy& DefaultCompositingComponent::getColorGradingLut1() const
		{
			return mColorGradingLut1;
		}

		inline void DefaultCompositingComponent::setColorGradingLut1(const AssetProxy& colorGradingLut1)
		{
			assignAndPromoteChange(mColorGradingLut1, colorGradingLut1, COLOR_GRADING_LUT_1);
		}

		inline float DefaultCompositingComponent::getColorGradingLutFactor() const
		{
			return mColorGradingLutFactor;
		}

		inline void DefaultCompositingComponent::setColorGradingLutFactor(float colorGradingLutFactor)
		{
			assignAndPromoteChange(mColorGradingLutFactor, colorGradingLutFactor, COLOR_GRADING_LUT_FACTOR);
		}

		inline float DefaultCompositingComponent::getColorGradingBlend() const
		{
			return mColorGradingBlend;
		}

		inline void DefaultCompositingComponent::setColorGradingBlend(float colorGradingBlend)
		{
			assignAndPromoteChange(mColorGradingBlend, colorGradingBlend, COLOR_GRADING_BLEND);
		}

		inline const Color3& DefaultCompositingComponent::getBlendColor() const
		{
			return mBlendColor;
		}

		inline void DefaultCompositingComponent::setBlendColor(const Color3& blendColor)
		{
			assignAndPromoteChange(mBlendColor, blendColor, BLEND_COLOR);
		}

		inline float DefaultCompositingComponent::getBlendIntensity() const
		{
			return mBlendIntensity;
		}

		inline void DefaultCompositingComponent::setBlendIntensity(float blendIntensity)
		{
			assignAndPromoteChange(mBlendIntensity, blendIntensity, BLEND_INTENSITY);
		}

		// Rain/wet surfaces
		inline bool DefaultCompositingComponent::isWetSurfacesEnabled() const
		{
			return mWetSurfacesEnabled;
		}

		inline void DefaultCompositingComponent::setWetSurfacesEnabled(bool wetSurfacesEnabled)
		{
			assignAndPromoteChange(mWetSurfacesEnabled, wetSurfacesEnabled, WET_SURFACES_ENABLED);
		}

		inline float DefaultCompositingComponent::getWetSurfacesIntensity() const
		{
			return mWetSurfacesIntensity;
		}

		inline void DefaultCompositingComponent::setWetSurfacesIntensity(float wetSurfacesIntensity)
		{
			assignAndPromoteChange(mWetSurfacesIntensity, wetSurfacesIntensity, WET_SURFACES_INTENSITY);
		}

		inline float DefaultCompositingComponent::getRainIntensity() const
		{
			return mRainIntensity;
		}

		inline void DefaultCompositingComponent::setRainIntensity(float rainIntensity)
		{
			assignAndPromoteChange(mRainIntensity, rainIntensity, RAIN_INTENSITY);
		}

		inline bool DefaultCompositingComponent::isBorderGroundEnabled() const
		{
			return mBorderGroundEnabled;
		}

		inline bool DefaultCompositingComponent::isBorderWallEnabled() const
		{
			return mBorderWallEnabled;
		}

		inline void DefaultCompositingComponent::setBorderWallEnabled(bool borderWallEnabled)
		{
			if (mBorderWallEnabled != borderWallEnabled)
			{
				mBorderWallEnabled = borderWallEnabled;
				updateBorderMeshVisibility();
				setMapBorderMaterialProperties();
				promotePropertyChange(BORDER_WALL_ENABLED);
			}
		}

		inline float DefaultCompositingComponent::getBorderFadeSpeed() const
		{
			return mBorderFadeSpeed;
		}

		inline void DefaultCompositingComponent::setBorderFadeSpeed(float borderFadeSpeed)
		{
			assignAndPromoteChange(mBorderFadeSpeed, borderFadeSpeed, BORDER_FADE_SPEED);
			setMapBorderMaterialProperties();
		}

		inline float DefaultCompositingComponent::getBorderLineThickness() const
		{
			return mBorderLineThickness;
		}

		inline void DefaultCompositingComponent::setBorderLineThickness(float borderLineThickness)
		{
			assignAndPromoteChange(mBorderLineThickness, borderLineThickness, BORDER_LINE_THICKNESS);
			setMapBorderMaterialProperties();
		}

		inline const Color4& DefaultCompositingComponent::getBorderLineColor() const
		{
			return mBorderLineColor;
		}

		inline void DefaultCompositingComponent::setBorderLineColor(const Color4& borderLineColor)
		{
			assignAndPromoteChange(mBorderLineColor, borderLineColor, BORDER_LINE_COLOR);
			setMapBorderMaterialProperties();
			setMapBorderAmbientMaterialProperties();
		}

		inline float DefaultCompositingComponent::getBorderLineEmissiveIntensity() const
		{
			return mBorderLineEmissiveIntensity;
		}

		inline void DefaultCompositingComponent::setBorderLineEmissiveIntensity(float borderLineEmmissiveIntensity)
		{
			assignAndPromoteChange(mBorderLineEmissiveIntensity, borderLineEmmissiveIntensity, BORDER_LINE_EMISSIVE_INTENSITY);
			setMapBorderMaterialProperties();
			setMapBorderAmbientMaterialProperties();
		}

		inline const Color4& DefaultCompositingComponent::getBorderOuterColor() const
		{
			return mBorderOuterColor;
		}

		inline void DefaultCompositingComponent::setBorderOuterColor(const Color4& borderOuterColor)
		{
			assignAndPromoteChange(mBorderOuterColor, borderOuterColor, BORDER_OUTER_COLOR);
			setMapBorderMaterialProperties();
			setMapBorderAmbientMaterialProperties();
		}

		inline const AssetProxy& DefaultCompositingComponent::getBorderTexture() const
		{
			return mBorderTexture;
		}

		inline void DefaultCompositingComponent::setBorderTexture(const AssetProxy& assetProxy)
		{
			assignAndPromoteChange(mBorderTexture, assetProxy, BORDER_TEXTURE);
			setMapBorderMaterialProperties();
		}

		inline float DefaultCompositingComponent::getBorderTextureScaling() const
		{
			return mBorderTextureScaling;
		}

		inline void DefaultCompositingComponent::setBorderTextureScaling(float borderTextureScaling)
		{
			assignAndPromoteChange(mBorderTextureScaling, borderTextureScaling, BORDER_TEXTURE_SCALING);
			setMapBorderMaterialProperties();
		}

		inline float DefaultCompositingComponent::getBorderTextureScrollSpeedX() const
		{
			return mBorderTextureScrollSpeedX;
		}

		inline void DefaultCompositingComponent::setBorderTextureScrollSpeedX(float borderTextureScrollSpeed)
		{
			assignAndPromoteChange(mBorderTextureScrollSpeedX, borderTextureScrollSpeed, BORDER_TEXTURE_SCROLL_SPEEDX);
			setMapBorderMaterialProperties();
		}

		inline float DefaultCompositingComponent::getBorderTextureScrollSpeedY() const
		{
			return mBorderTextureScrollSpeedY;
		}

		inline void DefaultCompositingComponent::setBorderTextureScrollSpeedY(float borderTextureScrollSpeed)
		{
			assignAndPromoteChange(mBorderTextureScrollSpeedY, borderTextureScrollSpeed, BORDER_TEXTURE_SCROLL_SPEEDY);
			setMapBorderMaterialProperties();
		}

		inline const Color4& DefaultCompositingComponent::getBorderTextureBlendColor() const
		{
			return mBorderTextureBlendColor;
		}

		inline void DefaultCompositingComponent::setBorderTextureBlendColor(const Color4& borderTextureBlendColor)
		{
			assignAndPromoteChange(mBorderTextureBlendColor, borderTextureBlendColor, BORDER_TEXTURE_BLEND_COLOR);
			setMapBorderMaterialProperties();
		}

		inline float DefaultCompositingComponent::getBorderTextureIntensity() const
		{
			return mBorderTextureIntensity;
		}

		inline void DefaultCompositingComponent::setBorderTextureIntensity(float borderTextureIntensity)
		{
			assignAndPromoteChange(mBorderTextureIntensity, borderTextureIntensity, BORDER_TEXTURE_INTENSITY);
			setMapBorderMaterialProperties();
		}

		inline float DefaultCompositingComponent::getBorderTextureEmissiveIntensity() const
		{
			return mBorderTextureEmissiveIntensity;
		}

		inline void DefaultCompositingComponent::setBorderTextureEmissiveIntensity(float borderTextureEmissiveIntensity)
		{
			assignAndPromoteChange(mBorderTextureEmissiveIntensity, borderTextureEmissiveIntensity, BORDER_TEXTURE_EMISSIVE_INTENSITY);
			setMapBorderMaterialProperties();
		}

		inline const AssetProxy& DefaultCompositingComponent::getBorderTexture2() const
		{
			return mBorderTexture2;
		}

		inline void DefaultCompositingComponent::setBorderTexture2(const AssetProxy& assetProxy)
		{
			assignAndPromoteChange(mBorderTexture2, assetProxy, BORDER_TEXTURE2);
			setMapBorderMaterialProperties();
		}

		inline float DefaultCompositingComponent::getBorderTexture2Scaling() const
		{
			return mBorderTexture2Scaling;
		}

		inline void DefaultCompositingComponent::setBorderTexture2Scaling(float borderTextureScaling)
		{
			assignAndPromoteChange(mBorderTexture2Scaling, borderTextureScaling, BORDER_TEXTURE2_SCALING);
			setMapBorderMaterialProperties();
		}

		inline float DefaultCompositingComponent::getBorderTexture2ScrollSpeedX() const
		{
			return mBorderTexture2ScrollSpeedX;
		}

		inline void DefaultCompositingComponent::setBorderTexture2ScrollSpeedX(float borderTextureScrollSpeed)
		{
			assignAndPromoteChange(mBorderTexture2ScrollSpeedX, borderTextureScrollSpeed, BORDER_TEXTURE2_SCROLL_SPEEDX);
			setMapBorderMaterialProperties();
		}

		inline float DefaultCompositingComponent::getBorderTexture2ScrollSpeedY() const
		{
			return mBorderTexture2ScrollSpeedY;
		}

		inline void DefaultCompositingComponent::setBorderTexture2ScrollSpeedY(float borderTextureScrollSpeed)
		{
			assignAndPromoteChange(mBorderTexture2ScrollSpeedY, borderTextureScrollSpeed, BORDER_TEXTURE2_SCROLL_SPEEDY);
			setMapBorderMaterialProperties();
		}

		inline const Color4& DefaultCompositingComponent::getBorderTexture2BlendColor() const
		{
			return mBorderTexture2BlendColor;
		}

		inline void DefaultCompositingComponent::setBorderTexture2BlendColor(const Color4& borderTextureBlendColor)
		{
			assignAndPromoteChange(mBorderTexture2BlendColor, borderTextureBlendColor, BORDER_TEXTURE2_BLEND_COLOR);
			setMapBorderMaterialProperties();
		}

		inline float DefaultCompositingComponent::getBorderTexture2Intensity() const
		{
			return mBorderTexture2Intensity;
		}

		inline void DefaultCompositingComponent::setBorderTexture2Intensity(float borderTextureIntensity)
		{
			assignAndPromoteChange(mBorderTexture2Intensity, borderTextureIntensity, BORDER_TEXTURE2_INTENSITY);
			setMapBorderMaterialProperties();
		}

		inline float DefaultCompositingComponent::getBorderTexture2EmissiveIntensity() const
		{
			return mBorderTexture2EmissiveIntensity;
		}

		inline void DefaultCompositingComponent::setBorderTexture2EmissiveIntensity(float borderTextureEmmissiveIntensity)
		{
			assignAndPromoteChange(mBorderTexture2EmissiveIntensity, borderTextureEmmissiveIntensity, BORDER_TEXTURE2_EMISSIVE_INTENSITY);
			setMapBorderMaterialProperties();
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // compositing
} // qsf

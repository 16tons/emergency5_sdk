// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"

#include <qsf/job/JobProxy.h>
#include <qsf/reflection/type/CampQsfColor3.h>
#include <qsf/reflection/type/CampQsfColor4.h>
#include <qsf/reflection/type/CampQsfAssetProxy.h>
#include <qsf/renderer/compositor/CompositingComponent.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    EMERGENCY 5 supervisor compositing component interface
	*
	*  @note
	*    - Compositor workspace configurator counterpart is "qsf::compositing::DefaultCompositorWorkspaceConfigurator" & "em5::SupervisorCompositorWorkspaceConfigurator"
	*
	*  @todo
	*    - TODO(co) Cleanup
	*/
	class EM5_API_EXPORT SupervisorCompositingComponent : public qsf::CompositingComponent
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 COMPONENT_ID;						///< "qsf::SupervisorCompositingComponent" unique component ID
		// Shader based animation // TODO(co) Find a better home for this properties (environment component is out of the question because this in here is already highly shader specialiced)
		static const uint32 GLOBAL_ANIMATION_SPEED;				///< "GlobalAnimationSpeed" unique class property ID inside the class
		static const uint32 FIRE_ANIMATION_SPEED;				///< "FireAnimationSpeed" unique class property ID inside the class
		static const uint32 WATER_ANIMATION_SPEED;				///< "WaterAnimationSpeed" unique class property ID inside the class
		static const uint32 VEGETATION_ANIMATION_SPEED;			///< "VegetationAnimationSpeed" unique class property ID inside the class
		static const uint32 VEGETATION_ANIMATION_FREQUENCY;		///< "VegetationAnimationFrequency" unique class property ID inside the class
		static const uint32 VEGETATION_ANIMATION_WEIGHT;		///< "VegetationAnimationWeight" unique class property ID inside the class
		static const uint32 VEGETATION_ANIMATION_BENDING;		///< "VegetationAnimationBending" unique class property ID inside the class
		static const uint32 VEGETATION_ANIMATION_INDIVIDUALITY;	///< "VegetationAnimationIndividuality" unique class property ID inside the class
		static const uint32 VEGETATION_ANIMATION_CROSSWIND;		///< "VegetationAnimationCrosswind" unique class property ID inside the class
		// Environment
		static const uint32 GLOBAL_REFLECTION_MAP_0;			///< "GlobalReflectionMap0" unique class property ID inside the class
		static const uint32 GLOBAL_REFLECTION_MAP_0_INTENSITY;	///< "GlobalReflectionMap0Intensity" unique class property ID inside the class
		static const uint32 GLOBAL_REFLECTION_MAP_1;			///< "GlobalReflectionMap1" unique class property ID inside the class
		static const uint32 GLOBAL_REFLECTION_MAP_1_INTENSITY;	///< "GlobalReflectionMap1Intensity" unique class property ID inside the class
		static const uint32 GLOBAL_REFLECTION_MAP_FACTOR;		///< "GlobalReflectionMapFactor" unique class property ID inside the class
		static const uint32 GLOBAL_REFLECTION_SATURATION;		///< "GlobalReflectionSaturation" unique class property ID inside the class
		static const uint32 FOGGED_SKY;							///< "FoggedSky" unique class property ID inside the class
		// Global intensity
		static const uint32 GLOBAL_SPECULAR_INTENSITY;			///< "GlobalSpecularIntensity" unique class property ID inside the class
		static const uint32 GLOBAL_GLOSSINESS_INTENSITY;		///< "GlobalGlossinessIntensity" unique class property ID inside the class
		static const uint32 GLOBAL_SSAO_INTENSITY;				///< "GlobalSsaoIntensity" unique class property ID inside the class
		static const uint32 CLOUDS_INTENSITY;					///< "CloudsIntensity" unique class property ID inside the class
		// HDR
		static const uint32 HDR_EXPOSURE;						///< "Exposure" unique class property ID inside the class
		static const uint32 HDR_KEY;							///< "Key" unique class property ID inside the class
		// Depth grading
		static const uint32 DEPTH_GRADING_NEAR_COLOR;			///< "DepthGradingNearColor" unique class property ID inside the class
		static const qsf::Color3 DEFAULT_DEPTH_GRADING_NEAR_COLOR;	///< Default depth grading near color
		static const uint32 DEPTH_GRADING_FAR_COLOR;			///< "DepthGradingFarColor" unique class property ID inside the class
		static const qsf::Color3 DEFAULT_DEPTH_GRADING_FAR_COLOR;	///< Default depth grading far color
		// Post processing
		static const uint32 LENS_BLEND_INTENSITY;				///< "LensBlendIntensity" unique class property ID inside the class
		static const uint32 BLOOM_INTENSITY;					///< "BloomIntensity" unique class property ID inside the class
		static const uint32 VIGNETTE_RADIUS;					///< "VignetteRadius" unique class property ID inside the class
		static const uint32 VIGNETTE_DARKNESS;					///< "VignetteDarkness" unique class property ID inside the class
		static const uint32 COLOR_GRADING_LUT_0;				///< "ColorGradingLut0" unique class property ID inside the class
		static const uint32 COLOR_GRADING_LUT_1;				///< "ColorGradingLut1" unique class property ID inside the class
		static const uint32 COLOR_GRADING_LUT_FACTOR;			///< "ColorGradingLutFactor" unique class property ID inside the class
		static const uint32 COLOR_GRADING_BLEND;				///< "ColorGradingBlend" unique class property ID inside the class
		static const uint32 BLEND_COLOR;						///< "BlendColor" unique class property ID inside the class
		static const uint32 BLEND_INTENSITY;					///< "BlendIntensity" unique class property ID inside the class
		// Wet surfaces/Rain
		static const uint32 WET_SURFACES_ENABLED;				///< "WetSurfacesEnabled" unique class property ID inside the class
		static const uint32 WET_SURFACES_INTENSITY;				///< "WetSurfacesIntensity" unique class property ID inside the class
		static const uint32 RAIN_INTENSITY;						///< "RainIntensity" unique class property ID inside the class


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] prototype
		*    The prototype this component is in, no null pointer allowed
		*/
		explicit SupervisorCompositingComponent(qsf::Prototype* prototype);

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~SupervisorCompositingComponent();

		// Shader based animation
		inline float getGlobalAnimationSpeed() const;
		inline void setGlobalAnimationSpeed(float globalAnimationSpeed);

		inline float getWaterAnimationSpeed() const;
		inline void setWaterAnimationSpeed(float waterAnimationSpeed);

		inline float getFireAnimationSpeed() const;
		inline void setFireAnimationSpeed(float fireAnimationSpeed);

		inline float getVegetationAnimationSpeed() const;
		inline void setVegetationAnimationSpeed(float vegetationAnimationSpeed);

		inline float getVegetationAnimationFrequency() const;
		inline void setVegetationAnimationFrequency(float vegetationAnimationFrequency);

		inline float getVegetationAnimationWeight() const;
		inline void setVegetationAnimationWeight(float vegetationAnimationWeight);

		inline float getVegetationAnimationBending() const;
		inline void setVegetationAnimationBending(float vegetationAnimationBending);

		inline float getVegetationAnimationIndividuality() const;
		inline void setVegetationAnimationIndividuality(float vegetationAnimationIndividuality);

		inline float getVegetationAnimationCrosswind() const;
		inline void setVegetationAnimationCrosswind(float vegetationAnimationCrosswind);

		// Environment
		inline const qsf::AssetProxy& getGlobalReflectionMap0() const;
		inline void setGlobalReflectionMap0(const qsf::AssetProxy& globalReflectionMap0);
		inline float getGlobalReflectionMap0Intensity() const;
		inline void setGlobalReflectionMap0Intensity(float globalReflectionMap0Intensity);
		inline const qsf::AssetProxy& getGlobalReflectionMap1() const;
		inline void setGlobalReflectionMap1(const qsf::AssetProxy& globalReflectionMap1);
		inline float getGlobalReflectionMap1Intensity() const;
		inline void setGlobalReflectionMap1Intensity(float globalReflectionMap1Intensity);
		inline float getGlobalReflectionMapFactor() const;
		inline void setGlobalReflectionMapFactor(float globalReflectionMapFactor);
		inline float getGlobalReflectionSaturation() const;
		inline void setGlobalReflectionSaturation(float globalReflectionSaturation);
		inline bool getFoggedSky() const;
		inline void setFoggedSky(bool foggedSky);

		// Global intensity
		inline float getGlobalSpecularIntensity() const;
		inline void setGlobalSpecularIntensity(float globalSpecularIntensity);
		inline float getGlobalGlossinessIntensity() const;
		inline void setGlobalGlossinessIntensity(float globalGlossinessIntensity);
		inline float getGlobalSsaoIntensity() const;
		inline void setGlobalSsaoIntensity(float globalSsaoIntensity);
		inline float getCloudsIntensity() const;
		inline void setCloudsIntensity(float cloudsIntensity);

		// HDR
		inline float getHdrExposure() const;
		inline void setHdrExposure(float hdrExposure);
		inline float getHdrKey() const;
		inline void setHdrKey(float hdrKey);

		// Depth grading
		inline const qsf::Color3& getDepthGradingNearColor() const;
		inline void setDepthGradingNearColor(const qsf::Color3& depthGradingNearColor);
		inline const qsf::Color3& getDepthGradingFarColor() const;
		inline void setDepthGradingFarColor(const qsf::Color3& depthGradingFarColor);

		// Post processing
		inline float getLensBlendIntensity() const;
		inline void setLensBlendIntensity(float lensBlendIntensity);
		inline float getBloomIntensity() const;
		inline void setBloomIntensity(float bloomIntensity);
		inline float getVignetteRadius() const;
		inline void setVignetteRadius(float vignetteRadius);
		inline float getVignetteDarkness() const;
		inline void setVignetteDarkness(float vignetteDarkness);
		inline const qsf::AssetProxy& getColorGradingLut0() const;
		inline void setColorGradingLut0(const qsf::AssetProxy& colorGradingLut0);
		inline const qsf::AssetProxy& getColorGradingLut1() const;
		inline void setColorGradingLut1(const qsf::AssetProxy& colorGradingLut1);
		inline float getColorGradingLutFactor() const;
		inline void setColorGradingLutFactor(float colorGradingLutFactor);
		inline float getColorGradingBlend() const;
		inline void setColorGradingBlend(float colorGradingBlend);
		inline const qsf::Color3& getBlendColor() const;
		inline void setBlendColor(const qsf::Color3& blendColor);
		inline float getBlendIntensity() const;
		inline void setBlendIntensity(float blendIntensity);

		// Rain/Wet Surfaces
		inline bool isWetSurfacesEnabled() const;
		inline void setWetSurfacesEnabled(bool enabled);
		inline float getWetSurfacesIntensity() const;
		inline void setWetSurfacesIntensity(float wetSurfacesIntensity);
		inline float getRainIntensity() const;
		inline void setRainIntensity(float rainIntensity);


	//[-------------------------------------------------------]
	//[ Public virtual qsf::CompositingComponent methods      ]
	//[-------------------------------------------------------]
	public:
		virtual std::string getCompositorWorkspaceDefinitionName() const override;
		virtual void onPreCompositorWorkspaceUpdate(const qsf::CameraComponent& cameraComponent) override;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Component methods              ]
	//[-------------------------------------------------------]
	protected:
		inline virtual bool implementsOnComponentPropertyChange() const override  { return true; }


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Connected to the CAMP reflection system
		// Shader based animation
		float mGlobalAnimationSpeed;
		float mFireAnimationSpeed;
		float mWaterAnimationSpeed;
		float mVegetationAnimationSpeed;
		float mVegetationAnimationFrequency;
		float mVegetationAnimationWeight;
		float mVegetationAnimationBending;
		float mVegetationAnimationIndividuality;
		float mVegetationAnimationCrosswind;
		// Environment
		qsf::AssetProxy mGlobalReflectionMap0;
		float	   mGlobalReflectionMap0Intensity;
		qsf::AssetProxy mGlobalReflectionMap1;
		float	   mGlobalReflectionMap1Intensity;
		float	   mGlobalReflectionMapFactor;
		float	   mGlobalReflectionSaturation;
		bool	   mFoggedSky;
		// Global intensity
		float mGlobalSpecularIntensity;
		float mGlobalGlossinessIntensity;
		float mGlobalSsaoIntensity;
		float mCloudsIntensity;
		// HDR
		float mHdrExposure;
		float mHdrKey;
		// Depth grading
		qsf::Color3 mDepthGradingNearColor;	///< Depth grading near color
		qsf::Color3 mDepthGradingFarColor;	///< Depth grading far color
		// Post processing
		float	   mLensBlendIntensity;
		float	   mBloomIntensity;
		float	   mVignetteRadius;
		float	   mVignetteDarkness;
		qsf::AssetProxy mColorGradingLut0;
		qsf::AssetProxy mColorGradingLut1;
		float	   mColorGradingLutFactor;
		float	   mColorGradingBlend;
		qsf::Color3	   mBlendColor;
		float	   mBlendIntensity;
		// Wet surfaces/Rain
		bool  mWetSurfacesEnabled;
		float mWetSurfacesIntensity;
		float mRainIntensity;


	//[-------------------------------------------------------]
	//[ CAMP reflection system                                ]
	//[-------------------------------------------------------]
	QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "em5/gui/hud/hint/SupervisorCompositingComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(em5::SupervisorCompositingComponent)

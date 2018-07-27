// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/job/JobProxy.h"
#include "qsf/reflection/type/CampQsfColor3.h"
#include "qsf/reflection/type/CampQsfColor4.h"
#include "qsf/reflection/type/CampQsfAssetProxy.h"
#include "qsf/renderer/compositor/CompositingComponent.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Ogre
{
	class SceneNode;
	class SceneManager;
	namespace v1
	{
		class Entity;
	}
}
namespace qsf
{
	namespace compositing
	{
		class CompositingSettingsGroup;
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace compositing
	{


		//[-------------------------------------------------------]
		//[ Classes                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Default compositing component interface
		*
		*  @remarks
		*    A default compositing component implementation which is a good start point or
		*    might already fullfill all visual requirements of a project.
		*
		*  @note
		*    - Compositor workspace configurator counterpart is "qsf::compositing::DefaultCompositorWorkspaceConfigurator"
		*/
		class QSF_API_EXPORT DefaultCompositingComponent : public CompositingComponent
		{


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			static const uint32 COMPONENT_ID;						///< "qsf::DefaultCompositingComponent" unique component ID
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
			static const Color3 DEFAULT_DEPTH_GRADING_NEAR_COLOR;	///< Default depth grading near color
			static const uint32 DEPTH_GRADING_FAR_COLOR;			///< "DepthGradingFarColor" unique class property ID inside the class
			static const Color3 DEFAULT_DEPTH_GRADING_FAR_COLOR;	///< Default depth grading far color
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
			// Map border visualization
			static const uint32 BORDER_GROUND_ENABLED;				///< "BorderGroundEnabled" unique property ID
			static const uint32 BORDER_WALL_ENABLED;				///< "BorderWallEnabled" unique property ID
			static const uint32 BORDER_FADE_SPEED;					///< "BorderFadeSpeed" unique property ID
			static const uint32 BORDER_LINE_THICKNESS;				///< "BorderLineThickness" unique property ID
			static const uint32 BORDER_LINE_COLOR;					///< "BorderLineColor" unique property ID
			static const uint32 BORDER_LINE_EMISSIVE_INTENSITY;		///< "BorderLineEmissiveIntensity" unique property ID
			static const uint32 BORDER_OUTER_COLOR;					///< "BorderOuterColor" unique property ID
			static const uint32 BORDER_TEXTURE;						///< "BorderTexture" unique property ID
			static const uint32 BORDER_TEXTURE_SCALING;				///< "BorderTextureScaling" unique property ID
			static const uint32 BORDER_TEXTURE_SCROLL_SPEEDX;		///< "BorderTextureScrollSpeedX" unique property ID
			static const uint32 BORDER_TEXTURE_SCROLL_SPEEDY;		///< "BorderTextureScrollSpeedY" unique property ID
			static const uint32 BORDER_TEXTURE_BLEND_COLOR;			///< "BorderTextureBlendColor" unique property ID
			static const uint32 BORDER_TEXTURE_INTENSITY;			///< "BorderTextureIntensity" unique property ID
			static const uint32 BORDER_TEXTURE_EMISSIVE_INTENSITY;	///< "BorderTextureEmmissiveIntensity" unique property ID
			static const uint32 BORDER_TEXTURE2;					///< "BorderTexture2" unique property ID
			static const uint32 BORDER_TEXTURE2_SCALING;			///< "BorderTexture2Scaling" unique property ID
			static const uint32 BORDER_TEXTURE2_SCROLL_SPEEDX;		///< "BorderTexture2ScrollSpeedX" unique property ID
			static const uint32 BORDER_TEXTURE2_SCROLL_SPEEDY;		///< "BorderTexture2ScrollSpeedY" unique property ID
			static const uint32 BORDER_TEXTURE2_BLEND_COLOR;		///< "BorderTexture2BlendColor" unique property ID
			static const uint32 BORDER_TEXTURE2_INTENSITY;			///< "BorderTexture2Intensity" unique property ID
			static const uint32 BORDER_TEXTURE2_EMISSIVE_INTENSITY;	///< "BorderTexture2EmmissiveIntensity" unique property ID


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
			explicit DefaultCompositingComponent(Prototype* prototype);

			/**
			*  @brief
			*    Destructor
			*/
			inline virtual ~DefaultCompositingComponent();

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
			inline const AssetProxy& getGlobalReflectionMap0() const;
			inline void setGlobalReflectionMap0(const AssetProxy& globalReflectionMap0);
			inline float getGlobalReflectionMap0Intensity() const;
			inline void setGlobalReflectionMap0Intensity(float globalReflectionMap0Intensity);
			inline const AssetProxy& getGlobalReflectionMap1() const;
			inline void setGlobalReflectionMap1(const AssetProxy& globalReflectionMap1);
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
			inline const Color3& getDepthGradingNearColor() const;
			inline void setDepthGradingNearColor(const Color3& depthGradingNearColor);
			inline const Color3& getDepthGradingFarColor() const;
			inline void setDepthGradingFarColor(const Color3& depthGradingFarColor);

			// Post processing
			inline float getLensBlendIntensity() const;
			inline void setLensBlendIntensity(float lensBlendIntensity);
			inline float getBloomIntensity() const;
			inline void setBloomIntensity(float bloomIntensity);
			inline float getVignetteRadius() const;
			inline void setVignetteRadius(float vignetteRadius);
			inline float getVignetteDarkness() const;
			inline void setVignetteDarkness(float vignetteDarkness);
			inline const AssetProxy& getColorGradingLut0() const;
			inline void setColorGradingLut0(const AssetProxy& colorGradingLut0);
			inline const AssetProxy& getColorGradingLut1() const;
			inline void setColorGradingLut1(const AssetProxy& colorGradingLut1);
			inline float getColorGradingLutFactor() const;
			inline void setColorGradingLutFactor(float colorGradingLutFactor);
			inline float getColorGradingBlend() const;
			inline void setColorGradingBlend(float colorGradingBlend);
			inline const Color3& getBlendColor() const;
			inline void setBlendColor(const Color3& blendColor);
			inline float getBlendIntensity() const;
			inline void setBlendIntensity(float blendIntensity);

			// Rain/Wet Surfaces
			inline bool isWetSurfacesEnabled() const;
			inline void setWetSurfacesEnabled(bool enabled);
			inline float getWetSurfacesIntensity() const;
			inline void setWetSurfacesIntensity(float wetSurfacesIntensity);
			inline float getRainIntensity() const;
			inline void setRainIntensity(float rainIntensity);

			// Map border visualization
			inline bool isBorderGroundEnabled() const;
			void setBorderGroundEnabled(bool borderGroundEnabled);
			inline bool isBorderWallEnabled() const;
			inline void setBorderWallEnabled(bool borderWallEnabled);
			inline float getBorderFadeSpeed() const;
			inline void setBorderFadeSpeed(float borderFadeSpeed);
			inline float getBorderLineThickness() const;
			inline void setBorderLineThickness(float borderLineThickness);
			inline const Color4& getBorderLineColor() const;
			inline void setBorderLineColor(const Color4& borderLineColor);
			inline float getBorderLineEmissiveIntensity() const;
			inline void setBorderLineEmissiveIntensity(float borderLineEmmissiveIntensity);
			inline const Color4& getBorderOuterColor() const;
			inline void setBorderOuterColor(const Color4& borderOuterColor);
			inline const AssetProxy& getBorderTexture() const;
			inline void setBorderTexture(const AssetProxy& globalAssetId);
			inline float getBorderTextureScaling() const;
			inline void setBorderTextureScaling(float borderTextureScaling);
			inline float getBorderTextureScrollSpeedX() const;
			inline void setBorderTextureScrollSpeedX(float borderTextureScrollSpeed);
			inline float getBorderTextureScrollSpeedY() const;
			inline void setBorderTextureScrollSpeedY(float borderTextureScrollSpeed);
			inline const Color4& getBorderTextureBlendColor() const;
			inline void setBorderTextureBlendColor(const Color4& borderTextureBlendColor);
			inline float getBorderTextureIntensity() const;
			inline void setBorderTextureIntensity(float borderTextureIntensity);
			inline float getBorderTextureEmissiveIntensity() const;
			inline void setBorderTextureEmissiveIntensity(float borderTextureEmmissiveIntensity);
			inline const AssetProxy& getBorderTexture2() const;
			inline void setBorderTexture2(const AssetProxy& globalAssetId);
			inline float getBorderTexture2Scaling() const;
			inline void setBorderTexture2Scaling(float borderTextureScaling);
			inline float getBorderTexture2ScrollSpeedX() const;
			inline void setBorderTexture2ScrollSpeedX(float borderTextureScrollSpeed);
			inline float getBorderTexture2ScrollSpeedY() const;
			inline void setBorderTexture2ScrollSpeedY(float borderTextureScrollSpeed);
			inline const Color4& getBorderTexture2BlendColor() const;
			inline void setBorderTexture2BlendColor(const Color4& borderTextureBlendColor);
			inline float getBorderTexture2Intensity() const;
			inline void setBorderTexture2Intensity(float borderTextureIntensity);
			inline float getBorderTexture2EmissiveIntensity() const;
			inline void setBorderTexture2EmissiveIntensity(float borderTextureEmmissiveIntensity);


		//[-------------------------------------------------------]
		//[ Public virtual qsf::CompositingComponent methods      ]
		//[-------------------------------------------------------]
		public:
			virtual std::string getCompositorWorkspaceDefinitionName() const override;
			virtual void onPreCompositorWorkspaceUpdate(const CameraComponent& cameraComponent) override;


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::Component methods              ]
		//[-------------------------------------------------------]
		protected:
			inline virtual bool implementsOnComponentPropertyChange() const override  { return true; }
			virtual void onComponentPropertyChange(const Component& component, uint32 propertyId) override;
			virtual void onSetHidden(bool hidden) override;
			//[-------------------------------------------------------]
			//[ Lifecycle                                             ]
			//[-------------------------------------------------------]
			virtual bool onStartup() override;
			virtual void onShutdown() override;


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			/**
			*  @brief
			*    Get the OGRE scene manager instance
			*
			*  @return
			*    The OGRE scene manager instance, can be a null pointer, do no destroy the returned instance
			*/
			Ogre::SceneManager* getOgreSceneManager() const;

			// Map border
			void updateBorderMeshVisibility() const;
			void updateBorderMesh(const glm::vec3& min, const glm::vec3& max);

			//[-------------------------------------------------------]
			//[ Material properties                                   ]
			//[-------------------------------------------------------]
			/**
			*  @brief
			*    Set map border material properties
			*/
			void setMapBorderMaterialProperties() const;
			void setMapBorderMaterialPropertiesInternal() const;
			void setMapBorderAmbientMaterialProperties() const;
			void setMapBorderAmbientMaterialPropertiesInternal() const;


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
			AssetProxy mGlobalReflectionMap0;
			float	   mGlobalReflectionMap0Intensity;
			AssetProxy mGlobalReflectionMap1;
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
			Color3 mDepthGradingNearColor;	///< Depth grading near color
			Color3 mDepthGradingFarColor;	///< Depth grading far color
			// Post processing
			float	   mLensBlendIntensity;
			float	   mBloomIntensity;
			float	   mVignetteRadius;
			float	   mVignetteDarkness;
			AssetProxy mColorGradingLut0;
			AssetProxy mColorGradingLut1;
			float	   mColorGradingLutFactor;
			float	   mColorGradingBlend;
			Color3	   mBlendColor;
			float	   mBlendIntensity;
			// Wet surfaces/Rain
			bool  mWetSurfacesEnabled;
			float mWetSurfacesIntensity;
			float mRainIntensity;
			// Map border visualization
			bool		mBorderGroundEnabled;
			bool		mBorderWallEnabled;
			float		mBorderFadeSpeed;
			float		mBorderLineThickness;
			Color4		mBorderLineColor;
			float		mBorderLineEmissiveIntensity;
			Color4		mBorderOuterColor;
			AssetProxy	mBorderTexture;
			float		mBorderTextureScaling;
			float		mBorderTextureScrollSpeedX;
			float		mBorderTextureScrollSpeedY;
			Color4		mBorderTextureBlendColor;
			float		mBorderTextureIntensity;
			float		mBorderTextureEmissiveIntensity;
			AssetProxy	mBorderTexture2;
			float		mBorderTexture2Scaling;
			float		mBorderTexture2ScrollSpeedX;
			float		mBorderTexture2ScrollSpeedY;
			Color4		mBorderTexture2BlendColor;
			float		mBorderTexture2Intensity;
			float		mBorderTexture2EmissiveIntensity;
			// Internal
			const CompositingSettingsGroup&	mCompositingSettingsGroup;
			glm::vec3						mBorderMin;
			glm::vec3						mBorderMax;
			Ogre::v1::Entity*				mOgreEntityBorder;		///< OGRE entity instance, can be a null pointer
			Ogre::SceneNode*				mOgreSceneNodeBorder;	///< OGRE scene node instance, can be a null pointer


		//[-------------------------------------------------------]
		//[ CAMP reflection system                                ]
		//[-------------------------------------------------------]
		QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // compositing
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/renderer/compositor/DefaultCompositingComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::compositing::DefaultCompositingComponent)

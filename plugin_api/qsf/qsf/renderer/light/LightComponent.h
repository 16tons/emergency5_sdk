// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/reflection/type/CampQsfColor3.h"
#include "qsf/reflection/type/CampQsfAssetProxy.h"
#include "qsf/renderer/component/RendererComponent.h"
#include "qsf/debug/DebugDrawProxy.h"

#include <glm/gtc/quaternion.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class DeferredLight;
	class GridCellSceneNode;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Light component class
	*
	*  @todo
	*    - TODO(co) Remove "qsf::RendererComponent" inheritance?
	*/
	class QSF_API_EXPORT LightComponent : public RendererComponent
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	public:
		friend class OgreCompositorPassDeferredLight;


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 COMPONENT_ID;					///< "qsf::LightComponent" unique component ID
		// Type
		static const uint32 LIGHT_TYPE;						///< "LightType" unique class property ID inside the class
		// Color
		static const uint32 DIFFUSE_COLOR;					///< "DiffuseColor" unique class property ID inside the class
		static const uint32 DIFFUSE_INTENSITY;				///< "DiffuseIntensity" unique class property ID inside the class
		static const uint32 DIFFUSE_TEMPERATURE;			///< "DiffuseTemperature" unique class property ID inside the class
		static const uint32 TEXTURE;						///< "Texture" unique class property ID inside the class
		// Attenuation
		static const uint32 RANGE;							///< "Range" unique class property ID inside the class
		static const uint32 FALLOFF_SMOOTH;					///< "FalloffSmooth" unique class property ID inside the class
		// Spotlight
		static const uint32 SPOTLIGHT_OUTER_ANGLE;			///< "SpotlightOuterAngle" unique class property ID inside the class
		static const uint32 SPOTLIGHT_INNER_ANGLE;			///< "SpotlightInnerAngle" unique class property ID inside the class
		static const uint32 SPOTLIGHT_FALLOFF;				///< "SpotlightFalloff" unique class property ID inside the class
		static const uint32 SPOTLIGHT_NEAR_CLIP_DISTANCE;	///< "SpotlightNearClipDistance" unique class property ID inside the class
		// Shadow
		static const uint32 SHADOW_FAR_DISTANCE;			///< "ShadowFarDistance" unique class property ID inside the class
		static const uint32 SHADOW_NEAR_CLIP_DISTANCE;		///< "ShadowNearClipDistance" unique class property ID inside the class
		static const uint32 SHADOW_FAR_CLIP_DISTANCE;		///< "ShadowFarClipDistance" unique class property ID inside the class


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Light type
		*/
		enum LightType
		{
			POINT_LIGHT		  = 0,	///< Point light sources give off light equally in all directions, so require only position not direction
			DIRECTIONAL_LIGHT = 1,	///< Directional lights simulate parallel light beams from a distant source, hence have direction but no position
			SPOT_LIGHT		  = 2	///< Spot lights simulate a cone of light from a source so require position and direction, plus extra values for falloff
		};


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
		explicit LightComponent(Prototype* prototype);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~LightComponent();

		//[-------------------------------------------------------]
		//[ Type                                                  ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Return the light type
		*
		*  @return
		*    The light type
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline LightType getLightType() const;

		/**
		*  @brief
		*    Set the light type
		*
		*  @param[in] lightType
		*    The light type
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		void setLightType(LightType lightType);

		//[-------------------------------------------------------]
		//[ Color                                                 ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Return the color of the light
		*
		*  @return
		*    The color of the light, do no destroy the returned instance
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline const Color3& getDiffuseColor() const;

		/**
		*  @brief
		*    Set the of the light
		*
		*  @param[in] lightColor
		*    The color of the light
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline void setDiffuseColor(const Color3& lightColor);

		/**
		*  @brief
		*    Return the diffuse intensity of the light
		*
		*  @return
		*    The diffuse intensity of the light
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline float getDiffuseIntensity() const;

		/**
		*  @brief
		*    Set the diffuse intensity of the light
		*
		*  @param[in] diffuseIntensity
		*    The diffuse intensity of the light
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline void setDiffuseIntensity(float diffuseIntensity);

		/**
		*  @brief
		*    Return the diffuse temperature of the light
		*
		*  @return
		*    The diffuse temperature (in Kelvin) of the light
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline float getDiffuseTemperature() const;

		/**
		*  @brief
		*    Set the diffuse temperature of the light
		*
		*  @param[in] diffuseTemperature
		*    The diffuse temperature (in Kelvin) of the light
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline void setDiffuseTemperature(float diffuseTemperature);

		/**
		*  @brief
		*    Return the texture asset to use
		*
		*  @return
		*    The texture asset to use (e.g. "sample/texture/spot/bat"), do no destroy the returned instance
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline const AssetProxy& getTexture() const;

		/**
		*  @brief
		*    Set the texture asset to use
		*
		*  @param[in] texture
		*    The texture asset to use (e.g. "sample/texture/spot/bat")
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline void setTexture(const AssetProxy& texture);

		//[-------------------------------------------------------]
		//[ Attenuation                                           ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Return the absolute upper range of the light in world units
		*
		*  @return
		*    The absolute upper range of the light in world units
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline float getRange() const;

		/**
		*  @brief
		*    Set the absolute upper range of the light in world units
		*
		*  @param[in] range
		*    The absolute upper range of the light in world units
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		void setRange(float falloutSmooth);

		/**
		*  @brief
		*    Get the smoothness of the falloff inside the range
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline float getFalloffSmooth() const;

		/**
		*  @brief
		*    Set the smoothness of the falloff inside the range
		*
		*  @param[in] range
		*    Falloff between 0 and 1
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline void setFalloffSmooth(float falloffSmooth);

		//[-------------------------------------------------------]
		//[ Spotlight                                             ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Return the angle in degree covered by the outer cone of the light
		*
		*  @return
		*    The angle in degree covered by the outer cone of the light
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline float getSpotlightOuterAngle() const;

		/**
		*  @brief
		*    Set the angle in degree covered by the outer cone of the light
		*
		*  @param[in] spotlightOuterAngle
		*    The angle in degree covered by the outer cone of the light
		*	 NOTE: It will be clamped to MaxSpotlightAngle to prevent insane angles.
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		void setSpotlightOuterAngle(float spotlightOuterAngle);

		/**
		*  @brief
		*    Return the angle in degree covered by the bright inner cone of the light
		*
		*  @return
		*    The angle in degree covered by the bright inner cone of the light
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline float getSpotlightInnerAngle() const;

		/**
		*  @brief
		*    Set the angle in degree covered by the bright inner cone of the light
		*
		*  @param[in] spotlightInnerAngle
		*    The angle in degree covered by the bright inner cone of the light.
		*	 NOTE: It will be clamped to MaxSpotlightAngle to prevent insane angles.
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		void setSpotlightInnerAngle(float spotlightInnerAngle);

		/**
		*  @brief
		*    Return the rate of falloff between the inner and outer cones of the light
		*
		*  @return
		*    The rate of falloff between the inner and outer cones of the light. 1.0 means a linear falloff, less means slower falloff, higher means faster falloff.
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline float getSpotlightFalloff() const;

		/**
		*  @brief
		*    Set the rate of falloff between the inner and outer cones of the light
		*
		*  @param[in] spotlightFalloff
		*    The rate of falloff between the inner and outer cones of the light 1.0 means a linear falloff, less means slower falloff, higher means faster falloff.
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline void setSpotlightFalloff(float spotlightFalloff);

		/**
		*  @brief
		*    Return the near distance of the light
		*
		*  @return
		*    The near distance of the light
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*    - Set the near clip plane distance to be used by spotlights that use light clipping, allowing you to render spots as if they start from further down their frustum
		*/
		inline float getSpotlightNearClipDistance() const;

		/**
		*  @brief
		*    Set the near distance of the light
		*
		*  @param[in] range
		*    The near distance of the light
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*    - Set the near clip plane distance to be used by spotlights that use light clipping, allowing you to render spots as if they start from further down their frustum
		*/
		void setSpotlightNearClipDistance(float spotlightNearClipDistance);

		/**
		*  @brief
		*    How large canspotlight angles become?
		*
		*   @param[in] maxAngle
		*    Maximum angle allowed for inner and outer angle. 120° by default.
		*/
		inline float getMaxSpotlightAngle() const;

		/**
		*  @brief
		*    Prevent spotlight angles becoming too large
		*
		*  @param[in] maxAngle
		*    Maximum angle allowed for inner and outer angle. 120° by default.
		*/
		inline void setMaxSpotlightAngle(float maxAngle);

		//[-------------------------------------------------------]
		//[ Shadow                                                ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Return the maximum distance away from the camera that shadows by this light will be visible
		*
		*  @return
		*    The maximum distance away from the camera that shadows by this light will be visible; <0 to use the global setting
		*
		*  @remarks
		*    Shadow techniques can be expensive, therefore it is a good idea to limit them to being rendered close to the camera if possible,
		*    and to skip the expense of rendering shadows for distance objects. This method allows you to set the distance at which shadows
		*    will no longer be rendered.
		*
		*    Each shadow technique can interpret this subtely differently. For example, one technique may use this to eliminate casters, another
		*    might use it to attenuate the shadows themselves. You should tweak this value to suit your chosen shadow technique and scene setup.
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline float getShadowFarDistance() const;

		/**
		*  @brief
		*    Set the maximum distance away from the camera that shadows by this light will be visible
		*
		*  @param[in] shadowFarDistance
		*    The maximum distance away from the camera that shadows by this light will be visible; <0 to use the global setting
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*
		*  @see
		*   - "qsf::LightComponent::getShadowFarDistance()"
		*/
		inline void setShadowFarDistance(float shadowFarDistance);

		/**
		*  @brief
		*    Return the near clip plane distance to be used by the shadow camera, if this light casts texture shadows
		*
		*  @return
		*    The near clip plane distance to be used by the shadow camera, if this light casts texture shadows; <=0 to use the global setting
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline float getShadowNearClipDistance() const;

		/**
		*  @brief
		*    Set the near clip plane distance to be used by the shadow camera, if this light casts texture shadows
		*
		*  @param[in] shadowNearClipDistance
		*    The near clip plane distance to be used by the shadow camera, if this light casts texture shadows; <=0 to use the global setting
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline void setShadowNearClipDistance(float shadowNearClipDistance);

		/**
		*  @brief
		*    Return the far clip plane distance to be used by the shadow camera, if this light casts texture shadows
		*
		*  @return
		*    The far clip plane distance to be used by the shadow camera, if this light casts texture shadows; <0 to use the global setting
		*
		*  @remarks
		*    This is different from the "shadow far distance", which is always measured from the main camera. This distance is the far clip plane of the light camera.
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline float getShadowFarClipDistance() const;

		/**
		*  @brief
		*    Set the far clip plane distance to be used by the shadow camera, if this light casts texture shadows
		*
		*  @param[in] shadowFarClipDistance
		*    The far clip plane distance to be used by the shadow camera, if this light casts texture shadows; <0 to use the global setting
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*
		*  @see
		*   - "qsf::LightComponent::getShadowFarClipDistance()"
		*/
		inline void setShadowFarClipDistance(float shadowFarClipDistance);

		//[-------------------------------------------------------]
		//[ Internal                                              ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Return the world space position
		*
		*  @return
		*    The world space position, do no destroy the returned instance
		*
		*  @note
		*    - Don't access this method if you don't have to
		*/
		inline const glm::vec3& getWorldSpacePosition() const;

		/**
		*  @brief
		*    Return the world space rotation
		*
		*  @return
		*    The world space rotation, do no destroy the returned instance
		*
		*  @note
		*    - Don't access this method if you don't have to
		*/
		inline const glm::quat& getWorldSpaceRotation() const;

		/**
		*  @brief
		*    Set the world space rotation
		*
		*  @param[in] worldSpaceRotation
		*    The world space rotation to set
		*/
		inline void setWorldSpaceRotation(const glm::quat& worldSpaceRotation);

		/**
		*  @brief
		*    Get the intensity
		*
		*  @return
		*    Intensity value between 0 and 1, which can be set and changed freely
		*
		*  @note
		*    - Don't access this method if you don't have to
		*/
		inline float getIntensity() const;

		/**
		*  @brief
		*    Set the intensity
		*
		*  @param[in] intensity
		*    Intensity value between 0 and 1, which can be set and changed freely
		*
		*  @note
		*    - Don't access this method if you don't have to
		*/
		inline void setIntensity(float intensity);


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::RendererComponent methods      ]
	//[-------------------------------------------------------]
	protected:
		virtual void setOgreSceneNodeVisibility(bool visible) override;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Component methods              ]
	//[-------------------------------------------------------]
	protected:
		virtual void onComponentTransformChange(const Transform& transform, TransformChangeFlags flags) override;
		virtual void onSetSimulating(bool simulating) override;
		virtual void onSetDebug(bool debug) override;

		//[-------------------------------------------------------]
		//[ Lifecycle                                             ]
		//[-------------------------------------------------------]
		virtual bool onStartup() override;
		virtual void onShutdown() override;


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		glm::vec3	   mWorldSpacePosition;	///< Cached world space position
		glm::quat	   mWorldSpaceRotation;	///< Cached world space rotation
		DebugDrawProxy mDebugDrawProxy;		///< Debug draw proxy


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Connected to the CAMP reflection system
		// Type
		LightType mLightType;				///< The light type
		// Color
		Color3	   mDiffuseColor;			///< The diffuse color of the light
		float	   mDiffuseIntensity;		///< The diffuse intensity of the light
		float	   mDiffuseTemperature;		///< The diffuse temperature (in Kelvin) of the light
		AssetProxy mTexture;				///< The texture asset to use (e.g. "sample/texture/spot/bat")
		// Attenuation
		float mRange;						///< The absolute upper range of the light in world units
		float mFalloffSmooth;				///< Falloff smoothness inside the range of light
		// Spotlight
		float mSpotlightOuterAngle;			///< Angle in degree covered by the outer cone
		float mSpotlightInnerAngle;			///< Angle in degree covered by the bright inner cone
		float mSpotlightFalloff;			///< The rate of falloff between the inner and outer cones. 1.0 means a linear falloff, less means slower falloff, higher means faster falloff.
		float mSpotlightNearClipDistance;	///< The near distance
		// Shadow
		float mShadowFarDistance;			///< The maximum distance away from the camera that shadows by this light will be visible; <0 to use the global setting
		float mShadowNearClipDistance;		///< The near clip plane distance to be used by the shadow camera, if this light casts texture shadows; <=0 to use the global setting
		float mShadowFarClipDistance;		///< The far clip plane distance to be used by the shadow camera, if this light casts texture shadows; <0 to use the global setting
		// Internal only
		float				mMaxSpotlightAngle;	///< Maximum angle allowed for mSpotlightOuterAngle and mSpotlightInnerAngle
		GridCellSceneNode*	mGridCellSceneNode;
		DeferredLight*		mDeferredLight;		///< Can be a null pointer, we're responsible for freeing the memory, instance created and set via "qsf::OgreCompositorPassDeferredLight::renderLight()"
		// External changer
		float mIntensity;


	//[-------------------------------------------------------]
	//[ CAMP reflection system                                ]
	//[-------------------------------------------------------]
	QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/renderer/light/LightComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::LightComponent)
QSF_CAMP_TYPE_NONCOPYABLE(qsf::LightComponent::LightType)

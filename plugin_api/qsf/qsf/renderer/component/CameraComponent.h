// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/renderer/component/RendererComponent.h"
#include "qsf/math/Ray.h"

#include <glm/gtc/quaternion.hpp>

#include <vector>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Ogre
{
	class Camera;
}
namespace qsf
{
	class ParameterGroup;
	class RenderWindow;
	class Frustum;
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
	*    Camera component class
	*/
	class QSF_API_EXPORT CameraComponent : public RendererComponent
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class RenderWindow;


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 COMPONENT_ID;				///< "qsf::CameraComponent" unique component ID
		static const uint32 PROJECTION_TYPE;			///< "ProjectionType" unique class property ID inside the class
		static const uint32 FOV;						///< "Fov" unique class property ID inside the class
		static const uint32 NEAR_CLIP_DISTANCE;			///< "NearClipDistance" unique class property ID inside the class
		static const uint32 FAR_CLIP_DISTANCE;			///< "FarClipDistance" unique class property ID inside the class
		static const uint32 USE_RENDERING_DISTANCE;		///< "UseRenderingDistance" unique class property ID inside the class
		static const uint32 LOD_BIAS;					///< "LodBias" unique class property ID inside the class
		// Primarily relevant for depth of field (DOF)
		static const uint32 FOCAL_NEAR_PLANE_DEPTH;		///< "FocalNearPlaneDepth" unique class property ID inside the class
		static const uint32 FOCAL_PLANE_DEPTH;			///< "FocalPlaneDepth" unique class property ID inside the class
		static const uint32 FOCAL_FAR_PLANE_DEPTH;		///< "FocalFarPlaneDepth" unique class property ID inside the class
		static const uint32 FOCAL_BLURRINESS_CUTOFF;	///< "FocalBlurrinessCutoff" unique class property ID inside the class

		/**
		*  @brief
		*    Projection type
		*/
		enum ProjectionType
		{
			ORTHOGRAPHIC = 0,	///< Orthographic (architectural) projection
			PERSPECTIVE  = 1	///< Perspective (realistic) projection
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
		inline explicit CameraComponent(Prototype* prototype);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~CameraComponent();

		/**
		*  @brief
		*    Return the projection type
		*
		*  @return
		*    The projection type
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline ProjectionType getProjectionType() const;

		/**
		*  @brief
		*    Set the projection type
		*
		*  @param[in] projectionType
		*    Projection type to set
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		void setProjectionType(ProjectionType projectionType);

		/**
		*  @brief
		*    Return the field of view
		*
		*  @return
		*    The field of view (y-dimension) in degree
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline float getFov() const;

		/**
		*  @brief
		*    Set the field of view
		*
		*  @param[in] fov
		*    The new field of view (y-dimension) in degree, must be 0>= and <180 (automatically clamped in case of invalid values)
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		void setFov(float fov);

		/**
		*  @brief
		*    Return the near clip distance
		*
		*  @return
		*    The near clip distance
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline float getNearClipDistance() const;

		/**
		*  @brief
		*    Set the near clip distance
		*
		*  @param[in] nearClipDistance
		*    The new near clip distance, must be >0 and <="far clip distance" (automatically clamped in case of invalid values)
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		void setNearClipDistance(float nearClipDistance);

		/**
		*  @brief
		*    Return the far clip distance
		*
		*  @return
		*    The far clip distance
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline float getFarClipDistance() const;

		/**
		*  @brief
		*    Set the far clip distance
		*
		*  @param[in] farClipDistance
		*    The new far clip distance, must be >="near clip distance" (automatically clamped in case of invalid values)
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		void setFarClipDistance(float farClipDistance);

		/**
		*  @brief
		*    Return whether to use rendering distance calculations
		*
		*  @return
		*    "true" to use rendering distance calculations, else "false"
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline bool getUseRenderingDistance() const;

		/**
		*  @brief
		*    Set whether to use rendering distance calculations
		*
		*  @param[in] useRenderingDistance
		*    "true" to use rendering distance calculations, else "false"
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		void setUseRenderingDistance(bool useRenderingDistance);

		/**
		*  @brief
		*    Return the LOD bias
		*
		*  @return
		*    The factor to apply to the usual level of detail calculation. Higher values increase the detail, so 2.0 doubles the normal detail and 0.5 halves it.
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline float getLodBias() const;

		/**
		*  @brief
		*    Set the LOD bias
		*
		*  @param[in] lodBias
		*    The factor to apply to the usual level of detail calculation. Higher values increase the detail, so 2.0 doubles the normal detail and 0.5 halves it.
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		void setLodBias(float lodBias);

		/**
		*  @brief
		*    Return the focal near plane depth
		*
		*  @return
		*    The focal near plane depth, everything beyond the near plane is fully blurred
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*    - Primarily relevant for depth of field (DOF)
		*/
		inline float getFocalNearPlaneDepth() const;

		/**
		*  @brief
		*    Set the focal near plane depth
		*
		*  @param[in] focalNearPlaneDepth
		*    The new focal near plane depth, everything beyond the near plane is fully blurred
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*    - Primarily relevant for depth of field (DOF)
		*/
		void setFocalNearPlaneDepth(float focalNearPlaneDepth);

		/**
		*  @brief
		*    Return the focal plane depth
		*
		*  @return
		*    The focal plane depth, points on this plane are in focus
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*    - Primarily relevant for depth of field (DOF)
		*/
		inline float getFocalPlaneDepth() const;

		/**
		*  @brief
		*    Set the focal plane depth
		*
		*  @param[in] focalPlaneDepth
		*    The new focal plane depth, points on this plane are in focus
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*    - Primarily relevant for depth of field (DOF)
		*/
		void setFocalPlaneDepth(float focalPlaneDepth);

		/**
		*  @brief
		*    Return the focal far plane depth
		*
		*  @return
		*    The focal far plane depth, everything beyond the far plane is fully blurred
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*    - Primarily relevant for depth of field (DOF)
		*/
		inline float getFocalFarPlaneDepth() const;

		/**
		*  @brief
		*    Set the focal far plane depth
		*
		*  @param[in] focalFarPlaneDepth
		*    The new focal far plane depth, everything beyond the far plane is fully blurred
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*    - Primarily relevant for depth of field (DOF)
		*/
		void setFocalFarPlaneDepth(float focalFarPlaneDepth);

		/**
		*  @brief
		*    Return the depth blurriness cutoff
		*
		*  @return
		*    The depth blurriness cutoff, blurriness cutoff constant for objects behind the focal plane
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*    - Primarily relevant for depth of field (DOF)
		*/
		inline float getFocalBlurrinessCutoff() const;

		/**
		*  @brief
		*    Set the depth blurriness cutoff
		*
		*  @param[in] focalBlurrinessCutoff
		*    The new depth blurriness cutoff, blurriness cutoff constant for objects behind the focal plane
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*    - Primarily relevant for depth of field (DOF)
		*/
		void setFocalBlurrinessCutoff(float focalBlurrinessCutoff);

		/**
		*  @brief
		*    Return the ray that corresponds to the given viewport position
		*
		*  @param[in] positionX
		*    Position-x inside the viewport in the range from 0.0f (left) to 1.0f (right)
		*  @param[in] positionY
		*    Position-y inside the viewport in the range from 0.0f (top) to 1.0f (bottom)
		*
		*  @return
		*    The resulting world space ray
		*/
		Ray getRayAtViewportPosition(float positionX, float positionY) const;

		/**
		*  @brief
		*    Calculate and return the current camera view frustum
		*
		*  @param[out] outFrustum
		*    Reference to a frustum that will be filled
		*/
		void getViewFrustum(Frustum& outFrustum) const;

		/**
		*  @brief
		*    Return the world space frustum associated with the area between the given viewport coordinates
		*
		*  @param[in] left
		*    Left position-x inside the viewport in the range from 0.0f (left window border) to 1.0f (right window border)
		*  @param[in] top
		*    Top position-y inside the viewport in the range from 0.0f (top window border) to 1.0f (bottom window border)
		*  @param[in] right
		*    Right position-x inside the viewport in the range from 0.0f (left window border) to 1.0f (right window border)
		*  @param[in] bottom
		*    Bottom position-y inside the viewport in the range from 0.0f (top window border) to 1.0f (bottom window border)
		*  @param[out] outFrustum
		*    The resulting world space frustum
		*/
		void getViewRectangleFrustum(float left, float top, float right, float bottom, Frustum& outFrustum) const;

		/**
		*  @brief
		*    Return the list of render windows this camera component is assigned to
		*
		*  @return
		*    Reference to the internal list of render windows this camera component is assigned to, do not manipulate the list or destroy the render windows
		*/
		inline const std::vector<RenderWindow*>& getRenderWindows() const;

		//[-------------------------------------------------------]
		//[ Transform offset                                      ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Set a position offset
		*
		*  @param[in] offset
		*    Position offset parameter
		*/
		void setPositionOffset(const glm::vec3& offset);

		/**
		*  @brief
		*    Set a rotation offset
		*
		*  @param[in] offset
		*    Rotation offset parameter
		*/
		void setRotationOffset(const glm::quat& offset);

		//[-------------------------------------------------------]
		//[ Internal                                              ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Return the OGRE camera instance
		*
		*  @return
		*    The OGRE camera instance, can be a null pointer, do no destroy the returned instance
		*
		*  @note
		*    - Don't access the OGRE camera instance if you don't have to
		*/
		inline Ogre::Camera* getOgreCamera() const;

		/**
		*  @brief
		*    Emit the Boost signal telling the world about the upcoming compositor workspace update
		*
		*  @note
		*    - Don't call this method if you don't know what it's about
		*/
		void emitPreCompositorWorkspaceUpdate() const;


	//[-------------------------------------------------------]
	//[ Public virtual qsf::RendererComponent methods         ]
	//[-------------------------------------------------------]
	public:
		inline virtual Ogre::SceneNode* getOgreSceneNode() const override;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::RendererComponent methods      ]
	//[-------------------------------------------------------]
	protected:
		virtual void updateOgreObjectCastShadows() const override;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Component methods              ]
	//[-------------------------------------------------------]
	protected:
		virtual void onComponentTransformChange(const Transform& transform, TransformChangeFlags flags) override;
		virtual void onSetSimulating(bool simulating) override;

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
		*    Set the internal LOD bias
		*
		*  @note
		*    - Also incoorperates the global LOD bias setting before telling the OGRE camera instance about the LOD bias
		*/
		void setLodBiasInternal();

		/**
		*  @brief
		*    Called as soon as there's a property change inside the settings group manager
		*
		*  @param[in] parameterGroup
		*    Parameter group instance the changed property is in
		*  @param[in] propertyId
		*    Unique class property ID the component property which values was changed
		*/
		void onSettingsGroupPropertyChanged(const ParameterGroup& parameterGroup, uint32 propertyId);

		/**
		*  @brief
		*    Set the OGRE scene node position
		*/
		void setOgreSceneNodePosition(const glm::vec3& position) const;

		/**
		*  @brief
		*    Set the OGRE scene node rotation
		*/
		void setOgreSceneNodeRotation(const glm::quat& rotation) const;

		/**
		*  @brief
		*    Set the OGRE scene node scale
		*/
		void setOgreSceneNodeScale(const glm::vec3& scale) const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Connected to the CAMP reflection system
		ProjectionType mProjectionType;			///< Projection type
		float		   mFov;					///< Field of view in degree
		float		   mNearClipDistance;		///< Near clip distance
		float		   mFarClipDistance;		///< Far clip distance
		bool		   mUseRenderingDistance;	///< "true" to use rendering distance calculations, else "false"
		float		   mLodBias;				///< LOD bias
		// Connected to the CAMP reflection system: Primarily relevant for depth of field (DOF)
		float		   mFocalNearPlaneDepth;	///< Everything beyond the near plane is fully blurred
		float		   mFocalPlaneDepth;		///< Points on this plane are in focus
		float		   mFocalFarPlaneDepth;		///< Everything beyond the far plane is fully blurred
		float		   mFocalBlurrinessCutoff;	///< Blurriness cutoff constant for objects behind the focal plane
		// Internal only
		Ogre::Camera*				mOgreCamera;		///< OGRE camera instance, can be a null pointer
		Ogre::SceneNode*			mOgreSceneNode;		///< OGRE scene node instance, can be a null pointer
		std::vector<RenderWindow*>	mRenderWindows;		///< Render windows this camera component is assigned to, do not destroy the instances
		glm::vec3					mPositionOffset;	///< Position offset for animation components
		glm::quat					mRotationOffset;	///< Rotation offset for animation components


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
#include "qsf/renderer/component/CameraComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::CameraComponent)
QSF_CAMP_TYPE_NONCOPYABLE(qsf::CameraComponent::ProjectionType)

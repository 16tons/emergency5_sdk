// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/component/Component.h"
#include "qsf/selection/EntitySelectionManager.h"
#include "qsf/math/Transform.h"
#include "qsf/job/JobProxy.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class TransformComponent;
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
	*    Camera flight component class
	*
	*  @note
	*    - Interacts with "qsf::CameraControlComponent" so locked cameras are not moved
	*/
	class QSF_API_EXPORT CameraFlightComponent : public Component
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 COMPONENT_ID;	///< "qsf::CameraFlightComponent" unique component ID

		static const uint32 CAMERA_STOPPED_MESSAGE;	///< "qsf::CameraFlightComponent_CameraStopped" unique message ID


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
		inline explicit CameraFlightComponent(Prototype* prototype);

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~CameraFlightComponent();

		/**
		*  @brief
		*    Start flight to focus the given entity
		*
		*  @param[in] entity
		*    Entity to focus on
		*  @param[in] jumpDirectly
		*    Indicates if the camera position is set directly to the target position instead of doing a interpolation
		*/
		void flyToEntity(const Entity& entity, bool jumpDirectly = true);

		/**
		*  @brief
		*    Start flight to focus the given entities
		*
		*  @param[in] entityIdSet
		*    IDs of the entities to focus on
		*  @param[in] jumpDirectly
		*    Indicates if the camera position is set directly to the target position instead of doing a interpolation
		*/
		void flyToEntities(const EntitySelectionManager::IdSet& entityIdSet, bool jumpDirectly = true);

		/**
		*  @brief
		*    Start flight to focus the given world space position
		*
		*  @param[in] targetPosition
		*    World space target position
		*  @param[in] jumpDirectly
		*    Indicates if the camera position is set directly to the target position instead of doing a interpolation
		*/
		void flyToPosition(const glm::vec3& targetPosition, bool jumpDirectly = true);

		/**
		*  @brief
		*    Start flight to the given camera position. The position and the rotation are animated to fly to the given camera
		*
		*  @param[in] targetCameraEntity
		*    The camera entity which transform should be used as the end location
		*  @param[in] maximumAcceleration
		*    The maximum acceleration value (This value is also used for the maximum deceleration)
		*  @param[in] maximumSpeed
		*    The maximum speed value
		*  @param[in] jumpDirectly
		*    Indicates if the camera position is set directly to the target position instead of doing a interpolation
		*/
		void flyToCameraPosition(const Entity& targetCameraEntity, float maximumAcceleration, float maximumSpeed, bool jumpDirectly = true);

		/**
		*  @brief
		*    Start flight to the given transform. The position and the rotation are animated to fly to the given transform
		*
		*  @param[in] targetTransform
		*    The transform which should be used as the end location
		*  @param[in] maximumAcceleration
		*    The maximum acceleration value (This value is also used for the maximum deceleration)
		*  @param[in] maximumSpeed
		*    The maximum speed value
		*  @param[in] jumpDirectly
		*    Indicates if the camera position is set directly to the target position instead of doing a interpolation
		*/
		void flyToTransform(const Transform& targetTransform, float maximumAcceleration, float maximumSpeed, bool jumpDirectly = true);

		/**
		*  @brief
		*    Start flight to focus the given world space axis aligned bounding box
		*
		*  @param[in] targetPosition
		*    World space target position
		*  @param[in] bboxMinimum
		*    Minimum of the world space axis aligned bounding box
		*  @param[in] bboxMaximum
		*    Maximum of the world space axis aligned bounding box
		*  @param[in] jumpDirectly
		*    Indicates if the camera position is set directly to the target position instead of doing a interpolation
		*/
		void flyToAabb(const glm::vec3& targetPosition, const glm::vec3& bboxMinimum, const glm::vec3& bboxMaximum, bool jumpDirectly = true);

		/**
		*  @brief
		*    Start follow the given entity
		*
		*  @param[in] targetEntity
		*    The entity which the camera should follow (The entity must have a movable component)
		*  @param[in] zoomOut
		*   The zoom out value (how far away from the entity)
		*
		*  @note
		*    In this mode this component doesn't remove itself automatically
		*/
		void followEntity(const Entity& targetEntity, float zoomOut = 25.0f);


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Component methods              ]
	//[-------------------------------------------------------]
	protected:
		//[-------------------------------------------------------]
		//[ Lifecycle                                             ]
		//[-------------------------------------------------------]
		virtual bool onStartup() override;
		virtual void onShutdown() override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		bool isCameraControlComponentLocked() const;

		/**
		*  @brief
		*    Main update function that is passed to the job manager
		*
		*  @param[in] jobArguments
		*    Job arguments
		*/
		void updateJob(const JobArguments& jobArguments);

		void stopCameraFlight(bool targetReached);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		JobProxy	mUpdateJobProxy;	///< Job proxy

		// General settings
		float		mAcceleration;		///< Maximum acceleration at the beginning of the movement (in units/second^2)
		float		mDeceleration;		///< Maximum deceleration at the end of the movement (in units/second^2)
		float		mMaximumSpeed;		///< Maximum allowed movement speed (in units/second)
		float		mMaximumZoomout;	///< Maximum zoom-out value
		bool		mJumpDirectly;		///< If true the camera is directly set to the target position

		// Settings of current movement
		WeakPtr<TransformComponent> mTransformComponentToFollow;	///< The transform component of an entity which the camera should follow. We use here a WeakPtr to know when the targets vanishes unexpected
		Transform	mTargetTransform;							///< Transformation the component currently tries to reach; TODO(fw): Only the position is considered right now!
		float		mTargetZoomout;

		// Current state
		Transform	mCurrentTransform;	///< Last known transformation of the owning entity
		float		mCurrentMoveSpeed;	///< Current speed of movement (in units/second)

		bool		mInterpolateRotation; ///< Indicates if also the rotation should be modified due the movement


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
#include "qsf/renderer/utility/CameraFlightComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::CameraFlightComponent)

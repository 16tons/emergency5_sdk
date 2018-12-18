// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"

#include <qsf/base/WeakPtr.h>
#include <qsf/input/handler/simplecamera/OrbitingCameraInputHandler.h>
#include <qsf/message/MessageProxy.h>
#include <qsf/job/JobProxy.h>

#include <glm/gtc/quaternion.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Transform;
	class JobArguments;
}


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
	*    EMERGENCY 5 in-game camera input handler class
	*
	*  @remarks
	*    Rotates around a target point, which is always bound to the ground.
	*    Directly manipulates the "qsf::TransformComponent"-component without performing any kind of collision detection of physics.
	*/
	class EM5_API_EXPORT IngameCameraInputHandler : public qsf::OrbitingCameraInputHandler
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		explicit IngameCameraInputHandler(uint32 id);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~IngameCameraInputHandler();

		qsf::Entity* getCameraFollowEntity() const;
		void setCameraFollowEntity(qsf::Entity* followEntity);

		/**
		*  @brief
		*    Update the camera
		*/
		void updateCameraFollowEntity(const qsf::JobArguments& jobArguments);

		void internalUpdateInputHandler(const qsf::Time& timePassed, bool inputBlocked);

		float getMinimumZoomout() const;
		float getMaximumZoomout() const;
		void setMaximumZoomout(float maximumZoomout);
		void setMaximumPitch(float maximumPitch);
		float getCurrentZoomout() const;


	//[-------------------------------------------------------]
	//[ Public virtual qsf::InputHandler methods              ]
	//[-------------------------------------------------------]
	public:
		virtual void updateInputHandler(const qsf::Time& timePassed) override;


	//[-------------------------------------------------------]
	//[ Public virtual qsf::InputListener methods             ]
	//[-------------------------------------------------------]
	public:
		virtual void eventReceived(const qsf::InputEvent& inputEvent, Response& response) override;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::EntityInputHandler methods     ]
	//[-------------------------------------------------------]
	protected:
		virtual void startupForEntity(qsf::Entity& entity) override;
		virtual void shutdownForEntity(qsf::Entity& entity) override;


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		static const uint32 GROUND_MAP_ID;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		float getPitchFromZoomout(float zoomout) const;
		glm::vec3 getMovementByMouse() const;
		void correctCameraTransformation(qsf::Transform& transform, float& zoomout);
		float getKeyboardScrolling() const;
		bool isBlocked() const;

		// For follow entity mode
		void onSelectionChange(const qsf::MessageParameters& parameters);
		void abortCameraFollowMode(const qsf::MessageParameters& parameters);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Configuration
		float			   mMinimumZoomout;			///< Minimum zoom-out value for the camera in world units
		float			   mMaximumZoomout;			///< Maximum zoom-out value for the camera in world units
		float			   mMinimumPitch;			///< Minimum pitch value for the camera in degrees (should be negative; 0.0f means horizontal camera, -90.0f is a top down view)
		float			   mMaximumPitch;			///< Maximum pitch value for the camera in degrees
		bool			   mLinkPitchToZoomout;		///< If true, the pitch will be linked to the zoom-out; otherwise it can be directly set with the mouse
		float			   mMaximumVelocity;		///< Maximum speed for the camera
		float			   mMaximumAcceleration;	///< Maximum acceleration by input
		float			   mAutomaticDeceleration;	///< Maximum automatic deceleration; should be less than mMaximumAcceleration
		float			   mZoomSpeed;				///< Current zoom change rate
		float			   mZoomDeceleration;		///< Automatic slow down rate of zoom speed

		// State
		glm::vec3		   mMoveVelocity;			///< Speed vector for the camera movement
		bool			   mForceUpdate;
		float			   mCurrentZoomout;

		// Entity following
		qsf::WeakPtr<qsf::Entity>	mCameraFollowEntity;	///< The entity the camera follows
		bool						mFollowingHelicopter;	///< Set to "true" in case the camera is following a helicopter
		float						mLastFollowEntityYaw;
		qsf::MessageProxy			mOnSelectionChangedMessageProxy;
		qsf::MessageProxy			mOnMinimapClickedProxy;
		qsf::JobProxy				mCameraFollowJobProxy;


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
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(em5::IngameCameraInputHandler)

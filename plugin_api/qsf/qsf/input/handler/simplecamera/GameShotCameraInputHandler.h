// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/input/handler/simplecamera/OrbitingCameraInputHandler.h"


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
	*    Game-like camera input handler class
	*
	*  @note
	*    - This is basically a copy of the "em5::IngameCameraInputHandler", with minor changes
	*
	*  @todo
	*    - TODO(fw): The class name should be changed
	*/
	class QSF_API_EXPORT GameShotCameraInputHandler : public OrbitingCameraInputHandler
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		explicit GameShotCameraInputHandler(uint32 id);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~GameShotCameraInputHandler();


	//[-------------------------------------------------------]
	//[ Public virtual qsf::InputHandler methods              ]
	//[-------------------------------------------------------]
	public:
		virtual void updateInputHandler(const Time& timePassed) override;


	//[-------------------------------------------------------]
	//[ Public virtual qsf::InputListener methods             ]
	//[-------------------------------------------------------]
	public:
		virtual void eventReceived(const InputEvent& inputEvent, Response& response) override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		float getPitchFromZoomout(float zoomout) const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Configuration
		float		  mMinimumZoomout;			///< Minimum zoom-out value for the camera in world units
		float		  mMaximumZoomout;			///< Maximum zoom-out value for the camera in world units
		float		  mMinimumPitch;			///< Minimum pitch value for the camera in degrees (should be negative; 0.0f means horizontal camera, -90.0f is a top down view)
		float		  mMaximumPitch;			///< Maximum pitch value for the camera in degrees
		bool		  mLinkPitchToZoomout;		///< If true, the pitch will be linked to the zoom-out; otherwise it can be directly set with the mouse
		float		  mMaximumVelocity;			///< Maximum speed for the camera
		float		  mMaximumAcceleration;		///< Maximum acceleration by input
		float		  mAutomaticDeceleration;	///< Maximum automatic deceleration; should be less than mMaximumAcceleration
		float		  mZoomSpeed;				///< Current zoom change rate
		float		  mZoomDeceleration;		///< Automatic slow down rate of zoom speed

		// State
		glm::vec3	  mMoveVelocity;			///< Speed vector for the camera movement
		bool		  mForceUpdate;


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
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::GameShotCameraInputHandler)

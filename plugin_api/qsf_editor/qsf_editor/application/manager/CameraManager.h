// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/selection/entity/EntitySelectionManager.h"

#include <qsf/job/JobProxy.h>
#include <qsf/base/manager/Manager.h>
#include <qsf/message/MessageProxy.h>
#include <qsf/reflection/CampClass.h>
#include <qsf/reflection/object/ObjectProxy.h>

#include <QtCore/qobject.h>

#include <glm/gtc/quaternion.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Map;
	class Entity;
	class ParameterGroup;
	class CameraComponent;
	class FreeMoveInputHandler;
	namespace editor
	{
		class Application;
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace editor
	{


		//[-------------------------------------------------------]
		//[ Classes                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Camera manager class
		*
		*  @todo
		*    - TODO(co) Under construction, we might e.g. want to add Qt signals informing about a camera change
		*    - TODO(co) Review the interaction with render view (the concrete window into the world), this camera manager is just high level orchestration
		*/
		class QSF_EDITOR_API_EXPORT CameraManager : public QObject, public Manager
		{


		//[-------------------------------------------------------]
		//[ Qt definitions (MOC)                                  ]
		//[-------------------------------------------------------]
		Q_OBJECT	// All files using the Q_OBJECT macro need to be compiled using the Meta-Object Compiler (MOC) of Qt, else slots won't work!
					// (VisualStudio: Header file -> Right click -> Properties -> "Custom Build Tool")


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			// Input handler ID of the main camera input handler
			static const uint32 MAIN_CAMERA_INPUT_HANDLER_ID;

			// TODO(fw): It would be very nice to have some kind of pluginable camera modes instead of these hard-coded modes, to allow e.g. EM5 in-game camera inside the editor.
			enum CameraMode
			{
				CAMERA_MODE_NONE,
				CAMERA_MODE_FREE_MOVE,
				CAMERA_MODE_ORBITING,
				CAMERA_MODE_GAME,
				CAMERA_MODE_TOP_DOWN
			};


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Constructor
			*
			*  @param[in] application
			*    The owner application, must stay valid as long as this instance exists
			*/
			explicit CameraManager(Application& application);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~CameraManager();

			//[-------------------------------------------------------]
			//[ View                                                  ]
			//[-------------------------------------------------------]
			/**
			*  @brief
			*    Return the used camera component instance
			*
			*  @return
			*    The used camera component instance, can be a null pointer, do no destroy the returned instance
			*
			*  @note
			*    - Ease-of-use method working on the main render view
			*/
			CameraComponent* getCameraComponent() const;

			/**
			*  @brief
			*    Set the used camera component instance
			*
			*  @param[in] cameraComponent
			*    The used camera component instance, null pointer to set no camera
			*
			*  @note
			*    - Ease-of-use method working on the main render view
			*/
			void setCameraComponent(CameraComponent* cameraComponent);

			/**
			*  @brief
			*    Check whether or not currently the default camera is set
			*/
			bool isDefaultCameraSet() const;

			/**
			*  @brief
			*    Make the default camera to the currently used camera component instance
			*/
			void setDefaultCamera();

			//[-------------------------------------------------------]
			//[ Control                                               ]
			//[-------------------------------------------------------]
			/**
			*  @brief
			*    Get the current camera mode
			*
			*  @return
			*    The camera mode
			*/
			CameraMode getCameraMode() const;

			/**
			*  @brief
			*    Set camera mode
			*
			*  @param[in] mode
			*    New camera mode to set
			*  @param[in] updateSettings
			*    Update settings?
			*/
			void setCameraMode(CameraMode cameraMode, bool updateSettings = true);

			/**
			*  @brief
			*    Set camera movement speed factor to the current input handler
			*
			*  @param[in] speedFactor
			*    New camera movement speed factor to set
			*/
			void setCameraMovementSpeedFactor(float speedFactor);

			//[-------------------------------------------------------]
			//[ Fly camera to                                         ]
			//[-------------------------------------------------------]
			/**
			*  @brief
			*    Let the currently used camera fly to the given position
			*
			*  @param[in] targetPosition
			*    The world space target position to fly the currently used camera to
			*/
			void flyCameraToPosition(const glm::vec3& targetPosition);

			/**
			*  @brief
			*    Let the currently used camera fly to the given entity
			*
			*  @param[in] entity
			*    The entity to fly the currently used camera to
			*/
			void flyCameraToEntity(const Entity& entity);

			/**
			*  @brief
			*    Let the currently used camera fly to the given entity ID
			*
			*  @param[in] entityID
			*    The ID of the entity to fly the currently used camera to
			*/
			void flyCameraToEntityById(uint64 entityId);

			/**
			*  @brief
			*    Let the currently used camera fly to the given entities
			*
			*  @param[in] entityIdSet
			*    IDs of the entities to focus on
			*/
			void flyCameraToEntities(const EntitySelectionManager::IdSet& entityIdSet);


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			/**
			*  @brief
			*    Called in case a sequence playback has been started
			*/
			void onSequenceStart(const MessageParameters& parameters);

			/**
			*  @brief
			*    Called in case a sequence camera set event has occurred
			*/
			void onSequenceSetCamera(const MessageParameters& parameters);

			/**
			*  @brief
			*    Called in case a sequence playback has ended
			*/
			void onSequenceEnd(const MessageParameters& parameters);

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
			*    Called after the camera entity position has been changed
			*
			*  @param[in] previousPosition
			*    Previous position
			*  @param[in] newPosition
			*    New position
			*/
			void onPositionChanged(const glm::vec3& previousPosition, const glm::vec3& newPosition);

			/**
			*  @brief
			*    Position spy
			*
			*  @param[in] jobArguments
			*    Job arguments
			*/
			void updatePositionJob(const qsf::JobArguments& jobArguments);

			/**
			*  @brief
			*    Called after the camera entity rotation has been changed
			*
			*  @param[in] previousRotation
			*    Previous rotation
			*  @param[in] newRotation
			*    New rotation
			*/
			void onRotationChanged(const glm::quat& previousRotation, const glm::quat& newRotation);

			/**
			*  @brief
			*    Rotation spy
			*
			*  @param[in] jobArguments
			*    Job arguments
			*/
			void updateRotationJob(const qsf::JobArguments& jobArguments);

			/**
			*  @brief
			*    Set camera movement speed factor to given free move input handler
			*
			*  @param[in] speedFactor
			*    New camera movement speed factor to set
			*  @param[in] inputHandler
			*    The input handler for which the movement speed factor should be set
			*/
			void setCameraMovementSpeedFactor(float speedFactor, FreeMoveInputHandler& inputHandler);


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			Application*  mApplication;						///< The owner application, always valid, do not destroy the instance
			Map&		  mMap;								///< Used map instance, do not destroy the instance
			ObjectProxy   mCameraComponentBeforeSequence;	///< Camera component instance which was used as current camera before the sequence playback started
			MessageProxy  mMessageProxySequenceStart;		///< Message proxy for the sequence start
			MessageProxy  mMessageProxySequenceSetCamera;	///< Message proxy for the sequence set camera
			MessageProxy  mMessageProxySequenceEnd;			///< Message proxy for the sequence end
			CameraMode	  mCameraMode;						///< Camera mode
			uint64		  mDefaultCameraEntityId;

			// Settings
			float mCameraMoveSpeedFactor;

			// Camera position spy
			boost::signals2::connection mPositionChangedConnection;
			qsf::JobProxy				mPositionJobProxy;
			glm::vec3					mPreviousPosition;
			glm::vec3					mNewPosition;
			bool						mPositionPing;

			// Camera rotation spy
			boost::signals2::connection mRotationChangedConnection;
			qsf::JobProxy				mRotationJobProxy;
			glm::quat					mPreviousRotation;
			glm::quat					mNewRotation;
			bool						mRotationPing;


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::CameraManager::CameraMode)

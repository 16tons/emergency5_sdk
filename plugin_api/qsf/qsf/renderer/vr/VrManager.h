// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/input/InputListener.h"
#include "qsf/base/manager/Manager.h"
#include "qsf/renderer/component/CameraComponent.h"
#include "qsf/base/WeakPtr.h"

#include <glm/glm.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Ogre
{
	class Camera;
	class RenderTarget;
}
namespace qsf
{
	class GuiContext;
	class RenderWindow;
	class KeyboardEvent;
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
	*    Abstract virtual reality (VR) manager
	*
	*  @remarks
	*    Checklist
	*    - Make sure your Graphics Card Settings (in NVIDIA GeForce Experience or AMD Catalyst Control Center) don't explicitly enforce VSync.
	*      VSync must be switched off or leave the decision to the application (this option seems to be available for AMD only). If not done, you might experience jittering.
	*    - Ensure your render window is maximized for best visual quality. When changing the window size below full HD, the visual quality will suffer.
	*    - When using Oculus Rift and getting below a certain framerate, massive jittering/stuttering/ghosting starts.
	*/
	class QSF_API_EXPORT VrManager : public Manager, public InputListener
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~VrManager();

		// TODO(co) Comment
		inline RenderWindow* getRenderWindow() const;

		/**
		*  @brief
		*    Return the used camera component instance
		*
		*  @return
		*    The used camera component instance, can be a null pointer, do no destroy the returned instance
		*/
		inline CameraComponent* getCameraComponent() const;

		// TODO(co) Comment
		inline bool getEnableStereoRendering() const;
		inline void setEnableStereoRendering(bool enabled);
		inline bool getEnableHeadTracker() const;
		inline void setEnableHeadTracker(bool enabled);
		inline void setHeadtrackerGuiOgreCameraTransform(bool headtrackerGuiOgreCameraTransform);

		/**
		*  @brief
		*    Enable/disable default input
		*
		*  @note
		*    - Press any key to dismiss the health and security display
		*    - Press the r-key to recenter the pose
		*/
		void setDefaultInputEnabled(bool enabled);

		/**
		*  @brief
		*    Return the current head sensor position
		*/
		inline const glm::vec3& getHeadPosition() const;

		/**
		*  @brief
		*    Return the current head sensor rotation
		*/
		inline const glm::quat& getHeadRotation() const;


	//[-------------------------------------------------------]
	//[ Public virtual qsf:VrManager methods                  ]
	//[-------------------------------------------------------]
	public:
		virtual bool isHmdPresent() const = 0;
		virtual bool isRoomScale() const = 0;

		/**
		*  @brief
		*    Return whether or not the virtual reality manager is active
		*
		*  @return
		*    "true" if the virtual reality manager is active, else "false" (maybe "qsf::RendererSettingsGroup::VrEnabled" is set to "false" or no device is connected)
		*/
		virtual bool isActive() const = 0;

		virtual bool renderFrame(Ogre::Camera* ogreCamera, Ogre::Camera* guiOgreCamera, Ogre::RenderTarget& ogreRenderTarget) = 0;

		/**
		*  @brief
		*    Set the used camera component instance
		*
		*  @param[in] cameraComponent
		*    The used camera component instance, null pointer to set no camera
		*/
		virtual void setCameraComponentAndGuiContext(CameraComponent* cameraComponent, GuiContext* guiContext = nullptr) = 0;

		/**
		*  @brief
		*    Set the used render window instance
		*
		*  @param[in] renderWindow
		*    The used render window instance, null pointer to set no render window
		*/
		virtual void setRenderWindow(RenderWindow* renderWindow) = 0;

		/**
		*  @brief
		*    Re-centers the sensor orientation
		*
		*  @note
		*    - Normally this will recenter the (x,y,z) translational components and the yaw component of orientation
		*/
		virtual void recenterPose() = 0;


	//[-------------------------------------------------------]
	//[ Public virtual qsf::InputListener methods             ]
	//[-------------------------------------------------------]
	public:
		virtual void eventReceived(const InputEvent& inputEvent, Response& response) override;


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Constructor
		*/
		inline VrManager();


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		RenderWindow*			 mRenderWindow;
		WeakPtr<CameraComponent> mCameraComponent;	///< The used camera component instance, can be a null pointer
		bool					 mEnableStereoRendering;
		bool					 mEnableHeadTracker;
		bool					 mHeadtrackerGuiOgreCameraTransform;
		bool					 mEnableDefaultInput;
		bool					 mEnableRendering;
		glm::vec3				 mHeadPosition;
		glm::quat				 mHeadRotation;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void processKeyboardEvent(const KeyboardEvent& keyboardEvent);


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/renderer/vr/VrManager-inl.h"

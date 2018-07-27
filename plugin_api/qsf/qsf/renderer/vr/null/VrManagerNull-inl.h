// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline VrManagerNull::VrManagerNull()
	{
		// Nothing in here
	}

	inline VrManagerNull::~VrManagerNull()
	{
		// Nothing in here
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf:VrManager methods                  ]
	//[-------------------------------------------------------]
	inline bool VrManagerNull::isHmdPresent() const
	{
		return false;
	}

	inline bool VrManagerNull::isRoomScale() const
	{
		return false;
	}

	inline bool VrManagerNull::isActive() const
	{
		return false;
	}

	inline bool VrManagerNull::renderFrame(Ogre::Camera*, Ogre::Camera*, Ogre::RenderTarget&)
	{
		// Nope, perform standard rendering
		return false;
	}

	inline void VrManagerNull::setCameraComponentAndGuiContext(CameraComponent* cameraComponent, GuiContext*)
	{
		// Set new camera component
		mCameraComponent = cameraComponent;
	}

	inline void VrManagerNull::setRenderWindow(RenderWindow* renderWindow)
	{
		mRenderWindow = renderWindow;
	}

	inline void VrManagerNull::recenterPose()
	{
		// Nothing in here
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf

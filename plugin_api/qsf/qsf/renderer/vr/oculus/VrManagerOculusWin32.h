// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/renderer/vr/VrManager.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
typedef struct ovrHmdStruct* ovrSession;
typedef struct ovrEyeRenderDesc_ ovrEyeRenderDesc;
typedef struct ovrHmdDesc_ ovrHmdDesc;
typedef struct ovrMirrorTextureData* ovrMirrorTexture;
namespace qsf
{
	namespace detail
	{
		struct OculusTextureDirectX11;
	}
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
	*    MS Windows Oculus virtual reality (VR) manager
	*/
	class VrManagerOculusWin32 : public VrManager
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] silentInitialization
		*    "true" to perform a silent initialization and output no warnings or errors, "false"
		*/
		explicit VrManagerOculusWin32(bool silentInitialization);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~VrManagerOculusWin32();


	//[-------------------------------------------------------]
	//[ Public virtual qsf:VrManager methods                  ]
	//[-------------------------------------------------------]
	public:
		virtual bool isHmdPresent() const override;
		virtual bool isRoomScale() const override;
		virtual bool isActive() const override;
		virtual bool renderFrame(Ogre::Camera* ogreCamera, Ogre::Camera* guiOgreCamera, Ogre::RenderTarget& ogreRenderTarget) override;
		virtual void setCameraComponentAndGuiContext(CameraComponent* cameraComponent, GuiContext* guiContext = nullptr) override;
		virtual void setRenderWindow(RenderWindow* renderWindow) override;
		virtual void recenterPose() override;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		ovrSession						mOvrSession;
		ovrHmdDesc*						mOvrHmdDesc;
		long long						mFrameIndex;
		ovrEyeRenderDesc*				mOvrEyeRenderDesc;	///< Description of the VR
		detail::OculusTextureDirectX11* mLeftEyeRenderTexture;
		detail::OculusTextureDirectX11* mRightEyeRenderTexture;
		uint32							mOvrMirrorTextureWidth;
		uint32							mOvrMirrorTextureHeight;
		ovrMirrorTexture				mOvrMirrorTexture;
		bool							mRenderingInProgress;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf

// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/renderer/vr/VrManager.h"

#include <openvr/openvr.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Ogre
{
	class SceneNode;
	class RenderTexture;
	class Texture;
	template<typename T> class SharedPtr;
	typedef SharedPtr<Texture> TexturePtr;
	namespace v1
	{
		class Entity;
	}
}
namespace qsf
{
	class OpenVrRuntimeLinking;
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
	*    MS Windows OpenVR virtual reality (VR) manager
	*/
	class VrManagerOpenVrWin32 : public VrManager
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		VrManagerOpenVrWin32();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~VrManagerOpenVrWin32();


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
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void setupRenderModelForTrackedDevice(vr::TrackedDeviceIndex_t unTrackedDeviceIndex);
		void updateHmdMatrixPose();
		glm::mat4 getHmdEyeSpaceToHeadSpaceMatrix(vr::Hmd_Eye vrEye) const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		OpenVrRuntimeLinking*	   mOpenVrRuntimeLinking;
		vr::EGraphicsAPIConvention mVrGraphicsAPIConvention;
		vr::IVRSystem*			   mVrSystem;
		vr::IVRRenderModels*	   mVrRenderModels;
		bool					   mRenderingInProgress;
		// Transform
		vr::TrackedDevicePose_t	mVrTrackedDevicePose[vr::k_unMaxTrackedDeviceCount];
		glm::mat4				mDevicePoseMatrix[vr::k_unMaxTrackedDeviceCount];
		uint32_t				mNumberOfValidDevicePoses;
		glm::mat4				mHmdPoseMatrix;
		// Renderer resources
		Ogre::TexturePtr*	 mOgreTexturePtr;
		Ogre::RenderTexture* mOgreRenderTexture;
		Ogre::v1::Entity*	 mOgreEntity[vr::k_unMaxTrackedDeviceCount];
		Ogre::SceneNode*	 mOgreSceneNodes[vr::k_unMaxTrackedDeviceCount];


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf

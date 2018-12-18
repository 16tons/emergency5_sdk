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
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Null virtual reality (VR) manager
	*/
	class VrManagerNull : public VrManager
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		inline VrManagerNull();

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~VrManagerNull();


	//[-------------------------------------------------------]
	//[ Public virtual qsf:VrManager methods                  ]
	//[-------------------------------------------------------]
	public:
		inline virtual bool isHmdPresent() const override;
		inline virtual bool isRoomScale() const override;
		inline virtual bool isActive() const override;
		inline virtual bool renderFrame(Ogre::Camera* ogreCamera, Ogre::Camera* guiOgreCamera, Ogre::RenderTarget& ogreRenderTarget) override;
		inline virtual void setCameraComponentAndGuiContext(CameraComponent* cameraComponent, GuiContext* guiContext = nullptr) override;
		inline virtual void setRenderWindow(RenderWindow* renderWindow) override;
		inline virtual void recenterPose() override;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/renderer/vr/null/VrManagerNull-inl.h"

// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline VrManager::~VrManager()
	{
		// Disable the default input
		setDefaultInputEnabled(false);
	}

	inline RenderWindow* VrManager::getRenderWindow() const
	{
		return mRenderWindow;
	}

	inline CameraComponent* VrManager::getCameraComponent() const
	{
		return mCameraComponent.get();
	}

	inline bool VrManager::getEnableStereoRendering() const
	{
		return mEnableStereoRendering;
	}

	inline void VrManager::setEnableStereoRendering(bool enabled)
	{
		mEnableStereoRendering = enabled;
	}

	inline bool VrManager::getEnableHeadTracker() const
	{
		return mEnableHeadTracker;
	}

	inline void VrManager::setEnableHeadTracker(bool enabled)
	{
		mEnableHeadTracker = enabled;
	}

	inline void VrManager::setHeadtrackerGuiOgreCameraTransform(bool headtrackerGuiOgreCameraTransform)
	{
		mHeadtrackerGuiOgreCameraTransform = headtrackerGuiOgreCameraTransform;
	}

	inline const glm::vec3& VrManager::getHeadPosition() const
	{
		return mHeadPosition;
	}

	inline const glm::quat& VrManager::getHeadRotation() const
	{
		return mHeadRotation;
	}


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	inline VrManager::VrManager() :
		mRenderWindow(nullptr),
		mEnableStereoRendering(true),
		mEnableHeadTracker(true),
		mHeadtrackerGuiOgreCameraTransform(false),
		mEnableDefaultInput(false),
		mEnableRendering(true)
	{
		// Enable the default input by default
		setDefaultInputEnabled(true);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf

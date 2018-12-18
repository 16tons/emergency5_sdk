// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/component/overlay/OverlayManager.h"
#include "em5/component/overlay/OverlayComponent.h"
#include "em5/settings/GameSettingsGroup.h"

#include <qsf/renderer/component/CameraComponent.h>
#include <qsf/map/Map.h>
#include <qsf/QsfHelper.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	OverlayManager::OverlayManager()
	{
		mPreCompositorWorkspaceUpdateConnection = QSF_MAINMAP.PreCompositorWorkspaceUpdate.connect(boost::bind(&OverlayManager::onPreCompositorWorkspaceUpdate, this, _1));
	}

	OverlayManager::~OverlayManager()
	{
		mPreCompositorWorkspaceUpdateConnection.disconnect();
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void OverlayManager::onPreCompositorWorkspaceUpdate(const qsf::CameraComponent& cameraComponent)
	{
		// When a cutscene is running we do nothing
		if (GameSettingsGroup::getInstanceSafe().getCutsceneRunning())
			return;

		const Ogre::Camera* ogreCamera = cameraComponent.getOgreCamera();
		if (nullptr != ogreCamera)
		{
			// Update 2D screen positions of all visible overlays
			OverlayComponent* overlayComponent = mVisibleOverlayComponents.mFirst;
			while (nullptr != overlayComponent)
			{
				overlayComponent->onPreCompositorWorkspaceUpdate(*ogreCamera);

				// Go to next one
				overlayComponent = overlayComponent->mVisibleOverlayComponentsEntry.mNext;
			}
		}
	}

	void OverlayManager::addVisibleOverlayComponent(OverlayComponent& overlayComponent)
	{
		auto& entry = overlayComponent.mVisibleOverlayComponentsEntry;
		if (entry.mIsRegistered)
		{
			// Early out: already registered
			return;
		}

		// Insert as new first
		OverlayComponent* oldFirst = mVisibleOverlayComponents.mFirst;
		if (nullptr != oldFirst)
		{
			oldFirst->mVisibleOverlayComponentsEntry.mPrevious = &overlayComponent;
			entry.mNext = oldFirst;
		}
		mVisibleOverlayComponents.mFirst = &overlayComponent;

		QSF_ASSERT(nullptr == entry.mPrevious, "Inconsistency in linked list", QSF_REACT_NONE);

		// Entry is registered now
		entry.mIsRegistered = true;
	}

	void OverlayManager::removeVisibleOverlayComponent(OverlayComponent& overlayComponent)
	{
		auto& entry = overlayComponent.mVisibleOverlayComponentsEntry;
		if (!entry.mIsRegistered)
		{
			// Early out: not registered
			return;
		}

		// Remove from double linked list
		OverlayComponent* oldPrevious = entry.mPrevious;
		if (nullptr == entry.mPrevious)
		{
			// This is the first entry in the list
			mVisibleOverlayComponents.mFirst = entry.mNext;
		}
		else
		{
			entry.mPrevious->mVisibleOverlayComponentsEntry.mNext = entry.mNext;
			entry.mPrevious = nullptr;
		}

		if (nullptr != entry.mNext)
		{
			entry.mNext->mVisibleOverlayComponentsEntry.mPrevious = oldPrevious;
			entry.mNext = nullptr;
		}

		// Entry is not registered any more
		entry.mIsRegistered = false;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

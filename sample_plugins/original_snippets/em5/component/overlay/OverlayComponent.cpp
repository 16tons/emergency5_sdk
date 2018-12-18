// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/component/overlay/OverlayComponent.h"
#include "em5/component/overlay/OverlayManager.h"
#include "em5/game/Game.h"
#include "em5/gui/hud/overlay/OverlayWindow.h"
#include "em5/gui/hud/overlay/OverlayWindowManager.h"
#include "em5/EM5Helper.h"

#include <qsf/map/Map.h>
#include <qsf/map/Entity.h>
#include <qsf/plugin/QsfJobs.h>
#include <qsf/gui/GuiContext.h>
#include <qsf/renderer/mesh/MeshComponent.h>
#include <qsf/renderer/helper/OgreCameraHelper.h>

#include <Rocket/Core/Context.h>
#include <Rocket/Core/ElementDocument.h>

#include <OGRE/OgreSceneManager.h>
#include <OGRE/OgreViewport.h>
#include <OGRE/OgreEntity.h>
#include <OGRE/OgreCamera.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 OverlayComponent::COMPONENT_ID	= qsf::StringHash("em5::OverlayComponent");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	OverlayComponent::~OverlayComponent()
	{
		QSF_ASSERT(!mVisibleOverlayComponentsEntry.mIsRegistered, "Overlay component is still registered as visible in manager", QSF_REACT_NONE);
	}


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	OverlayComponent::OverlayComponent(qsf::Prototype* prototype, OverlayWindowManager& overlayWindowManager) :
		qsf::Component(prototype),
		mOverlayWindowManager(overlayWindowManager),
		mLastIsContentEmptyState(true), // Initially all is empty
		mScreenRendered(false),
		mScreenPosition(FLT_MIN, FLT_MIN)
	{
		// Overlays should disappear rather fast when the owner object is no longer visible on screen
		setNonvisibleUpdateTimeout(qsf::Time::fromSeconds(0.1f));
	}

	void OverlayComponent::checkEmptyStateChanged()
	{
		// Check if we have any changes
		const bool contentEmpty = isContentEmpty();
		if (mLastIsContentEmptyState != contentEmpty)
		{
			// Save new state
			mLastIsContentEmptyState = contentEmpty;

			// Register/unregister depending of the state
			if (mLastIsContentEmptyState)
			{
				unregisterMovableObjectRenderingListener();
			}
			else
			{
				// No new registration after shutdown
				if (isSimulating())
				{
					registerMovableObjectRenderingListener();
				}
				else
				{
					#ifndef ENDUSER
						QSF_WARN("'OverlayComponent::checkEmptyStateChanged' must not be called when component is not simulating", QSF_REACT_NONE);
					#endif
				}
			}
		}
	}

	void OverlayComponent::makeDirty()
	{
		mLastIsContentEmptyState = true;
		mScreenRendered = false;
		mScreenPosition.x = FLT_MIN;
		mScreenPosition.y = FLT_MIN;
	}

	void OverlayComponent::avoidDelayedDestruction()
	{
		mDestructionJobProxy.unregister();
	}

	void OverlayComponent::triggerDelayedDestruction()
	{
		mDestructionJobProxy.registerAt(qsf::QsfJobs::REALTIME, boost::bind(&OverlayComponent::onDelayedDestruction, this, _1), qsf::jobs::JobConfiguration(qsf::jobs::ORDERING_POST));
	}

	void OverlayComponent::onPreCompositorWorkspaceUpdate(const Ogre::Camera& ogreCamera)
	{
		QSF_CHECK(mOverlayWindowManager.valid(), "Invalid status overlay window manager weak pointer", return);
		OverlayWindow* overlayWindow = mOverlayWindowManager->getOverlayWindowInstance(getEntityId());

		// Show the status overlay window as soon as the overlay owner entity starts to be rendered on the screen
		// The bar or the icons are handled separately
		if (nullptr != overlayWindow && !overlayWindow->isVisible() && overlayWindow->isEnabled())
		{
			overlayWindow->show();
		}

		// We are currently in screen rendering state
		mScreenRendered = true;

		if (nullptr != overlayWindow && !isContentEmpty() && mVisibleMeshComponent.valid())
		{
			// Update the overlay GUI element
			glm::vec2 screenPosition;
			if (calculateScreenPosition(*mVisibleMeshComponent, ogreCamera, screenPosition))
			{
				// The status overlay window is inside screen bound make the window visible if hidden
				if (!overlayWindow->isVisible() && overlayWindow->isEnabled())
				{
					overlayWindow->show();
				}

				Rocket::Core::ElementDocument* document = overlayWindow->getRocketCoreElementDocument();
				if (nullptr != document)
				{
					updateContent(*document);

					if (mScreenPosition != screenPosition)
					{
						mScreenPosition = screenPosition;

						screenPosition -= glm::ceil(getContentWindowSize() * 0.5f);
						screenPosition -= glm::ceil(getContentWindowOffset() * 0.5f);

						document->SetProperty("left", Rocket::Core::Property(screenPosition.x, Rocket::Core::Property::PX));
						document->SetProperty("top", Rocket::Core::Property(screenPosition.y, Rocket::Core::Property::PX));
					}
				}
			}
			else if (overlayWindow->isVisible() && overlayWindow->isEnabled())
			{
				// Hide the status overlay window because the overlay window is now out of screen bounds
				overlayWindow->hide();
			}
		}
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Component methods              ]
	//[-------------------------------------------------------]
	void OverlayComponent::onSetSimulating(bool simulating)
	{
		if (!simulating)
		{
			unregisterMovableObjectRenderingListener();

			// Release libRocket status overlay window instance
			if (mOverlayWindowManager.valid())
			{
				mOverlayWindowManager->releaseOverlayWindowInstance(getEntityId());
			}

			if (mVisibleOverlayComponentsEntry.mIsRegistered)
			{
				EM5_GAME.getOverlayManager().removeVisibleOverlayComponent(*this);
			}

			mDestructionJobProxy.unregister();
		}
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::MovableObjectRenderingListener methods ]
	//[-------------------------------------------------------]
	qsf::Component* OverlayComponent::getMovableObjectRenderingListenerComponent()
	{
		return getEntity().getComponent<qsf::RendererComponent>();
	}

	void OverlayComponent::onObjectRenderingStartup()
	{
		QSF_CHECK(mOverlayWindowManager.valid(), "Invalid overlay window manager weak pointer", return);

		mJobProxy.registerAt(qsf::QsfJobs::REALTIME_GUI, boost::bind(&OverlayComponent::updateAnimation, this, _1));

		// Show the overlay window as soon as the overlay owner entity starts to be rendered on the screen
		// The bar or the icons are handled separately
		OverlayWindow* overlayWindow = mOverlayWindowManager->getOverlayWindowInstance(getEntityId());
		if (nullptr != overlayWindow && !overlayWindow->isVisible() && overlayWindow->isEnabled())
		{
			overlayWindow->hide();
		}

		// Register job
		mAnimationJobProxy.registerAt(qsf::QsfJobs::REALTIME_GUI, boost::bind(&OverlayComponent::onUpdateMovableObjectRenderingJob, this, _1));
	}

	void OverlayComponent::onObjectRendering(const qsf::Component& component, const Ogre::Camera& ogreCamera)
	{
		// Call the base implementation
		// We have to do it even when the current render state is not IRS_NONE
		// Because otherwise onObjectRenderingStartup and onObjectRenderingShutdown gets called every time
		// onObjectRenderingStartup gets called when OgreMovableObjectListener::objectRendering is called
		// After that onObjectRenderingShutdown is called due "dead clock" timed out
		// This effect could be "seen" when e.g. an overlay for a person was visible and only the shadow was visible to the camera viewport but the person itself not
		JobProxyMovableObjectRenderingListener::onObjectRendering(component, ogreCamera);

		// Make sure we're registered as visible overlay
		if (!mVisibleOverlayComponentsEntry.mIsRegistered)
		{
			EM5_GAME.getOverlayManager().addVisibleOverlayComponent(*this);
			mVisibleMeshComponent = static_cast<qsf::MeshComponent&>(const_cast<Component&>(component));
		}
	}

	void OverlayComponent::onObjectRenderingShutdown()
	{
		// Unregister as visible overlay
		mVisibleMeshComponent.clear();
		if (mVisibleOverlayComponentsEntry.mIsRegistered)
		{
			EM5_GAME.getOverlayManager().removeVisibleOverlayComponent(*this);
		}

		mJobProxy.unregister();

		if (mOverlayWindowManager.valid())
		{
			// Hide the overlay window as soon as the overlay owner entity gets no longer rendered on the screen
			OverlayWindow* overlayWindow = mOverlayWindowManager->getOverlayWindowInstance(getEntityId());
			if (nullptr != overlayWindow && overlayWindow->isVisible() && overlayWindow->isEnabled())
			{
				overlayWindow->hide();
			}
		}

		// Call the base implementation
		JobProxyMovableObjectRenderingListener::onObjectRenderingShutdown();
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	bool OverlayComponent::calculateScreenPosition(const qsf::MeshComponent& meshComponent, const Ogre::Camera& ogreCamera, glm::vec2& screenPosition) const
	{
		// Calculate the current screen position
		Ogre::v1::Entity* ogreEntity = meshComponent.getOgreEntity();
		if (nullptr != ogreEntity)
		{
			const Ogre::Vector3 center = ogreEntity->getWorldAabb().mCenter;
			const glm::vec3& worldSpaceOffset = getContentWorldSpaceOffset();
			const glm::vec3 point(center.x + worldSpaceOffset.x, center.y + worldSpaceOffset.y, center.z + worldSpaceOffset.z);

			if (qsf::OgreCameraHelper::projectWorldToClipSpace(ogreCamera, point, screenPosition))
			{
				screenPosition.x = (screenPosition.x / 2.0f) + 0.5f;
				screenPosition.y = 1.0f - ((screenPosition.y / 2.0f) + 0.5f);

				// Check whether we are in the viewport or not
				if (-0.1f <= screenPosition.x && 1.1f >= screenPosition.x && -0.1f <= screenPosition.y && 1.1f >= screenPosition.y)
				{
					const Rocket::Core::Vector2i& dimensions = mOverlayWindowManager->getGuiContext().getRocketCoreContext().GetDimensions();
					screenPosition.x *= dimensions.x;
					screenPosition.y *= dimensions.y;
					screenPosition = glm::floor(screenPosition);

					// Position updated
					return true;
				}
			}
		}

		// Position not updated
		return false;
	}

	void OverlayComponent::onDelayedDestruction(const qsf::JobArguments& jobArguments)
	{
		getEntity().destroyComponent(*this);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/component/effect/FadeEffectComponent.h"
#include "em5/map/MapHelper.h"
#include "em5/network/NetworkManager.h"
#include "em5/network/multiplayer/server/Host.h"
#include "em5/network/multiplayer/operationmessagehandler/FadeEntityHandler.h"
#include "em5/plugin/Jobs.h"
#include "em5/EM5Helper.h"

#include <qsf_game/component/base/HiddenComponent.h>
#include <qsf_game/component/highlight/GroundMarkerComponent.h>

#include <qsf/QsfHelper.h>
#include <qsf/map/Map.h>
#include <qsf/map/Entity.h>
#include <qsf/job/JobArguments.h>
#include <qsf/component/link/LinkComponent.h>
#include <qsf/renderer/RendererSystem.h>
#include <qsf/renderer/mesh/MeshComponent.h>
#include <qsf/renderer/material/MaterialSystem.h>
#include <qsf/renderer/material/material/MaterialProperty.h>
#include <qsf/renderer/material/material/MaterialVariationManager.h>
#include <qsf/base/ScratchBuffer.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 FadeEffectComponent::COMPONENT_ID = qsf::StringHash("em5::FadeEffectComponent");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	FadeEffectComponent::FadeEffectComponent(qsf::Prototype* prototype) :
		qsf::Component(prototype),
		mDeleteOwner(false),
		mFadeIn(false),
		mInternalVisibility(qsf::getUninitialized<float>()),
		mRendererVisibility(qsf::getUninitialized<float>())
	{
		// Get rid of this expensive thing as soon as possible
		setNonvisibleUpdateTimeout(qsf::Time::fromSeconds(0.2f));
	}

	FadeEffectComponent::~FadeEffectComponent()
	{
		// Nothing to do in here
	}

	void FadeEffectComponent::fadeIn(qsf::Time effectDuration)
	{
		init(true, effectDuration);
	}

	void FadeEffectComponent::fadeOut(qsf::Time effectDuration, bool deleteOwner)
	{
		init(false, effectDuration, deleteOwner);
	}

	void FadeEffectComponent::init(bool fadeIn, qsf::Time effectDuration, bool deleteOwner)
	{
		// In case one already requested the deletion of the owner, don't dare to touch fade effect component states, again (don't play with dead things)
		if (!mDeleteOwner)
		{
			{ // Multiplay related: Inform the host when a fade effect is initiated. This avoids polling for each entity in the map cache if the fade effect has been created/changed
				multiplayer::server::Host* multiplayerHost = EM5_NETWORK.getMultiplayerHost();
				if (nullptr != multiplayerHost)
				{
					multiplayer::OperationMessageHandlerManager& manager = multiplayerHost->getOperationMessageHandlerManager();
					multiplayer::FadeEntityHandler* handler = manager.getHandler<multiplayer::FadeEntityHandler>();
					if (nullptr != handler)
					{
						handler->onNewFadeEntity(getEntityId(), effectDuration, fadeIn);
					}
				}
			}

			// Ensure previous fading has been terminated
			deactivateJob();

			// Initialize all states to known values
			mEffectDuration		= effectDuration;
			mRunningTime		= qsf::Time::ZERO;
			mDeleteOwner		= deleteOwner;
			mFadeIn				= fadeIn;
			mInternalVisibility = qsf::getUninitialized<float>();
			mRendererVisibility = qsf::getUninitialized<float>();

			// Ready to rumble
			if (fadeIn)
			{
				// Start invisible
				setFadeEffect(0.0f);

				// Turns all components to visible state. This doesn't mean too much in EMERGENCY 5, mostly the mesh component support this, but here being sure.
				getEntity().setHidden(false);
				qsf::game::HiddenComponent::setHidden(getEntity(), false);
			}
			activateJob();
		}
	}

	bool FadeEffectComponent::isFading() const
	{
		return mUpdateJobProxy.isValid();
	}

	bool FadeEffectComponent::isFadeIn() const
	{
		return mFadeIn;
	}

	const qsf::Time& FadeEffectComponent::getFadeTime() const
	{
		return mEffectDuration;
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Component methods              ]
	//[-------------------------------------------------------]
	void FadeEffectComponent::onShutdown()
	{
		// Ensure a well defined state when the fade effect component is shutdown
		deactivateJob();
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::MovableObjectRenderingListener methods ]
	//[-------------------------------------------------------]
	qsf::Component* FadeEffectComponent::getMovableObjectRenderingListenerComponent()
	{
		// Prefer the entity's own renderer component if there is one
		qsf::RendererComponent* rendererComponent = getEntity().getComponent<qsf::RendererComponent>();

		if (nullptr != rendererComponent && rendererComponent->getId() == qsf::MeshComponent::COMPONENT_ID)
		{
			// TODO(fw): This is a hack for the paramedics team, they have an invisible dummy mesh in their main entity
			//  -> Global asset ID 167866 - Local asset name "em5/mesh/default/dummy_mesh" - Local asset ID 2405884623
			if (static_cast<qsf::MeshComponent*>(rendererComponent)->getMesh().getGlobalAssetId() == 167866)
				rendererComponent = nullptr;
		}

		if (nullptr == rendererComponent)
		{
			// Otherwise search child entities for something we can use as rendering listener
			qsf::EntityVectorScratchBuffer linkedEntities;
			qsf::LinkComponent::getConnectedLinkGroup(getEntity(), qsf::LinkComponent::TRANSFORM, true, linkedEntities);
			for (qsf::Entity* entity : linkedEntities)
			{
				// Ignore main entity
				if (entity != &getEntity())
				{
					// We're only interested in mesh components
					qsf::MeshComponent* meshComponent = entity->getComponent<qsf::MeshComponent>();
					if (nullptr != meshComponent)
					{
						return meshComponent;
					}
				}
			}
		}
		return rendererComponent;
	}

	void FadeEffectComponent::onObjectRendering(const qsf::Component& component, const Ogre::Camera& ogreCamera)
	{
		synchronizeInternalAndRendererVisibility();

		// Call the base implementation
		JobProxyMovableObjectRenderingListener::onObjectRendering(component, ogreCamera);
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void FadeEffectComponent::activateJob()
	{
		mRunningTime = qsf::Time::ZERO;

		// Is job already active?
		if (!mUpdateJobProxy.isValid())
		{
			// Activate job by registering via the job proxy
			mUpdateJobProxy.registerAt(Jobs::ANIMATION_PERSON, boost::bind(&FadeEffectComponent::updateJob, this, _1));
			registerMovableObjectRenderingListener();
		}
	}

	void FadeEffectComponent::deactivateJob()
	{
		if (mUpdateJobProxy.isValid())
		{
			mUpdateJobProxy.unregister();
			unregisterMovableObjectRenderingListener();
		}

		// Ensure an always clean fade effect finish
		if (mFadeIn)
		{
			// At the end, be fully visible
			setFadeEffect(1.0f);
		}
		else
		{
			// At the end, be fully invisible
			setFadeEffect(0.0f);
		}

		// We need a synchronization in any case to have a well-defined state when fading is done
		mRendererVisibility = qsf::getUninitialized<float>();
		synchronizeInternalAndRendererVisibility();
	}

	void FadeEffectComponent::updateJob(const qsf::JobArguments& jobArguments)
	{
		if (mRunningTime <= mEffectDuration)
		{
			// Time progress (do this for the first update to avoid zero division)
			mRunningTime += jobArguments.getTimePassed();

			if (mFadeIn)
			{
				updateFadeIn(jobArguments.getTimePassed());
			}
			else
			{
				updateFadeOut(jobArguments.getTimePassed());
			}
		}
		else
		{
			// Done
			deactivateJob();

			// Destroy the entity, if required
			if (mDeleteOwner)
			{
				MapHelper::destroyEntity(getEntity());
			}
			else
			{
				if (!mFadeIn)
				{
					// Only hide the entity, when we are at the end of an fade out
					getEntity().setHidden(true);
				}

				// Remove this component, it's not needed any more
				getEntity().destroyComponentById(COMPONENT_ID);
			}
		}
	}

	void FadeEffectComponent::updateFadeIn(const qsf::Time& secondsPassed)
	{
		if (mRunningTime >= mEffectDuration)
		{
			// At the end, be fully visible
			setFadeEffect(1.0f);
		}
		else
		{
			// Disappear animation
			setFadeEffect(mRunningTime / mEffectDuration);
		}
	}

	void FadeEffectComponent::updateFadeOut(const qsf::Time& secondsPassed)
	{
		if (mRunningTime >= mEffectDuration)
		{
			// At the end, be fully invisible
			setFadeEffect(0.0f);
		}
		else
		{
			// Disappear animation
			setFadeEffect(1.0f - mRunningTime / mEffectDuration);
		}
	}

	void FadeEffectComponent::setFadeEffect(float visibility)
	{
		mInternalVisibility = visibility;
	}

	void FadeEffectComponent::synchronizeInternalAndRendererVisibility()
	{
		if (mRendererVisibility != mInternalVisibility || qsf::isUninitialized(mRendererVisibility))
		{
			mRendererVisibility = mInternalVisibility;

			// Let's get to work
			if (mRendererVisibility <= 0.0f)
			{
				// Fully invisible
				updateMaterialAndInternalFadeAmount(true, 0.0f);
			}
			else if (mRendererVisibility >= 1.0f)
			{
				// Fully visible
				updateMaterialAndInternalFadeAmount(false, 1.0f);
			}
			else
			{
				// In transition
				updateMaterialAndInternalFadeAmount(true, mRendererVisibility);
			}
		}
	}

	void FadeEffectComponent::updateMaterialAndInternalFadeAmount(bool enableFadeEffect, float visibility) const
	{
		// Apply material change to all child entities
		qsf::EntityVectorScratchBuffer linkedEntities;
		qsf::LinkComponent::getConnectedLinkGroup(getEntity(), qsf::LinkComponent::TRANSFORM, true, linkedEntities);

		static qsf::MaterialVariationManager& materialVariationManager = QSF_MATERIAL.getMaterialVariationManager();
		const qsf::MaterialPropertyValue applyFadingMaterialPropertyValue(qsf::MaterialPropertyValue::fromBoolean(enableFadeEffect));
		const qsf::MaterialPropertyValue fadingAmountMaterialPropertyValue(qsf::MaterialPropertyValue::fromFloat(1.0f - visibility));
		for (qsf::Entity* entity : linkedEntities)
		{
			// We're only interested in mesh components
			qsf::MeshComponent* meshComponent = entity->getComponent<qsf::MeshComponent>();
			if (nullptr != meshComponent)
			{
				// Set material properties
				materialVariationManager.setEntityMaterialPropertyValue(*entity, "ApplyFading", applyFadingMaterialPropertyValue);
				materialVariationManager.setEntityMaterialPropertyValue(*entity, "FadingAmount", fadingAmountMaterialPropertyValue);
			}
		}

		// Update ground marker as well if there is one
		qsf::game::GroundMarkerComponent* groundMarkerComponent = getEntity().getComponent<qsf::game::GroundMarkerComponent>();
		if (nullptr != groundMarkerComponent)
		{
			groundMarkerComponent->setGroundMarkerFadeAlpha(visibility);
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

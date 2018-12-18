// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/component/multiplayer/GhostComponent.h"
#include "em5/component/vehicle/VehicleComponent.h"
#include "em5/game/player/PlayerManager.h"

#include <qsf/component/link/LinkComponent.h>
#include <qsf/map/Entity.h>
#include <qsf/renderer/RendererSystem.h>
#include <qsf/renderer/mesh/MeshComponent.h>
#include <qsf/renderer/material/MaterialSystem.h>
#include <qsf/renderer/material/material/MaterialProperty.h>
#include <qsf/renderer/material/material/MaterialVariationManager.h>
#include <qsf/base/ScratchBuffer.h>
#include <qsf/plugin/QsfJobs.h>
#include <qsf/QsfHelper.h>

#include <OGRE/OgreEntity.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 GhostComponent::COMPONENT_ID	 = qsf::StringHash("em5::GhostComponent");
	const uint32 GhostComponent::RENDER_QUEUE_ID = 91;	// "Debugging and visual helpers"-category so it has no impact on the image luminance calculation


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	void GhostComponent::setPlayerId(uint32 playerId)
	{
		if (mPlayerId != playerId)
		{
			const bool wasSet = qsf::isInitialized(mPlayerId);
			mPlayerId = playerId;

			if (isSimulating())
			{
				Player* player = PlayerManager::getLocalPlayer();
				if (nullptr != player)
				{
					const bool oldState = mInternalIsGhost;
					mLocalPlayerId = player->getPlayerIndex();
					if (mLocalPlayerId != mPlayerId)
					{
						mInternalIsGhost = true;
					}
					else if (wasSet)
					{
						// Only do an reset if we set the value
						mInternalIsGhost = false;
					}

					if (oldState != mInternalIsGhost)
					{
						// Force an update
						mRendererIsGhost = !mInternalIsGhost;
					}
				}
			}
		}
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Component methods                 ]
	//[-------------------------------------------------------]
	void GhostComponent::serialize(qsf::BinarySerializer& serializer)
	{
		// Nothing here
	}


	//[-------------------------------------------------------]
	//[ Protected virtual em5::Component methods              ]
	//[-------------------------------------------------------]
	bool GhostComponent::onStartup()
	{
		mAlphaValue = (nullptr != getEntity().getComponent<VehicleComponent>()) ? 0.30f : 0.50f;

		registerMovableObjectRenderingListener();

		// Register job
		mSynchronizeJobProxy.registerAt(qsf::QsfJobs::REALTIME, boost::bind(&GhostComponent::synchronizeInternalAndRendererDataJob, this, _1));

		// TODO(sw) Enable this, if you want to test the component inside the editor
		/*if (isActive())
		{
			mInternalIsGhost = true;
		}*/
		return true;
	}

	void GhostComponent::onShutdown()
	{
		// Unregister job
		mSynchronizeJobProxy.unregister();

		mInternalIsGhost = false;
		if (mPlayerId != mLocalPlayerId)
		{
			resetGhostValues(getEntity());
		}
		unregisterMovableObjectRenderingListener();
	}

	void GhostComponent::onComponentPropertyChange(const qsf::Component& component, uint32 propertyId)
	{
		if ((qsf::MeshComponent::MESH == propertyId || qsf::MeshComponent::ENTITY == propertyId) && nullptr != dynamic_cast<const qsf::MeshComponent*>(&component))
		{
			mRendererIsGhost = false;
		}
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::MovableObjectRenderingListener methods ]
	//[-------------------------------------------------------]
	qsf::Component* GhostComponent::getMovableObjectRenderingListenerComponent()
	{
		return getEntity().getComponent<qsf::RendererComponent>();
	}

	void GhostComponent::onObjectRendering(const qsf::Component& component, const Ogre::Camera& ogreCamera)
	{
		if (mApplyDelay > 0)
		{
			// TODO(sw) This is an ugly hack!!!!
			// We have to force an re-apply of the ghost material values after 2 frames, when the values are applied the first time...
			// Otherwise some child entities of e.g. units might not be transparent (e.g. doors or wheels of road vehicles)
			--mApplyDelay;

			if (mApplyDelay == 0 && mInternalIsGhost)
			{
				// Force an update only if it is a ghost
				mRendererIsGhost = !mInternalIsGhost;
			}
		}

		// Call the base implementation
		JobProxyMovableObjectRenderingListener::onObjectRendering(component, ogreCamera);
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void GhostComponent::setupGhostValues(qsf::Entity& entity)
	{
		static qsf::MaterialVariationManager& materialVariationManager = QSF_MATERIAL.getMaterialVariationManager();

		qsf::EntityVectorScratchBuffer linkedEntities;
		qsf::LinkComponent::getConnectedLinkGroup(entity, qsf::LinkComponent::TRANSFORM, true, linkedEntities);

		for (qsf::Entity* linkedEntity : linkedEntities)
		{
			// Mark the entity as a ghost if it has a mesh component
			qsf::MeshComponent* meshComponent = linkedEntity->getComponent<qsf::MeshComponent>();
			if (nullptr != meshComponent)
			{
				materialVariationManager.setEntityMaterialPropertyValue(*linkedEntity, "SrcBlend", qsf::MaterialPropertyValue::fromBlend(qsf::Renderer::Blend::SRC_ALPHA));
				materialVariationManager.setEntityMaterialPropertyValue(*linkedEntity, "DestBlend", qsf::MaterialPropertyValue::fromBlend(qsf::Renderer::Blend::INV_SRC_ALPHA));
				materialVariationManager.setEntityMaterialPropertyValue(*linkedEntity, qsf::MaterialProperty::DEPTH_WRITE_MASK_ID, qsf::MaterialPropertyValue::fromDepthWriteMask(qsf::Renderer::DepthWriteMask::ZERO));

				{ // We can't just set the diffuse color to white, some materials like vehicle windows have a black diffuse color by intent
				  // (the artists painted the windows inside the diffuse texture for some reason white)
					Ogre::v1::Entity* ogreEntity = meshComponent->getOgreEntity();
					if (nullptr != ogreEntity)
					{
						const size_t numSubEntities = ogreEntity->getNumSubEntities();
						for (size_t subEntityIndex = 0; subEntityIndex < numSubEntities; ++subEntityIndex)
						{
							const float* float4Value = materialVariationManager.getOriginalMaterialPropertyValue(*linkedEntity, "DiffuseColor", static_cast<uint32>(subEntityIndex)).getFloat4Value();
							qsf::MaterialPropertyValue diffuseColor(qsf::MaterialPropertyValue::fromFloat4(float4Value[0], float4Value[1], float4Value[2], mAlphaValue));
							materialVariationManager.setOgreSubEntityMaterial(*ogreEntity, subEntityIndex, "DiffuseColor", diffuseColor);
						}
					}
				}

				// Set render queue ID
				meshComponent->setRenderQueueId(RENDER_QUEUE_ID);
			}
		}
	}

	void GhostComponent::resetGhostValues(qsf::Entity& entity)
	{
		static qsf::MaterialVariationManager& materialVariationManager = QSF_MATERIAL.getMaterialVariationManager();

		qsf::EntityVectorScratchBuffer linkedEntities;
		qsf::LinkComponent::getConnectedLinkGroup(entity, qsf::LinkComponent::TRANSFORM, true, linkedEntities);

		for (qsf::Entity* linkedEntity : linkedEntities)
		{
			// Reset the ghost values
			qsf::MeshComponent* meshComponent = linkedEntity->getComponent<qsf::MeshComponent>();
			if (nullptr != meshComponent)
			{
				materialVariationManager.setEntityMaterialPropertyValue(*linkedEntity, "SrcBlend", materialVariationManager.getOriginalMaterialPropertyValue(*linkedEntity, "SrcBlend"));
				materialVariationManager.setEntityMaterialPropertyValue(*linkedEntity, "DestBlend", materialVariationManager.getOriginalMaterialPropertyValue(*linkedEntity, "DestBlend"));
				materialVariationManager.setEntityMaterialPropertyValue(*linkedEntity, qsf::MaterialProperty::DEPTH_WRITE_MASK_ID, materialVariationManager.getOriginalMaterialPropertyValue(*linkedEntity, qsf::MaterialProperty::DEPTH_WRITE_MASK_ID));

				{ // We can't just set the diffuse color to white, some materials like vehicle windows have a black diffuse color by intent
				  // (the artists painted the windows inside the diffuse texture for some reason white)
					Ogre::v1::Entity* ogreEntity = meshComponent->getOgreEntity();
					if (nullptr != ogreEntity)
					{
						const size_t numSubEntities = ogreEntity->getNumSubEntities();
						for (size_t subEntityIndex = 0; subEntityIndex < numSubEntities; ++subEntityIndex)
						{
							materialVariationManager.setOgreSubEntityMaterial(*ogreEntity, subEntityIndex, "DiffuseColor", materialVariationManager.getOriginalMaterialPropertyValue(*linkedEntity, "DiffuseColor", static_cast<uint32>(subEntityIndex)));
						}
					}
				}

				meshComponent->setRenderQueueId(qsf::getUninitialized<uint32>());
			}
		}
	}

	void GhostComponent::synchronizeInternalAndRendererDataJob(const qsf::JobArguments&)
	{
		if (mInternalIsGhost != mRendererIsGhost)
		{
			mRendererIsGhost = mInternalIsGhost;
			if (mInternalIsGhost)
			{
				setupGhostValues(getEntity());
			}
			else
			{
				resetGhostValues(getEntity());
			}
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

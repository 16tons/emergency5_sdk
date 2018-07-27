// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/component/vegetation/HedgeComponent.h"
#include "em5/action/base/DisappearAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/component/effect/FadeEffectComponent.h"
#include "em5/fire/component/FireComponent.h"
#include "em5/plugin/Messages.h"

#include <qsf/component/base/TransformComponent.h>
#include <qsf/component/link/LinkComponent.h>
#include <qsf/physics/collision/CollisionComponent.h>
#include <qsf/renderer/RendererSystem.h>
#include <qsf/renderer/mesh/MeshComponent.h>
#include <qsf/renderer/material/MaterialSystem.h>
#include <qsf/renderer/material/material/MaterialProperty.h>
#include <qsf/renderer/material/material/MaterialVariationManager.h>
#include <qsf/logic/action/ActionComponent.h>
#include <qsf/message/MessageSystem.h>
#include <qsf/map/Entity.h>
#include <qsf/map/Map.h>
#include <qsf/map/query/GroundMapQuery.h>
#include <qsf/base/ScratchBuffer.h>
#include <qsf/plugin/QsfJobs.h>
#include <qsf/QsfHelper.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 HedgeComponent::COMPONENT_ID	= qsf::StringHash("em5::HedgeComponent");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	void HedgeComponent::cutHedge()
	{
		if (mInternalFlags.isClear(HEDGE_IS_CUT))
		{
			const qsf::TransformComponent* transformComponent = getEntity().getComponent<qsf::TransformComponent>();
			if (nullptr != transformComponent)
			{
				// Save the current scale
				mScaleTemp = transformComponent->getScale();

				// Add a disappear action
				qsf::ActionComponent& actionComponent = getEntity().getOrCreateComponentSafe<qsf::ActionComponent>();
				actionComponent.clearPlan();

				// TODO(mk) Move Hard coded time in extra file or member
				actionComponent.pushAction<DisappearAction>(action::AUTOMATISM_STD).init(qsf::Time::fromSeconds(1.0f), false);
				mInternalFlags.set(HEDGE_IS_CUT, true);

				// Disable collision
				setCollisionEnabled(false);

				sendGetsDamagedMessage();
			}
		}
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::DamageComponent methods           ]
	//[-------------------------------------------------------]
	void HedgeComponent::cleanup()
	{
		DamageComponent::cleanup();

		// TODO(tl): Cleanup should set the component back and not set it to certain values !

		if (mInternalFlags.isSet(HEDGE_IS_CUT))
		{
			qsf::Entity& entity = getEntity();

			// Unhide the entity
			entity.setHidden(false);

			// Also do fast fade in. Because the child will still be invisible....
			entity.getOrCreateComponentSafe<FadeEffectComponent>().fadeIn(qsf::Time::ZERO);

			// Update the transformation
			qsf::TransformComponent* transformComponent = entity.getComponent<qsf::TransformComponent>();
			if (nullptr != transformComponent)
			{
				transformComponent->setScale(mScaleTemp);
			}

			// Clean up and destroy ground decal
			if (qsf::isInitialized(mGroundDecalEntityId))
			{
				entity.getMap().destroyEntityById(mGroundDecalEntityId);
				mGroundDecalEntityId = qsf::getUninitialized<uint64>();
			}

			// Update the mesh
			qsf::MeshComponent* meshComponent = entity.getComponent<qsf::MeshComponent>();
			if (nullptr != meshComponent)
			{
				meshComponent->setActive(true);
			}

			// Enable collision
			setCollisionEnabled(true);

			mInternalFlags.set(HEDGE_IS_CUT, false);
		}
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Component methods                 ]
	//[-------------------------------------------------------]
	bool HedgeComponent::onStartup()
	{
		return true;
	}

	void HedgeComponent::serialize(qsf::BinarySerializer& serializer)
	{
		bool hedgeIsCutFlag = false;
		if (serializer.isReading())
		{
			serializer.serialize(hedgeIsCutFlag);
			mInternalFlags.set(HEDGE_IS_CUT, hedgeIsCutFlag);
		}
		else
		{
			hedgeIsCutFlag = mInternalFlags.isSet(HEDGE_IS_CUT);
			serializer.serialize(hedgeIsCutFlag);
		}
	}


	//[-------------------------------------------------------]
	//[ Protected virtual em5::DamageComponent methods        ]
	//[-------------------------------------------------------]
	void HedgeComponent::onSetIntact()
	{
		// Turn off fire
		setFireStateIntact();

		// Set the state of smut & emissive burn overlays
		qsf::Entity& entity = getEntity();
		qsf::MeshComponent* meshComponent = entity.getComponent<qsf::MeshComponent>();
		if (nullptr != meshComponent)
		{
			// Clean up and destroy ground decal
			if (qsf::isInitialized(mGroundDecalEntityId))
			{
				entity.getMap().destroyEntityById(mGroundDecalEntityId);
				qsf::setUninitialized(mGroundDecalEntityId);
			}

			// Show original mesh
			meshComponent->setActive(true);

			setDamageOverlaysActive(false, false);

			// Update fire value
			static qsf::MaterialVariationManager& materialVariationManager = QSF_MATERIAL.getMaterialVariationManager();
			materialVariationManager.setEntityMaterialPropertyValue(entity, "FireGlowIntensity", qsf::MaterialPropertyValue::fromFloat(0.0f));
		}
	}

	void HedgeComponent::onSetDamagedLevel1()
	{
		const FireComponent* fireComponent = getBurningFireComponent();
		if (nullptr != fireComponent)
		{
			setDamageOverlaysActive(true, true);
		}

		sendGetsDamagedMessage();
	}

	void HedgeComponent::onSetDamagedLevel2()
	{
		onSetDamagedLevel1();
	}

	void HedgeComponent::onSetDestroyed()
	{
		const qsf::Entity& entity = getEntity();
		const FireComponent* fireComponent = getBurningFireComponent();
		if (nullptr != fireComponent)
		{
			setDamageOverlaysActive(true, true);

			// POOF - Hide hedge mesh and show burnt ground decal
			qsf::MeshComponent* meshComponent = entity.getComponent<qsf::MeshComponent>();
			if (nullptr != meshComponent)
			{
				meshComponent->setActive(false);

				// TODO(ae) EM4 router update: Router->UpdateObjectTerrain

				qsf::Map& map = entity.getMap();

				glm::vec3 min;
				glm::vec3 max;
				meshComponent->getWorldAxisAlignedBoundingBox(min, max);

				glm::vec3 center = (min + max) * 0.5f;
				qsf::GroundMapQuery(map).getTerrainHeightAtXZPosition(center.x, center.z, center.y);
				center.y += 0.025f;

				qsf::Entity* groundDecal = map.createEntity();
				qsf::MeshComponent& groundDecalMesh = groundDecal->getOrCreateComponentSafe<qsf::MeshComponent>();
				groundDecalMesh.setMesh(getDestroyedGroundDecalMesh());
				groundDecalMesh.setActive(true);

				qsf::TransformComponent& groundDecalTransformComponent = groundDecal->getOrCreateComponentSafe<qsf::TransformComponent>();
				groundDecalTransformComponent.setPosition(center);

				mGroundDecalEntityId = groundDecal->getId();
			}
		}

		// Turn off fire
		setFireStateDestroyed();
	}

	void HedgeComponent::onSetFireLifePercentage(float fireLifePercentage)
	{
		if (fireLifePercentage == 0.0f)
		{
			setDamageState(DAMAGESTATE_DESTROYED);
		}
		else if (fireLifePercentage < 1.0f && getDamageState() < DAMAGESTATE_DAMAGE_LEVEL1)
		{
			setDamageState(DAMAGESTATE_DAMAGE_LEVEL1);
		}
	}

	void HedgeComponent::onFireStarted(FreeplayEvent* freeplayEvent)
	{
		// Call base implementation
		DamageComponent::onFireStarted(freeplayEvent);

		// For burning hedges, use at least damage state 1
		if (getDamageState() < DAMAGESTATE_DAMAGE_LEVEL1)
		{
			setDamageState(DAMAGESTATE_DAMAGE_LEVEL1);
		}

		setupFireFadeAnimation();
	}

	void HedgeComponent::setCustomParametersForFireGlowAndSmutOverlay(const qsf::MeshComponent& meshComponent)
	{
		DamageComponent::setCustomParametersForFireGlowAndSmutOverlay(meshComponent);

		// Alpha reference
		// -> This component has the control, even inside the editor. If an artist wants to play around with materials he just needs to remove this component.
		if (isRunning())
		{
			static qsf::MaterialVariationManager& materialVariationManager = QSF_MATERIAL.getMaterialVariationManager();
			const float originalAlphaReference = materialVariationManager.getOriginalMaterialPropertyValue(meshComponent.getEntity(), "AlphaReference").getFloatValue();
			const float originalAlphaReferenceMax = materialVariationManager.getOriginalMaterialPropertyValue(meshComponent.getEntity(), "AlphaReferenceMax").getFloatValue();
			const float alphaReference = std::min(std::max(originalAlphaReferenceMax * mPeakFireEnergyPercent, originalAlphaReference), originalAlphaReferenceMax);
			materialVariationManager.setEntityMaterialPropertyValue(meshComponent.getEntity(), "AlphaReference", qsf::MaterialPropertyValue::fromFloat(alphaReference));
		}
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void HedgeComponent::setDamageOverlaysActive(bool smutActive, bool emissiveActive)
	{
		if (isRunning())
		{
			static qsf::MaterialVariationManager& materialVariationManager = QSF_MATERIAL.getMaterialVariationManager();

			// Set the state of smut & emissive burn overlays
			qsf::EntityVectorScratchBuffer linkedEntities;
			qsf::LinkComponent::getConnectedLinkGroup(getEntity(), qsf::LinkComponent::TRANSFORM, true, linkedEntities);
			for (qsf::Entity* linkedEntity : *linkedEntities)
			{
				materialVariationManager.setEntityMaterialPropertyValue(*linkedEntity, "ApplySmut", qsf::MaterialPropertyValue::fromBoolean(smutActive));				// Enable smut map
				materialVariationManager.setEntityMaterialPropertyValue(*linkedEntity, "UseEmissiveMap", qsf::MaterialPropertyValue::fromBoolean(emissiveActive));	// Enable emissive map
				materialVariationManager.setEntityMaterialPropertyValue(*linkedEntity, "ApplyFireGlow", qsf::MaterialPropertyValue::fromBoolean(emissiveActive));		// Enable Perlin noise on emissive map
			}
		}
	}

	void HedgeComponent::sendGetsDamagedMessage()
	{
		// Emit message
		qsf::MessageParameters parameters;
		parameters.setParameter("EntityId", getEntityId());
		QSF_MESSAGE.emitMessage(qsf::MessageConfiguration(Messages::EM5_COMPONENT_GETSDAMAGED_VEGETATION, getEntityId()), parameters);
	}

	void HedgeComponent::setCollisionEnabled(bool collisionEnabled)
	{
		qsf::CollisionComponent* collisionComponent = getEntity().getComponent<qsf::CollisionComponent>();
		if (nullptr != collisionComponent)
		{
			if (collisionEnabled)
			{
				collisionComponent->startup(isRunning());
			}
			else
			{
				collisionComponent->shutdown();
			}
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/component/vegetation/TreeComponent.h"
#include "em5/component/vegetation/TreeFallDownAnimation.h"
#include "em5/action/ActionPriority.h"
#include "em5/component/effect/FadeEffectComponent.h"
#include "em5/fire/component/FireComponent.h"
#include "em5/map/EntityHelper.h"
#include "em5/plugin/Messages.h"

#include <qsf/component/base/TransformComponent.h>
#include <qsf/component/link/LinkComponent.h>
#include <qsf/job/JobArguments.h>
#include <qsf/logic/action/ActionComponent.h>
#include <qsf/math/Math.h>
#include <qsf/math/CoordinateSystem.h>
#include <qsf/message/MessageSystem.h>
#include <qsf/renderer/RendererSystem.h>
#include <qsf/renderer/mesh/MeshComponent.h>
#include <qsf/renderer/particles/ParticlesComponent.h>
#include <qsf/renderer/material/MaterialSystem.h>
#include <qsf/renderer/material/material/MaterialProperty.h>
#include <qsf/renderer/material/material/MaterialVariationManager.h>
#include <qsf/base/ScratchBuffer.h>
#include <qsf/QsfHelper.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 TreeComponent::COMPONENT_ID	= qsf::StringHash("em5::TreeComponent");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	void TreeComponent::cutTree(qsf::Entity& firefighterEntity)
	{
		if (mInternalFlags.isClear(TREE_IS_CUT))
		{
			// Start animation
			QSF_SAFE_DELETE(mTreeFallDownAnimation);
			mTreeFallDownAnimation = new TreeFallDownAnimation(*this, firefighterEntity);
			mTreeFallDownAnimation->start();

			// Set state accordingly
			mInternalFlags.set(TREE_IS_CUT, true);

			// Deactivate the fire component, tree can't burn from now on
			FireComponent* fireComponent = getEntity().getComponent<FireComponent>();
			if (nullptr != fireComponent)
			{
				fireComponent->setActive(false);
			}

			// Save original rotation of the treetop
			const qsf::LinkComponent* linkComponent = getEntity().getComponent<qsf::LinkComponent>();
			if (nullptr != linkComponent)
			{
				for (const qsf::LinkComponent* childLinkComponent : linkComponent->getChildLinks())
				{
					qsf::TransformComponent* transformComponent = childLinkComponent->getEntity().getComponent<qsf::TransformComponent>();
					if (nullptr != transformComponent)
					{
						mInitialChildRotation = transformComponent->getRotation();
					}
				}
			}
		}
	}

	bool TreeComponent::isIntact() const
	{
		return !mInternalFlags.isSet(TREE_IS_CUT);
	}

	qsf::Entity* TreeComponent::getTreeTop()
	{
		const qsf::LinkComponent* linkComponent = getEntity().getComponent<qsf::LinkComponent>();
		if (nullptr != linkComponent)
		{
			for (const qsf::LinkComponent* childLinkComponent : linkComponent->getChildLinks())
			{
				// TODO(sw) This works only correctly, when the tree top is the only child with a mesh component
				qsf::Entity& childEntity = childLinkComponent->getEntity();
				qsf::TransformComponent* transformComponent = childEntity.getComponent<qsf::TransformComponent>();
				qsf::MeshComponent* meshComponent = childEntity.getComponent<qsf::MeshComponent>();
				if (nullptr != transformComponent && nullptr != meshComponent)
				{
					return &childEntity;
				}
			}
		}
		return nullptr;
	}

	bool TreeComponent::getWasCut() const
	{
		return mInternalFlags.isSet(TREE_IS_CUT);
	}

	void TreeComponent::setWasCut(bool wasCut)
	{
		mInternalFlags.set(TREE_IS_CUT, wasCut);
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::DamageComponent methods           ]
	//[-------------------------------------------------------]
	void TreeComponent::cleanup()
	{
		DamageComponent::cleanup();

		// TODO(tl): Cleanup should set the component back and not set it to certain values! Can be source of bugs

		// Reactivate the fire component
		FireComponent* fireComponent = getEntity().getComponent<FireComponent>();
		if (nullptr != fireComponent)
		{
			fireComponent->setActive(true);
		}

		// Get the link component
		const qsf::LinkComponent* linkComponent = getEntity().getComponent<qsf::LinkComponent>();
		if (nullptr != linkComponent)
		{
			for (const qsf::LinkComponent* childLinkComponent : linkComponent->getChildLinks())
			{
				qsf::Entity& linkedEntity = childLinkComponent->getEntity();

				// Unhide the child
				linkedEntity.setHidden(false);

				// Only change Meshs not particles / lights ec.
				qsf::MeshComponent* meshComponent = linkedEntity.getComponent<qsf::MeshComponent>();
				if (nullptr != meshComponent)
				{
					// Also do fast fade in. Because the child will still be invisible....
					linkedEntity.getOrCreateComponentSafe<FadeEffectComponent>().fadeIn(qsf::Time::ZERO);

					{ // Update the transform
						qsf::TransformComponent* transformComponent = linkedEntity.getComponent<qsf::TransformComponent>();
						if (nullptr != transformComponent)
						{
							// Reset scale
							transformComponent->setScale(qsf::Math::GLM_VEC3_UNIT_XYZ);

							// Only do rotation reset, when the tree was cut..
							if (mInternalFlags.isSet(TREE_IS_CUT))
							{
								// Reset rotation
								transformComponent->setRotation(mInitialChildRotation);
							}
						}
					}

					// Update the mesh
					meshComponent->setActive(true);
				}
			}
		}

		// Stop animation
		destroyTreeFallDownAnimation();
		getEntity().destroyComponent<qsf::ActionComponent>();

		mInternalFlags.set(TREE_IS_CUT, false);
	}


	//[-------------------------------------------------------]
	//[ Protected virtual em5::DamageComponent methods        ]
	//[-------------------------------------------------------]
	void TreeComponent::onSetIntact()
	{
		// Turn off fire
		setFireStateIntact();

		setDamageOverlaysActive(false, false);
		updateMaterialAndLeafParticles(true);
		updateTintableMesh();
	}

	void TreeComponent::onSetDamagedLevel1()
	{
		setDamageOverlaysActive(true, true);
		updateMaterialAndLeafParticles(false);
		updateTintableMesh();

		// Emit message
		qsf::MessageParameters parameters;
		parameters.setParameter("EntityId", getEntityId());
		QSF_MESSAGE.emitMessage(qsf::MessageConfiguration(Messages::EM5_COMPONENT_GETSDAMAGED_VEGETATION, getEntityId()), parameters);
	}

	void TreeComponent::onSetDamagedLevel2()
	{
		onSetDamagedLevel1();
	}

	void TreeComponent::onSetDestroyed()
	{
		// Turn off fire
		setFireStateDestroyed();

		setDamageOverlaysActive(true, true);
		updateMaterialAndLeafParticles(false);
		updateTintableMesh();
	}

	void TreeComponent::onSetFireLifePercentage(float fireLifePercentage)
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

	void TreeComponent::onFireStarted(FreeplayEvent* freeplayEvent)
	{
		// Call base implementation
		DamageComponent::onFireStarted(freeplayEvent);

		// For burning trees, use at least damage state 1
		if (getDamageState() < DAMAGESTATE_DAMAGE_LEVEL1)
		{
			setDamageState(DAMAGESTATE_DAMAGE_LEVEL1);
		}

		setupFireFadeAnimation();
	}

	void TreeComponent::setCustomParametersForFireGlowAndSmutOverlay(const qsf::MeshComponent& meshComponent)
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
	//[ Protected virtual qsf::Component methods              ]
	//[-------------------------------------------------------]
	void TreeComponent::onSetSimulating(bool simulating)
	{
		DamageComponent::onSetSimulating(simulating);

		if (!simulating)
		{
			destroyTreeFallDownAnimation();
		}
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void TreeComponent::updateMaterialAndLeafParticles(bool treeParticlesActive) const
	{
		qsf::EntityVectorScratchBuffer linkedEntities;
		qsf::LinkComponent::getConnectedLinkGroup(getEntity(), qsf::LinkComponent::TRANSFORM, true, linkedEntities);
		for (const qsf::Entity* linkedEntity : linkedEntities)
		{
			qsf::ParticlesComponent* particlesComponent = linkedEntity->getComponent<qsf::ParticlesComponent>();
			if (nullptr != particlesComponent && boost::algorithm::icontains(particlesComponent->getParticles().getLocalAssetName(), "falling_leaves"))
			{
				particlesComponent->setEmitterEnabled(treeParticlesActive);
			}
		}
	}

	void TreeComponent::setDamageOverlaysActive(bool smutActive, bool emissiveActive) const
	{
		if (isRunning())
		{
			static qsf::MaterialVariationManager& materialVariationManager = QSF_MATERIAL.getMaterialVariationManager();

			// Set the state of smut & emissive burn overlays
			qsf::EntityVectorScratchBuffer linkedEntities;
			qsf::LinkComponent::getConnectedLinkGroup(getEntity(), qsf::LinkComponent::TRANSFORM, true, linkedEntities);
			for (qsf::Entity* linkedEntity : linkedEntities)
			{
				// TODO(fw): With these options enabled, trees look very weird, so I had to disable them for the GC version
				materialVariationManager.setEntityMaterialPropertyValue(*linkedEntity, "ApplySmut", qsf::MaterialPropertyValue::fromBoolean(smutActive));				// Enable smut map
				materialVariationManager.setEntityMaterialPropertyValue(*linkedEntity, "UseEmissiveMap", qsf::MaterialPropertyValue::fromBoolean(emissiveActive));	// Enable emissive map
				materialVariationManager.setEntityMaterialPropertyValue(*linkedEntity, "ApplyFireGlow", qsf::MaterialPropertyValue::fromBoolean(emissiveActive));		// Enable Perlin noise on emissive map
			}
		}
	}

	void TreeComponent::destroyTreeFallDownAnimation()
	{
		QSF_SAFE_DELETE(mTreeFallDownAnimation);
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Component methods                 ]
	//[-------------------------------------------------------]
	void TreeComponent::serialize(qsf::BinarySerializer& serializer)
	{
		bool treeIsCutFlag = false;
		if (serializer.isReading())
		{
			serializer.serialize(treeIsCutFlag);
			mInternalFlags.set(TREE_IS_CUT, treeIsCutFlag);
		}
		else
		{
			treeIsCutFlag = mInternalFlags.isSet(TREE_IS_CUT);
			serializer.serialize(treeIsCutFlag);
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

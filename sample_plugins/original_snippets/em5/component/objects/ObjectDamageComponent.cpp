// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/component/objects/ObjectDamageComponent.h"
#include "em5/component/effect/FadeEffectComponent.h"
#include "em5/component/person/PersonComponent.h"
#include "em5/fire/component/ComplexFireComponent.h"
#include "em5/plugin/Messages.h"
#include "em5/logic/AtmosphericRadioMessageHelper.h"

#include <qsf/component/base/TransformComponent.h>
#include <qsf/component/link/LinkComponent.h>
#include <qsf/logic/action/ActionComponent.h>
#include <qsf/math/Math.h>
#include <qsf/message/MessageSystem.h>
#include <qsf/renderer/RendererSystem.h>
#include <qsf/renderer/mesh/MeshComponent.h>
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
	const uint32 ObjectDamageComponent::COMPONENT_ID	= qsf::StringHash("em5::ObjectDamageComponent");


	//[-------------------------------------------------------]
	//[ Public virtual em5::DamageComponent methods           ]
	//[-------------------------------------------------------]
	void ObjectDamageComponent::cleanup()
	{
		DamageComponent::cleanup();

		// TODO(tl): Cleanup should set the component back and not set it to certain values ! Can be source of bugs

		// Reactivate the fire
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

				// Only change meshes not particles / lights ec.
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
						}
					}

					// Update the mesh
					meshComponent->setActive(true);
				}
			}
		}
	}


	//[-------------------------------------------------------]
	//[ Protected virtual em5::DamageComponent methods        ]
	//[-------------------------------------------------------]
	void ObjectDamageComponent::onSetIntact()
	{
		// Turn off fire
		setFireStateIntact();

		setDamageOverlaysActive(false, false);
		updateTintableMesh();
	}

	void ObjectDamageComponent::onSetDamagedLevel1()
	{
		setDamageOverlaysActive(true, true);
		updateTintableMesh();

		// Emit message ID
		qsf::MessageParameters parameters;
		parameters.setParameter("EntityId", getEntityId());
		QSF_MESSAGE.emitMessage(qsf::MessageConfiguration(Messages::EM5_COMPONENT_GETSDAMAGED_VEGETATION, getEntityId()), parameters);
	}

	void ObjectDamageComponent::onSetDamagedLevel2()
	{
		// The same as damage state 1
		onSetDamagedLevel1();
	}

	void ObjectDamageComponent::onSetDestroyed()
	{
		// Turn off fire
		setFireStateDestroyed();

		setDamageOverlaysActive(true, true);
		updateTintableMesh();
	}

	void ObjectDamageComponent::onSetFireLifePercentage(float fireLifePercentage)
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

	void ObjectDamageComponent::onFireStarted(FreeplayEvent* freeplayEvent)
	{
		// Call base implementation
		DamageComponent::onFireStarted(freeplayEvent);

		// For burning objects, use damage state 1
		if (getDamageState() < DAMAGESTATE_DAMAGE_LEVEL1)
		{
			setDamageState(DAMAGESTATE_DAMAGE_LEVEL1);
		}

		setupFireFadeAnimation();
	}

	void ObjectDamageComponent::onExplosion(FireComponent* fireComponent)
	{
		FireComponent* newFireComponent = fireComponent;

		// Get a new FireComponent if there is no one given
		if (newFireComponent == nullptr)
		{
			newFireComponent = getEntity().getComponent<FireComponent>();
		}

		// If there is a fire component start explosion on this
		if (newFireComponent != nullptr)
		{
			newFireComponent->showOnExplosionFireParticles();
		}
		// Else search for a complex fire component
		else
		{
			ComplexFireComponent* complexFireComponent = getEntity().getComponent<ComplexFireComponent>();
			if (complexFireComponent != nullptr)
			{
				std::vector<FireComponent*> fireComponents;
				complexFireComponent->getAllFireComponents(fireComponents);
				for (FireComponent* fireComp : fireComponents)
				{
					fireComp->showOnExplosionFireParticles();
				}
			}
		}

		// Play atmospheric hint
		AtmosphericRadioMessageHelper::triggerAtmoHintExplosion(getEntity());
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void ObjectDamageComponent::setDamageOverlaysActive(bool smutActive, bool emissiveActive)
	{
		if (isRunning())
		{
			static qsf::MaterialVariationManager& materialVariationManager = QSF_MATERIAL.getMaterialVariationManager();

			// Set the state of smut & emissive burn overlays
			qsf::EntityVectorScratchBuffer linkedEntities;
			qsf::LinkComponent::getConnectedLinkGroup(getEntity(), qsf::LinkComponent::TRANSFORM, true, linkedEntities);
			for (qsf::Entity* linkedEntity : linkedEntities)
			{
				// Please, don't apply fire glow, damage etc. to persons
				if (nullptr == linkedEntity->getComponent<PersonComponent>())
				{
					materialVariationManager.setEntityMaterialPropertyValue(*linkedEntity, "ApplySmut", qsf::MaterialPropertyValue::fromBoolean(smutActive));			// Enable smut map
					materialVariationManager.setEntityMaterialPropertyValue(*linkedEntity, "UseEmissiveMap", qsf::MaterialPropertyValue::fromBoolean(emissiveActive));	// Enable emissive map
					materialVariationManager.setEntityMaterialPropertyValue(*linkedEntity, "ApplyFireGlow", qsf::MaterialPropertyValue::fromBoolean(emissiveActive));	// Enable Perlin noise on emissive map
				}
			}
		}
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Component methods                 ]
	//[-------------------------------------------------------]
	bool ObjectDamageComponent::onStartup()
	{
		return DamageComponent::onStartup();
	}

	void ObjectDamageComponent::serialize(qsf::BinarySerializer& serializer)
	{
		// Nothing to serialize at the moment
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/component/damage/DamageComponent.h"
#include "em5/fire/component/FireComponent.h"
#include "em5/fire/component/ComplexFireComponent.h"
#include "em5/game/Game.h"
#include "em5/plugin/GameCounters.h"
#include "em5/map/EntityHelper.h"
#include "em5/EM5Helper.h"

#include <qsf_compositing/component/TintableMeshComponent.h>

#include <qsf/QsfHelper.h>
#include <qsf/component/link/LinkComponent.h>
#include <qsf/map/Entity.h>
#include <qsf/plugin/QsfJobs.h>
#include <qsf/renderer/RendererSystem.h>
#include <qsf/renderer/material/MaterialSystem.h>
#include <qsf/renderer/material/material/MaterialProperty.h>
#include <qsf/renderer/material/material/MaterialVariationManager.h>

#include <OGRE/OgreEntity.h>
#include <OGRE/OgreSubEntity.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 DamageComponent::COMPONENT_ID		= qsf::StringHash("em5::DamageComponent");
	const uint32 DamageComponent::DAMAGE			= qsf::StringHash("Damage");
	const uint32 DamageComponent::DAMAGE_STATE		= qsf::StringHash("DamageState");
	const uint32 DamageComponent::DAMAGE_SIMULATION	= qsf::StringHash("DamageSimulation");


	//[-------------------------------------------------------]
	//[ Public em5::DamageComponent methods                   ]
	//[-------------------------------------------------------]
	void DamageComponent::setDamageState(DamageState damageState)
	{
		if (mDamageState != damageState)
		{
			mDamageState = damageState;

			if (isRunning())
			{
				internalDamageStateUpdated();
			}

			// Promote the property change
			promotePropertyChange(DAMAGE_STATE);
		}
	}

	const FireComponent* DamageComponent::getBurningFireComponent() const
	{
		const FireComponent* fireComponent = getEntity().getComponent<FireComponent>();
		if (nullptr != fireComponent)
		{
			if (!fireComponent->isBurning())
			{
				return nullptr;
			}
		}
		else
		{
			// Search in complex fire component
			const ComplexFireComponent* complexFireComponent = getEntity().getComponent<ComplexFireComponent>();
			if (nullptr != complexFireComponent)
			{
				std::vector<FireComponent*> fireComponents;
				complexFireComponent->getAllFireComponents(fireComponents);

				// Find the fire component with the highest energy
				for (auto component : fireComponents)
				{
					if (component->isBurning() && (fireComponent == nullptr || fireComponent->getEnergy() < component->getEnergy()))
					{
						fireComponent = component;
					}
				}
			}
		}

		return fireComponent;
	}

	void DamageComponent::showExplosionFireParticles()
	{
		onExplosion(nullptr);
	}

	void DamageComponent::setFireExtinguishBlocked(bool blocked)
	{
		FireComponent* fireComponent = getEntity().getComponent<FireComponent>();
		if (nullptr != fireComponent)
		{
			fireComponent->setExtinguishBlocked(blocked);
		}
		else
		{
			// Search in complex fire component
			ComplexFireComponent* complexFireComponent = getEntity().getComponent<ComplexFireComponent>();
			if (nullptr != complexFireComponent)
			{
				std::vector<FireComponent*> fireComponents;
				complexFireComponent->getAllFireComponents(fireComponents);

				// Find the fire component with the highest energy
				for (auto component : fireComponents)
				{
					component->setExtinguishBlocked(blocked);
				}
			}
		}
	}

	void DamageComponent::setCustomParametersForFireGlowAndSmutOverlay(const qsf::MeshComponent& meshComponent)
	{
		const float cutFirstPercent = 0.10f;
		const float firePercent = std::min(1.0f, std::max(0.0f, (mPeakFireEnergyPercent - cutFirstPercent) / (1.0f - cutFirstPercent)));
		static qsf::MaterialVariationManager& materialVariationManager = QSF_MATERIAL.getMaterialVariationManager();

		// TODO(co) Update EMERGENCY 5 heat renderer
		// Set the heat for the heat renderer, using ogre custom parameter 20 (we need to do this every time the object turns visible)
		// meshComponent.setOgreCustomParameter(20, glm::vec4(mPeakFireEnergyPercent, 0.0f, 0.0f, 0.0f));

		// Set smut and fire glow intensity
		const float smutOverlayIntensity = 1.0f - pow(firePercent - 1.0f, 4.0f);
		qsf::Entity& entity = meshComponent.getEntity();
		materialVariationManager.setEntityMaterialPropertyValue(entity, "SmutOverlayIntensity", qsf::MaterialPropertyValue::fromFloat(smutOverlayIntensity));
		materialVariationManager.setEntityMaterialPropertyValue(entity, "FireGlowIntensity", qsf::MaterialPropertyValue::fromFloat(mEmissiveMapStrength * mFireGlowIntensity));
	}

	void DamageComponent::internalDamageStateUpdated(bool isStartup)
	{
		// Call sub-class implementation
		onSetDamageState(mDamageState);

		switch (mDamageState)
		{
			case DAMAGESTATE_INTACT:
			{
				mPeakFireEnergyPercent = 0.0f;
				mEmissiveMapStrength = 0.0f;

				onSetIntact();
				break;
			}

			case DAMAGESTATE_DAMAGE_LEVEL1:
				if (isStartup || !isSimulating())
				{
					// Ensure we have a nice preview inside the editor or during initial component startup
					mPeakFireEnergyPercent = 0.3f;
					mEmissiveMapStrength = 0.3f;
				}
				onSetDamagedLevel1();
				break;

			case DAMAGESTATE_DAMAGE_LEVEL2:
				if (isStartup || !isSimulating())
				{
					// Ensure we have a nice preview inside the editor or during initial component startup
					mPeakFireEnergyPercent = 0.6f;
					mEmissiveMapStrength = 0.6f;
				}

				// Remove ghost effect when active
				EntityHelper::teardownEntityAsGhost(getEntity());
				onSetDamagedLevel2();
				break;

			case DAMAGESTATE_DESTROYED:
				if (isStartup || !isSimulating())
				{
					// Ensure we have a nice preview inside the editor or during initial component startup
					mPeakFireEnergyPercent = 1.0f;
					mEmissiveMapStrength = 0.0f;
				}

				// Remove ghost effect when active
				EntityHelper::teardownEntityAsGhost(getEntity());
				onSetDestroyed();
				break;

			case DAMAGESTATE_INVALID:
				// Nothing to do in this case
				break;
		}

		// The state has been changed, ensure the fire glow and smut overlay visualization is correct
		setCustomParametersForFireGlowAndSmutOverlay();

		{ // Update the fire component particles accordingly
			FireComponent* fireComponent = getEntity().getComponent<FireComponent>();
			if (nullptr != fireComponent)
			{
				fireComponent->updateParticlesVisibility();
			}
			else
			{
				ComplexFireComponent* complexFireComponent = getEntity().getComponent<ComplexFireComponent>();
				if (nullptr != complexFireComponent)
				{
					complexFireComponent->updateParticlesVisibility();
				}
			}
		}
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::DamageComponent methods           ]
	//[-------------------------------------------------------]
	void DamageComponent::receiveDamage(float amount, damagetype::DamageType damageType, qsf::Component* sender)
	{
		// In case of fire damage, we have a special callback
		if (damageType == damagetype::FIRE && amount > 0.0f)
		{
			const qsf::Time currentTimestamp = EM5_GAME.getSimulationClock().getCurrentTime();
			const bool damageJustStarted = (currentTimestamp - mLastReceivedFireDamageTimestamp >= qsf::Time::fromSeconds(0.5f));
			mLastReceivedFireDamageTimestamp = currentTimestamp;

			// Call virtual method
			onFireDamageReceived(amount, sender, damageJustStarted);
		}
	}

	bool DamageComponent::checkReceiveDamage(damagetype::DamageType damageType, qsf::Component* sender)
	{
		return getDamageSimulation();
	}

	void DamageComponent::cleanup()
	{
		setDamageState(DAMAGESTATE_INTACT);
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Component methods                 ]
	//[-------------------------------------------------------]
	void DamageComponent::onSetSimulating(bool simulating)
	{
		if (simulating)
		{
			// It's valid that a damage component is not intact on startup, ensure all depending states are consistent
			// -> Performance: No need to also do this also for the intact state, we consider this as the initial state and don't switch between simulation off, change properties, switch simulation on
			if (mDamageState != DAMAGESTATE_INTACT)
			{
				internalDamageStateUpdated(true);
			}
		}
		else
		{
			// We going out of simulation business check if we need to decrement a game counter

			// Reset material variation
			if (mDamageState != DAMAGESTATE_INTACT)
			{
				QSF_MATERIAL.getMaterialVariationManager().resetEntityMaterialPropertyValues(getEntity());
			}

			if (isBurning())
			{
				GameCounters::decrementGameCounter(GameCounters::OBJECTS_BURNING);
			}
		}
	}

	void DamageComponent::serialize(qsf::BinarySerializer& serializer)
	{
		// Call base class implementation
		qsf::Component::serialize(serializer);

		serializer.serialize(mIsDamageSimulationActive);
		serializer.serializeAs<int8>(mDamageState);
	}

	bool DamageComponent::onStartup()
	{
		registerMovableObjectRenderingListener();

		// Done
		return true;
	}

	void DamageComponent::onShutdown()
	{
		unregisterMovableObjectRenderingListener();
	}


	//[-------------------------------------------------------]
	//[ Protected virtual em5::DamageComponent methods        ]
	//[-------------------------------------------------------]
	void DamageComponent::onFireStarted(FreeplayEvent* freeplayEvent)
	{
		// Counting other burning elements
		GameCounters::incrementGameCounter(GameCounters::OBJECTS_BURNING);
	}

	void DamageComponent::onFireStopped()
	{
		// Counting other burning elements
		GameCounters::decrementGameCounter(GameCounters::OBJECTS_BURNING);
	}


	//[-------------------------------------------------------]
	//[ Protected static methods                              ]
	//[-------------------------------------------------------]
	void DamageComponent::exchangeDamageMaterials(qsf::Entity& entity, bool setDamaged)
	{
		qsf::MaterialVariationManager::Materials materials;

		static qsf::MaterialVariationManager& materialVariationManager = QSF_MATERIAL.getMaterialVariationManager();
		if (setDamaged)
		{
			// Use the damage materials
			qsf::MeshComponent* meshComponent = entity.getComponent<qsf::MeshComponent>();
			if (nullptr != meshComponent)
			{
				const Ogre::v1::Entity* ogreEntity = meshComponent->getOgreEntity();
				if (nullptr != ogreEntity)
				{
					const size_t numSubEntities = ogreEntity->getNumSubEntities();
					for (size_t subEntityIndex = 0; subEntityIndex < numSubEntities; ++subEntityIndex)
					{
						const Ogre::v1::SubEntity* subEntity = ogreEntity->getSubEntity(subEntityIndex);
						QSF_ASSERT(nullptr != subEntity, "em5::DamageComponent::exchangeDamageMaterials: Invalid OGRE sub-entity", QSF_REACT_NONE);

						try
						{
							const qsf::GlobalAssetId materialGlobalAssetId = boost::lexical_cast<uint64>(materialVariationManager.getOriginalMaterialName(entity.getId(), *ogreEntity, subEntityIndex));
							const qsf::GlobalAssetId damageMaterialGlobalAssetId = getDamageMaterialName(materialGlobalAssetId);

							materials.push_back(qsf::isInitialized(damageMaterialGlobalAssetId) ? damageMaterialGlobalAssetId : materialGlobalAssetId);
						}
						catch (const std::exception&)
						{
							// Nothing here
						}
					}
				}
			}
		}
		else
		{
			// Use the original materials, so nothing to do in here
		}

		// Overwrite entity materials
		materialVariationManager.overwriteEntityMaterials(entity, materials);
	}

	qsf::GlobalAssetId DamageComponent::getDamageMaterialName(qsf::GlobalAssetId originalMaterialGlobalAssetId)
	{
		const qsf::AssetProxy currentMaterial(originalMaterialGlobalAssetId);
		if (!currentMaterial.isValid())
		{
			return qsf::getUninitialized<qsf::GlobalAssetId>();
		}

		const std::string& originalAssetName = currentMaterial.getLocalAssetName();

		// Find a replacement material and check if there is a material with "_dmg" at the end or just before the last underscore
		qsf::GlobalAssetId replacementGlobalAssetId = qsf::AssetProxy(originalAssetName + "_dmg").getGlobalAssetId();
		if (qsf::isUninitialized(replacementGlobalAssetId))
		{
			const std::size_t underscorePosition = originalAssetName.find_last_of('_');
			if (underscorePosition != std::string::npos)
			{
				std::string newDamageAssetName = originalAssetName;
				newDamageAssetName.insert(underscorePosition, "_dmg");
				replacementGlobalAssetId = qsf::AssetProxy(newDamageAssetName).getGlobalAssetId();
			}
		}

		return replacementGlobalAssetId;
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::MovableObjectRenderingListener methods ]
	//[-------------------------------------------------------]
	qsf::Component* DamageComponent::getMovableObjectRenderingListenerComponent()
	{
		return getEntity().getComponent<qsf::RendererComponent>();
	}

	void DamageComponent::onObjectRenderingStartup()
	{
		// Call the base implementation
		JobProxyMovableObjectRenderingListener::onObjectRenderingStartup();

		if (isSimulating())
		{
			if (mDamageState != DAMAGESTATE_INTACT)
			{
				setCustomParametersForFireGlowAndSmutOverlay();
			}
		}
		else
		{
			// Inside the editor, ensure the visual state is consistent to the internal state
			// -> If we're not careful, we'll confuse the artists
			// -> We can't do this inside "onStartup()" because at this point in time link components might not have been started, yet (example, the leaves of a tree might not get initially updated correctly)
			internalDamageStateUpdated();
		}
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::JobProxyMovableObjectRenderingListener methods ]
	//[-------------------------------------------------------]
	void DamageComponent::onUpdateMovableObjectRenderingJob(const qsf::JobArguments& jobArguments)
	{
		if (mFireFadeAnimationJobProxy.isValid())
		{
			setCustomParametersForFireGlowAndSmutOverlay();
		}

		// Call the base implementation
		JobProxyMovableObjectRenderingListener::onUpdateMovableObjectRenderingJob(jobArguments);
	}


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	void DamageComponent::setFireStateIntact()
	{
		// Turn off fire
		FireComponent* fireComponent = getEntity().getComponent<FireComponent>();
		if (nullptr != fireComponent)
		{
			fireComponent->cleanup();
		}
		else
		{
			ComplexFireComponent* complexFireComponent = getEntity().getComponent<ComplexFireComponent>();
			if (nullptr != complexFireComponent)
			{
				complexFireComponent->cleanup();
			}
		}
	}

	void DamageComponent::setFireStateDestroyed()
	{
		// Turn off fire
		FireComponent* fireComponent = getEntity().getComponent<FireComponent>();
		if (nullptr != fireComponent)
		{
			fireComponent->destroyByFire();
		}
		else
		{
			ComplexFireComponent* complexFireComponent = getEntity().getComponent<ComplexFireComponent>();
			if (nullptr != complexFireComponent)
			{
				complexFireComponent->destroyByFire();
			}
		}
	}

	void DamageComponent::setFireLifePercentage(float fireLifePercentage)
	{
		if (mFireLifePercentage != fireLifePercentage)
		{
			mFireLifePercentage = fireLifePercentage;

			// Call sub-class implementation
			onSetFireLifePercentage(mFireLifePercentage);
		}
	}

	void DamageComponent::updateTintableMesh()
	{
		if (isRunning())
		{
			qsf::compositing::TintableMeshComponent* tintableMeshComponent = getEntity().getComponent<qsf::compositing::TintableMeshComponent>();
			if (nullptr != tintableMeshComponent)
			{
				tintableMeshComponent->updateMaterial(true);
			}
		}
	}

	void DamageComponent::setupFireFadeAnimation()
	{
		const FireComponent* fireComponent = getBurningFireComponent();
		if (nullptr != fireComponent && !mFireFadeAnimationJobProxy.isValid())
		{
			mFireFadeAnimationJobProxy.registerAt(qsf::QsfJobs::ANIMATION_MESH, boost::bind(&DamageComponent::updateFireFadeAnimation, this, _1));
		}
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void DamageComponent::updateFireFadeAnimation(const qsf::JobArguments& jobArguments)
	{
		const FireComponent* fireComponent = getBurningFireComponent();
		float targetValue = 0.0f;
		const float step = 0.1f;
		if (nullptr != fireComponent)
		{
			targetValue = fireComponent->getEnergyPercent();
			mPeakFireEnergyPercent = glm::max(targetValue, mPeakFireEnergyPercent);
		}

		const float stepPerUpdate = step * jobArguments.getSecondsPassed();
		const float diff = glm::abs(mEmissiveMapStrength - targetValue);

		if (diff != 0.0f)
		{
			if (diff <= stepPerUpdate)
			{
				mEmissiveMapStrength = targetValue;
			}
			else
			{
				if (mEmissiveMapStrength > targetValue)
				{
					mEmissiveMapStrength -= stepPerUpdate;
				}
				else if (mEmissiveMapStrength < targetValue)
				{
					mEmissiveMapStrength += stepPerUpdate;
				}
			}
		}

		// Unregister after fade-out
		if (nullptr == fireComponent && mEmissiveMapStrength == 0.0f)
		{
			setCustomParametersForFireGlowAndSmutOverlay();
			mFireFadeAnimationJobProxy.unregister();
		}
	}

	void DamageComponent::setCustomParametersForFireGlowAndSmutOverlay()
	{
		// Set the state of smut & emissive burn overlays
		const qsf::MeshComponent* meshComponent = getEntity().getComponent<qsf::MeshComponent>();
		if (nullptr != meshComponent)
		{
			setCustomParametersForFireGlowAndSmutOverlay(*meshComponent);
		}

		// Get the link component and set the states for smut & emissive burn overlay
		const qsf::LinkComponent* linkComponent = getEntity().getComponent<qsf::LinkComponent>();
		if (nullptr != linkComponent)
		{
			for (const qsf::LinkComponent* childLinkComponent : linkComponent->getChildLinks())
			{
				// Set the state of smut & emissive burn overlays
				meshComponent = childLinkComponent->getEntity().getComponent<qsf::MeshComponent>();
				if (nullptr != meshComponent)
				{
					setCustomParametersForFireGlowAndSmutOverlay(*meshComponent);
				}
			}
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

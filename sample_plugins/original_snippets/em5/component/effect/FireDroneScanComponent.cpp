// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/component/effect/FireDroneScanComponent.h"
#include "em5/base/GameplayAssets.h"
#include "em5/component/effect/HighlightComponent.h"
#include "em5/fire/component/FireComponent.h"
#include "em5/plugin/Jobs.h"

#include <qsf/component/base/TransformComponent.h>
#include <qsf/component/link/LinkComponent.h>
#include <qsf/renderer/light/LightComponent.h>
#include <qsf/map/Entity.h>
#include <qsf/map/query/GroundMapQuery.h>
#include <qsf/map/query/ComponentMapQuery.h>

#include <glm/gtx/norm.hpp>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Local helper methods                                  ]
	//[-------------------------------------------------------]
	namespace detail
	{
		template<typename V, typename T>
		inline T lerp(const V t, const T a, const T b)
		{
			return (a + t * (b - a));
		}
	}


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 FireDroneScanComponent::COMPONENT_ID	= qsf::StringHash("em5::FireDroneScanComponent");


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	FireDroneScanComponent::FireDroneScanComponent(qsf::Prototype* prototype) :
		GroundSpotComponent(prototype),
		mActivateTime(qsf::Time::ZERO),
		mFadeInDuration(qsf::Time::fromSeconds(0.4f)),
		mFadeOutDuration(qsf::Time::fromSeconds(0.8f)),
		mWaitDuration(qsf::Time::fromSeconds(0.5f)),
		mBaseColor(qsf::Color3::BLUE),
		mHighlightColor(qsf::Color3::RED),
		mWarningColor(qsf::Color3::YELLOW),
		mGroundTexture(assets::TEXTURE_FIREFIGHTING_DRONESCAN_GROUND),
		mTextureIntensity(10.f)
	{
		// Nothing to do here
	}

	FireDroneScanComponent::~FireDroneScanComponent()
	{
		// Nothing to do here
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Component methods              ]
	//[-------------------------------------------------------]
	void FireDroneScanComponent::onShutdown()
	{
		destroyEffect();

		// Call base class implementation
		GroundSpotComponent::onShutdown();
	}

	void FireDroneScanComponent::onSetActive(bool active)
	{
		if (isRunning())
		{
			if (active)
			{
				// Set texture of base class to create an light with the base texture
				setSpotLightTexture(mGroundTexture);

				// Manual change the height, do this here to avoid problems with initialization
				mGroundSpotLightHeight = mGroundSpotLightRadius * 1.5f;

				// Register the update job
				mAnimationJobProxy.registerAt(Jobs::ANIMATION_HIGHLIGHT, boost::bind(&FireDroneScanComponent::updateAnimation, this, _1));
			}
			else
			{
				destroyEffect();
			}
		}

		// Call base class implementation
		GroundSpotComponent::onSetActive(active);

		// Change some predefined values
		if (mGroundSpotLightEntity.valid())
		{
			qsf::LightComponent* lightComponent = mGroundSpotLightEntity->getComponent<qsf::LightComponent>();
			if (nullptr != lightComponent)
			{
				lightComponent->setSpotlightNearClipDistance(mGroundSpotLightHeight * 0.85f);
				lightComponent->setDiffuseIntensity(mTextureIntensity);
			}

			// Force first update with zero time difference to achieve an clean collection of the first entities
			mActivateTime = mFadeInDuration + mFadeOutDuration + mWaitDuration;
			updateAnimation(qsf::JobArguments());
		}
	}

	void FireDroneScanComponent::onSetSimulating(bool simulating)
	{
		if (simulating)
		{
			// Don't call base class here, we want to manually activate the effect
		}
		else
		{
			destroyEffect();

			// Call base class implementation
			GroundSpotComponent::onSetSimulating(simulating);
		}
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void FireDroneScanComponent::updateAnimation(const qsf::JobArguments& jobArguments)
	{
		if (mGroundSpotLightEntity.valid())
		{
			glm::vec3 effectPosition = getEntity().getOrCreateComponentSafe<qsf::TransformComponent>().getPosition();
			qsf::GroundMapQuery(getEntity().getMap()).getHeightAtXZPosition(effectPosition.x, effectPosition.z, effectPosition.y);

			// Adjust height offset to improve the light
			effectPosition.y += getGroundSpotLightHeight();

			mGroundSpotLightEntity->getOrCreateComponentSafe<qsf::TransformComponent>().setPosition(effectPosition);

			// Eliminate 3D
			glm::vec3 groundPosition = effectPosition;
			groundPosition.y = 0.0f;

			// Update the pulse value over time
			float pulseValue = 0;
			if (mActivateTime <= mFadeInDuration)
			{
				pulseValue = mActivateTime.getSeconds() / mFadeInDuration.getSeconds();
			}
			else if (mActivateTime < (mFadeInDuration + mFadeOutDuration))
			{
				pulseValue = 1 - ((mActivateTime.getSeconds() - mFadeInDuration.getSeconds()) / mFadeOutDuration.getSeconds());
			}

			// Optimization: To avoid constant allocations/deallocations, use a static instance (not multi-threading safe, of course)
			static std::vector<uint64> removedEntityIds;
			removedEntityIds.clear();

			for (const auto& pair : mFoundObjects)
			{
				qsf::Entity* entity = pair.second.get();
				if (nullptr == entity)
				{
					// Entity reference got invalid
					removedEntityIds.push_back(pair.first);
					continue;
				}

				// Get the parent (remember the existence of event-meta entities, take only hard linked entities as parent)
				HighlightComponent& highlightComponent = qsf::LinkComponent::getTopmostAncestorPrototype(*entity, qsf::LinkComponent::PARENT_DEPENDING_EXISTENCE).getComponentSafe<HighlightComponent>();

				// Optimization: To avoid constant allocations/deallocations, use a static instance (not multi-threading safe, of course)
				static std::vector<FireComponent*> fireComponents;
				fireComponents.clear();
				FireComponent::getAllFireComponents(*entity, fireComponents);

				// Remove object from highlighting as soon as object leaves the scan-range
				if (!isFireComponentInDistance(groundPosition, fireComponents))
				{
					// Disable custom highlight
					highlightComponent.setHighlightFlags(HighlightComponent::FLAG_CUSTOM, false);
					removedEntityIds.push_back(pair.first);
				}
				else if (highlightComponent.getCurrentHighlightType() == HighlightComponent::FLAG_CUSTOM)
				{
					// Update custom highlight values
					highlightComponent.setPulseEnabled(true);
					highlightComponent.setManualPulseEnabled(true);
					highlightComponent.setPulseValue(pulseValue);
					highlightComponent.updateMaterial();
					highlightComponent.setRimLightColor(calcHighLightColorForFireComponents(fireComponents));
					highlightComponent.setRimLightPower(0.4f);
				}
			}

			for (uint64 entityId : removedEntityIds)
			{
				mFoundObjects.erase(entityId);
			}

			// Update time
			mActivateTime += jobArguments.getTimePassed();
			const qsf::Time fullTime = mFadeInDuration + mFadeOutDuration + mWaitDuration;
			if (mActivateTime >= fullTime)
			{
				mActivateTime -= fullTime;
				collectFireEntitiesInRange(groundPosition);
			}
		}
	}

	void FireDroneScanComponent::collectFireEntitiesInRange(const glm::vec3& effectPosition)
	{
		// To achieve the simulation of an radar search, use a map query and check for angles
		// -> Optimization: To avoid constant allocations/deallocations, use a static instance (not multi-threading safe, of course)
		static std::vector<FireComponent*> fireComponentsInRange;
		fireComponentsInRange.clear();
		qsf::ComponentMapQuery(getEntity().getMap()).getInstancesInCircle(effectPosition, mGroundSpotLightRadius, fireComponentsInRange);

		// Update all fire components in range
		const uint64 thisEntityId = getEntityId();
		for (FireComponent* fireComponent : fireComponentsInRange)
		{
			// Get the parent (remember the existence of event-meta entities, take only hard linked entities as parent)
			qsf::Entity& entity = static_cast<qsf::Entity&>(qsf::LinkComponent::getTopmostAncestorPrototype(fireComponent->getEntity(), qsf::LinkComponent::PARENT_DEPENDING_EXISTENCE));

			// Don't highlight the drone
			if (entity.getId() != thisEntityId)
			{
				// Highlight and select entity in case it is burning and not any other highlighting type is active
				HighlightComponent* highlightComponent = entity.getOrCreateComponent<HighlightComponent>();
				if (highlightComponent->getCurrentHighlightType() != HighlightComponent::FLAG_CUSTOM)
				{
					highlightComponent->setHighlightFlags(HighlightComponent::FLAG_CUSTOM, true);
					highlightComponent->setPulseEnabled(true);
					highlightComponent->setManualPulseEnabled(true);
					highlightComponent->setRimLightEnabled(true);
					highlightComponent->updateMaterial();

					mFoundObjects.emplace(entity.getId(), &entity);
				}
			}
		}
	}

	void FireDroneScanComponent::destroyEffect()
	{
		mAnimationJobProxy.unregister();

		for (const auto& pair : mFoundObjects)
		{
			qsf::Entity* entity = pair.second.get();
			if (nullptr != entity)
			{
				entity->getComponentSafe<HighlightComponent>().setHighlightFlags(HighlightComponent::FLAG_CUSTOM, false);
			}
		}
		mFoundObjects.clear();
	}

	bool FireDroneScanComponent::isFireComponentInDistance(const glm::vec3& drone2dPosition, const std::vector<FireComponent*>& fireComponents)
	{
		bool result = false;
		for (FireComponent* fireComponent : fireComponents)
		{
			const qsf::TransformComponent& entityTransformComponent = fireComponent->getEntity().getOrCreateComponentSafe<qsf::TransformComponent>();
			glm::vec3 position2D = entityTransformComponent.getPosition();
			position2D.y = 0.0f;

			if (glm::distance2(position2D, drone2dPosition) < mGroundSpotLightRadius * mGroundSpotLightRadius)
			{
				// Found at least one component in range
				result = true;
				break;
			}
		}

		return result;
	}

	qsf::Color3 FireDroneScanComponent::calcHighLightColorForFireComponents(const std::vector<FireComponent*>& fireComponents)
	{
		// For complex entities we want the top most burning state, thats why we cycle through all fire entities and save the "worst" state
		qsf::Color3 resultColor = mBaseColor;
		float currentCombustionFactor = 1.0f;
		for (FireComponent* fireComponent : fireComponents)
		{
			const float relativeCombustion = fireComponent->getRelativeCombustionEnergy();
			if (fireComponent->isBurning())
			{
				resultColor = mHighlightColor;
				break;	// It can't be more worse, abort here
			}
			else if (relativeCombustion < 0.9f && relativeCombustion < currentCombustionFactor)
			{
				currentCombustionFactor = relativeCombustion;

				float lerpFactor = 1 - (relativeCombustion / 0.9f);
				resultColor = detail::lerp(lerpFactor, mWarningColor, mHighlightColor);
			}
		}

		return resultColor;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

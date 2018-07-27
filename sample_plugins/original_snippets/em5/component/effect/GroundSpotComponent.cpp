// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/component/effect/GroundSpotComponent.h"
#include "em5/base/GameplayAssets.h"

#include <qsf/component/base/TransformComponent.h>
#include <qsf/component/link/LinkComponent.h>
#include <qsf/map/Map.h>
#include <qsf/map/Entity.h>
#include <qsf/renderer/light/LightComponent.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 GroundSpotComponent::COMPONENT_ID = qsf::StringHash("em5::GroundSpotComponent");


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	GroundSpotComponent::GroundSpotComponent(qsf::Prototype* prototype) :
		qsf::Component(prototype),
		mGroundSpotLightRadius(0.0f),
		mGroundSpotLightEntity(nullptr),
		mGroundSpotLightHeight(3.0f),
		mNearClipFactor(0.0f),
		mFarClipFactor(2.0f),
		mSpotLightTexture("qsf/texture/default/missing")
	{
		// Nothing to do here
	}

	void GroundSpotComponent::setRadius(float radius)
	{
		mGroundSpotLightRadius = radius;
	}

	void GroundSpotComponent::setClipFactors(float nearClipFactor, float farClipFactor)
	{
		mNearClipFactor = nearClipFactor;
		mFarClipFactor = farClipFactor;
	}

	void GroundSpotComponent::setSpotLightTexture(const std::string& assetId)
	{
		mSpotLightTexture = assetId;

		// In case the entity is already created, change the used texture
		if (mGroundSpotLightEntity.valid())
		{
			mGroundSpotLightEntity->getOrCreateComponentSafe<qsf::LightComponent>().setTexture(qsf::AssetProxy(assetId));

			// Force update
			setupLightForEntity(mGroundSpotLightEntity.getSafe());
		}
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Component methods              ]
	//[-------------------------------------------------------]
	void GroundSpotComponent::onShutdown()
	{
		destroyGroundLight();
	}

	void GroundSpotComponent::onSetActive(bool active)
	{
		if (active)
		{
			createGroundLight();
			setupGroundLight();
		}
		else
		{
			destroyGroundLight();
		}
	}

	void GroundSpotComponent::onSetSimulating(bool simulating)
	{
		if (simulating)
		{
			createGroundLight();
			setupGroundLight();
		}
		else
		{
			destroyGroundLight();
		}
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	qsf::Entity* GroundSpotComponent::createSpotLightEntity(const std::string& textureAssetName)
	{
		// Let there be spot light
		qsf::Entity* spotlightEntity = getEntity().getMap().createObjectByLocalPrefabAssetId(assets::PREFAB_SPOTLIGHT);
		if (nullptr != spotlightEntity)
		{
			// Don't serialize this
			spotlightEntity->setSerializable(false);

			// Place it slightly above the entity. Not so much that it hit's ceilings or bridges.
			// Light must look downwards
			const float halfSqrt = sqrt(0.5f);
			qsf::TransformComponent& transformComponentLight = spotlightEntity->getOrCreateComponentSafe<qsf::TransformComponent>();
			qsf::TransformComponent& transformComponent = getEntity().getOrCreateComponentSafe<qsf::TransformComponent>();
			transformComponentLight.setPositionAndRotation(transformComponent.getPosition() + glm::vec3(0, mGroundSpotLightHeight, 0), glm::quat(halfSqrt, -halfSqrt, 0.0f, 0.0f));

			// Link to entity
			qsf::LinkComponent& linkComponent = spotlightEntity->getOrCreateComponentSafe<qsf::LinkComponent>();
			linkComponent.linkToParent(getEntity());

			// Ensure the spotlight radius
			qsf::LightComponent& lightComponent = spotlightEntity->getOrCreateComponentSafe<qsf::LightComponent>();

			qsf::AssetProxy asset(textureAssetName);
			lightComponent.setTexture(asset);
			lightComponent.setDiffuseIntensity(250.0f);	// TODO(mz): Crazy high value as otherwise texture fade out too fast at the border (which they shouldn't...)
			lightComponent.setFalloffSmooth(1.0f);
			lightComponent.setSpotlightNearClipDistance(mGroundSpotLightHeight * mNearClipFactor);
			lightComponent.setRange(mGroundSpotLightHeight * mFarClipFactor);
		}

		return spotlightEntity;
	}

	void GroundSpotComponent::setupLightForEntity(qsf::Entity& entity)
	{
		// Ensure the spotlight radius
		qsf::LightComponent& lightComponent = entity.getOrCreateComponentSafe<qsf::LightComponent>();

		// Get the angle which we need for the ground
		float angle = atan(mGroundSpotLightRadius / mGroundSpotLightHeight);
		angle *= 180.f / glm::pi<float>();
		angle *= 2.f;

		// Use spot-lights to show the circle on ground
		lightComponent.setMaxSpotlightAngle(179.f);
		lightComponent.setSpotlightInnerAngle(angle);
		lightComponent.setSpotlightOuterAngle(angle + 0.01f);	// TODO(mz): Outer angle slightly larger as light inverts when it's same as the inner one. Will be fixed in shader one day (so say we all).
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void GroundSpotComponent::createGroundLight()
	{
		if (!mGroundSpotLightEntity.valid())
		{
			mGroundSpotLightEntity = createSpotLightEntity(mSpotLightTexture);
		}
	}

	void GroundSpotComponent::destroyGroundLight()
	{
		if (mGroundSpotLightEntity.valid())
		{
			// Destroy the spotlight
			getEntity().getMap().destroyEntityById(mGroundSpotLightEntity->getId());
		}
	}

	void GroundSpotComponent::setupGroundLight()
	{
		if (mGroundSpotLightEntity.valid())
		{
			setupLightForEntity(mGroundSpotLightEntity.getSafe());
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

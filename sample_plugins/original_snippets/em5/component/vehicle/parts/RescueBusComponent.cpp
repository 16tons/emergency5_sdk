// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/component/vehicle/parts/RescueBusComponent.h"
#include "em5/debug/GameDebugGroup.h"
#include "em5/game/animation/AnimationHelper.h"
#include "em5/map/EntityHelper.h"
#include "em5/map/CollisionHelper.h"
#include "em5/audio/AudioHelper.h"

#include <qsf_ai/navigation/NavigationComponent.h>

#include <qsf/component/base/TransformComponent.h>
#include <qsf/component/base/MetadataComponent.h>
#include <qsf/component/link/LinkComponent.h>
#include <qsf/physics/collision/BulletBoxCollisionComponent.h>
#include <qsf/map/Entity.h>
#include <qsf/map/Map.h>
#include <qsf/plugin/QsfJobs.h>
#include <qsf/debug/request/TextDebugDrawRequest.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 RescueBusComponent::COMPONENT_ID = qsf::StringHash("em5::RescueBusComponent");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	RescueBusComponent::RescueBusComponent(qsf::Prototype* prototype) :
		qsf::Component(prototype),
		mRescueBusState(RESCUEBUSSTATE_HIDDEN)
	{
		// Nothing to do in here
	}

	RescueBusComponent::~RescueBusComponent()
	{
		// Nothing to do in here
	}

	bool RescueBusComponent::isHidden() const
	{
		return (mRescueBusState == RESCUEBUSSTATE_HIDDEN);
	}

	bool RescueBusComponent::isAnimationPlaying() const
	{
		return (mRescueBusState == RESCUEBUSSTATE_EXTENDING_ANIMATION || mRescueBusState == RESCUEBUSSTATE_HIDDING_ANIMATION);
	}

	bool RescueBusComponent::isExtended() const
	{
		return (mRescueBusState == RESCUEBUSSTATE_EXTENDED);
	}

	void RescueBusComponent::startExtending()
	{
		// Anything to do?
		if (mRescueBusState == RESCUEBUSSTATE_EXTENDING_ANIMATION || mRescueBusState == RESCUEBUSSTATE_EXTENDED)
			return;

		enableCollisionShapeExtended();

		// Change state
		mRescueBusState = RESCUEBUSSTATE_EXTENDING_ANIMATION;

		AudioHelper::playCraneCockpitDown(mAudioProxy, getEntity());

		registerRescueBusParts();

		if (mRescueBusSide.valid())
		{
			AnimationHelper animationHelper(mRescueBusSide.getSafe());
			animationHelper.playAnimation(animationHelper.getAnimationImtSidesOpen());
		}

		// Activate job so we get regular update calls
		activateJob();
	}

	void RescueBusComponent::hideExtending()
	{
		// Anything to do?
		if (mRescueBusState == RESCUEBUSSTATE_HIDDING_ANIMATION || mRescueBusState == RESCUEBUSSTATE_HIDDEN)
			return;

		// Change state
		mRescueBusState = RESCUEBUSSTATE_HIDDING_ANIMATION;

		AudioHelper::playCraneCockpitUp(mAudioProxy, getEntity());

		if (mRescueBusSide.valid())
		{
			AnimationHelper animationHelper(mRescueBusSide.getSafe());
			animationHelper.playAnimation(animationHelper.getAnimationImtSidesOpen(), false, true);
		}

		// Activate job so we get regular update calls
		activateJob();
	}

	const glm::vec3& RescueBusComponent::getCollisionExtended() const
	{
		return mDeployedCollisionExtend;
	}

	void RescueBusComponent::setCollisionExtended(const glm::vec3& collisionExtentExtended)
	{
		mDeployedCollisionExtend = collisionExtentExtended;
	}

	bool RescueBusComponent::checkCollisionForExtending()
	{
		// Get the parent entity, this is the entity with the collision info
		qsf::LinkComponent* linkComponent = getEntity().getComponent<qsf::LinkComponent>();
		if (nullptr == linkComponent)
			return false;

		qsf::Entity* parentEntity = static_cast<qsf::Entity*>(linkComponent->getParentPrototype());
		if (nullptr == parentEntity)
			return false;

		qsf::TransformComponent& transformComponent = parentEntity->getOrCreateComponentSafe<qsf::TransformComponent>();
		// Take the position of the vehicle and check if the collisionExtents bounding box gives a collision
		qsf::Transform goalTransform(transformComponent.getPosition(), transformComponent.getRotation());
		CollisionHelper collisionHelper(parentEntity->getMap());
		return collisionHelper.hasCollisionEM3IgnoreEntity(mDeployedCollisionExtend, goalTransform, *parentEntity);
	}

	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void RescueBusComponent::activateJob()
	{
		// Is job already active?
		if (!mUpdateJobProxy.isValid())
		{
			// Activate job by registering via the job proxy
			mUpdateJobProxy.registerAt(qsf::QsfJobs::ANIMATION_GENERAL, boost::bind(&RescueBusComponent::updateJob, this, _1));
		}
	}

	void RescueBusComponent::deactivateJob()
	{
		mAudioProxy.stop();
		mUpdateJobProxy.unregister();
		mDebugDrawProxy.unregister();
	}

	void RescueBusComponent::updateJob(const qsf::JobArguments& jobArguments)
	{
		switch (mRescueBusState)
		{
			case RESCUEBUSSTATE_EXTENDING_ANIMATION:
			{
				const bool showDebug = GameDebugGroup::getInstanceSafe().getShowDebugTextComponent();
				if (showDebug)
				{
					mDebugDrawProxy.registerForComponent(*this);
					mDebugDrawProxy.addRequest(qsf::TextDebugDrawRequest("extending", EntityHelper(getEntity()).getPosition()));
				}

				AnimationHelper animationHelper(mRescueBusSide.getSafe());
				if (!animationHelper.isAnimationPlaying())
				{
					enableNavigationComponent(false);

					mRescueBusState = RESCUEBUSSTATE_EXTENDED;

					deactivateJob();
				}
				break;
			}

			case RESCUEBUSSTATE_HIDDING_ANIMATION:
			{
				const bool showDebug = GameDebugGroup::getInstanceSafe().getShowDebugTextComponent();
				if (showDebug)
				{
					mDebugDrawProxy.registerForComponent(*this);
					mDebugDrawProxy.addRequest(qsf::TextDebugDrawRequest("hidding", EntityHelper(getEntity()).getPosition()));
				}

				AnimationHelper animationHelper(mRescueBusSide.getSafe());
				if (!animationHelper.isAnimationPlaying())
				{
					mRescueBusState = RESCUEBUSSTATE_HIDDEN;

					disableCollisionShapeExtended();

					enableNavigationComponent(true);

					deactivateJob();
				}
				break;
			}

			case RESCUEBUSSTATE_EXTENDED:
			case RESCUEBUSSTATE_HIDDEN:
				// Nothing to do in this case
				break;
		}
	}

	void RescueBusComponent::registerRescueBusParts()
	{
		mRescueBusSide = getEntity();
	}

	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Component methods              ]
	//[-------------------------------------------------------]
	void RescueBusComponent::onShutdown()
	{
		// Unregister jobs
		mUpdateJobProxy.unregister();
		mDebugDrawProxy.unregister();

		// Call base class implementation
		Component::onShutdown();
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void RescueBusComponent::enableCollisionShapeExtended()
	{
		// Here we create a extra collision box for the entity of the rescue bus component.
		// This component has to be part of a special entity from a vehicle, otherwise we would override the original entity collision
		qsf::BulletBoxCollisionComponent* bulletBoxCollisionComponent = getEntity().getComponent<qsf::BulletBoxCollisionComponent>();
		if (nullptr != bulletBoxCollisionComponent)
		{
			bulletBoxCollisionComponent->setActive(true);
		}
	}

	void RescueBusComponent::disableCollisionShapeExtended()
	{
		qsf::BulletBoxCollisionComponent* bulletBoxCollisionComponent = getEntity().getComponent<qsf::BulletBoxCollisionComponent>();
		if (nullptr != bulletBoxCollisionComponent)
		{
			bulletBoxCollisionComponent->setActive(false);
		}
	}

	void RescueBusComponent::enableNavigationComponent(bool enable)
	{
		qsf::ai::NavigationComponent* navigationComponent = qsf::LinkComponent::getTopmostAncestorPrototype(getEntity(), qsf::LinkComponent::SELECT_PARENT).getComponent<qsf::ai::NavigationComponent>();
		if (nullptr != navigationComponent)
		{
			navigationComponent->setActive(enable);
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

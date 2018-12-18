// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/component/person/GaperComponent.h"
#include "em5/action/base/PlayAnimationAction.h"
#include "em5/action/equipment/GetEquipmentAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/action/equipment/UseEquipmentAction.h"
#include "em5/component/movement/MoveToDespawnComponent.h"
#include "em5/game/animation/AnimationHelper.h"
#include "em5/map/EntityHelper.h"
#include "em5/plugin/Jobs.h"

#include <qsf/math/Random.h>
#include <qsf/job/JobArguments.h>
#include <qsf/logic/action/ActionComponent.h>
#include <qsf/renderer/component/RendererComponent.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 GaperComponent::COMPONENT_ID	= qsf::StringHash("em5::GaperComponent");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	GaperComponent::GaperComponent(qsf::Prototype* prototype) :
		qsf::Component(prototype),
		mTimeGaping(qsf::Time::ZERO),
		mUseMobilephone(false)
	{
		// Nothing here
	}

	GaperComponent::~GaperComponent()
	{
		if (mUseMobilephone)
		{
			EntityHelper(getEntity()).deleteActiveEquipment();
		}
	}

	bool GaperComponent::getUseMobilephone() const
	{
		return mUseMobilephone;
	}

	void GaperComponent::setUseMobilephone(bool useMobilephone)
	{
		mUseMobilephone = useMobilephone;
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Component methods                 ]
	//[-------------------------------------------------------]
	void GaperComponent::serialize(qsf::BinarySerializer& serializer)
	{
		// TODO(mk) Write the serialize function
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Component methods              ]
	//[-------------------------------------------------------]
	void GaperComponent::onSetSimulating(bool simulating)
	{
		if (simulating)
		{
			mTimeGaping = qsf::Time::ZERO;

			{ // Register job
				qsf::jobs::JobConfiguration jobConfiguration;
				jobConfiguration.mTimeBetweenCalls = qsf::Time::fromMilliseconds(1000);
				mAnimatingJobProxy.registerAt(Jobs::ANIMATION_PERSON, boost::bind(&GaperComponent::updateAnimation, this, _1), jobConfiguration);
			}
			registerMovableObjectRenderingListener();

			{ // Enforce a first update to not have a T-pose in the first frame
				const AnimationHelper animationHelper(getEntity());
				qsf::ActionComponent& actionComponent = getEntity().getOrCreateComponentSafe<qsf::ActionComponent>();
				actionComponent.pushAction<PlayAnimationAction>(action::AUTOMATISM_STD, qsf::action::APPEND_TO_BACK).init(animationHelper.getAnimationCivilGape(), true);
				AnimationHelper::AnimationMaskData animationMaskData;
				actionComponent.pushAction<PlayAnimationAction>(action::AUTOMATISM_STD, qsf::action::APPEND_TO_BACK).init(animationHelper.getIdleAnimation(animationMaskData), false, true);
				actionComponent.updateCurrentAction(qsf::Clock());

				{ // No armada of synchronous gaper borgs, please
					const qsf::MeshAnimationComponent& meshAnimationComponent = getEntity().getOrCreateComponentSafe<qsf::MeshAnimationComponent>();
					qsf::MeshAnimationChannel* meshAnimationChannel = meshAnimationComponent.getAnimationChannel(qsf::AssetProxy(meshAnimationComponent.getCurrentAnimation()));
					if (nullptr != meshAnimationChannel)
					{
						meshAnimationChannel->setTimePosition(meshAnimationChannel->getTimePosition() + qsf::Time::fromMilliseconds(qsf::Random::getRandomInt(0, 2000)));
					}
				}
			}
		}
		else
		{
			// Unregister job
			mAnimatingJobProxy.unregister();
			unregisterMovableObjectRenderingListener();
		}
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::MovableObjectRenderingListener methods ]
	//[-------------------------------------------------------]
	qsf::Component* GaperComponent::getMovableObjectRenderingListenerComponent()
	{
		return getEntity().getComponent<qsf::RendererComponent>();
	}

	void GaperComponent::onObjectRenderingStartup()
	{
		// Call the base implementation
		JobProxyMovableObjectRenderingListener::onObjectRenderingStartup();

		// Put the animation job proxy into berserk-mode
		mAnimatingJobProxy.changeTimeBetweenCalls(qsf::Time::fromMilliseconds(0));
	}

	void GaperComponent::onObjectRenderingShutdown()
	{
		// Put the animation job proxy into sleepy-mode
		mAnimatingJobProxy.changeTimeBetweenCalls(qsf::Time::fromMilliseconds(1000));

		// Call the base implementation
		JobProxyMovableObjectRenderingListener::onObjectRenderingShutdown();
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void GaperComponent::updateAnimation(const qsf::JobArguments& jobArguments)
	{
		if (preCheck())
		{
			if (mTimeGaping <= qsf::Time::ZERO)
			{
				pushAnimation();
				resetWaitTimer();
			}
			else
			{
				mTimeGaping -= jobArguments.getTimePassed();
			}
		}
	}

	void GaperComponent::resetWaitTimer()
	{
		static const float minimum = 7;
		static const float maximum = 20;

		mTimeGaping = qsf::Time::fromSeconds(qsf::Random::getRandomFloat(minimum, maximum));
	}

	bool GaperComponent::preCheck()
	{
		qsf::ActionComponent* actionComponent = getEntity().getOrCreateComponent<qsf::ActionComponent>();
		if (!actionComponent->getPlan().isEmpty())
			return false;

		EntityHelper entityHelper(getEntity());
		if (entityHelper.isArrested())
			return false;

		if (entityHelper.isEntityHardLinked())
			return false;

		if (entityHelper.isPersonInjured())
			return false;

		// Gapers don't need a "move to despawn" component
		getEntity().destroyComponent<MoveToDespawnComponent>();

		return true;
	}

	void GaperComponent::pushAnimation()
	{
		qsf::ActionComponent& actionComponent = getEntity().getOrCreateComponentSafe<qsf::ActionComponent>();

		if (mUseMobilephone)
		{
			actionComponent.clearPlan();
			actionComponent.pushAction<UseEquipmentAction>(action::COMMAND_LOW).init(UseEquipmentAction::EQUIPMENT_MOBILE);
		}
		else
		{
			// Push special gape animation
			const AnimationHelper animationHelper(getEntity());
			actionComponent.pushAction<PlayAnimationAction>(action::AUTOMATISM_STD, qsf::action::APPEND_TO_BACK).init(animationHelper.getAnimationCivilGape(), true);
			AnimationHelper::AnimationMaskData animationMaskData;
			actionComponent.pushAction<PlayAnimationAction>(action::AUTOMATISM_STD, qsf::action::APPEND_TO_BACK).init(animationHelper.getIdleAnimation(animationMaskData), false, true);
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

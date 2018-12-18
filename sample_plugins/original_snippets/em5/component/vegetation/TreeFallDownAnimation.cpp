// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/component/vegetation/TreeFallDownAnimation.h"
#include "em5/component/vegetation/TreeComponent.h"
#include "em5/action/base/DisappearAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/fire/component/FireComponent.h"
#include "em5/audio/AudioHelper.h"
#include "em5/audio/AudioProxy.h"
#include "em5/plugin/Jobs.h"
#include "em5/plugin/Messages.h"

#include <qsf/logic/action/ActionComponent.h>
#include <qsf/logic/action/WaitAction.h>
#include <qsf/component/base/TransformComponent.h>
#include <qsf/component/link/LinkComponent.h>
#include <qsf/message/MessageSystem.h>
#include <qsf/math/Math.h>
#include <qsf/math/CoordinateSystem.h>
#include <qsf/map/Map.h>
#include <qsf/QsfHelper.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	TreeFallDownAnimation::TreeFallDownAnimation(TreeComponent& treeComponent, qsf::Entity& firefighterEntity) :
		mTreeComponent(treeComponent),
		mLinkComponent(treeComponent.getEntity().getComponent<qsf::LinkComponent>()),
		mFirefighter(firefighterEntity),
		mState(STATE_INIT),
		mSecondsPassed(0.0f),
		mLastDegree(0.0f)
	{
		// Nothing here
	}

	TreeFallDownAnimation::~TreeFallDownAnimation()
	{
		// Nothing here
	}

	void TreeFallDownAnimation::start()
	{
		mUpdateJob.registerAt(Jobs::ANIMATION_TREE, boost::bind(&TreeFallDownAnimation::updateAnimation, this, _1));
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void TreeFallDownAnimation::updateAnimation(const qsf::JobArguments& jobArgs)
	{
		// Time progress
		mSecondsPassed += jobArgs.getTimePassed().getSeconds();

		// Get the link component
		qsf::Entity& treeEntity = mTreeComponent->getEntity();

		switch (mState)
		{
			case STATE_INIT:
			{
				// Set it to default
				mSecondsPassed = 0.0f;
				mLastDegree = 0.0f;

				// Play audio
				AudioProxy audioProxy;
				AudioHelper::playTreeDownSound(audioProxy, treeEntity);
				audioProxy.detach();
				audioProxy.synchronizeWithMultiplay();

				mFallAxis = qsf::CoordinateSystem::getRight();

				if (mFirefighter.valid())
				{
					glm::vec3 firefighterPosition = mFirefighter->getOrCreateComponentSafe<qsf::TransformComponent>().getPosition();
					glm::vec3 treePosition = treeEntity.getOrCreateComponentSafe<qsf::TransformComponent>().getPosition();
					glm::vec3 direction = treePosition - firefighterPosition;
					direction.y = 0.0f;
					mFallAxis = glm::normalize(direction);
				}

				mState = STATE_WAITING;
				// Fall through by design
			}

			case STATE_WAITING:
			{
				// To improve the timing between the cutting and the falling, add a small delay here
				if (mSecondsPassed >= 0.5f)
				{
					mState = STATE_TREE_FALLING;
					mSecondsPassed = 0.0f;
				}
				break;
			}

			case STATE_TREE_FALLING:
			{
				// Calculate the degree
				// TODO(tl): Not physically correct, just fast hacked for visible improvement
				const float time = mSecondsPassed * 1.2f;
				const float fall = pow(time, 4.0f) * 0.5f;
				const float bounce = pow(time - 5.8f, 4.0f) * 0.5f + 70.0f;
				const float slow = time < 7.0f ? 90.0f : -160.0f / time + 94.0f;
				const float degree = std::max(std::min(std::min(std::min(fall, bounce), slow), 80.0f), 0.0f);

				if (mLinkComponent.valid())
				{
					const glm::quat offset = qsf::Math::getRotationAroundAxis(mFallAxis, glm::radians(degree - mLastDegree));

					for (const qsf::LinkComponent* childLinkComponent : mLinkComponent->getChildLinks())
					{
						qsf::TransformComponent* transformComponent = childLinkComponent->getEntity().getComponent<qsf::TransformComponent>();
						if (nullptr != transformComponent)
						{
							transformComponent->setRotation(transformComponent->getRotation() * offset);
						}
					}
				}
				mLastDegree = degree;

				if (mSecondsPassed >= 13.0f)
				{
					mState = STATE_TREE_CUT;
				}
				break;
			}

			case STATE_TREE_CUT:
			{
				// Deactivate the fire
				FireComponent* fireComponent = treeEntity.getComponent<FireComponent>();
				if (nullptr != fireComponent)
				{
					fireComponent->setActive(false);
				}

				if (mLinkComponent.valid())
				{
					for (const qsf::LinkComponent* childLinkComponent : mLinkComponent->getChildLinks())
					{
						qsf::Entity& childEntity = childLinkComponent->getEntity();

						// Add a disappear action
						qsf::ActionComponent& actionComponent = childEntity.getOrCreateComponentSafe<qsf::ActionComponent>();
						actionComponent.clearPlan();

						// The child entity should disappear but not be deleted (needed for the cleanup)
						actionComponent.pushAction<DisappearAction>(action::BLOCKING).init(qsf::Time::fromSeconds(3.0f), false);
					}
				}

				// Send message
				qsf::MessageParameters parameters;
				parameters.setParameter("EntityId", treeEntity.getId());
				QSF_MESSAGE.emitMessage(qsf::MessageConfiguration(Messages::EM5_COMPONENT_GETSDAMAGED_VEGETATION, treeEntity.getId()), parameters);

				mUpdateJob.unregister();

				// As last step: Cleanup this instance
				mTreeComponent->destroyTreeFallDownAnimation();

				break;
			}
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

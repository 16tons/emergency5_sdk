// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/action/ambulance/PickupPersonByParamedicAction.h"
#include "em5/command/ambulance/PickupPersonByParamedicCommand.h"
#include "em5/command/CommandContext.h"
#include "em5/component/event/EventIdComponent.h"
#include "em5/logic/local/base/CarryPersonLogic.h"
#include "em5/logic/local/ReserveLogic.h"
#include "em5/health/HealthHelper.h"
#include "em5/specs/ActionRangeSpecsGroup.h"
#include "em5/plugin/Messages.h"
#include "em5/game/animation/AnimationHelper.h"
#include "em5/audio/AudioHelper.h"
#include "em5/map/EntityHelper.h"

#include <qsf_game/base/DistanceHelper.h>
#include <qsf_game/command/CommandSystem.h>
#include <qsf_game/QsfGameHelper.h>

#include <qsf_ai/navigation/em4Router/RouterComponent.h>
#include <qsf_ai/navigation/NavigationComponent.h>

#include <qsf/component/link/LinkComponent.h>
#include <qsf/component/base/MetadataComponent.h>
#include <qsf/renderer/animation/MeshAnimationChannel.h>
#include <qsf/message/MessageSystem.h>
#include <qsf/logic/action/WaitAction.h>
#include <qsf/logic/action/ActionComponent.h>
#include <qsf/map/Map.h>
#include <qsf/QsfHelper.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const qsf::NamedIdentifier PickupPersonByParamedicAction::ACTION_ID = "em5::PickupPersonByParamedicAction";


	//[-------------------------------------------------------]
	//[ static constants                                      ]
	//[-------------------------------------------------------]
	// TODO(mk) Put this hard coded time value in extra specs file
	static const qsf::Time PICKUPPERSONACTION_WAIT_TIME = qsf::Time::fromSeconds(0.5f);
	static const qsf::Time PICKUPPERSONACTION_WAIT_BLOCK = qsf::Time::fromSeconds(10.0f);	// This time is used to push an wait action to keep the persons in action, so they don't use there idle animation


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	PickupPersonByParamedicAction::PickupPersonByParamedicAction() :
		Action(ACTION_ID),
		mTargetEntityId(qsf::getUninitialized<uint64>()),
		mKeepTargetReservation(false),
		mState(STATE_INIT),
		mStrecherId(qsf::getUninitialized<uint64>()),
		mFrontParamedicId(qsf::getUninitialized<uint64>()),
		mBackParamedicId(qsf::getUninitialized<uint64>())
	{
		// Nothing here
	}

	PickupPersonByParamedicAction::~PickupPersonByParamedicAction()
	{
		if (!mKeepTargetReservation)
		{
			// Remove reservation again
			ReserveLogic::deleteOwnReservation(getMap(), mTargetEntityId, getEntityId());
		}

		// Clear animation
		AnimationHelper(getEntity()).clearAnimation();
	}

	void PickupPersonByParamedicAction::init(qsf::Entity& targetEntity)
	{
		mTargetEntityId = targetEntity.getId();

		// To avoid multiple actors at the same target, we reserve the target here.
		// It is only checked in command for automatism. If the action starts, the logic is changed. Only reserve if no other reserve the target (its not bad if a moveTo reservation is missing).
		// Reserve the entity
		if (ReserveLogic::checkReservation(targetEntity, getEntityId()))
		{
			ReserveLogic::createReservation<ReserveMoveToTargetLogic>(targetEntity, getEntityId());
		}
	}

	uint64 PickupPersonByParamedicAction::getTargetEntityId() const
	{
		return mTargetEntityId;
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Actions methods                   ]
	//[-------------------------------------------------------]
	void PickupPersonByParamedicAction::serialize(qsf::BinarySerializer& serializer)
	{
		serializer.serialize(mTargetEntityId);
		serializer.serialize(mKeepTargetReservation);
		serializer.serialize(mStrecherId);
		serializer.serialize(mFrontParamedicId);
		serializer.serialize(mBackParamedicId);
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	bool PickupPersonByParamedicAction::onStartup()
	{
		qsf::Entity& callerEntity = getEntity();
		qsf::Entity* targetEntity = getMap().getEntityById(mTargetEntityId);
		if (nullptr == targetEntity)
			return false;

		// Make the distance check
		const float maxDistance = ActionRangeSpecsGroup::getInstanceSafe().getTreatPersonDistance();
		const float currentDistance = qsf::game::DistanceHelper::getDistance(*targetEntity, callerEntity);
		if (currentDistance > maxDistance)
			return false;

		// Build context
		CommandContext commandContext;
		commandContext.mCaller = &callerEntity;
		commandContext.mTargetEntity = targetEntity;

		// Call "checkCaller" && "checkContext" of the commando for validation
		PickupPersonByParamedicCommand* pickupPersonByParamedicCommand = static_cast<PickupPersonByParamedicCommand*>(QSFGAME_COMMAND.getCommandManager().getById<PickupPersonByParamedicCommand>(PickupPersonByParamedicCommand::PLUGINABLE_ID));
		QSF_CHECK(nullptr != pickupPersonByParamedicCommand, "PickupPersonByParamedicAction::onStartup(): Could not find pickup person by paramedic command instance", return false);
		if (!(pickupPersonByParamedicCommand->checkCallerWithoutPriority(*commandContext.mCaller) && pickupPersonByParamedicCommand->checkContext(commandContext)))
			return false;

		// Check for the reserve logic
		if (!ReserveLogic::checkReservation(*targetEntity, getEntityId()))
			return false;

		// Try to get event id component lock
		if (!EventIdComponent::tryLockForTeam(*targetEntity, getEntity()))
		{
			return false;
		}

		findIdFromChildLinks();
		return true;
	}

	qsf::action::Result PickupPersonByParamedicAction::updateAction(const qsf::Clock&)
	{
		// Get the entity of the caller
		qsf::Entity& callerEntity = getEntity();

		// Get the target entity instance
		qsf::Entity* targetEntity = getMap().getEntityById(mTargetEntityId);
		QSF_CHECK(nullptr != targetEntity, "PickupPersonByParamedicAction::updateAction() unable to retrieve the target entity instance", return qsf::action::RESULT_DONE);

		qsf::Entity* strecherEntity = getMap().getEntityById(mStrecherId);
		QSF_CHECK(nullptr != strecherEntity, "PickupPersonByParamedicAction::updateAction() unable to retrieve the stretcher entity instance", return qsf::action::RESULT_DONE);

		qsf::Entity* frontParamedic = getMap().getEntityById(mFrontParamedicId);
		QSF_CHECK(nullptr != frontParamedic, "PickupPersonByParamedicAction::updateAction() unable to retrieve the first paramedic entity instance", return qsf::action::RESULT_DONE);

		qsf::Entity* backParamedic = getMap().getEntityById(mBackParamedicId);
		QSF_CHECK(nullptr != backParamedic, "PickupPersonByParamedicAction::updateAction() unable to retrieve the second paramedic entity instance", return qsf::action::RESULT_DONE);

		switch (mState)
		{
			case STATE_INIT:
			{
				// Change reserve logic from "moveto" to "paramedic" (has higher priority)
				ReserveLogic::deleteOwnReservation(*targetEntity, getEntityId());
				ReserveLogic::createReservation<ReserveParamedicLogic>(*targetEntity, getEntityId());

				mState = STATE_LOWER_STRETCHER;

				// Fall through by design
			}

			case STATE_LOWER_STRETCHER:
			{
				EntityHelper(*targetEntity).enableShineThroughEffect();

				startLowerStrechAnimation(*strecherEntity);
				startLowerParamedicsAnimation(*frontParamedic, *backParamedic);

				// Play audio
				AudioHelper::playMedicStretcherFold(mAudioProxy, getEntity(), qsf::Time::fromSeconds(2.7f));
				mAudioProxy.synchronizeWithMultiplay();

				mState = STATE_DELAY1;
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_DELAY1:
			{
				if (isAnimationPlaying())
				{
					// Wait for ending of the animation
					return qsf::action::RESULT_CONTINUE;
				}

				pushWaitAction(getEntity(), PICKUPPERSONACTION_WAIT_TIME);
				// TODO(mk) Currently the positioning on weak PC can lead to errors. so don't use the fade effect now
				//targetEntity->getOrCreateComponentSafe<FadeEffectComponent>().fadeOut(qsf::Time::fromSeconds(0.5f));

				mState = STATE_LINK_PATIENT;
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_LINK_PATIENT:
			{
				startVictimAnimation(*targetEntity);
				HealthHelper(*targetEntity).linkToStretcher(*strecherEntity);
				EntityHelper::setupEntityAsGhost(*targetEntity, getEntity());

				// Set the carried person to the carry person logic
				{
					// Get the carry person logic
					CarryPersonLogic* carryPersonLogic = EntityHelper(callerEntity).getGameLogic<CarryPersonLogic>();
					if (nullptr == carryPersonLogic)
						return qsf::action::RESULT_DONE;

					// Set the carried person to the logic
					carryPersonLogic->setCarryPerson(mTargetEntityId);

					// After linking the target, we can keep the target reservation
					mKeepTargetReservation = true;

					// Remove the collision box at last
				}

				sendMessage();

				pushWaitAction(getEntity(), PICKUPPERSONACTION_WAIT_TIME);

				mState = STATE_RAISE_STRETCHER;
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_RAISE_STRETCHER:
			{
				startRaiseStrechAnimation(*strecherEntity);
				startRaiseParamedicsAnimation(*frontParamedic, *backParamedic);

				// Play audio
				AudioHelper::playMedicStretcherFold(mAudioProxy, getEntity(), qsf::Time::fromSeconds(0.3f));
				mAudioProxy.synchronizeWithMultiplay();

				mState = STATE_END;
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_END:
			{
				if (isAnimationPlaying())
				{
					// Wait for ending of the animation
					return qsf::action::RESULT_CONTINUE;
				}

				// Removed collision for the target (otherwise we would collide with the paramedic team).
				// Do it after the fade in/fade out effect (this is working with the hidden component, this makes indirect activates the router component again).
				qsf::ai::RouterComponent* routerComponent = targetEntity->getComponent<qsf::ai::RouterComponent>();
				if (nullptr != routerComponent)
				{
					routerComponent->setActive(false);
					routerComponent->removeCollisionBox();
				}

				// With an active navigation component we are still using ai reservations from this entity, so we need to disable it
				qsf::ai::NavigationComponent* navigationComponent = targetEntity->getComponent<qsf::ai::NavigationComponent>();
				if (nullptr != navigationComponent)
				{
					navigationComponent->setActive(false);
				}

				// No break here by intent
			}
		}

		return qsf::action::RESULT_DONE;
	}

	void PickupPersonByParamedicAction::onShutdown()
	{
		EventIdComponent::releaseTeamLock(mTargetEntityId, getEntity());
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void PickupPersonByParamedicAction::findIdFromChildLinks()
	{
		// Get the paramedics and the stretcher childs
		const qsf::LinkComponent* linkComponent = getEntity().getOrCreateComponent<qsf::LinkComponent>();
		if (linkComponent != nullptr)
		{
			// We test here for tags but it is also possible to use the array position to find the right entity
			for (const qsf::LinkComponent* childComponent : linkComponent->getChildLinks())
			{
				qsf::Entity& childEntity = childComponent->getEntity();

				// Get now the childs from the object
				qsf::LinkComponent* childLinkComponent = childEntity.getOrCreateComponent<qsf::LinkComponent>();
				if (childLinkComponent != nullptr)
				{
					const qsf::MetadataComponent* metadataComponent = childEntity.getOrCreateComponent<qsf::MetadataComponent>();
					// TODO(mk) Here are 3 hardcoded tags, move them to specs file (or something else)
					if (metadataComponent->getTags() == "stretcher")
					{
						mStrecherId = childEntity.getId();
					}
					else if (metadataComponent->getTags() == "first_paramedic")
					{
						mFrontParamedicId = childEntity.getId();
					}
					else if (metadataComponent->getTags() == "second_paramedic")
					{
						mBackParamedicId = childEntity.getId();
					}
				}
			}
		}
	}

	void PickupPersonByParamedicAction::startLowerStrechAnimation(qsf::Entity& stretcherEntity)
	{
		const float animationBlendTime = 0.1f;

		qsf::AssetProxy animation(AnimationHelper(stretcherEntity).getAnimationEquipmentStretcherLower());
		qsf::MeshAnimationComponent& meshAnimationComponent = stretcherEntity.getOrCreateComponentSafe<qsf::MeshAnimationComponent>();
		mStrecherAnimation = meshAnimationComponent.blendToAnimation(animation, false, qsf::Time::fromSeconds(animationBlendTime));
	}

	void PickupPersonByParamedicAction::startLowerParamedicsAnimation(qsf::Entity& frontParamedic, qsf::Entity& backParamedic)
	{
		const float animationBlendTime = 0.1f;

		qsf::AssetProxy frontAnimation(AnimationHelper(frontParamedic).getAnimationParamedicLowerStretcherFront());
		qsf::MeshAnimationComponent& frontMeshAnimationComponent = frontParamedic.getOrCreateComponentSafe<qsf::MeshAnimationComponent>();
		mFrontParamedicAnimation = frontMeshAnimationComponent.blendToAnimation(frontAnimation, false, qsf::Time::fromSeconds(animationBlendTime));

		// This time is used to push an wait action to keep the persons in action, so they don't use there idle animation
		pushWaitAction(frontParamedic, PICKUPPERSONACTION_WAIT_BLOCK);

		qsf::AssetProxy backAnimation(AnimationHelper(backParamedic).getAnimationParamedicLowerStretcherBack());
		qsf::MeshAnimationComponent& backMeshAnimationComponent = backParamedic.getOrCreateComponentSafe<qsf::MeshAnimationComponent>();
		mBackParamedicAnimation = backMeshAnimationComponent.blendToAnimation(backAnimation, false, qsf::Time::fromSeconds(animationBlendTime));

		// This time is used to push an wait action to keep the persons in action, so they don't use there idle animation
		pushWaitAction(backParamedic, PICKUPPERSONACTION_WAIT_BLOCK);
	}

	void PickupPersonByParamedicAction::startRaiseStrechAnimation(qsf::Entity& strecherEntity)
	{
		const float animationBlendTime = 0.1f;

		qsf::AssetProxy animation(AnimationHelper(strecherEntity).getAnimationEquipmentStretcherRaise());
		qsf::MeshAnimationComponent& meshAnimationComponent = strecherEntity.getOrCreateComponentSafe<qsf::MeshAnimationComponent>();
		mStrecherAnimation = meshAnimationComponent.blendToAnimation(animation, false, qsf::Time::fromSeconds(animationBlendTime));
	}

	void PickupPersonByParamedicAction::startRaiseParamedicsAnimation(qsf::Entity& frontParamedic, qsf::Entity& backParamedic)
	{
		// Play the raise animations
		qsf::AssetProxy frontAnimation(AnimationHelper(frontParamedic).getAnimationParamedicRaiseStretcherFront());
		qsf::MeshAnimationComponent& frontMeshAnimationComponent = frontParamedic.getOrCreateComponentSafe<qsf::MeshAnimationComponent>();
		mFrontParamedicAnimation = frontMeshAnimationComponent.playAnimation(frontAnimation, false);

		qsf::AssetProxy backAnimation(AnimationHelper(backParamedic).getAnimationParamedicRaiseStretcherBack());
		qsf::MeshAnimationComponent& backMeshAnimationComponent = backParamedic.getOrCreateComponentSafe<qsf::MeshAnimationComponent>();
		mBackParamedicAnimation = backMeshAnimationComponent.playAnimation(backAnimation, false);
	}

	void PickupPersonByParamedicAction::startVictimAnimation(qsf::Entity& patientEntity) const
	{
		bool isDead = (patientEntity.getComponent<HealthComponent>() ? patientEntity.getComponent<HealthComponent>()->isDead() : false);
		qsf::MeshAnimationComponent& meshAnimationComponent = patientEntity.getOrCreateComponentSafe<qsf::MeshAnimationComponent>();
		qsf::AssetProxy victimAnimation(AnimationHelper(patientEntity).getAnimationVictimOnStretcher());

		if (isDead)
		{
			meshAnimationComponent.playAnimation(victimAnimation, false);
			if (meshAnimationComponent.getAnimationChannel(victimAnimation))
			{
				// To avoid moving body after dead, we disable the animation speed (currently we haven't a good dead animation)
				meshAnimationComponent.getAnimationChannel(victimAnimation)->setSpeed(0.0f);
			}
		}
		else
		{
			meshAnimationComponent.blendToAnimation(victimAnimation, true, qsf::Time::fromSeconds(0.3f));
		}

		// TODO(mk) Currently the positioning with disabled mesh component (happens after fade effect is finished) on weak PC can lead to errors. so don't use the fade effect now
		//patientEntity.getOrCreateComponentSafe<FadeEffectComponent>().fadeIn(qsf::Time::fromSeconds(0.5f));
	}

	void PickupPersonByParamedicAction::pushWaitAction(qsf::Entity& entity, const qsf::Time& waitTime) const
	{
		qsf::ActionComponent& actionComponent = entity.getOrCreateComponentSafe<qsf::ActionComponent>();
		actionComponent.pushAction<qsf::WaitAction>(getPriority(), qsf::action::INSERT_AT_FRONT).init(waitTime);
	}

	bool PickupPersonByParamedicAction::isAnimationPlaying() const
	{
		if (mFrontParamedicAnimation.valid() && !mFrontParamedicAnimation->isFinished())
		{
			return true;
		}

		if (mStrecherAnimation.valid() && !mStrecherAnimation->isFinished())
		{
			return true;
		}

		if (mBackParamedicAnimation.valid() && !mBackParamedicAnimation->isFinished())
		{
			return true;
		}

		return false;
	}

	void PickupPersonByParamedicAction::sendMessage() const
	{
		QSF_MESSAGE.emitMessage(qsf::MessageConfiguration(Messages::EM5_ACTION_PICKUP_PERSON_BY_PARAMEDIC, mTargetEntityId));
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

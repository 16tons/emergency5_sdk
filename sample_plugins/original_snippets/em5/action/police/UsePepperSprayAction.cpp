// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/action/police/UsePepperSprayAction.h"
#include "em5/action/move/TurnToAction.h"
#include "em5/base/EquipmentAssets.h"
#include "em5/component/person/PersonComponent.h"
#include "em5/command/police/UsePepperSprayCommand.h"
#include "em5/command/CommandContext.h"
#include "em5/specs/ActionRangeSpecsGroup.h"
#include "em5/game/animation/AnimationHelper.h"
#include "em5/audio/AudioHelper.h"
#include "em5/map/EntityHelper.h"
#include "em5/map/MapHelper.h"
#include "em5/plugin/Messages.h"

#include <qsf_game/base/DistanceHelper.h>
#include <qsf_game/command/CommandManager.h>
#include <qsf_game/command/CommandSystem.h>
#include <qsf_game/component/base/BoneLinkComponent.h>
#include <qsf_game/equipment/EquipmentComponent.h>
#include <qsf_game/QsfGameHelper.h>

#include <qsf/logic/action/ActionComponent.h>
#include <qsf/map/Map.h>
#include <qsf/message/MessageSystem.h>
#include <qsf/component/link/LinkComponent.h>
#include <qsf/renderer/particles/ParticlesComponent.h>
#include <qsf/QsfHelper.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const qsf::NamedIdentifier UsePepperSprayAction::ACTION_ID = "em5::UsePepperSprayAction";


	//[-------------------------------------------------------]
	//[ Private static definitions                            ]
	//[-------------------------------------------------------]
	// TODO(co) Don't throw it in as global stuff, put it inside the class. I assume this is a candidate for the game specifications so the game designers can tweak it.
	static const qsf::Time   TIME_PEPPER_SPRAY_IS_IN_USE     = qsf::Time::fromSeconds(2.0);
	static const qsf::Time   TIME_PEPPER_SPRAY_STUNES_TARGET = qsf::Time::fromSeconds(10.0);
	static const std::string PEPPER_SPRAY_PARTICLES_EFFECT   = "em5/particles/particles/pfefferspray";	// The name/path of the pepper spray particle effect asset	// TODO(co) Path? Do you mean local asset name or global asset ID?


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	UsePepperSprayAction::UsePepperSprayAction() :
		Action(ACTION_ID),
		mTargetEntityId(qsf::getUninitialized<uint64>()),
		mCurrentState(STATE_INIT),
		mTargetYaw(0.0f),
		mTimeAnimationBlending(qsf::Time::fromSeconds(0.3f))
	{
		// Nothing here
	}

	UsePepperSprayAction::~UsePepperSprayAction()
	{
		// In case we action is aborted, cancel all action specific graphics and animations via the "onShutdown". So nothing to do in here.
	}

	void UsePepperSprayAction::init(const qsf::Entity& targetEntity)
	{
		mTargetEntityId = targetEntity.getId();
	}

	uint64 UsePepperSprayAction::getTargetEntityId() const
	{
		return mTargetEntityId;
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Actions methods                   ]
	//[-------------------------------------------------------]
	void UsePepperSprayAction::serialize(qsf::BinarySerializer& serializer)
	{
		serializer.serialize(mTargetEntityId);
		serializer.serializeAs<uint16>(mCurrentState);
		serializer.serialize(mTimePepperSprayIsInUse);
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	bool UsePepperSprayAction::onStartup()
	{
		return validateUserAndTarget();
	}

	qsf::action::Result UsePepperSprayAction::updateAction(const qsf::Clock& clock)
	{
		if (validateUserAndTarget())
		{
			switch (mCurrentState)
			{
				case STATE_INIT:
				{
					// Somehow the targetpoint is not turning the policemen to the target, to turn to target manually to avoid starting the effect without having the actor in the right angle
					getComponent().pushAction<TurnToAction>(getPriority(), qsf::action::INSERT_AT_FRONT).init(mTargetEntityId);

					mCurrentState = STATE_BRING_SPRAY_IN_POSITION;
					return qsf::action::RESULT_CONTINUE;
				}

				case STATE_BRING_SPRAY_IN_POSITION:
				{
					// Start caller animation (take this!)
					AnimationHelper entityAnimationHelper(getEntity());
					entityAnimationHelper.playAnimation(entityAnimationHelper.getAnimationPolicePeppersprayStandingLoop(), true, false, mTimeAnimationBlending);

					mCurrentState = STATE_STARTUSING_PEPPERSPRAY;
					return qsf::action::RESULT_CONTINUE;
				}

				case STATE_STARTUSING_PEPPERSPRAY:
				{
					mTimeAnimationBlending -= clock.getTimePassed();
					if (mTimeAnimationBlending > qsf::Time::ZERO)
					{
						// Wait for animation finish
						return qsf::action::RESULT_CONTINUE;
					}

					// Create and link pepper spray particle effect
					mEffectEntity = createPepperSprayEffect();
					if (mEffectEntity.valid())
					{
						linkEffectEntity();
					}

					setTargetStunned();

					// Play audio
					AudioHelper::playPolicePepperSpray(mAudioProxy, getEntity());
					mAudioProxy.synchronizeWithMultiplay();

					qsf::Entity* targetEntity = getMap().getEntityById(mTargetEntityId);
					mTargetYaw = TurnToAction::calcYawRotationToTarget(getEntity(), *targetEntity, false);

					mTimePepperSprayIsInUse = qsf::Time::ZERO;
					mCurrentState = STATE_USEPEPPERSPRAY;

					// Fall through by design
				}

				case STATE_USEPEPPERSPRAY:
				{
					// Turn caller to target
					TurnToAction::turnToTarget(getEntity(), clock.getTimePassed().getSeconds(), mTargetYaw);

					// Particle effect should play for 2 seconds
					mTimePepperSprayIsInUse += clock.getTimePassed();
					if (mTimePepperSprayIsInUse < TIME_PEPPER_SPRAY_IS_IN_USE)
					{
						// Continue using pepper spray
						return qsf::action::RESULT_CONTINUE;
					}
					// Stop audio
					mAudioProxy.stop();
					mAudioProxy.synchronizeWithMultiplay();

					mCurrentState = STATE_DONE;

					// Fall through by design
				}

				case STATE_DONE:
				{
					// Clean up in destructor
					break;
				}
			}
		}

		// Emit message "Use pepper spray finished"
		QSF_MESSAGE.emitMessage(qsf::MessageConfiguration(Messages::EM5_ACTION_POLICE_USED_PEPPER_SPRAY, mTargetEntityId));

		return qsf::action::RESULT_DONE;
	}

	void UsePepperSprayAction::onShutdown()
	{
		if (mEffectEntity.valid())
		{
			getMap().destroyEntityById(mEffectEntity->getId());
		}

		// Clear policeman animation
		AnimationHelper(getEntity()).clearAnimation();
	}

	void UsePepperSprayAction::onPause()
	{
		// Reset the action
		onShutdown();
		mCurrentState = STATE_BRING_SPRAY_IN_POSITION;
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	qsf::Entity* UsePepperSprayAction::createPepperSprayEffect()
	{
		qsf::Map& map = getEntity().getMap();
		// Show the particle effect for fire extinguisher
		qsf::Entity* effectEntity = MapHelper(map).createObjectByLocalPrefabAssetId(equipment::PREFAB_POLICE_PEPPERSPRAY_EFFECT);
		if (nullptr != effectEntity)
		{
			// Show the particle particle
			qsf::ParticlesComponent* particlesComponent = effectEntity->getOrCreateComponent<qsf::ParticlesComponent>();
			if (nullptr != particlesComponent)
			{
				particlesComponent->setActive(true);
				particlesComponent->showAnimation();
				particlesComponent->setEmitterEnabled(true);
			}
		}
		return effectEntity;
	}

	void UsePepperSprayAction::linkEffectEntity()
	{
		qsf::game::EquipmentComponent* equipmentComponent = EntityHelper(getEntity()).getActiveEquipmentComponent();
		if (nullptr != equipmentComponent)
		{
			uint64 equipmentEntityId = equipmentComponent->getEntityId();

			// Link effect entity to equipment
			qsf::LinkComponent& linkComponent = mEffectEntity->getOrCreateComponentSafe<qsf::LinkComponent>();
			linkComponent.setParentId(equipmentEntityId);
			linkComponent.setParentLinkType(qsf::LinkComponent::HARD_LINK);

			// Link to bone of the parent
			qsf::game::BoneLinkComponent* boneLinkComponent = mEffectEntity->getOrCreateComponent<qsf::game::BoneLinkComponent>();
			if (nullptr != boneLinkComponent)
			{
				boneLinkComponent->setBoneName(equipment::STANDARD_BONE_EFFECT);
			}
		}
	}

	bool UsePepperSprayAction::validateUserAndTarget() const
	{
		// Get the caller and target entity
		qsf::Entity& callerEntity = getEntity();
		qsf::Entity* targetEntity = getMap().getEntityById(mTargetEntityId);
		if (nullptr == targetEntity)
			return false;

		// Make the distance check
		const float maxDistance = ActionRangeSpecsGroup::getInstanceSafe().getArrestGangsterDistance();
		const float currentDistance = qsf::game::DistanceHelper::getDistance(*targetEntity, callerEntity);
		if (currentDistance > maxDistance)
			return false;

		// Build context
		CommandContext commandContext;
		commandContext.mCaller = &callerEntity;
		commandContext.mTargetEntity = targetEntity;

		// Call "checkCaller" && "checkContext" of the commando for validation
		UsePepperSprayCommand* usePepperSprayCommand = static_cast<UsePepperSprayCommand*>(QSFGAME_COMMAND.getCommandManager().getById<UsePepperSprayCommand>(UsePepperSprayCommand::PLUGINABLE_ID));
		QSF_CHECK(nullptr != usePepperSprayCommand, "UsePepperSprayAction::validateUserAndTarget(): Could not find use pepperspray command instance", return false);
		if (!(usePepperSprayCommand->checkCaller(*commandContext.mCaller) && usePepperSprayCommand->checkContext(commandContext)))
			return false;

		return true;
	}

	void UsePepperSprayAction::setTargetStunned()
	{
		// Get the target entity instance
		const qsf::Entity* targetEntity = getMap().getEntityById(mTargetEntityId);
		QSF_CHECK(nullptr != targetEntity, "UsePepperSprayAction: unable to retrieve the target entity instance", return);

		// The person will be stunned now
		// -> We checked the person component already, so we can use it safely
		targetEntity->getComponentSafe<PersonComponent>().setStunned(getEntityId(), TIME_PEPPER_SPRAY_STUNES_TARGET, true);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

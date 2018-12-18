// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/action/vegetation/CutTreeAction.h"
#include "em5/action/base/DisappearAction.h"
#include "em5/action/base/PlayAnimationAction.h"
#include "em5/action/move/TurnToAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/audio/AudioHelper.h"
#include "em5/base/EquipmentAssets.h"
#include "em5/base/GameplayAssets.h"
#include "em5/command/vegetation/CutTreeCommand.h"
#include "em5/command/component/CommandableComponent.h"
#include "em5/command/CommandContext.h"
#include "em5/component/objects/BuryComponent.h"
#include "em5/component/vegetation/TreeComponent.h"
#include "em5/component/vegetation/HedgeComponent.h"
#include "em5/fire/component/FireComponent.h"
#include "em5/game/animation/AnimationHelper.h"
#include "em5/logic/local/ReserveLogic.h"
#include "em5/map/EntityHelper.h"
#include "em5/map/MapHelper.h"
#include "em5/plugin/Messages.h"
#include "em5/specs/ActionRangeSpecsGroup.h"
#include "em5/specs/LogicSpecsGroup.h"

#include <qsf_game/base/DistanceHelper.h>
#include <qsf_game/command/CommandManager.h>
#include <qsf_game/command/CommandSystem.h>
#include <qsf_game/component/base/BoneLinkComponent.h>
#include <qsf_game/equipment/EquipmentComponent.h>
#include <qsf_game/QsfGameHelper.h>

#include <qsf/base/PropertyDictionary.h>
#include <qsf/component/base/TransformComponent.h>
#include <qsf/component/link/LinkComponent.h>
#include <qsf/logic/action/ActionComponent.h>
#include <qsf/map/Map.h>
#include <qsf/math/CoordinateSystem.h>
#include <qsf/math/Math.h>
#include <qsf/math/EulerAngles.h>
#include <qsf/message/MessageSystem.h>
#include <qsf/renderer/RendererSystem.h>
#include <qsf/renderer/mesh/MeshComponent.h>
#include <qsf/renderer/particles/ParticlesComponent.h>
#include <qsf/renderer/material/MaterialSystem.h>
#include <qsf/renderer/material/material/MaterialProperty.h>
#include <qsf/renderer/material/material/MaterialVariationManager.h>
#include <qsf/QsfHelper.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const qsf::NamedIdentifier CutTreeAction::ACTION_ID					= "em5::CutTreeAction";
	const qsf::NamedIdentifier CutTreeAction::PARAMETER_TARGET_ENTITYID = "targetEntityId";	// TODO(co) Shouldn't this be "TargetEntityId"?

	static const std::string CHAINSAW_SMOKE_EFFECT_BONE = "effect_bone_smoke";


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	CutTreeAction::CutTreeAction() :
		Action(ACTION_ID),
		mState(STATE_INIT),
		mReactivateFireComponent(true)
	{
		// Nothing here
	}

	CutTreeAction::CutTreeAction(qsf::PropertyDictionary* parameters) :
		Action(ACTION_ID),
		mState(STATE_INIT)
	{
		QSF_CHECK(nullptr != parameters, "Found null pointer for action generic parameters!", QSF_REACT_THROW);

		uint64 targetEntityId = qsf::getUninitialized<uint64>();
		QSF_CHECK(parameters->get(PARAMETER_TARGET_ENTITYID, targetEntityId), "Missing mandatory parameter '"  << PARAMETER_TARGET_ENTITYID.getName() << "' in CutTreeAction parameters!", QSF_REACT_THROW);
		mTargetEntity = getMap().getEntityById(targetEntityId);
	}

	CutTreeAction::~CutTreeAction()
	{
		stopChainsawEffect();

		if (mTargetEntity.valid())
		{
			if (mReactivateFireComponent)
			{
				FireComponent* fireComponent = mTargetEntity->getComponent<FireComponent>();
				if (nullptr != fireComponent)
				{
					// Reactivate the fire component
					fireComponent->setActive(true);
				}
			}

			ReserveLogic::deleteOwnReservation(getMap(), mTargetEntity->getId(), getEntityId());
		}
	}

	void CutTreeAction::initByPointer(qsf::Entity* target)
	{
		if (nullptr != target)
		{
			init(*target);
		}
	}

	void CutTreeAction::init(qsf::Entity& targetEntity)
	{
		mTargetEntity = targetEntity;
	}

	uint64 CutTreeAction::getTargetEntityId() const
	{
		return mTargetEntity.valid() ? mTargetEntity->getId() : qsf::getUninitialized<uint64>();
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Actions methods                   ]
	//[-------------------------------------------------------]
	void CutTreeAction::serialize(qsf::BinarySerializer& serializer)
	{
		// TODO(fw): Implement this
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	bool CutTreeAction::onStartup()
	{
		// Get the caller and target entity
		qsf::Entity& callerEntity = getEntity();
		if (!mTargetEntity.valid())
			return false;

		// Make the distance check
		const float maxDistance = ActionRangeSpecsGroup::getInstanceSafe().getCutTreeDistance();
		const float currentDistance = qsf::game::DistanceHelper::getDistance(*mTargetEntity, callerEntity);
		if (maxDistance < currentDistance)
			return false;

		// Build context
		CommandContext commandContext;
		commandContext.mCaller = &callerEntity;
		commandContext.mTargetEntity = mTargetEntity.get();

		// Call "checkCaller" && "checkContext" of the commando for validation
		CutTreeCommand* cutTreeCommand = static_cast<CutTreeCommand*>(QSFGAME_COMMAND.getCommandManager().getById<CutTreeCommand>(CutTreeCommand::PLUGINABLE_ID));
		QSF_CHECK(nullptr != cutTreeCommand, "CutTreeAction::validateUserAndTarget(): Could not find use cuttree command instance", return false);
		if (!(cutTreeCommand->checkCaller(*commandContext.mCaller) && cutTreeCommand->checkContext(commandContext)))
			return false;

		// Check for the reserve logic
		if (!ReserveLogic::checkReservation(*mTargetEntity, getEntityId()))
			return false;

		return true;
	}

	qsf::action::Result CutTreeAction::updateAction(const qsf::Clock& clock)
	{
		switch (mState)
		{
			case STATE_INIT:
			{
				mTimeRunning = qsf::Time::fromSeconds(LogicSpecsGroup::getInstanceSafe().getCutTreeTime());
				playBeginUseChainSawAnimation();

				if (EntityHelper(getEntity()).isUnit(CommandableComponent::UNITTAG_FIREFIGHTERS_FIREFIGHTER_HISTORIC))
				{
					// Play historic start saw audio
					AudioHelper::playFirefighterHistoricSawStart(mAudioProxy, getEntity());
					mAudioProxy.synchronizeWithMultiplay();
				}
				else
				{
					// Play start saw audio
					AudioHelper::playFirefighterSawStart(mAudioProxy, getEntity());
					mAudioProxy.synchronizeWithMultiplay();
				}

				// Stop burning for the tree (in case we abort the action, we reactivate it)
				if (mTargetEntity.valid())
				{
					FireComponent* fireComponent = mTargetEntity->getComponent<FireComponent>();
					if (nullptr != fireComponent)
					{
						// Turn off the fire for the tree
						fireComponent->setActive(false);
					}

					// Reserve the target
					ReserveLogic::createReservation<ReserveCutTreeLogic>(*mTargetEntity, getEntityId());
				}

				mState = STATE_USE_SAW;
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_USE_SAW:
			{
				if (AnimationHelper(getEntity()).isAnimationPlaying())
				{
					// Wait for ending of the animation
					return qsf::action::RESULT_CONTINUE;
				}

				playUseChainSawAnimation();
				startChainsawEffect();

				if (EntityHelper(getEntity()).isUnit(CommandableComponent::UNITTAG_FIREFIGHTERS_FIREFIGHTER_HISTORIC))
				{
					// Play historic audio
					AudioHelper::playFirefighterHistoricSawRunning(mAudioProxy, getEntity());
					mAudioProxy.synchronizeWithMultiplay();
				}
				else
				{
					// Play audio
					AudioHelper::playFirefighterSawRunning(mAudioProxy, getEntity());
					mAudioProxy.synchronizeWithMultiplay();
				}

				mState = STATE_CUT_TREE;
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_CUT_TREE:
			{
				if (mTimeRunning > qsf::Time::ZERO)
				{
					mTimeRunning -= clock.getTimePassed();

					// Wait for end
					return qsf::action::RESULT_CONTINUE;
				}

				// We finish the cutting, don't reactivate the fire component
				mReactivateFireComponent = false;

				playEndUseChainSawAnimation();
				stopChainsawEffect();
				startFallTreeEffect();
				unburyTarget();

				// Emit a message
				QSF_MESSAGE.emitMessage(qsf::MessageConfiguration(Messages::EM5_ACTION_CUT_VEGETATION, getTargetEntityId()));

				mAudioProxy.stop();
				mAudioProxy.synchronizeWithMultiplay();
				// Fall through by design
			}
		}

		return qsf::action::RESULT_DONE;
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void CutTreeAction::playBeginUseChainSawAnimation()
	{
		qsf::ActionComponent& actionComponent = getComponent();
		actionComponent.pushAction<TurnToAction>(action::BLOCKING, qsf::action::INSERT_BEFORE_CURRENT).init(getTargetEntityId());
		actionComponent.pushAction<PlayAnimationAction>(action::BLOCKING, qsf::action::INSERT_BEFORE_CURRENT).init(AnimationHelper(getEntity()).getAnimationFirefighterUseSaw1());

		// Start UV animation on the chainsaw blades
		QSF_MATERIAL.getMaterialVariationManager().setEntityMaterialPropertyValue(getEntity(), "UseUvScrolling", qsf::MaterialPropertyValue::fromBoolean(true));
	}

	void CutTreeAction::playUseChainSawAnimation()
	{
		qsf::ActionComponent& actionComponent = getComponent();
		actionComponent.pushAction<PlayAnimationAction>(action::BLOCKING, qsf::action::INSERT_BEFORE_CURRENT).init(AnimationHelper(getEntity()).getAnimationFirefighterUseSaw2(), false, true);
	}

	void CutTreeAction::playEndUseChainSawAnimation()
	{
		qsf::ActionComponent& actionComponent = getComponent();
		actionComponent.pushAction<PlayAnimationAction>(action::BLOCKING, qsf::action::INSERT_BEFORE_CURRENT).init(AnimationHelper(getEntity()).getAnimationFirefighterUseSaw3());

		// End UV animation on the chainsaw blades
		QSF_MATERIAL.getMaterialVariationManager().setEntityMaterialPropertyValue(getEntity(), "UseUvScrolling", qsf::MaterialPropertyValue::fromBoolean(false));
	}

	void CutTreeAction::startFallTreeEffect()
	{
		if (mTargetEntity.valid())
		{
			// Get the tree component there
			TreeComponent* treeComponent = mTargetEntity->getComponent<TreeComponent>();
			if (nullptr != treeComponent)
			{
				// The tree is chopped now
				treeComponent->cutTree(getEntity());
			}

			// else, we deal with an hedge
			HedgeComponent* hedgeComponent = mTargetEntity->getComponent<HedgeComponent>();
			if (nullptr != hedgeComponent)
			{
				hedgeComponent->cutHedge();
			}
		}
	}

	void CutTreeAction::startChainsawEffect()
	{
		qsf::game::EquipmentComponent* equipmentComponent = EntityHelper(getEntity()).getActiveEquipmentComponent();
		if (nullptr == equipmentComponent)
			return;

		qsf::Entity& chainsawEntity = equipmentComponent->getEntity();
		MapHelper mapHelper(getMap());
		if (!mParticleEffectEntity.valid())
		{
			mParticleEffectEntity = mapHelper.createObjectByLocalPrefabAssetId(assets::PREFAB_CHAINSAW_EFFECT);
		}

		if (mParticleEffectEntity.valid())
		{
			// Link particle on chainsaw effect bone
			mParticleEffectEntity->getOrCreateComponentSafe<qsf::LinkComponent>().setParentId(chainsawEntity.getId());
			mParticleEffectEntity->getOrCreateComponentSafe<qsf::game::BoneLinkComponent>().setBoneName(equipment::STANDARD_BONE_EFFECT);

			qsf::TransformComponent& transformComponent = mParticleEffectEntity->getOrCreateComponentSafe<qsf::TransformComponent>();
			glm::quat newRotation = transformComponent.getRotation() * qsf::Math::getRotationAroundAxis(qsf::CoordinateSystem::getRight(), glm::pi<float>());
			transformComponent.setRotation(newRotation);

			qsf::ParticlesComponent* particleComponent = mParticleEffectEntity->getComponent<qsf::ParticlesComponent>();
			if (particleComponent)
			{
				particleComponent->setEmitterEnabled(true);
			}
		}
		// The smoke on the back of the chain saw
		if (!mParticleSmokeEffectEntity.valid())
		{
			mParticleSmokeEffectEntity = mapHelper.createObjectByLocalPrefabAssetId(assets::PREFAB_CHAINSAW_EFFECT_SMOKE);
		}

		if (mParticleSmokeEffectEntity.valid())
		{
			qsf::TransformComponent& transformComponent = mParticleSmokeEffectEntity->getOrCreateComponentSafe<qsf::TransformComponent>();
			transformComponent.setScale(glm::vec3(2.f, 2.f, 2.f));

			// Link particle on chainsaw effect bone
			mParticleSmokeEffectEntity->getOrCreateComponentSafe<qsf::LinkComponent>().setParentId(chainsawEntity.getId());
			qsf::game::BoneLinkComponent& boneLinkComponent = mParticleSmokeEffectEntity->getOrCreateComponentSafe<qsf::game::BoneLinkComponent>();
			// TODO(mk) Not sure if this gives an visual change...
			boneLinkComponent.setBoneName(CHAINSAW_SMOKE_EFFECT_BONE);
			boneLinkComponent.setLocalRotation(qsf::EulerAngles::eulerToQuaternion(glm::radians(-90.f), 0, 0));

			qsf::ParticlesComponent* particleComponent = mParticleSmokeEffectEntity->getComponent<qsf::ParticlesComponent>();
			if (particleComponent)
			{
				particleComponent->setEmitterEnabled(true);
			}
		}
	}

	void CutTreeAction::stopChainsawEffect()
	{
		if (mParticleEffectEntity.valid())
		{
			MapHelper::destroyEntity(mParticleEffectEntity.getSafe());
		}
		if (mParticleSmokeEffectEntity.valid())
		{
			MapHelper::destroyEntity(mParticleSmokeEffectEntity.getSafe());
		}
	}

	void CutTreeAction::unburyTarget()
	{
		if (mTargetEntity.valid())
		{
			// Get the tree component there
			BuryComponent* buryComponent = mTargetEntity->getComponent<BuryComponent>();
			if (nullptr != buryComponent && buryComponent->getCutableBySaw())
			{
				buryComponent->unlinkAndShowBuriedEntities();
				mTargetEntity->destroyComponent<BuryComponent>();

				// Add a fade out effect on the target
				qsf::ActionComponent* actionComponent = mTargetEntity->getOrCreateComponent<qsf::ActionComponent>();
				if (nullptr != actionComponent)
				{
					qsf::MeshComponent* meshComponent = mTargetEntity->getComponent<qsf::MeshComponent>();
					if (nullptr != meshComponent)
					{
						meshComponent->setCastShadows(false);
					}
					actionComponent->pushAction<DisappearAction>(action::BLOCKING).init(qsf::Time::fromSeconds(2.0f));
				}
			}
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

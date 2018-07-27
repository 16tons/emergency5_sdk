// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/action/fire/EnterBoatAction.h"
#include "em5/action/base/PlayAnimationAction.h"
#include "em5/action/base/DisappearAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/base/ContainerCategory.h"
#include "em5/base/GameplayAssets.h"
#include "em5/command/base/EnterVehicleCommand.h"
#include "em5/command/CommandContext.h"
#include "em5/command/component/CommandableComponent.h"
#include "em5/game/animation/AnimationHelper.h"
#include "em5/health/HealthComponent.h"
#include "em5/logic/local/ReserveLogic.h"
#include "em5/specs/ActionRangeSpecsGroup.h"
#include "em5/component/effect/FadeEffectComponent.h"
#include "em5/plugin/Messages.h"
#include "em5/map/MapHelper.h"
#include "em5/map/EntityHelper.h"

#include <qsf_game/base/DistanceHelper.h>
#include <qsf_game/command/CommandSystem.h>
#include <qsf_game/QsfGameHelper.h>

#include <qsf/component/link/LinkComponent.h>
#include <qsf/renderer/particles/ParticlesComponent.h>
#include <qsf/map/Map.h>
#include <qsf/message/MessageSystem.h>
#include <qsf/logic/action/ActionComponent.h>
#include <qsf/component/base/TransformComponent.h>
#include <qsf/QsfHelper.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const qsf::NamedIdentifier EnterBoatAction::ACTION_ID = "em5::EnterBoatAction";


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	EnterBoatAction::EnterBoatAction() :
		Action(ACTION_ID),
		mTargetEntityId(qsf::getUninitialized<uint64>()),
		mState(STATE_INIT),
		mEnterBoatParticleId(qsf::getUninitialized<uint64>())
	{
		// Nothing here
	}

	EnterBoatAction::~EnterBoatAction()
	{
		// Remove reservation again
		ReserveLogic::deleteOwnReservation(getMap(), mTargetEntityId, getEntityId());
	}

	void EnterBoatAction::init(const qsf::Entity& targetEntity)
	{
		mTargetEntityId = targetEntity.getId();
	}

	uint64 EnterBoatAction::getTargetId() const
	{
		return mTargetEntityId;
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Actions methods                   ]
	//[-------------------------------------------------------]
	void EnterBoatAction::serialize(qsf::BinarySerializer& serializer)
	{
		serializer.serialize(mTargetEntityId);
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	bool EnterBoatAction::onStartup()
	{
		return checkActionCondition();
	}

	void EnterBoatAction::onShutdown()
	{
		// Destroy the created splash particles
		if (qsf::isInitialized(mEnterBoatParticleId))
		{
			getMap().destroyEntityById(mEnterBoatParticleId);
		}
	}

	qsf::action::Result EnterBoatAction::updateAction(const qsf::Clock&)
	{
		// Get the most important objects first
		qsf::Entity* targetEntity = getMap().getEntityById(mTargetEntityId);
		if (nullptr == targetEntity)
			return qsf::action::RESULT_DONE;

		// Handle the current state
		switch (mState)
		{
			case STATE_INIT:
			{
				if (!checkActionCondition())
				{
					// Action condition has changed, maybe we have no free seat or the boat is gone
					return qsf::action::RESULT_DONE;
				}

				// Check for the reserve logic
				if (!ReserveLogic::checkReservation(*targetEntity, getEntityId()))
				{
					// Wait till the reservation is unlocked
					return qsf::action::RESULT_CONTINUE;
				}

				// Reserve the entity
				ReserveLogic::createReservation<ReserveEnterVehicleLogic>(*targetEntity, getEntityId());

				// Show animation to enter the boat and disappear (reverse order because inserted at front)
				getComponent().pushAction<DisappearAction>(action::BLOCKING, qsf::action::INSERT_AT_FRONT).init(qsf::Time::fromSeconds(1.0f), false);
				getComponent().pushAction<PlayAnimationAction>(action::BLOCKING, qsf::action::INSERT_AT_FRONT).init(AnimationHelper(getEntity()).getAnimationDiverEnterBoat(), true);

				// Add particles to the person for dropping water
				linkParticle(getMap().createObjectByLocalPrefabAssetId(assets::PARTICLE_WATERSPRAY));
				linkParticle(getMap().createObjectByLocalPrefabAssetId(assets::PARTICLE_WATERDROPS));

				// Add particles on top of water (person pivot should be there at this moment)
				qsf::Entity* particleWaterSpray2 = getMap().createObjectByLocalPrefabAssetId(assets::PARTICLE_WATERSPRAY2);
				if (particleWaterSpray2 != nullptr)
				{
					const qsf::TransformComponent& transformComponent = getEntity().getOrCreateComponentSafe<qsf::TransformComponent>();
					qsf::TransformComponent& particleTransformComponent = particleWaterSpray2->getOrCreateComponentSafe<qsf::TransformComponent>();
					particleTransformComponent.setPosition(transformComponent.getPosition());
					mEnterBoatParticleId = particleWaterSpray2->getId();
				}

				mState = STATE_HAS_ENTERED;

				// Give it 1 frame to start the animation
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_HAS_ENTERED:
			{
				if (!getEntity().getOrCreateComponentSafe<FadeEffectComponent>().isFading())
				{
					// Make diver invincible
					targetEntity->getOrCreateComponentSafe<HealthComponent>().setInvincible(true);

					// Unlink and destroy all particles
					unlinkParticles();

					// Enter the vehicle now
					EntityHelper(getEntity()).enterContainer(*targetEntity, container::CONTAINERTYPE_SQUAD);

					qsf::MessageParameters parameters;
					parameters.setParameter("boatId",  mTargetEntityId);
					parameters.setParameter("diverId", getEntityId());
					QSF_MESSAGE.emitMessage(qsf::MessageConfiguration(Messages::EM5_ACTION_PERSON_ENTER_BOAT, mTargetEntityId, getEntityId()), parameters);

					return qsf::action::RESULT_DONE;
				}

				return qsf::action::RESULT_CONTINUE;
			}
		}

		// Error
		return qsf::action::RESULT_DONE;
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	bool EnterBoatAction::checkActionCondition()
	{
		// Get the caller and target entity
		qsf::Entity& callerEntity = getEntity();
		qsf::Entity* targetEntity = callerEntity.getMap().getEntityById(mTargetEntityId);
		if (nullptr == targetEntity)
			return false;

		// Build context
		CommandContext commandContext;
		commandContext.mCaller = &callerEntity;
		commandContext.mTargetEntity = targetEntity;

		// Call "checkCaller" && "checkContext" of the commando for validation
		EnterVehicleCommand* enterVehicleCommand = static_cast<EnterVehicleCommand*>(QSFGAME_COMMAND.getCommandManager().getById<EnterVehicleCommand>(EnterVehicleCommand::PLUGINABLE_ID));
		QSF_CHECK(nullptr != enterVehicleCommand, "EnterBoatAction::onStartup(): Could not find enter vehicle command instance", return false);
		if (!(enterVehicleCommand->checkCallerWithoutPriority(*commandContext.mCaller) && enterVehicleCommand->checkContext(commandContext)))
			return false;

		// Make the distance check
		float maxDistance = ActionRangeSpecsGroup::getInstanceSafe().getEnterVehicleDistance();
		const float currentDistance = qsf::game::DistanceHelper::get2dDistance(*targetEntity, callerEntity);
		// TODO(mk) Here is an extra check if the caller is an RTW, for this we need an bigger distance. Find a better way for this.
		const CommandableComponent* commandableComponent = callerEntity.getComponent<CommandableComponent>();
		if (nullptr != commandableComponent && commandableComponent->isAmbulanceParamedics())
		{
			maxDistance *= 2;
		}

		if (currentDistance > maxDistance)
			return false;

		// Every check is valid, return true
		return true;
	}

	void EnterBoatAction::linkParticle(qsf::Entity* particleEntity) const
	{
		if (particleEntity != nullptr)
		{
			qsf::LinkComponent& linkComponent = particleEntity->getOrCreateComponentSafe<qsf::LinkComponent>();
			linkComponent.setParentId(getEntityId());
			linkComponent.setLocalPosition(glm::vec3(0.0f, 0.0f, 0.0f)); // Artists told me to use no offset
		}
	}

	void EnterBoatAction::unlinkParticles() const
	{
		for (const qsf::LinkComponent* childLink : getEntity().getOrCreateComponentSafe<qsf::LinkComponent>().getChildLinks())
		{
			if (nullptr != childLink->getEntity().getComponent<qsf::ParticlesComponent>())
			{
				MapHelper::destroyEntity(childLink->getEntity());
			}
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/game/animation/PersonAnimationController.h"
#include "em5/game/animation/AnimationHelper.h"
#include "em5/action/move/MoveAction.h"
#include "em5/action/police/PlaceBarrierPoleAction.h"
#include "em5/ai/MovementModes.h"
#include "em5/base/EquipmentAssets.h"
#include "em5/component/movement/PersonMagnetComponent.h"
#include "em5/component/objects/BuryComponent.h"
#include "em5/component/objects/CallForRescueComponent.h"
#include "em5/component/person/PersonComponent.h"
#include "em5/command/component/CommandableComponent.h"
#include "em5/health/injury/Injury.h"
#include "em5/health/HealthComponent.h"
#include "em5/health/action/InjuredAction.h"
#include "em5/logic/local/firefighters/DiverLogic.h"
#include "em5/map/EntityHelper.h"
#include "em5/plugin/Messages.h"

#include <qsf_ai/navigation/NavigationComponent.h>

#include <qsf/component/base/TransformComponent.h>
#include <qsf/component/base/MetadataComponent.h>
#include <qsf/component/link/LinkComponent.h>
#include <qsf/component/move/MovableComponent.h>
#include <qsf/logic/gamelogic/GameLogicComponent.h>
#include <qsf/logic/action/WaitAction.h>
#include <qsf/map/Entity.h>
#include <qsf/map/Map.h>
#include <qsf/math/Random.h>
#include <qsf/message/MessageSystem.h>
#include <qsf/renderer/animation/MeshAnimationTestComponent.h>
#include <qsf/renderer/animation/MeshAnimationChannel.h>
#include <qsf/QsfHelper.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	std::string PersonAnimationController::getInjuredAnimation(qsf::Entity& entity)
	{
		const HealthComponent* healthComponent = entity.getComponent<HealthComponent>();
		if (healthComponent == nullptr)
			return std::string();

		AnimationHelper animationHelper(entity);
		std::string animationName = animationHelper.getAnimationGenericLightLying();	// Used as fallback

		if (healthComponent->isDiagnosisComplete() || healthComponent->isTreatmentActive() || healthComponent->isDead())
		{
			// Change nothing if a doctor is interacting with the patient, he will push actions to the target and need specific animations to finish his operations
			animationName.clear();
		}
		else
		{
			const Injury* injury = healthComponent->getInjury();
			if (nullptr != injury)
			{
				animationName = injury->getAnimationName();

				// Detect swimmer and drowning persons
				if (injury->getIsDrowning())
				{
					HealthComponent::State healthState = healthComponent->getState();
					const qsf::LinkComponent* linkComponent = entity.getComponent<qsf::LinkComponent>();
					if (linkComponent != nullptr && linkComponent->hasParent())	// Is carried?
					{
						animationName = animationHelper.getAnimationDrowningGetRescuedMove();
					}
					else if ((healthState == HealthComponent::STATE_INJURED && (healthComponent->getLifeEnergy() < (healthComponent->getMaximumLifeEnergy() * 0.1f)))
						|| healthState == HealthComponent::STATE_DEATHFIGHT)
					{
						// Lower 10%, show danger animation
						animationName = animationHelper.getAnimationInjuredDrowningDanger();
					}
					else if (healthState == HealthComponent::STATE_HEALTHY ||
						healthState == HealthComponent::STATE_INJURED)
					{
						// Normal animation
						animationName = animationHelper.getAnimationInjuredDrowning();
					}
					else
					{
						// Dead animation
						animationName = animationHelper.getAnimationInjuredDrowningDead();
					}
				}
				else
				{
					CallForRescueComponent* callForRescueComponent = entity.getComponent<CallForRescueComponent>();
					if (nullptr != callForRescueComponent && callForRescueComponent->getCanRescuedByDLK())
					{
						// Some special event persons have the tag "DLK_TARGET" they are hanging on a bridge or building. Currently used in me02, me06 and me14
						animationName = animationHelper.getAnimationHangingHelpless();
					}
				}
			}
		}

		const qsf::LinkComponent* linkComponent = entity.getComponent<qsf::LinkComponent>();
		if (nullptr != linkComponent)
		{
			{ // Detect if patient is on stretcher
				const CommandableComponent* parentCommandableComponent = linkComponent->getComponentFromEntityOrLinkedParent<CommandableComponent>();
				if (nullptr != parentCommandableComponent && (parentCommandableComponent->isAmbulanceParamedics() || parentCommandableComponent->isTechHeli()))
				{
					animationName = animationHelper.getAnimationVictimOnStretcher();
				}
			}

			qsf::Prototype* parentPrototype = linkComponent->getParentPrototype();
			if (parentPrototype != nullptr)
			{
				// If pulled
				EntityHelper parentEntityHelper(*static_cast<qsf::Entity*>(parentPrototype));
				if (parentEntityHelper.isPullingPerson())
				{
					animationName = AnimationHelper(entity).getAnimationInjuredPulledLoop();
				}
				else if (parentEntityHelper.isCarryingAnotherPerson())
				{
					// Carried (lead) and contaminated
					if (EntityHelper(entity).isPersonContaminated())
					{
						animationName.clear(); // Part of movement animations
					}
					// Carried by diver
					else
					{
						const qsf::GameLogicComponent* gameLogicComponent = parentPrototype->getComponent<qsf::GameLogicComponent>();
						if (nullptr != gameLogicComponent)
						{
							const DiverLogic* diverLogic = gameLogicComponent->getGameLogic<DiverLogic>();
							if (nullptr != diverLogic)
							{
								// Still in water?
								if (diverLogic->isSwimming())
								{
									animationName = animationHelper.getAnimationDrowningGetRescuedMove();
								}
							}
						}
					}
				}
			}
		}

		return animationName;
	}


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	PersonAnimationController::PersonAnimationController(qsf::Entity& callerEntity) :
		mEntity(callerEntity),
		mMovementState(MOVEMENTSTATE_UNDEFINED),
		mFormerMovementState(MOVEMENTSTATE_UNDEFINED),
		mPersonType(PERSONTYPE_OTHER),
		mIsSwimming(false)
	{
		// Get person type
		const CommandableComponent* commandableComponent = mEntity.getComponent<CommandableComponent>();
		if (commandableComponent != nullptr)
		{
			if (commandableComponent->isFireFighterDiver())
			{
				mPersonType = PERSONTYPE_DIVER;
			}
		}

		// Acquire the actually moving entity (relevant e.g. for paramedics team)
		checkMovingEntity();
	}

	PersonAnimationController::~PersonAnimationController()
	{
		// Nothing to do in here
	}

	void PersonAnimationController::updateAnimation()
	{
		// Get the movement state to apply
		const MovementState currentMovementState = calculateMovementState();

		bool updateNeeded = (mMovementState != currentMovementState);

		// Special handling for diver: Is he swimming now?
		// TODO(fw): Better place this in a method like "isUpdateNeeded" ?
		// TODO(fw): Would be nice if this was not necessary, but the diver logic signalled us to enforce an update
		if (mPersonType == PERSONTYPE_DIVER)
		{
			const qsf::GameLogicComponent& gameLogicComponent = mEntity.getOrCreateComponentSafe<qsf::GameLogicComponent>();
			const DiverLogic* diverLogic = gameLogicComponent.getGameLogic<DiverLogic>();
			if (nullptr != diverLogic)
			{
				if (diverLogic->isSwimming() != mIsSwimming)
				{
					mIsSwimming = !mIsSwimming;
					updateNeeded = true;
				}
			}
		}

		// Set the new movement state, if it has changed
		if (updateNeeded)
		{
			// Backup new state
			mFormerMovementState = mMovementState;
			mMovementState = currentMovementState;

			setAnimationState();
		}
	}

	void PersonAnimationController::resetAnimationState()
	{
		mMovementState = MOVEMENTSTATE_UNDEFINED;
	}

	void PersonAnimationController::forceIdleAnimationUpdate()
	{
		mFormerMovementState = mMovementState;
		mMovementState = MOVEMENTSTATE_IDLE;

		setAnimationState();
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	PersonAnimationController::MovementState PersonAnimationController::calculateMovementState()
	{
		// Check if person is injured
		//  -> We know it is when there is an injured action
		//  -> Note that there is the special case of plague infected persons, which can walk around sometimes (in ME102) - these don't have an injured animation
		const qsf::ActionComponent* actionComponent = mEntity.getComponent<qsf::ActionComponent>();
		if (actionComponent != nullptr)
		{
			const qsf::Action* currentAction = actionComponent->getCurrentAction();
			if (currentAction != nullptr && currentAction->getTypeId() == InjuredAction::ACTION_ID)
			{
				const qsf::LinkComponent* linkComponent = mEntity.getComponent<qsf::LinkComponent>();
				if (nullptr == linkComponent || nullptr == linkComponent->getParentPrototype() || linkComponent->getComponentFromEntityOrLinkedParent<BuryComponent>() != nullptr)
				{
					// Normal injured person
					return MOVEMENTSTATE_INJURED;
				}
			}
		}

		checkMovingEntity();

		// Keep current movement state bug take aware for the undefined state
		MovementState currentMovementState = mMovementState;
		float currentMovementSpeed = mMovableComponent->getSpeed();
		float currentForwardAcceleration = mMovableComponent->getForwardAcceleration();
		if (currentMovementSpeed < 0.0f)	// for MovementModes::MOVEMENT_MODE_WALK_BACKWARDS
		{
			currentMovementSpeed *= -1.0f;
			currentForwardAcceleration *= -1.0f;
		}

		const MovementModes::Modes movementmodeId = static_cast<MovementModes::Modes>(mNavigationComponent->getMovementModeId());
		const float maxSpeed = mNavigationComponent->getMaxForwardSpeed();
		const float idleThreshold = (mMovementState == MOVEMENTSTATE_IDLE ? 0.05f : 0.5f);
		const float runToIdleThreshold = movementmodeId == MovementModes::MOVEMENT_MODE_RUN ? 0.2f : 0.45f;
		const CommandableComponent* commandableComponent = mEntity.getComponent<CommandableComponent>();

		// TODO(fw): In some cases, the threshold of 0.6f is too high (sliding visible)
		//  -> Maybe we can solve this in a general way?

		if ((currentForwardAcceleration >= 0.0f && currentMovementSpeed > maxSpeed * idleThreshold) || currentMovementSpeed > maxSpeed * 0.9f)
		{
			currentMovementState = MOVEMENTSTATE_WALK;
		}
		// Check if person is stopping
		else
		{
			qsf::Action* currentAction = mActionComponent->getCurrentAction();
			if (nullptr == commandableComponent)
			{
				// TODO(sw) Check if this "simplification" for civilians doesn't have nasty side effects

				// For civilians
				if (nullptr != currentAction && (currentAction->getTypeId() == MoveAction::ACTION_ID || currentAction->getTypeId() == qsf::WaitAction::ACTION_ID))
				{
					// For civilian with a move action the check is easy -> speed = 0 or gets below the threshold => idle
					if (currentMovementSpeed <= 0.0f || currentMovementSpeed < maxSpeed * runToIdleThreshold)
					{
						currentMovementState = MOVEMENTSTATE_IDLE;
					}
				}
				else if (nullptr == currentAction)
				{
					// No action => idle
					currentMovementState = MOVEMENTSTATE_IDLE;
				}
			}
			else
			{
				const bool standing = (currentMovementSpeed < maxSpeed * 0.05f);
				const bool lowSpeed = (currentMovementSpeed < maxSpeed * runToIdleThreshold);

				if (lowSpeed && currentForwardAcceleration < 0.0f)
				{
					currentMovementState = MOVEMENTSTATE_IDLE;
				}
				else if (standing && currentForwardAcceleration == 0.0f && currentMovementState != MOVEMENTSTATE_UNDEFINED)
				{
					currentMovementState = MOVEMENTSTATE_IDLE;
				}
				// In case person has nothing to do, start idle
				else if (nullptr == currentAction)
				{
					currentMovementState = MOVEMENTSTATE_IDLE;
				}
			}
		}

		return currentMovementState;
	}

	bool PersonAnimationController::setInjuredAnimation()
	{
		// The collapse animations (comes from InjuredAction) are played and blocks the PersonAnimatinController update. Here we should have more detailed animation states

		// Has a parent (firefighter pulling or carrying the contaminated...)
		const qsf::LinkComponent* linkComponent = mEntity.getComponent<qsf::LinkComponent>();
		if (nullptr != linkComponent && nullptr != linkComponent->getParentPrototype() && linkComponent->getComponentFromEntityOrLinkedParent<BuryComponent>() == nullptr)
			return false;

		checkMovingEntity();

		const std::string injuredAnimationName = getInjuredAnimation(mEntity);
		if (!injuredAnimationName.empty())
		{
			// Play looped injury animation
			qsf::MeshAnimationComponent& meshAnimationComponent = mEntity.getOrCreateComponentSafe<qsf::MeshAnimationComponent>();
			const qsf::AssetProxy injuredAnimation(injuredAnimationName);
			if (!meshAnimationComponent.isAnimationPlaying(injuredAnimation))
			{
				InjuredAction* injuredAction = mActionComponent->getAction<InjuredAction>();

				if (mFormerMovementState == MOVEMENTSTATE_UNDEFINED && (injuredAction == nullptr || !injuredAction->getPlayCollapseAnimation()))
				{
					// No blending for persons with "preedited" injuries, like buried persons
					// Special case for the first animation, skip the blending for them
					qsf::MeshAnimationChannel* channel = meshAnimationComponent.playAnimation(injuredAnimation, true);

					if (nullptr != channel)
					{
						// Start at a random offset in animation
						channel->setOffset(qsf::Random::getRandomFloat(0.0f, 1.0f));
					}
				}
				else
				{
					meshAnimationComponent.blendToAnimation(injuredAnimation, true, qsf::Time::fromSeconds(0.5f));
				}
			}

			return true;
		}

		return false;
	}

	void PersonAnimationController::setAnimationState()
	{
		// Handle injured persons
		if (mMovementState == MOVEMENTSTATE_INJURED)
		{
			if (setInjuredAnimation())
				return;
		}

		// In case it is an enabled mesh animation test component, don't automatically animate
		//  -> This happens e.g. inside cutscenes with pre-edited persons that get animated by the cutscene
		const qsf::MeshAnimationComponent& meshAnimationComponent = mEntity.getOrCreateComponentSafe<qsf::MeshAnimationComponent>();
		if (meshAnimationComponent.getId() != qsf::MeshAnimationTestComponent::COMPONENT_ID || !static_cast<const qsf::MeshAnimationTestComponent&>(meshAnimationComponent).isTestComponentUpdatesEnabled())
		{
			EntityHelper entityHelper(mEntity);
			bool isInjured = entityHelper.isPersonInjured();
			if (isInjured)
			{
				qsf::ActionComponent* actionComponent = mEntity.getComponent<qsf::ActionComponent>();
				if (nullptr == actionComponent || nullptr == actionComponent->getAction<InjuredAction>())
				{
					// Person is injured, but can still walk
					//  -> This is a pretty nasty exception made infected persons in ME102
					isInjured = false;
				}
			}

			// We cannot early out, when person is injured, because for a contaminated person a movement animation must be set, when guided by a unit

			// Test if we have a special animation situation e.g. like the ambulance team
			const CommandableComponent* commandableComponent = mEntity.getComponent<CommandableComponent>();
			if (commandableComponent != nullptr && !isInjured)
			{
				// Only set unit specific movement animation, when unit is not injured

				if (commandableComponent->isAmbulanceParamedics())
				{
					setMovementAnimationParamedics();
					return;
				}
				else if (commandableComponent->isAmbulanceDoctor())
				{
					setMovementAnimationDoctor();
					return;
				}
				else if (commandableComponent->isFireFighterPerson() || commandableComponent->isFireFighterPersonHistoric())
				{
					setMovementAnimationFireFighter();
					return;
				}
				else if (mPersonType == PERSONTYPE_DIVER)
				{
					setMovementAnimationDiver();
					return;
				}
				else if (commandableComponent->isAmbulanceRescueDogLeader())
				{
					setMovementAnimationResueDogLeader();
					return;
				}
				else if (commandableComponent->isPolicePerson() || commandableComponent->isPoliceGuardsmanHistoric())
				{
					setMovementAnimationPolicePerson();
					return;
				}
				else if (commandableComponent->isPoliceSEKPerson())
				{
					setMovementAnimationSekSpecialist();
					return;
				}
				else if (commandableComponent->isPoliceDogLeader())
				{
					setMovementAnimationPoliceDogLeader();
					return;
				}
			}
			else
			{
				if (entityHelper.isArrested())
				{
					setMovementAnimationArrested();
					return;
				}
				else if (entityHelper.isPersonContaminated())
				{
					// Has a parent (firefighter pulling or carrying the contaminated)
					const qsf::LinkComponent* linkComponent = mEntity.getComponent<qsf::LinkComponent>();
					if (nullptr != linkComponent)
					{
						qsf::Prototype* parentPrototype = linkComponent->getParentPrototype();
						if (parentPrototype != nullptr)
						{
							// if carried (with lead)
							EntityHelper parentEntityHelper(*static_cast<qsf::Entity*>(parentPrototype));
							if (parentEntityHelper.isCarryingAnotherPerson())
							{
								setMovementContaminatedLead();
								return;
							}
						}
					}
				}
			}

			if (!isInjured)
			{
				// Only set "default" animation, when person not injured
				setMovementAnimationUnit();
			}
		}
	}

	void PersonAnimationController::setMovementAnimationParamedics()
	{
		const qsf::LinkComponent* linkComponent = mEntity.getOrCreateComponent<qsf::LinkComponent>();
		if (linkComponent != nullptr)
		{
			// We test here for tags but it is also possible to use the array position to find the right entity
			for (auto& childComponent: linkComponent->getChildLinks())
			{
				qsf::Entity& childEntity = childComponent->getEntity();

				std::string animationName;
				float animationBlendTime = 0.5f;

				const qsf::MetadataComponent* metadataComponent = childEntity.getOrCreateComponent<qsf::MetadataComponent>();
				if (metadataComponent->getTags() == "stretcher")
				{
					// TODO(ag) Maybe something happens here in future
				}
				else if (metadataComponent->getTags() == "first_paramedic")
				{
					switch (mMovementState)
					{
						case MOVEMENTSTATE_IDLE:
							animationName = AnimationHelper(childEntity).getAnimationParamedicIdleFront();
							break;

						case MOVEMENTSTATE_WALK:
							animationName = AnimationHelper(childEntity).getAnimationParamedicRunFront();
							animationBlendTime = 0.4f;
							break;

						case MOVEMENTSTATE_UNDEFINED:
							// Nothing to do in this case
							break;
					}
				}
				else if (metadataComponent->getTags() == "second_paramedic")
				{
					switch (mMovementState)
					{
						case MOVEMENTSTATE_IDLE:
							animationName = AnimationHelper(childEntity).getAnimationParamedicIdleBack();
							break;

						case MOVEMENTSTATE_WALK:
							animationName = AnimationHelper(childEntity).getAnimationParamedicRunBack();
							animationBlendTime = 0.4f;
							break;

						case MOVEMENTSTATE_UNDEFINED:
							// Nothing to do in this case
							break;
					}
				}

				blendToAnimation(childEntity, animationName, animationBlendTime);
			}
		}
	}

	void PersonAnimationController::setMovementAnimationDoctor()
	{
		std::string animationName;
		static const float animationBlendTime = 0.4f;	// TODO(mk): copy-paste blending time, no fine-tuning from me

		EntityHelper entityHelper(mEntity);
		// Leading contaminated?
		if (entityHelper.isCarrying(EntityHelper::CARRY_PERSON))
		{
			qsf::Entity* carriedEntity = mEntity.getMap().getEntityById(entityHelper.getCarryTarget(EntityHelper::CARRY_PERSON));
			if (carriedEntity != nullptr && EntityHelper(*carriedEntity).isPersonContaminated())
			{
				if (mMovementState == MOVEMENTSTATE_WALK)
				{
					animationName = AnimationHelper(mEntity).getAnimationFiremanLeadContaminatedLoop();
				}
				else
				{
					animationName = AnimationHelper(mEntity).getAnimationFiremanIdleContaminatedLoop();
				}
			}
		}

		if (!blendToAnimation(mEntity, animationName, animationBlendTime))
		{
			setMovementAnimationUnit();
		}
	}

	void PersonAnimationController::setMovementAnimationFireFighter()
	{
		std::string animationName;
		static const float animationBlendTime = 0.4f;	// TODO(mz): copy-paste blending time, no fine-tuning from me

		mEntity.getOrCreateComponentSafe<qsf::GameLogicComponent>();

		// Check if unit is pulling someONE
		EntityHelper entityHelper(mEntity);
		if (entityHelper.isCarrying(EntityHelper::PULL_PERSON))
		{
			if (mMovementState == MOVEMENTSTATE_WALK)
			{
				animationName = AnimationHelper(mEntity).getAnimationFiremanPullLoop();
			}
			else
			{
				animationName = AnimationHelper(mEntity).getAnimationFiremanPullIdleLoop();
			}
		}
		// Leading someone? (In ME310, it's not necessarily a contaminated person)
		else if (entityHelper.isCarrying(EntityHelper::CARRY_PERSON))
		{
			qsf::Entity* carriedEntity = mEntity.getMap().getEntityById(entityHelper.getCarryTarget(EntityHelper::CARRY_PERSON));
			if (carriedEntity != nullptr)
			{
				if (mMovementState == MOVEMENTSTATE_WALK)
				{
					animationName = AnimationHelper(mEntity).getAnimationFiremanLeadContaminatedLoop();
				}
				else
				{
					animationName = AnimationHelper(mEntity).getAnimationFiremanIdleContaminatedLoop();
				}
			}
		}
		else
		{
			// Or maybe the unit is pulling someTHING
			if (entityHelper.isCarrying(EntityHelper::PULL_OBJECT))
			{
				// No idling - next time he stops the object carried is dropped
				if (mMovementState == MOVEMENTSTATE_WALK)
				{
					animationName = AnimationHelper(mEntity).getAnimationFiremanPullObjectLoop();
				}
				else
				{
					// No idling - next time the unit stops the object carried is dropped,
					// but also don't allow setting another animation for idle
					return;
				}
			}
		}

		if (!blendToAnimation(mEntity, animationName, animationBlendTime))
		{
			setMovementAnimationUnit();
		}
	}

	void PersonAnimationController::setMovementAnimationDiver()
	{
		std::string animationName;
		float animationBlendTime = 0.4f;

		// Is diver on ground where he should walk (can be in shallow water) and is he carrying someone?
		bool isSwimming = false;
		bool isCarryingPerson = false;

		// Diver logic knows it all
		const qsf::GameLogicComponent& gameLogicComponent = mEntity.getOrCreateComponentSafe<qsf::GameLogicComponent>();
		const DiverLogic* diverLogic = gameLogicComponent.getGameLogic<DiverLogic>();
		if (nullptr != diverLogic)
		{
			isSwimming = diverLogic->isSwimming();
			isCarryingPerson = diverLogic->isCarryingPerson();
		}

		// Is the diver carrying a person?
		if (isCarryingPerson)
		{
			if (isSwimming)
			{
				animationName = AnimationHelper(mEntity).getAnimationRescueDrowningWater();
			}
			else
			{
				animationName = AnimationHelper(mEntity).getAnimationRescueDrowningLand();

				if (nullptr != diverLogic)
				{
					// Give carried person the right animation
					qsf::Entity* carriedPerson = mEntity.getMap().getEntityById(diverLogic->getCarryPerson());
					if (nullptr != carriedPerson)
					{
						blendToAnimation(*carriedPerson, AnimationHelper(*carriedPerson).getAnimationInjuredPulledLoop(), 0.4f);
					}
				}
			}
		}
		// Moving around all alone
		else
		{
			switch (mMovementState)
			{
				case MOVEMENTSTATE_IDLE:
					if (isSwimming)
						animationName = AnimationHelper(mEntity).getAnimationDiverIdleWater();
					else
						animationName = AnimationHelper(mEntity).getAnimationDiverIdleLand();
					break;

				case MOVEMENTSTATE_WALK:
					if (isSwimming)
						animationName = AnimationHelper(mEntity).getAnimationDiverSwimming();
					else if (mNavigationComponent->getMaxForwardSpeed() >= 3.0f)		// Special handling for the faster diver in EM 20 years
						animationName = AnimationHelper(mEntity).getWalkAnimation();
					else
						animationName = AnimationHelper(mEntity).getAnimationDiverWalking();
					break;

				case MOVEMENTSTATE_UNDEFINED:
					// Nothing to do in this case
					break;
			}
		}

		blendToAnimation(mEntity, animationName, animationBlendTime);
	}

	void PersonAnimationController::setMovementAnimationResueDogLeader()
	{
		// Set the movement animation for the rescue dog, combined with the leader (if not combined, the rescue dog get an own update call)
		const qsf::LinkComponent* linkComponent = mEntity.getOrCreateComponent<qsf::LinkComponent>();
		if (linkComponent != nullptr)
		{
			for (auto& childComponent: linkComponent->getChildLinks())
			{
				qsf::Entity& childEntity = childComponent->getEntity();

				PersonComponent* personComponent = childEntity.getComponent<PersonComponent>();
				if (nullptr == personComponent)
					continue;

				if (personComponent->getGender() != PersonComponent::GENDER_ANIMAL)
					continue;

				// Set animation
				std::string animationName;
				AnimationHelper::AnimationMaskData animationMaskData;
				switch (mMovementState)
				{
					case MOVEMENTSTATE_IDLE:
						animationName = AnimationHelper(childEntity).getIdleAnimation(animationMaskData);
						break;

					case MOVEMENTSTATE_WALK:
						animationName = AnimationHelper(childEntity).getRunAnimation(animationMaskData);
						break;

					case MOVEMENTSTATE_UNDEFINED:
						// Nothing to do in this case
						break;
				}

				blendToAnimation(childEntity, animationName, 0.4f);
			}
		}

		// Movement animation for rescuedog leader
		setMovementAnimationUnit();
	}

	void PersonAnimationController::setMovementAnimationPolicePerson()
	{
		std::string animationName;
		float animationBlendTime = 0.5f;
		const EntityHelper entityHelper(mEntity);

		// Carrying someone?
		if (entityHelper.isCarrying(EntityHelper::CARRY_PERSON))
		{
			// Currently the only reason police units carry someone is that they have arrested them
			switch (mMovementState)
			{
				case MOVEMENTSTATE_IDLE:
					animationName = AnimationHelper(mEntity).getAnimationPoliceLeadArrestedIdle();
					break;

				case MOVEMENTSTATE_WALK:
					animationName = AnimationHelper(mEntity).getAnimationPoliceLeadArrestedLoop();
					animationBlendTime = 0.4f;
					break;

				case MOVEMENTSTATE_UNDEFINED:
					// Nothing to do in this case
					break;
			}
		}
		// Active megaphone?
		else if (entityHelper.checkIsActiveEquipment(equipment::POLICE_MEGAPHONE))
		{
			const qsf::Entity* equipmentEntity = entityHelper.getActiveEquipmentEntity();
			if (nullptr != equipmentEntity)
			{
				// Magnet component available and enabled?
				const PersonMagnetComponent* personMagnetComponent = equipmentEntity->getComponent<PersonMagnetComponent>();
				if (nullptr != personMagnetComponent && personMagnetComponent->isMagnetEnabled())
				{
					switch (mMovementState)
					{
						case MOVEMENTSTATE_IDLE:
							animationName = AnimationHelper(mEntity).getAnimationPoliceMegaphoneIdleLoop();
							break;

						case MOVEMENTSTATE_WALK:
							animationName = AnimationHelper(mEntity).getAnimationPoliceMegaphoneWalkingLoop();
							animationBlendTime = 0.4f;
							break;

						case MOVEMENTSTATE_UNDEFINED:
							// Nothing to do in this case
							break;
					}
				}
			}
		}
		// Building the tape barrier?
		else if (entityHelper.checkIsActiveEquipment(equipment::POLICE_BARRIER_TAPE_ROLL))
		{
			const PlaceBarrierPoleAction* barrierPoleAction = mEntity.getOrCreateComponentSafe<qsf::ActionComponent>().getAction<PlaceBarrierPoleAction>();
			if (nullptr != barrierPoleAction && barrierPoleAction->getActionState() == PlaceBarrierPoleAction::STATE_DROP_POLE1 && mMovementState == MOVEMENTSTATE_WALK)
			{
				animationName = AnimationHelper(mEntity).getAnimationPoliceBarrierTapeLoop();
			}
		}

		if (!blendToAnimation(mEntity, animationName, animationBlendTime))
		{
			setMovementAnimationUnit();
		}
	}

	void PersonAnimationController::setMovementAnimationSekSpecialist()
	{
		const EntityHelper entityHelper(mEntity);

		std::string animationName;
		float animationBlendTime = 0.4f;	// TODO(mz) I guess value should depend on the previous animation. I just noticed if it's too long the animation ends up stuck halfways in some cases.

		if (entityHelper.checkIsActiveEquipment(equipment::POLICE_SEK_PISTOL) || entityHelper.checkIsActiveEquipment(equipment::POLICE_PISTOL))
		{
			// Walking/idling with pistol
			switch (mMovementState)
			{
				case MOVEMENTSTATE_IDLE:
					animationName = AnimationHelper(mEntity).getAnimationSEKSpecialistIdlePistol();
					break;

				case MOVEMENTSTATE_WALK:
					animationName = AnimationHelper(mEntity).getAnimationSEKSpecialistRunPistol02Loop();
					break;

				case MOVEMENTSTATE_UNDEFINED:
					// Nothing to do in this case
					break;
			}
		}
		else if (entityHelper.checkIsActiveEquipment(equipment::POLICE_SNIPERRIFLE))
		{
			// Walking/idling with rifle
			switch (mMovementState)
			{
				case MOVEMENTSTATE_IDLE:
					animationName = AnimationHelper(mEntity).getAnimationSEKSpecialistIdleRifle();
					break;

				case MOVEMENTSTATE_WALK:
					animationName = AnimationHelper(mEntity).getAnimationSEKSpecialistRunRifle02Loop();
					break;

				case MOVEMENTSTATE_UNDEFINED:
					// Nothing to do in this case
					break;
			}
		}
		// Carrying someone?
		else if (EntityHelper(mEntity).isCarrying(EntityHelper::CARRY_PERSON))
		{
			// Currently the only reason sek-units carry someone is that they have arrested them
			switch (mMovementState)
			{
				case MOVEMENTSTATE_IDLE:
					animationName = AnimationHelper(mEntity).getAnimationSEKSpecialistLeadArrestedIdle();
					break;

				case MOVEMENTSTATE_WALK:
					animationName = AnimationHelper(mEntity).getAnimationSEKSpecialistLeadArrestedLoop();
					animationBlendTime = 0.4f;
					break;

				case MOVEMENTSTATE_UNDEFINED:
					// Nothing to do in this case
					break;
			}
		}
		else if (mMovementState == MOVEMENTSTATE_IDLE)
		{
			// Yeah, this unit got its own idle. Though hard to tell if this should be here or in AnimationHelper::getIdleAnimation.
			animationName = AnimationHelper(mEntity).getAnimationSEKSpecialistIdle();
		}

		if (!blendToAnimation(mEntity, animationName, animationBlendTime))
		{
			setMovementAnimationUnit();
		}
	}

	void PersonAnimationController::setMovementAnimationPoliceDogLeader()
	{
		std::string animationName;
		float animationBlendTime = 0.5f;
		const EntityHelper entityHelper(mEntity);

		// Carrying someone?
		if (entityHelper.isCarrying(EntityHelper::CARRY_PERSON))
		{
			// Currently the only reason police units carry someone is that they have arrested them
			switch (mMovementState)
			{
				case MOVEMENTSTATE_IDLE:
					animationName = AnimationHelper(mEntity).getAnimationPoliceLeadArrestedIdle();
					break;

				case MOVEMENTSTATE_WALK:
					animationName = AnimationHelper(mEntity).getAnimationPoliceLeadArrestedLoop();
					animationBlendTime = 0.4f;
					break;

				case MOVEMENTSTATE_UNDEFINED:
					// Nothing to do in this case
					break;
			}
		}

		if (!blendToAnimation(mEntity, animationName, animationBlendTime))
		{
			setMovementAnimationUnit();
		}
	}

	void PersonAnimationController::setMovementAnimationArrested()
	{
		std::string animationName;
		float animationBlendTime = 0.5f;

		switch (mMovementState)
		{
			case MOVEMENTSTATE_IDLE:
				animationName = AnimationHelper(mEntity).getAnimationGangsterIdleArrested();
				break;

			case MOVEMENTSTATE_WALK:
				animationName = AnimationHelper(mEntity).getAnimationGangsterWalkArrestedLoop();
				animationBlendTime = 0.4f;
				break;

			case MOVEMENTSTATE_UNDEFINED:
				// Nothing to do in this case
				break;
		}

		if (!blendToAnimation(mEntity, animationName, animationBlendTime))
		{
			setMovementAnimationUnit();
		}
	}

	void PersonAnimationController::setMovementContaminatedLead()
	{
		std::string animationName;
		float animationBlendTime = 0.5f;

		switch (mMovementState)
		{
			case MOVEMENTSTATE_IDLE:
				animationName = AnimationHelper(mEntity).getAnimationContaminatedIdleLoop();
				break;

			case MOVEMENTSTATE_WALK:
				animationName = AnimationHelper(mEntity).getAnimationContaminatedWalkLoop();
				animationBlendTime = 0.4f;
				break;

			case MOVEMENTSTATE_UNDEFINED:
				// Nothing to do in this case
				break;
		}

		if (!blendToAnimation(mEntity, animationName, animationBlendTime))
		{
			setMovementAnimationUnit();
		}
	}

	void PersonAnimationController::setMovementAnimationUnit()
	{
		// TODO(fw): Could we merge this code with the paramedic animation?
		qsf::MeshAnimationComponent& meshAnimationComponent = mEntity.getOrCreateComponentSafe<qsf::MeshAnimationComponent>();

		AnimationHelper::AnimationMaskData animationMaskData;
		std::string animationName;
		float animationBlendTime = 0.5f;
		switch (mMovementState)
		{
			case MOVEMENTSTATE_IDLE:
				animationName = AnimationHelper(mEntity).getIdleAnimation(animationMaskData);
				break;

			case MOVEMENTSTATE_WALK:
				animationName = AnimationHelper(mEntity).getRunAnimation(animationMaskData);
				animationBlendTime = 0.4f;
				break;

			case MOVEMENTSTATE_UNDEFINED:
				// Nothing to do in this case
				break;
		}

		const qsf::AssetProxy walkAnimation(animationName);
		if (animationMaskData.mMaskAnimation.isValid())
		{
			// Use mask animation
			std::vector<std::string> boneNames(1, "Bip01_Neck");
			meshAnimationComponent.blendToMaskedAnimation(animationMaskData.mMaskAnimation, walkAnimation, boneNames, animationMaskData.mLoopAnimation, qsf::Time::fromSeconds(animationBlendTime));
		}
		else
		{
			// Use normal animation
			qsf::MeshAnimationChannel* channel = meshAnimationComponent.blendToAnimation(walkAnimation, true, qsf::Time::fromSeconds(animationBlendTime));
			QSF_ASSERT(channel != nullptr, "PersonAnimationController::setMovementAnimationUnit() could not blend to animation: " << animationName, QSF_REACT_NONE);
		}
	}

	bool PersonAnimationController::blendToAnimation(qsf::Entity& entity, const std::string& animationName, float animationBlendTime) const
	{
		if (!animationName.empty())
		{
			qsf::MeshAnimationComponent& meshAnimationComponent = entity.getOrCreateComponentSafe<qsf::MeshAnimationComponent>();
			const qsf::AssetProxy animationProxy(animationName);
			qsf::MeshAnimationChannel* channel = meshAnimationComponent.getAnimationChannel(animationProxy);
			if (channel == nullptr || channel->getWeightDelta() < 0.0f)
			{
				qsf::MeshAnimationChannel* channel = meshAnimationComponent.blendToAnimation(animationProxy, true, qsf::Time::fromSeconds(animationBlendTime));
				QSF_ASSERT(channel != nullptr, "PersonAnimationController::blendToAnimation() could not blend to animation: " << animationName, return false);
			}
			return true;
		}
		return false;
	}

	void PersonAnimationController::checkMovingEntity()
	{
		// For linked entities we are interested in the movement of the parent
		qsf::Entity* movingEntity = &mEntity;
		const qsf::LinkComponent* linkComponent = mEntity.getComponent<qsf::LinkComponent>();
		if (nullptr != linkComponent)
		{
			qsf::Prototype* parentPrototype = linkComponent->getParentPrototype();
			if (parentPrototype != nullptr)
			{
				// Exclude parent entities that are buildings
				//  -> We also check for a building light timer, as buildings in border area may have their building component removed (see "em5::MapHelper::prepareMapBorderLayer")
				if (nullptr == parentPrototype->getComponentById("em5::BuildingComponent") &&
					nullptr == parentPrototype->getComponentById("em5::BuildingLightTimeControlComponent"))
				{
					movingEntity = static_cast<qsf::Entity*>(parentPrototype);
				}
			}
		}

		const bool forceNewComponents = (mMovingEntity.get() != movingEntity);
		mMovingEntity = movingEntity;

		// Update component pointers
		if (forceNewComponents || !mMovableComponent.valid())
		{
			mMovableComponent = movingEntity->getOrCreateComponentSafe<qsf::MovableComponent>();
		}
		if (forceNewComponents || !mNavigationComponent.valid())
		{
			mNavigationComponent = movingEntity->getOrCreateComponentSafe<qsf::ai::NavigationComponent>();
		}
		if (forceNewComponents || !mActionComponent.valid())
		{
			mActionComponent = movingEntity->getOrCreateComponent<qsf::ActionComponent>();
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

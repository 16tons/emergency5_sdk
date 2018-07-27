// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/action/ambulance/ExitRescueHeliAction.h"
#include "em5/action/move/LinearMoveAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/base/GameplayAssets.h"
#include "em5/component/vehicle/HelicopterComponent.h"
#include "em5/component/effect/FadeEffectComponent.h"
#include "em5/command/component/CommandableComponent.h"
#include "em5/game/animation/AnimationHelper.h"
#include "em5/game/targetpoint/EnterRescueHeliTargetPointProvider.h"
#include "em5/map/CollisionHelper.h"
#include "em5/map/EntityHelper.h"
#include "em5/map/MapHelper.h"

#include <qsf_game/component/base/HiddenComponent.h>

#include <qsf/logic/action/ActionComponent.h>
#include <qsf/component/link/LinkComponent.h>
#include <qsf/component/move/MovableComponent.h>
#include <qsf/component/base/MetadataComponent.h>
#include <qsf/component/base/TransformComponent.h>
#include <qsf/link/link/prototype/ContainerLink.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const qsf::NamedIdentifier ExitRescueHeliAction::ACTION_ID	= "em5::ExitRescueHeliAction";


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	ExitRescueHeliAction::ExitRescueHeliAction() :
		ExitVehicleAction(ACTION_ID),
		mGeneralState(GENREALSTATE_INIT),
		mSubState(STATE_INIT),
		mUnloadDoctor(false),
		mUnloadParamedics(false),
		mEntityIdToExit(qsf::getUninitialized<uint64>())
	{
		// Nothing here
	}

	ExitRescueHeliAction::ExitRescueHeliAction(uint32 actionID) :
		ExitVehicleAction(actionID),
		mGeneralState(GENREALSTATE_INIT),
		mSubState(STATE_INIT),
		mUnloadDoctor(false),
		mUnloadParamedics(false),
		mEntityIdToExit(qsf::getUninitialized<uint64>())
	{
		// Nothing here
	}

	ExitRescueHeliAction::~ExitRescueHeliAction()
	{
		deleteOpticalEntities();
	}

	void ExitRescueHeliAction::init(uint64 personIdToExit)
	{
		mEntityIdToExit = personIdToExit;

		// Init the base class
		ExitVehicleAction::init(personIdToExit);
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Actions methods                   ]
	//[-------------------------------------------------------]
	void ExitRescueHeliAction::serialize(qsf::BinarySerializer& serializer)
	{
		// TODO(mk) To the serialization
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	bool ExitRescueHeliAction::onStartup()
	{
		mHelicopterComponent = getEntity().getComponent<HelicopterComponent>();
		if (!mHelicopterComponent.valid())
			return false;

		if (mHelicopterComponent->isFlying())
			return false;

		// Call base class (need it because there variables to initialize, ignore return value)
		ExitVehicleAction::onStartup();

		return true;
	}

	qsf::action::Result ExitRescueHeliAction::updateAction(const qsf::Clock& clock)
	{
		// Here we have two separate processes, happen at the same time. Both of them are optional, it depends on the passenger inside the rescue helicopter.
		// 1. Process is the exit from the doctor (if available) from the co driver door. This is handled by the ExitVehicle base action.
		// 2. It the process of the paramedic team, both persons are handled via an separate animation and combined to an paramedic team.

		switch (mGeneralState)
		{
			case GENREALSTATE_INIT:
			{
				detectStartingPersonal();

				mGeneralState = GENREALSTATE_UPDATE;

				// Fall through by design
			}

			case GENREALSTATE_UPDATE:
			{
				qsf::action::Result baseActionResult = qsf::action::RESULT_DONE;
				qsf::action::Result actionResult = qsf::action::RESULT_DONE;

				// Update the base action
				if (mUnloadDoctor)
				{
					// Call the base class implementation
					baseActionResult = ExitVehicleAction::updateAction(clock);
				}
				if (mUnloadParamedics)
				{
					// Handle the paramedic actions
					actionResult = updateSubAction(clock);
				}

				// Detect ending
				if (baseActionResult != qsf::action::RESULT_DONE ||
					actionResult != qsf::action::RESULT_DONE)
				{
					// At least one action needs to be updated
					return qsf::action::RESULT_CONTINUE;
				}

				mGeneralState = GENREALSTATE_DONE;

				// Fall through by design
			}

			case GENREALSTATE_DONE:
			{
				// Fall through by design
			}
		}

		// Wait for ending of the animation
		return qsf::action::RESULT_DONE;
	}


	//[-------------------------------------------------------]
	//[ Protected                                             ]
	//[-------------------------------------------------------]
	void ExitRescueHeliAction::createOpticalEntities(qsf::Entity& helicopterEntity, bool playAnimationReverse)
	{
		qsf::TransformComponent& transformComponent = helicopterEntity.getOrCreateComponentSafe<qsf::TransformComponent>();
		const glm::vec3& helicopterPosition = transformComponent.getPosition();
		const glm::quat& helicoterRotation = transformComponent.getRotation();

		// Create the two new guys and the stretcher at helicopter position
		// We use here the map helper, because these entities are relevant for multiplay ;)
		MapHelper mapHelper(getMap());

		mStretcherEntity = mapHelper.createObjectByLocalPrefabAssetId(assets::PREFAB_AMBULANCE_STRETCHER);
		if (mStretcherEntity.valid())
		{
			EntityHelper::setupEntityAsGhost(mStretcherEntity.getSafe(), getEntity());
			mStretcherEntity->getOrCreateComponentSafe<qsf::TransformComponent>().setPositionAndRotation(helicopterPosition, helicoterRotation);
			AnimationHelper(mStretcherEntity.getSafe()).playAnimation("em5/skeleton/person/rescueheli_stretcher_leave", false, playAnimationReverse);
			if ((mHelicopterComponent.valid() && EntityHelper(mHelicopterComponent->getEntity()).isEntityVisibleOnScreen()))
			{
				// Avoid visual one frame glitches
				mStretcherEntity->getOrCreateComponentSafe<qsf::MeshAnimationComponent>().enforceGraphicsSynchronization();
			}
		}

		mParamedicFrontEntity = mapHelper.createObjectByLocalPrefabAssetId(assets::PREFAB_SINGLE_PARAMEDIC);
		if (mParamedicFrontEntity.valid())
		{
			EntityHelper::setupEntityAsGhost(mParamedicFrontEntity.getSafe(), getEntity());
			std::string animationName = "em5/skeleton/person/rescueheli_sani_01_leave";
			if (playAnimationReverse)
			{
				animationName = "em5/skeleton/person/rescueheli_sani_01_enter";
			}
			setupParamedicForLeaving(mParamedicFrontEntity.getSafe(), animationName, !playAnimationReverse, false, transformComponent);
		}

		mParamedicBackEntity = mapHelper.createObjectByLocalPrefabAssetId(assets::PREFAB_SINGLE_PARAMEDIC);
		if (mParamedicBackEntity.valid())
		{
			EntityHelper::setupEntityAsGhost(mParamedicBackEntity.getSafe(), getEntity());
			setupParamedicForLeaving(mParamedicBackEntity.getSafe(), "em5/skeleton/person/rescueheli_sani_02_leave", !playAnimationReverse, playAnimationReverse, transformComponent);
		}
	}

	void ExitRescueHeliAction::deleteOpticalEntities()
	{
		// Cleanup our created entities.
		// We use here also the map helper, because these entities are relevant for multiplay ;)
		MapHelper mapHelper(getMap());
		if (mParamedicFrontEntity.valid())
		{
			mapHelper.destroyEntityById(mParamedicFrontEntity->getId());
		}

		if (mParamedicBackEntity.valid())
		{
			mapHelper.destroyEntityById(mParamedicBackEntity->getId());
		}

		if (mStretcherEntity.valid())
		{
			mapHelper.destroyEntityById(mStretcherEntity->getId());
		}
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void ExitRescueHeliAction::detectStartingPersonal()
	{
		// Special case for single person exit
		if (qsf::isInitialized(mEntityIdToExit))
		{
			qsf::Entity* entityToExit = getMap().getEntityById(mEntityIdToExit);
			if (nullptr != entityToExit)
			{
				if (EntityHelper(*entityToExit).getParentEntity() == &getEntity())
				{
					CommandableComponent* commandableComponent = entityToExit->getComponent<CommandableComponent>();
					if (nullptr != commandableComponent)
					{
						if (commandableComponent->isAmbulanceParamedics())
						{
							mParamedicTeam = entityToExit;
							mUnloadParamedics = true;
							return;
						}
						else
						{
							mUnloadDoctor = true;
							return;
						}
					}
				}
			}
		}

		for (auto iterator : mHelicopterComponent->getLinks())
		{
			// Check if we found our units
			if (mUnloadParamedics && mUnloadDoctor)
				break;

			qsf::Entity& linkedEntity = (*iterator.second).getTargetEntity();
			if (!EntityHelper(linkedEntity).isEntityPerson())
			{
				// An other child entity, maybe light or something else
				continue;
			}

			CommandableComponent* commandableComponent = linkedEntity.getComponent<CommandableComponent>();
			if (nullptr != commandableComponent && commandableComponent->isAmbulanceParamedics())
			{
				// Found paramedic
				mParamedicTeam = &linkedEntity;
				mUnloadParamedics = true;
				continue;
			}

			// Found some one else, must be a doctor.
			// Here we act like it is a doctor, we also support other kind of units, because they use the normal "exit from vehicle" routine.
			// For easier naming we call this unit(s) doctor
			mUnloadDoctor = true;
		}
	}

	qsf::action::Result ExitRescueHeliAction::updateSubAction(const qsf::Clock&)
	{
		switch (mSubState)
		{
			case STATE_INIT:
			{
				prepareParamedic();
				createOpticalEntities(getEntity());
				mHelicopterComponent->openDoor(DoorComponent::DOORTYPE_TRUNK, true);
				mHelicopterComponent->openDoor(DoorComponent::DOORTYPE_PASSENGER, true);

				mSubState = STATE_UPDATE;
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_UPDATE:
			{
				if (mHelicopterComponent->areAllDoorsOfTypeInState(DoorComponent::DOORTYPE_PASSENGER, DoorComponent::DOOR_OPEN))
				{
					// Close door after it is open
					mHelicopterComponent->openDoor(DoorComponent::DOORTYPE_PASSENGER, false);
				}

				if ((mParamedicBackEntity.valid() && AnimationHelper(mParamedicBackEntity.getSafe()).isAnimationPlaying()) ||
					(mParamedicFrontEntity.valid() && AnimationHelper(mParamedicFrontEntity.getSafe()).isAnimationPlaying()) ||
					(mStretcherEntity.valid() && AnimationHelper(mStretcherEntity.getSafe()).isAnimationPlaying()))
				{
					// Wait till the animation is finished
					return qsf::action::RESULT_CONTINUE;
				}

				deleteOpticalEntities();

				// Show the final paramedic
				qsf::game::HiddenComponent::setHidden(mParamedicTeam.getSafe(), false);
				mHelicopterComponent->openDoor(DoorComponent::DOORTYPE_TRUNK, false);

				// Avoid visual one frame glitches
				if (mParamedicTeam.valid() && EntityHelper(getEntity()).isEntityVisibleOnScreen())
				{
					const qsf::LinkComponent* linkComponent = mParamedicTeam->getOrCreateComponent<qsf::LinkComponent>();
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
								if (metadataComponent->getTags() == "first_paramedic" || metadataComponent->getTags() == "second_paramedic")
								{
									qsf::MeshAnimationComponent* meshAnimationComponent = childEntity.getComponent<qsf::MeshAnimationComponent>();
									if (nullptr != meshAnimationComponent)
									{
										meshAnimationComponent->enforceGraphicsSynchronization();
									}
								}
							}
						}
					}
				}

				mSubState = STATE_LINEAR_MOVE;
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_LINEAR_MOVE:
			{
				// Check if the target position is free
				const glm::vec3& spawnPosition = EntityHelper(mParamedicTeam.getSafe()).getPosition();
				glm::vec3 freeTarget = spawnPosition;

				{
					const CollisionHelper collisionHelper(getMap());
					if (!collisionHelper.findFreePositionEM3(mParamedicTeam.getSafe(), spawnPosition, 2.0f, 0.1f, freeTarget))
					{
						collisionHelper.findFreePositionEM3(mParamedicTeam.getSafe(), spawnPosition, 5.0f, 0.1f, freeTarget);
					}
				}

				// Lets move to the free position
				if (freeTarget != spawnPosition)
				{
					qsf::ActionComponent& actionComponent = mParamedicTeam.getSafe().getOrCreateComponentSafe<qsf::ActionComponent>();
					actionComponent.pushAction<LinearMoveAction>(action::BLOCKING, qsf::action::INSERT_AT_FRONT).init(freeTarget);
				}

				mSubState = STATE_DONE;
				// Fall through by design
			}

			case STATE_DONE:
			{
				// Finish routine
				return qsf::action::RESULT_DONE;
			}
		}

		// Error
		return qsf::action::RESULT_DONE;
	}

	void ExitRescueHeliAction::prepareParamedic()
	{
		// Place the paramedic on the final position and hide him
		EntityHelper(mParamedicTeam.getSafe()).leaveContainer(false);

		const qsf::TransformComponent& transformComponent = getEntity().getOrCreateComponentSafe<qsf::TransformComponent>();
		const qsf::Transform& callerTransform = transformComponent.getTransform();
		qsf::TransformComponent& paramedicTransformComponent = mParamedicTeam->getOrCreateComponentSafe<qsf::TransformComponent>();

		// Set position and rotation
		paramedicTransformComponent.setPositionAndRotation(callerTransform.vec3PositionLocalToWorld(EnterRescueHeliTargetPointProvider::FINAL_PARAMEDIC_OFFSET),
														   callerTransform.quatRotationLocalToWorld(EnterRescueHeliTargetPointProvider::FINAL_PARAMEDIC_ROTATION));

		// After entering an vehicle the entity is "faded out". To make it visible again we have to fade in again. We don't want to see the effect here, but need the result from the material changes.
		mParamedicTeam->getOrCreateComponentSafe<FadeEffectComponent>().fadeIn(qsf::Time::ZERO);

		// Ensure the gameplay paramedic team is invisible at this point in time, the animation is seen instead
		qsf::game::HiddenComponent::setHidden(mParamedicTeam.getSafe(), true);
	}

	void ExitRescueHeliAction::setupParamedicForLeaving(qsf::Entity& entity, const std::string& animationName, bool fadeIn, bool playAnimationReverse, const qsf::TransformComponent& helicopterTransformComponent)
	{
		// No AI movement please
		EntityHelper(entity).makeImmovable();
		entity.destroyComponentById(qsf::StringHash("em5::CommandableComponent"));

		// Destroy person component -> Without this the leave animation won't be played...
		entity.destroyComponentById(qsf::StringHash("em5::PersonComponent"));

		{ // Set transform
			qsf::MovableComponent* movableComponent = entity.getComponent<qsf::MovableComponent>();
			if (nullptr != movableComponent)
			{
				movableComponent->warpToPositionAndRotation(helicopterTransformComponent.getPosition(), helicopterTransformComponent.getRotation());
			}
			else
			{
				entity.getOrCreateComponentSafe<qsf::TransformComponent>().setPositionAndRotation(helicopterTransformComponent.getPosition(), helicopterTransformComponent.getRotation());
			}
		}

		if (fadeIn)
		{
			entity.getOrCreateComponentSafe<FadeEffectComponent>().fadeIn();
		}

		// Play animation
		// TODO(co) The "!playAnimationReverse && "-part is a hack: For some reason the one parametric won't be animated else. So still a glitch, but only 1/3 of it and hence less visible.
		AnimationHelper(entity).playAnimation(animationName, false, playAnimationReverse);
		if (!playAnimationReverse && (mHelicopterComponent.valid() && EntityHelper(mHelicopterComponent->getEntity()).isEntityVisibleOnScreen()))
		{
			// Avoid visual one frame glitches
			entity.getOrCreateComponentSafe<qsf::MeshAnimationComponent>().enforceGraphicsSynchronization();
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/action/gangster/ThrowWeaponAction.h"
#include "em5/action/gangster/BeThrownWeaponAction.h"
#include "em5/action/base/PlayAnimationAction.h"
#include "em5/action/move/TurnToAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/component/event/EventIdComponent.h"
#include "em5/component/equipment/InventoryComponent.h"
#include "em5/game/animation/AnimationHelper.h"
#include "em5/logic/local/gangsters/GangsterBaseLogic.h"
#include "em5/map/EntityHelper.h"

#include <qsf_game/base/DistanceHelper.h>
#include <qsf_game/component/base/BoneLinkComponent.h>

#include <qsf/logic/action/ActionComponent.h>
#include <qsf/logic/gamelogic/GameLogicComponent.h>
#include <qsf/component/base/TransformComponent.h>
#include <qsf/map/Map.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const qsf::NamedIdentifier ThrowWeaponAction::ACTION_ID = "em5::ThrowWeaponAction";


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	ThrowWeaponAction::ThrowWeaponAction() :
		Action(ACTION_ID),
		mTargetEntityId(qsf::getUninitialized<uint64>()),
		mThrownWeapon(weapon::STONE),
		mCurrentState(STATE_INIT),
		mThrowableId(qsf::getUninitialized<uint64>())
	{
		// Nothing here
	}

	ThrowWeaponAction::~ThrowWeaponAction()
	{
		// Nothing here
	}

	void ThrowWeaponAction::init(const qsf::Entity& targetEntity, weapon::Weapon thrownWeapon)
	{
		mTargetEntityId = targetEntity.getId();

		mThrownWeapon = thrownWeapon;

		switch (thrownWeapon)
		{
			case weapon::FIST:			// Fall-through by design
			case weapon::BASEBALLBAT:	// Fall-through by design
			case weapon::KNIFE:			// Fall-through by design
			case weapon::PISTOL:		// Fall-through by design
			case weapon::RIFLE:			// Fall-through by design
			case weapon::PEPPERSPRAY:	// Fall-through by design
			{
				// All the above (besides STONE) are no valid values!
				QSF_CHECK(false, "Wrong ThrownWeapon specified, falling back to Molotov cocktail!", QSF_REACT_NONE);

				mThrownWeapon = weapon::MOLOTOV;
				break;
			}

			case weapon::NOFIGHT:
			case weapon::MOLOTOV:
			case weapon::STONE:
			case weapon::TORCH:
			case weapon::SWORD:
			case weapon::STUNGRENADE:
				// Nothing to do in this case
				break;
		}
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Actions methods                   ]
	//[-------------------------------------------------------]
	void ThrowWeaponAction::serialize(qsf::BinarySerializer& serializer)
	{
		serializer.serialize(mTargetEntityId);
		serializer.serializeAs<uint8>(mThrownWeapon);
		serializer.serializeAs<uint8>(mCurrentState);
		serializer.serialize(mThrowableId);
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	bool ThrowWeaponAction::onStartup()
	{
		// Get the caller and target entity
		const qsf::Entity& callerEntity = getEntity();
		const qsf::Entity* targetEntity = callerEntity.getMap().getEntityById(mTargetEntityId);
		if (nullptr == targetEntity)
			return false;

		// TODO(ks) Correct min/max distances!
		// TODO(ks) Do ALL necessary checks!

		// Make the distance check
		const float minDistance = 5.0f;
		const float currentDistance = qsf::game::DistanceHelper::getDistance(*targetEntity, callerEntity);

		if (currentDistance < minDistance)
			return false;

		return true;
	}

	void ThrowWeaponAction::onShutdown()
	{
		if (mThrowableEntity.valid())
		{
			getMap().destroyEntityById(mThrowableEntity->getId());
		}

		// Clear animation
		AnimationHelper(getEntity()).clearAnimation();
	}

	qsf::action::Result ThrowWeaponAction::updateAction(const qsf::Clock&)
	{
		qsf::Entity& entity = getEntity();

		switch (mCurrentState)
		{
			case STATE_INIT:
			{
				// Hide the gangster's current weapon as he is going to equip a thrown weapon
				qsf::GameLogicComponent* gameLogicComponent = entity.getComponent<qsf::GameLogicComponent>();
				if (nullptr != gameLogicComponent)
				{
					GangsterBaseLogic* gangsterLogic = gameLogicComponent->getGameLogic<GangsterBaseLogic>();
					if (nullptr != gangsterLogic)
					{
						// Hide weapon, if any...
						gangsterLogic->hideCurrentWeapon();
					}
				}

				// Create entity from prefab
				switch (mThrownWeapon)
				{
					case weapon::STONE:
						mThrowableEntity = getEquipmentFromPrefabAndSetAsActive(entity, "equipment/equipment_persons_stone");
						break;

					case weapon::MOLOTOV:
						mThrowableEntity = getEquipmentFromPrefabAndSetAsActive(entity, "default/equipment_persons_molotov");
						break;

					default:
						break;
				}

				// Check whether everything worked fine
				QSF_CHECK(mThrowableEntity.valid(), "Could not create throwable weapon entity from prefab.", return qsf::action::RESULT_DONE);

				entity.getOrCreateComponent<qsf::ActionComponent>()->pushAction<TurnToAction>(action::COMMAND_HIGH, qsf::action::INSERT_AT_FRONT).init(mTargetEntityId);

				mCurrentState = STATE_THROW;
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_THROW:
			{
				// Start throwing animation (don't wait for it to finish as we have to react in between)
				PlayAnimationAction& playAnimationAction = entity.getOrCreateComponent<qsf::ActionComponent>()->pushAction<PlayAnimationAction>(action::COMMAND_HIGH, qsf::action::INSERT_AT_FRONT);
				playAnimationAction.init(AnimationHelper(entity).getAnimationThrowObject(), false, false, false, qsf::Time::ZERO);

				mCurrentState = STATE_FLYING;
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_FLYING:
			{
				// Wait until throwing animation is at the right position
				qsf::MeshAnimationComponent* meshAnimationComponent = entity.getComponent<qsf::MeshAnimationComponent>();
				if (nullptr != meshAnimationComponent)
				{
					qsf::MeshAnimationChannel* channel = meshAnimationComponent->getAnimationChannel(qsf::AssetProxy(AnimationHelper(entity).getAnimationThrowObject()));
					if (nullptr != channel)
					{
						if (channel->getOffset() < 0.45f)
						{
							return qsf::action::RESULT_CONTINUE;
						}
					}
				}

				// Assuming the gangster has a valid transform component...
				glm::vec3 weaponPos = entity.getOrCreateComponentSafe<qsf::TransformComponent>().getPosition();;

				QSF_WARN_IF(!mThrowableEntity.valid(), "ThrowWeaponAction: throw able object is invalid, can't delete it", QSF_REACT_NONE)
				if (mThrowableEntity.valid())
				{
					// Get the component that links the throwing weapon to the gangster
					qsf::game::BoneLinkComponent* boneLinkComponent = mThrowableEntity->getComponent<qsf::game::BoneLinkComponent>();

					// It should exist, but do some checks
					if (nullptr != boneLinkComponent)
					{
						// Compute the weapon position (world space) from relative bone link transformation...
						weaponPos += boneLinkComponent->getLocalPosition();

						// ... and remove the link - this allows us to throw away the weapon
						mThrowableEntity->destroyComponent<qsf::game::BoneLinkComponent>();
					}

					// Destroy the thrown weapon that is still located in the gangster's hand
					getMap().destroyEntityById(mThrowableEntity->getId());
				}

				// Create the object that is flying through the air
				qsf::Entity* flyingObject = nullptr;

				switch (mThrownWeapon)
				{
					case weapon::STONE:
						flyingObject = getMap().createObjectByLocalPrefabAssetId(qsf::StringHash("em5/prefab/equipment/equipment_persons_stone"));
						break;

					case weapon::MOLOTOV:
						flyingObject = getMap().createObjectByLocalPrefabAssetId(qsf::StringHash("em5/prefab/particles/molotov_cocktail_bottle"));
						break;

					default:
						break;
				}

				// Check whether everything worked fine
				QSF_CHECK(nullptr != flyingObject, "Could not create thrown weapon entity from prefab.", return qsf::action::RESULT_DONE);

				// Use freeplay event from gangster
				FreeplayEvent* freeplayEvent = EntityHelper(entity).getFreeplayEvent();
				if (nullptr != freeplayEvent)
				{
					flyingObject->getOrCreateComponentSafe<EventIdComponent>().setEvent(*freeplayEvent, eventspreadreason::NO_REASON);
				}

				// Set the correct weapon position
				qsf::TransformComponent& flyingObjectTransformComponent = flyingObject->getOrCreateComponentSafe<qsf::TransformComponent>();
				flyingObjectTransformComponent.setPosition(weaponPos + glm::vec3(0.0f, 1.4f, 0.0f)); // Assume the weapon is thrown from somewhere higher than where the person's hands are
				flyingObjectTransformComponent.setScale(glm::vec3(1.0f));

				qsf::ActionComponent& flyingObjectActionComponent = flyingObject->getOrCreateComponentSafe<qsf::ActionComponent>();
				flyingObjectActionComponent.clearPlan();
				flyingObjectActionComponent.pushAction<BeThrownWeaponAction>(action::BLOCKING, qsf::action::INSERT_AT_FRONT).init(getEntityId(), mTargetEntityId, mThrownWeapon);

				mCurrentState = STATE_FLYING2;
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_FLYING2:
			{
				qsf::MeshAnimationComponent* meshAnimationComponent = entity.getComponent<qsf::MeshAnimationComponent>();
				if (nullptr != meshAnimationComponent)
				{
					qsf::MeshAnimationChannel* channel = meshAnimationComponent->getAnimationChannel(qsf::AssetProxy(AnimationHelper(entity).getAnimationThrowObject()));
					if (nullptr != channel && !channel->isFinished())
					{
						return qsf::action::RESULT_CONTINUE;
					}
				}

				mCurrentState = STATE_DONE;
				// No break by intent
			}

			case STATE_DONE:
			{
				if (AnimationHelper(entity).isAnimationPlaying())
				{
					return qsf::action::RESULT_CONTINUE;
				}
				// No break by intent
			}
		}

		return qsf::action::RESULT_DONE;

	}

	qsf::Entity* ThrowWeaponAction::getEquipmentFromPrefabAndSetAsActive(qsf::Entity& parent, const std::string& prefab)
	{
		// Get inventory
		InventoryComponent* inventoryComponent = parent.getOrCreateComponent<InventoryComponent>();

		// Create entity from prefab
		qsf::Entity* equipmentEntity = getMap().createObjectByLocalPrefabAssetId(qsf::StringHash("em5/prefab/" + prefab));

		if (nullptr != equipmentEntity)
		{
			// Make entity the active one
			inventoryComponent->setActiveEquipmentId(equipmentEntity->getId());

			// Link equipment entity to parent entity (here: the gangster)
			EntityHelper(*equipmentEntity).linkEquipmentToEntity(parent);

			// Set the equipment layer as the same layer as the gangster entity
			EntityHelper(parent).moveEntityInOwnerLayer(*equipmentEntity);
		}

		// If anything went wrong, this is a nullptr
		return equipmentEntity;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

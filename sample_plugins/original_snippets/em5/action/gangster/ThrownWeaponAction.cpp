// Copyright (C) 2012-2016 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/action/gangster/ThrownWeaponAction.h"
#include "em5/action/gangster/BeThrownWeaponAction.h"
#include "em5/action/move/TurnToAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/game/animation/AnimationHelper.h"
#include "em5/map/EntityHelper.h"
#include "em5/game/animation/AnimationHelper.h"
#include "em5/action/base/PlayAnimationAction.h"
#include "em5/component/event/EventIdComponent.h"
#include "em5/component/equipment/InventoryComponent.h"

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
	const qsf::NamedIdentifier ThrownWeaponAction::ACTION_ID = "em5::ThrownWeaponAction";


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	ThrownWeaponAction::ThrownWeaponAction() :
		Action(ACTION_ID),
		mTargetEntityId(qsf::getUninitialized<uint64>()),
		mThrownWeapon(GangsterBaseLogic::WEAPON_STONE),
		mCurrentState(STATE_INIT),
		mThrowableId(qsf::getUninitialized<uint64>())
	{
		// Nothing here
	}

	ThrownWeaponAction::~ThrownWeaponAction()
	{
		// Clear animation
		AnimationHelper(getEntity()).clearAnimation();
	}

	void ThrownWeaponAction::init(const qsf::Entity& targetEntity, GangsterBaseLogic::Weapon thrownWeapon)
	{
		mTargetEntityId = targetEntity.getId();

		mThrownWeapon = thrownWeapon;

		switch (thrownWeapon)
		{
			case GangsterBaseLogic::WEAPON_FIST:			// Fall-through by design
			case GangsterBaseLogic::WEAPON_BASEBALLBAT:		// Fall-through by design
			case GangsterBaseLogic::WEAPON_KNIFE:			// Fall-through by design
			case GangsterBaseLogic::WEAPON_PISTOL:			// Fall-through by design
			case GangsterBaseLogic::WEAPON_RIFLE:			// Fall-through by design
			case GangsterBaseLogic::WEAPON_PEPPER_SPRAY:	// Fall-through by design
			{
				// All the above (besides STONE) are no valid values!
				QSF_CHECK(false, "Wrong ThrownWeapon specified, falling back to Molotov cocktail!", QSF_REACT_NONE);

				mThrownWeapon = GangsterBaseLogic::WEAPON_MOLOTOV_COCKTAIL;
				break;
			}

			case GangsterBaseLogic::WEAPON_NO_FIGHT:
			case GangsterBaseLogic::WEAPON_MOLOTOV_COCKTAIL:
			case GangsterBaseLogic::WEAPON_STONE:
			case GangsterBaseLogic::WEAPON_STUNGRENADE:
				// Nothing to do in this case
				break;
		}
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Actions methods                   ]
	//[-------------------------------------------------------]
	void ThrownWeaponAction::serialize(qsf::BinarySerializer& serializer)
	{
		serializer.serialize(mTargetEntityId);
		serializer.serializeAs<uint8>(mThrownWeapon);
		serializer.serializeAs<uint8>(mCurrentState);
		serializer.serialize(mThrowableId);
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	bool ThrownWeaponAction::onStartup()
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

	qsf::action::Result ThrownWeaponAction::updateAction(const qsf::Clock&)
	{
		qsf::Entity* gangsterEntity = &getEntity();

		switch (mCurrentState)
		{
			case STATE_INIT:
			{
				// Hide the gangster's current weapon as he is going to equip a thrown weapon
				qsf::GameLogicComponent* gameLogicComponent = gangsterEntity->getComponent<qsf::GameLogicComponent>();
				if (nullptr != gameLogicComponent)
				{
					GangsterBaseLogic* gangsterLogic = gameLogicComponent->getGameLogic<GangsterBaseLogic>();
					if (nullptr != gangsterLogic)
					{
						// Hide weapon, if any...
						gangsterLogic->hideCurrentWeapon();
					}
				}

				qsf::Entity* throwable = nullptr;

				if (qsf::isInitialized(mThrowableId))
				{
					throwable = getMap().getEntityById(mThrowableId);

					// Check whether everything worked fine
					QSF_CHECK(throwable, "Could not create thrown weapon entity from prefab.", return qsf::action::RESULT_DONE);
				}
				else
				{
					// Create entity from prefab

					switch (mThrownWeapon)
					{
						case GangsterBaseLogic::WEAPON_STONE:
							throwable = getEquipmentFromPrefabAndSetAsActive(gangsterEntity, "equipment/equipment_persons_stone");
							break;

						case GangsterBaseLogic::WEAPON_MOLOTOV_COCKTAIL:
							throwable = getEquipmentFromPrefabAndSetAsActive(gangsterEntity, "default/equipment_persons_molotov");
							break;

						default:
							break;
					}

					// Check whether everything worked fine
					QSF_CHECK(throwable, "Could not create thrown weapon entity from prefab.", return qsf::action::RESULT_DONE);

					// Store the ID
					mThrowableId = throwable->getId();
				}

				// Get the gangster transform component
				qsf::TransformComponent* gangsterTC = gangsterEntity->getComponent<qsf::TransformComponent>();
				if (nullptr == gangsterTC)
				{
					// Invalid transform component, delete the throwable and end the action
					getMap().destroyEntityById(mThrowableId);
					return qsf::action::RESULT_DONE;
				}

				gangsterEntity->getOrCreateComponent<qsf::ActionComponent>()->pushAction<TurnToAction>(action::COMMAND_HIGH, qsf::action::INSERT_AT_FRONT).init(mTargetEntityId);

				mCurrentState = STATE_THROW;
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_THROW:
			{
				// Enqueue a throwing animation (that blocks this very action)
				PlayAnimationAction& current = gangsterEntity->getOrCreateComponent<qsf::ActionComponent>()->pushAction<PlayAnimationAction>(action::COMMAND_HIGH, qsf::action::INSERT_AT_FRONT);// .init(AnimationHelper(*gangsterEntity).getAnimationThrowObject(), true, false, false, qsf::Time::ZERO);
				current.init(AnimationHelper(*gangsterEntity).getAnimationThrowObject(), true, false, false, qsf::Time::ZERO);
				current.setAnimationOffsetRange(0.0f, 0.45f);

				mCurrentState = STATE_FLYING;
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_FLYING:
			{
				PlayAnimationAction& current = gangsterEntity->getOrCreateComponent<qsf::ActionComponent>()->pushAction<PlayAnimationAction>(action::COMMAND_HIGH, qsf::action::INSERT_AT_FRONT);// .init(AnimationHelper(*gangsterEntity).getAnimationThrowObject(), true, false, false, qsf::Time::ZERO);
				current.init(AnimationHelper(*gangsterEntity).getAnimationThrowObject(), false, false, false, qsf::Time::ZERO);
				current.setAnimationOffsetRange(0.47f, 1.0f);

				// Assuming the gangster has a valid transform component...
				const glm::vec3& gangsterPos = gangsterEntity->getComponent<qsf::TransformComponent>()->getPosition();
				glm::vec3 weaponPos = gangsterPos;

				qsf::Entity* throwable = getMap().getEntityById(mThrowableId);

				// Get the component that links the throwing weapon to the gangster
				qsf::game::BoneLinkComponent* boneLinkComponent = throwable->getComponent<qsf::game::BoneLinkComponent>();

				// It should exist, but do some checks
				if (nullptr != boneLinkComponent)
				{
					// Compute the weapon position (world space) from relative bone link transformation...
					weaponPos += boneLinkComponent->getLocalPosition();

					// ... and remove the link - this allows us to throw away the weapon
					throwable->destroyComponent<qsf::game::BoneLinkComponent>();
				}

				// Create the object that is flying through the air
				qsf::Entity* flyingObject = nullptr;

				switch (mThrownWeapon)
				{
					case GangsterBaseLogic::WEAPON_STONE:
						flyingObject = getMap().createObjectByLocalPrefabAssetId(qsf::StringHash("em5/prefab/equipment/equipment_persons_stone"));
						break;

					case GangsterBaseLogic::WEAPON_MOLOTOV_COCKTAIL:
						flyingObject = getMap().createObjectByLocalPrefabAssetId(qsf::StringHash("em5/prefab/particles/molotov_cocktail_bottle"));
						break;

					default:
						break;
				}

				// Check whether everything worked fine
				QSF_CHECK(flyingObject, "Could not create thrown weapon entity from prefab.", return qsf::action::RESULT_DONE);

				// Get freeplay event from gangster
				FreeplayEvent* freeplayEvent = nullptr;

				// Assume gangster has an EventIdComponent and is part of an event
				EventIdComponent* eventIdComponent = getEntity().getComponent<EventIdComponent>();
				if (nullptr != eventIdComponent)
				{
					freeplayEvent = eventIdComponent->getFreeplayEvent();
				}

				if (nullptr != freeplayEvent)
				{
					flyingObject->getOrCreateComponentSafe<EventIdComponent>().setEvent(*freeplayEvent, eventspreadreason::NO_REASON);
				}

				qsf::TransformComponent* flyingObjectTC = flyingObject->getOrCreateComponent<qsf::TransformComponent>();

				// Set the correct weapon position
				flyingObjectTC->setPosition(weaponPos + glm::vec3(0, 1.4f, 0)); // Assume the weapon is thrown from somewhere higher than where the person's hands are
				flyingObjectTC->setScale(glm::vec3(1.0f));
				qsf::ActionComponent* flyingObjectAC = flyingObject->getOrCreateComponent<qsf::ActionComponent>();
				flyingObjectAC->clearPlan();
				flyingObjectAC->pushAction<BeThrownWeaponAction>(action::BLOCKING, qsf::action::INSERT_AT_FRONT).init(getEntityId(), mTargetEntityId, mThrownWeapon);

				mCurrentState = STATE_END_ANIMATION;
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_END_ANIMATION:
			{
				// Destroy the thrown weapon that is still located in the gangster's hand
				getMap().destroyEntityById(mThrowableId);

				mCurrentState = STATE_DONE;
			}

			case STATE_DONE:
			{
				if (AnimationHelper(*gangsterEntity).isAnimationPlaying())
				{
					return qsf::action::RESULT_CONTINUE;
				}

				return qsf::action::RESULT_DONE;

				// Woop woop!
			}

			default:
				break;
		}

		return qsf::action::RESULT_DONE;

	}

	qsf::Entity* ThrownWeaponAction::getEquipmentFromPrefabAndSetAsActive(qsf::Entity* parent, const std::string& prefab)
	{
		// Get inventory
		InventoryComponent* inventoryComponent = parent->getOrCreateComponent<InventoryComponent>();

		// Create entity from prefab
		qsf::Entity* equipmentEntity = parent->getMap().createObjectByLocalPrefabAssetId(qsf::StringHash("em5/prefab/" + prefab));

		if (nullptr != equipmentEntity)
		{
			// Make entity the active one
			inventoryComponent->setActiveEquipmentId(equipmentEntity->getId());

			// Link equipment entity to parent entity (here: the gangster)
			EntityHelper(*equipmentEntity).linkEquipmentToEntity(parent->getId());

			// Set the equipment layer as the same layer as the gangster entity
			EntityHelper(*parent).moveEntityInOwnerLayer(*equipmentEntity);
		}

		// If anything went wrong, this is a nullptr
		return equipmentEntity;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

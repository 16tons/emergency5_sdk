// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/action/equipment/UnloadCartAction.h"
#include "em5/action/base/PlayAnimationAction.h"
#include "em5/action/base/DisappearAction.h"
#include "em5/action/move/TurnToAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/base/GameplayAssets.h"
#include "em5/component/person/PersonComponent.h"
#include "em5/game/animation/AnimationHelper.h"
#include "em5/map/EntityHelper.h"
#include "em5/map/MapHelper.h"

#include <qsf_game/component/base/BoneToBoneLinkComponent.h>
#include <qsf_game/component/event/EventTagComponent.h>
#include <qsf_game/base/DistanceHelper.h>

#include <qsf/logic/action/WaitAction.h>
#include <qsf/component/base/MetadataComponent.h>
#include <qsf/component/base/TransformComponent.h>
#include <qsf/component/link/LinkComponent.h>
#include <qsf/component/rotation/LinearRotationComponent.h>
#include <qsf/logic/action/ActionComponent.h>
#include <qsf/map/query/ComponentMapQuery.h>
#include <qsf/map/Entity.h>
#include <qsf/map/Map.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const qsf::NamedIdentifier UnloadCartAction::ACTION_ID = "em5::UnloadCartAction";


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	UnloadCartAction::UnloadCartAction() :
		Action(ACTION_ID),
		mCurrentState(STATE_INIT),
		mUnloadCart(false),
		mStepNumber(1),
		mCartWheelsTurnSpeedMultiplierBackup(1.0f)
	{
		// Nothing here
	}

	UnloadCartAction::~UnloadCartAction()
	{
		// Nothing here
	}

	void UnloadCartAction::init(bool unloadCart, int stepNumber)
	{
		mUnloadCart = unloadCart;
		mStepNumber = stepNumber;
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Actions methods                   ]
	//[-------------------------------------------------------]
	void UnloadCartAction::serialize(qsf::BinarySerializer& serializer)
	{
		// TODO(mk) serialize this class
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	qsf::action::Result UnloadCartAction::updateAction(const qsf::Clock&)
	{
		switch (mCurrentState)
		{
			case STATE_INIT:
			{
				// Hack for setting the walking animation of the medieval person
				getEntity().getOrCreateComponentSafe<PersonComponent>().setPersonFlags(PersonComponent::FLAG_NEW_SKELETON, true);
				getEntity().getOrCreateComponentSafe<qsf::MetadataComponent>().setTags("cart_man");

				// Get the cart entity
				qsf::LinkComponent* linkComponent = getEntity().getComponent<qsf::LinkComponent>();
				if (nullptr != linkComponent)
				{
					// Take the first child entity, hope this is correct
					auto childLinks = linkComponent->getChildLinks();
					mCartEntity = (*childLinks.begin())->getEntity();

					// Turn off wheel turning while action
					setCartWheels(false);
				}

				if (mUnloadCart)
				{
					pushTurnAction();

					mCurrentState = STATE_TURN_TO_SPAWN;
					return qsf::action::RESULT_CONTINUE;
				}

				loadCartWithDeadBodies();
				getComponent().pushAction<qsf::WaitAction>().init(qsf::Time::fromSeconds(1.0f));

				mCurrentState = STATE_DONE;
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_TURN_TO_SPAWN:
			{
				// Play unload animation
				playUnloadAnimation();

				mCurrentState = STATE_PLAY_UNLOAD_ANIMATION;
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_PLAY_UNLOAD_ANIMATION:
			{
				if (AnimationHelper(getEntity()).isAnimationPlaying())
				{
					// Wait for animation ending
					return qsf::action::RESULT_CONTINUE;
				}

				// Link the cart back to the parent-person
				if (mCartEntity.valid())
				{
					mCartEntity->getOrCreateComponentSafe<qsf::LinkComponent>().linkToParent(getEntity());
				}

				mCurrentState = STATE_DONE;
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_DONE:
			{
				setCartWheels(true);

				// Hack for setting the walking animation of the cart
				AnimationHelper(mCartEntity.getSafe()).playAnimation("em5/skeleton/person/em5x_vehicle_cart_walk", true);
				break;
			}
		}

		return qsf::action::RESULT_DONE;
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void UnloadCartAction::pushTurnAction()
	{
		const auto& eventTagComponents = qsf::ComponentMapQuery(getMap()).getAllInstances<qsf::game::EventTagComponent>();
		if (eventTagComponents.empty())
			return;

		qsf::Entity* nearstUnloadPoint = nullptr;
		float nearestDistance = 0;
		for (qsf::game::EventTagComponent* eventTagComponent : eventTagComponents)
		{
			if (eventTagComponent->getEventTagsAsString().find("UNLOAD_CART_POINT") != std::string::npos)
			{
				if (nearstUnloadPoint == nullptr)
				{
					nearstUnloadPoint = &eventTagComponent->getEntity();
					nearestDistance = qsf::game::DistanceHelper::getDistance(*nearstUnloadPoint, getEntity());
				}
				else
				{
					float newDistance = qsf::game::DistanceHelper::getDistance(eventTagComponent->getEntity(), getEntity());
					if (newDistance < nearestDistance)
					{
						// Found eventtag near to the entity
						nearstUnloadPoint = &eventTagComponent->getEntity();
						nearestDistance = newDistance;
					}
				}
			}
		}

		if (nearstUnloadPoint == nullptr)
			return; // internal error

		const glm::quat spawnpointRotation = nearstUnloadPoint->getComponentSafe<qsf::TransformComponent>().getRotation();

		// Push turn action
		getComponent().pushAction<TurnToAction>(action::DAILY_ROUTINE, qsf::action::INSERT_BEFORE_CURRENT).initWithRotation(spawnpointRotation, 135.0f);
	}

	void UnloadCartAction::playUnloadAnimation()
	{
		// Play animation for all the child entities
		if (mCartEntity.valid())
		{
			// Animation for dead bodies
			qsf::LinkComponent* linkComponent = mCartEntity->getComponent<qsf::LinkComponent>();
			if (nullptr != linkComponent)
			{
				std::vector<qsf::Entity*> deadBodyEntities;

				for (qsf::LinkComponent* childLinkComponent : linkComponent->getChildLinks())
				{
					qsf::Entity& childEntity = childLinkComponent->getEntity();
					qsf::MetadataComponent* metadataComponent = childEntity.getComponent<qsf::MetadataComponent>();

					if (nullptr != metadataComponent)
					{
						if (metadataComponent->getTags() == "body_1")
						{
							AnimationHelper(childEntity).playAnimation("em5/skeleton/default/anim_dead_body_1_unload");
							deadBodyEntities.push_back(&childEntity);
						}
						else if (metadataComponent->getTags() == "body_2")
						{
							AnimationHelper(childEntity).playAnimation("em5/skeleton/default/anim_dead_body_2_unload");
							deadBodyEntities.push_back(&childEntity);
						}
						else if (metadataComponent->getTags() == "body_3")
						{
							AnimationHelper(childEntity).playAnimation("em5/skeleton/default/anim_dead_body_3_unload");
							deadBodyEntities.push_back(&childEntity);
						}
					}
				}

				// Delete the dead body entity after we loop over them
				for (qsf::Entity* deadBodyEntity : deadBodyEntities)
				{
					pushFadeOutAction(*deadBodyEntity);
				}
				linkComponent->unlinkFromParent();
			}

			// Animation for the cart
			AnimationHelper(mCartEntity.getSafe()).playAnimation("em5/skeleton/default/em5x_vehicle_cart_unload");
		}

		AnimationHelper(getEntity()).playAnimation("em5/skeleton/default/em5x_unload_cart");

		// Destroy linkage to have a nice animation
		getEntity().destroyComponent<qsf::LinkComponent>();
	}

	void UnloadCartAction::loadCartWithDeadBodies()
	{
		if (!mCartEntity.valid())
			return;

		if (mStepNumber == 1)
		{
			loadCartWithDeadBody(assets::PREFAB_MEDIEVAL_DEAD_BODY1, "body_1", "leiche_1Pelvis", "anim_dead_body_1_walk");
		}
		else if (mStepNumber == 2)
		{
			loadCartWithDeadBody(assets::PREFAB_MEDIEVAL_DEAD_BODY2, "body_2", "leiche_2Pelvis", "anim_dead_body_2_walk");
		}
		else
		{
			loadCartWithDeadBody(assets::PREFAB_MEDIEVAL_DEAD_BODY3, "body_3", "leiche_3Pelvis", "anim_dead_body_3_walk");
		}
	}

	void UnloadCartAction::loadCartWithDeadBody(qsf::LocalAssetId deadBodyAsset, const std::string& metadataTag, const std::string& boneNameToTag, const std::string& animationName)
	{
		qsf::LinkComponent* parentLinkComponent = mCartEntity->getComponent<qsf::LinkComponent>();
		if (parentLinkComponent)
		{
			for (qsf::LinkComponent* childLink : parentLinkComponent->getChildLinks())
			{
				qsf::MetadataComponent* metadataComponent = childLink->getEntity().getComponent<qsf::MetadataComponent>();
				if (metadataComponent != nullptr && metadataComponent->getTags() == metadataTag)
				{
					// An entity with this meta tag is already linked, stop creating an new entity
					return;
				}
			}
		}

		qsf::Entity* deadBody = MapHelper(getMap()).createObjectByLocalPrefabAssetId(deadBodyAsset);
		if (nullptr != deadBody)
		{
			// Ensure that the dead body is inside the same layer as the cart entity, else after map
			// restart spooky dead bodies will certainly stick inside the ground and scare poor citizens
			EntityHelper(*mCartEntity).moveEntityInOwnerLayer(*deadBody);

			deadBody->getOrCreateComponentSafe<qsf::LinkComponent>().linkToParent(*mCartEntity);

			// Safe extra identifier inside the metadata component
			deadBody->getOrCreateComponentSafe<qsf::MetadataComponent>().setTags(metadataTag);

			qsf::game::BoneToBoneLinkComponent& boneToBoneLinkComponent = deadBody->getOrCreateComponentSafe<qsf::game::BoneToBoneLinkComponent>(false);
			boneToBoneLinkComponent.setBoneName("bip_02_cart");
			boneToBoneLinkComponent.setParentBoneName1("bip_02_cart");
			boneToBoneLinkComponent.setChildBoneName1(boneNameToTag);
			boneToBoneLinkComponent.startup(getMap().isRunning());

			AnimationHelper(*deadBody).playAnimation("em5/skeleton/default/" + animationName, true);
			EntityHelper(*deadBody).fadeIn();
		}
	}

	void UnloadCartAction::pushFadeOutAction(qsf::Entity& entity)
	{
		// Push wait and fadeout
		qsf::ActionComponent& actionComponent = entity.getOrCreateComponentSafe<qsf::ActionComponent>();
		actionComponent.pushAction<qsf::WaitAction>().init(qsf::Time::fromSeconds(2.0f));
		actionComponent.pushAction<DisappearAction>().init(qsf::Time::fromSeconds(1.0f), true);
	}

	void UnloadCartAction::setCartWheels(bool startWheels)
	{
		if (!mCartEntity.valid())
			return;

		// The cart entity has to linked wheels with a linear rotation component, activate or deactivate it
		qsf::LinkComponent* linkComponent = mCartEntity->getComponent<qsf::LinkComponent>();
		if (nullptr != linkComponent)
		{
			for (qsf::LinkComponent* childLinkComponent : linkComponent->getChildLinks())
			{
				qsf::LinearRotationComponent* linearRotationComponent = childLinkComponent->getComponentFromEntityOrLinkedChild<qsf::LinearRotationComponent>();
				if (nullptr != linearRotationComponent)
				{
					if (startWheels)
					{
						linearRotationComponent->setMultiplier(mCartWheelsTurnSpeedMultiplierBackup);
					}
					else
					{
						mCartWheelsTurnSpeedMultiplierBackup = linearRotationComponent->getMultiplier();
						linearRotationComponent->setMultiplier(0);
					}
				}
			}
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

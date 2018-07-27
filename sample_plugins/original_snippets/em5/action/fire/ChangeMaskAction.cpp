// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/action/fire/ChangeMaskAction.h"
#include "em5/action/base/PlayAnimationAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/base/EquipmentAssets.h"
#include "em5/command/component/CommandableComponent.h"
#include "em5/component/effect/FadeEffectComponent.h"
#include "em5/game/animation/AnimationHelper.h"
#include "em5/game/units/UnitHelper.h"
#include "em5/logic/local/firefighters/FiremanLogic.h"
#include "em5/map/EntityHelper.h"

#include <qsf/logic/action/ActionComponent.h>
#include <qsf/logic/action/WaitAction.h>
#include <qsf/logic/gamelogic/GameLogicComponent.h>
#include <qsf/map/Map.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const qsf::NamedIdentifier ChangeMaskAction::ACTION_ID = "em5::ChangeMaskAction";


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	ChangeMaskAction::ChangeMaskAction() :
		Action(ACTION_ID),
		mActionType(ADD_MASK),
		mState(STATE_INIT),
		mFadeEquipmentTime(qsf::Time::fromSeconds(0.5))
	{
		// Nothing here
	}

	ChangeMaskAction::~ChangeMaskAction()
	{
		// Nothing here
	}

	void ChangeMaskAction::init(ActionType actionType)
	{
		mActionType = actionType;
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Actions methods                   ]
	//[-------------------------------------------------------]
	void ChangeMaskAction::serialize(qsf::BinarySerializer& serializer)
	{
		serializer.serializeAs<uint8>(mState);
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	bool ChangeMaskAction::onStartup()
	{
		CommandableComponent* commandableComponent = getEntity().getComponent<CommandableComponent>();
		if (nullptr != commandableComponent && (commandableComponent->isFireFighterPersonHistoric() || commandableComponent->isPoliceGuardsmanHistoric() ||
			UnitHelper(getEntity()).isFireFighterPersonWithABCSuitActive()))
		{
			return false;
		}

		// Anything to do at all?
		FiremanLogic* firemanLogic = EntityHelper(getEntity()).getGameLogic<FiremanLogic>();
		if (nullptr != firemanLogic)
		{
			const FiremanLogic::Clothing targetClothing = (mActionType == ADD_MASK) ? FiremanLogic::CLOTHING_DUST_MASK : FiremanLogic::CLOTHING_DEFAULT;
			if (firemanLogic->getActiveClothing() == targetClothing)
				return false;
		}

		return true;
	}

	qsf::action::Result ChangeMaskAction::updateAction(const qsf::Clock& clock)
	{
		switch (mState)
		{
			case STATE_INIT:
			{
				fadeEquipment(false);
				mState = STATE_FADE_OUT_EQUIPMENT;

				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_FADE_OUT_EQUIPMENT:
			{
				mTimeUntilStateSwitch -= clock.getTimePassed();
				if (mTimeUntilStateSwitch.getSeconds() <= 0 && getEntity().getComponent<FadeEffectComponent>() == nullptr)
				{
					if (mActionType == ADD_MASK)
					{
						// Create a temporary mask (until we switch the mesh)
						createMask();

						// Play some transition animation
						getComponent().pushAction<PlayAnimationAction>(getPriority(), qsf::action::INSERT_AT_FRONT).init(AnimationHelper(getEntity()).getAnimationFiremanPutOnMask(), true, false, false, qsf::Time::fromSeconds(0.3f));
					}

					mState = STATE_SWITCH_MESH;
				}

				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_SWITCH_MESH:
			{
				EntityHelper entityHelper(getEntity());

				bool entityVisibleOnScreen = false;
				if (mActionType == ADD_MASK)
				{
					// Remove mask again, but before doing so rescue the entity screen visibility state
					entityVisibleOnScreen = entityHelper.isEntityVisibleOnScreen();
					destroyMask();
				}

				// Change the mesh
				FiremanLogic* firemanLogic = entityHelper.getGameLogic<FiremanLogic>();
				if (nullptr != firemanLogic)
				{
					firemanLogic->setActiveClothing((mActionType == ADD_MASK) ? FiremanLogic::CLOTHING_DUST_MASK : FiremanLogic::CLOTHING_DEFAULT);
				}

				AnimationHelper animationHelper(getEntity());
				if (mActionType == ADD_MASK)
				{
					// Preventing T-pose
					animationHelper.playAnimation(animationHelper.getAnimationFiremanPutOnMask(), false, true);

					// Avoid visual one frame glitches
					if (entityVisibleOnScreen)
					{
						qsf::MeshAnimationComponent* meshAnimationComponent = getEntity().getComponent<qsf::MeshAnimationComponent>();
						if (nullptr != meshAnimationComponent)
						{
							meshAnimationComponent->enforceGraphicsSynchronization();
						}
					}

					// ... but not playing the full animation
					animationHelper.clearAnimation();

					// Wait a little longer to give animation a chance to have a nice transition
					// Also we want equipment fade-in a little later.
					getComponent().pushAction<qsf::WaitAction>(getPriority(), qsf::action::INSERT_AT_FRONT).init(qsf::Time::fromSeconds(0.2f));

					mState = STATE_START_FADE_IN;
				}
				else if (mActionType == REMOVE_MASK)
				{
					// Preventing T-pose
					animationHelper.playAnimation(animationHelper.getAnimationFiremanPutOnMask(), false, false);

					// Create a temporary mask
					createMask();

					// Play some transition animation
					getComponent().pushAction<PlayAnimationAction>(getPriority(), qsf::action::INSERT_AT_FRONT).init(animationHelper.getAnimationFiremanPutOnMask(), true, false, true);

					mState = STATE_REMOVE_MASK;
				}

				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_REMOVE_MASK:
			{
				// Remove mask again
				destroyMask();

				mState = STATE_START_FADE_IN;

				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_START_FADE_IN:
			{
				// Fade-in the equipment
				fadeEquipment(true);

				mState = STATE_FADE_IN;

				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_FADE_IN:
			{
				mTimeUntilStateSwitch -= clock.getTimePassed();

				if (mTimeUntilStateSwitch.getSeconds() > 0 || getEntity().getComponent<FadeEffectComponent>() != nullptr)
					return qsf::action::RESULT_CONTINUE;
			}

			// Immediately go to the next state; no break by intent
		}

		// Clear animation as animation state is usually wrong now
		AnimationHelper(getEntity()).clearAnimation();

		// Done
		return qsf::action::RESULT_DONE;
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void ChangeMaskAction::createMask()
	{
		std::string mashPrefabAsset("em5/prefab/equipment/");
		mashPrefabAsset += equipment::DUST_MASK;
		mMaskEntity = getEntity().getMap().createObjectByLocalPrefabAssetId(qsf::StringHash(mashPrefabAsset));
		if (mMaskEntity.valid())
		{
			EntityHelper entityHelper(*mMaskEntity);
			entityHelper.linkEquipmentToEntity(getEntity());

			// Enabling the shine through effect here is necessary to avoid crashes in the renderer with the "12345703_shadow_caster" material
			entityHelper.enableShineThroughEffect();
		}
	}

	void ChangeMaskAction::destroyMask()
	{
		if (mMaskEntity.valid())
		{
			getEntity().getMap().destroyEntityById(mMaskEntity->getId());
			mMaskEntity = nullptr;
		}
	}

	void ChangeMaskAction::fadeEquipment(bool fadeIn)
	{
		// Fade in/out (but not delete) the equipment
		qsf::Entity* equipmentEntity = EntityHelper(getEntity()).getActiveEquipmentEntity();
		if (nullptr != equipmentEntity)
		{
			FadeEffectComponent& fadeEffectComponent = equipmentEntity->getOrCreateComponentSafe<FadeEffectComponent>();
			if (fadeIn)
			{
				fadeEffectComponent.fadeIn(mFadeEquipmentTime);
			}
			else
			{
				fadeEffectComponent.fadeOut(mFadeEquipmentTime);
			}
			mTimeUntilStateSwitch = mFadeEquipmentTime;
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/logic/local/gangsters/eventspecific/GangsterBlackoutMini.h"
#include "em5/action/base/EnterVehicleAction.h"
#include "em5/action/base/ExitVehicleAction.h"
#include "em5/action/move/MoveAction.h"
#include "em5/ai/MovementModes.h"
#include "em5/ai/MoverType.h"
#include "em5/component/event/EventIdComponent.h"
#include "em5/component/objects/BombComponent.h"
#include "em5/game/minimap/MinimapHelper.h"
#include "em5/game/targetpoint/GotoObjectTargetPointProvider.h"
#include "em5/game/targetpoint/EnterVehicleTargetPointProvider.h"
#include "em5/map/EntityHelper.h"
#include "em5/EM5Helper.h"

#include <qsf_game/base/DistanceHelper.h>

#include <qsf_ai/navigation/NavigationComponent.h>
#include <qsf_ai/navigation/goal/ReachObjectGoal.h>

#include <qsf/logic/action/ActionComponent.h>
#include <qsf/logic/action/WaitAction.h>
#include <qsf/map/Map.h>
#include <qsf/message/MessageSystem.h>
#include <qsf/QsfHelper.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 GangsterBlackoutMini::GAMELOGIC_TYPE_ID = qsf::StringHash("em5::GangsterBlackoutMini");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	GangsterBlackoutMini::GangsterBlackoutMini() :
		GangsterBaseLogic(GAMELOGIC_TYPE_ID),
		mState(STATE_INIT)
	{
		// Nothing here
	}

	void GangsterBlackoutMini::setCar(qsf::Entity& entity)
	{
		mCar = entity;
	}

	void GangsterBlackoutMini::setTargetPointForDriving(qsf::Entity& entity)
	{
		mTargetPointForDriving = entity;
	}

	void GangsterBlackoutMini::setBombPoint(qsf::Entity& entity)
	{
		mBombPoint = entity;
	}

	void GangsterBlackoutMini::setTriggerPoint(qsf::Entity& entity)
	{
		mTriggerPoint = entity;
	}

	void GangsterBlackoutMini::setTimeToDefuseBomb(qsf::Time time)
	{
		mTimeToDefuseBomb = time;
	}

	void GangsterBlackoutMini::setTimeToWaitForExplosion(qsf::Time time)
	{
		mTimeToWaitForExplosion = time;
	}


	//[-------------------------------------------------------]
	//[ Protected virtual em5::GangsterBaseLogic methods      ]
	//[-------------------------------------------------------]
	void GangsterBlackoutMini::onSimulationUpdate(const qsf::JobArguments& jobArguments)
	{
		// Nothing special to do in here, all gangster behavior is handled in "onIdle"
	}

	void GangsterBlackoutMini::onIdle()
	{
		qsf::Entity& entity = *getEntity();
		qsf::ActionComponent& actionComponent = mActionComponent.getSafe();

		switch (mState)
		{
			case STATE_INIT:
			{
				mState = STATE_ENTER_CAR;
				// Fallthrough by design
			}

			case STATE_ENTER_CAR:
			{
				if (nullptr == EntityHelper(entity).getContainerEntity())
				{
					actionComponent.pushAction<MoveAction>().init(new qsf::ai::ReachObjectGoal(entity, mCar.getSafe(), EnterVehicleTargetPointProvider::TARGET_POINT_ID), MovementModes::MOVEMENT_MODE_RUN);
					actionComponent.pushAction<EnterVehicleAction>().init(*mCar);

					// We will need updates after the car was entered
					mNeedsUpdateInContainer = true;
					break;
				}

				// Switch to car control by the AI
				qsf::ai::NavigationComponent& navigationComponent = getEntity()->getOrCreateComponentSafe<qsf::ai::NavigationComponent>();
				qsf::ai::NavigationComponent& carNavigationComponent = mCar->getOrCreateComponentSafe<qsf::ai::NavigationComponent>();
				carNavigationComponent.setActive(true);
				carNavigationComponent.setMoverType(MoverType::GANGSTER_VEHICLE);
				navigationComponent.makeDriverOf(carNavigationComponent);

				// Create mini-map icon
				// TODO(fw): It would be so much better to automatically control this, like the status icon
				MinimapHelper::registerIcon(*mCar, MiniMapIconType::GANGSTER_IN_CAR);

				mState = STATE_DRIVE_TO_TARGET;
				// Fallthrough by design
			}

			case STATE_DRIVE_TO_TARGET:
			{
				if (qsf::game::DistanceHelper::get2dDistance(entity, mTargetPointForDriving.getSafe()) > 5.0f)
				{
					// TODO(fw): Movement mode is not really the right one; but the car should go over traffic lights
					actionComponent.pushAction<MoveAction>().init(new qsf::ai::ReachObjectGoal(entity, mTargetPointForDriving.getSafe(), GotoObjectTargetPointProvider::TARGET_POINT_ID), MovementModes::MOVEMENT_MODE_RUN);
					break;
				}

				// Get out of the car
				qsf::ai::NavigationComponent& navigationComponent = entity.getOrCreateComponentSafe<qsf::ai::NavigationComponent>();
				navigationComponent.dissolveDriverVehicleRelation();
				navigationComponent.setMoverType(MoverType::GANGSTER_PERSON);

				// Add the exit vehicle action
				mCar->getOrCreateComponentSafe<qsf::ActionComponent>().pushAction<ExitVehicleAction>();

				// Unregister mini-map icon
				// TODO(fw): It would be so much better to automatically control this, like the status icon
				MinimapHelper::unregisterIcon(*mCar);

				mState = STATE_GO_TO_BOMB_POINT;
				// Fallthrough by design
			}

			case STATE_GO_TO_BOMB_POINT:
			{
				if (qsf::game::DistanceHelper::get2dDistance(entity, mBombPoint.getSafe()) > 2.0f)
				{
					actionComponent.pushAction<MoveAction>().init(new qsf::ai::ReachObjectGoal(entity, mBombPoint.getSafe(), GotoObjectTargetPointProvider::TARGET_POINT_ID), MovementModes::MOVEMENT_MODE_RUN);
					break;
				}

				mState = STATE_LAY_BOMB;
				// Fallthrough by design
			}

			case STATE_LAY_BOMB:
			{
				FreeplayEvent* freeplayEvent = EntityHelper(entity).getFreeplayEvent();
				QSF_CHECK(nullptr != freeplayEvent, "Gangster is not part of a freeplay event", QSF_REACT_THROW);

				// Create and initialize the bomb
				{
					mBombEntity = QSF_MAKE_REF(QSF_MAINMAP.createObjectByLocalPrefabAssetId(qsf::StringHash("em5/prefab/default/timebomb_large")));

					qsf::TransformComponent& transformComponent = mBombEntity->getOrCreateComponentSafe<qsf::TransformComponent>();
					transformComponent.setPosition(EntityHelper(mBombPoint.getSafe()).getPosition());

					BombComponent& bombComponent = mBombEntity->getOrCreateComponentSafe<BombComponent>();
					bombComponent.setTimeRunning(qsf::Time::fromHours(1.0f));   // Bomb gets triggered by the gangster, not by timer; we use the usual logic anyway
					bombComponent.setTimeToDefuse(mTimeToDefuseBomb);
					bombComponent.startBombTimer(*freeplayEvent);

					// This adds the bomb objective
					EventIdComponent::addToEvent(*mBombEntity, *freeplayEvent, eventspreadreason::GANGSTER);

					// TODO(fw): Push the bomb inside the event layer so it gets cleaned up

				}

				mState = STATE_GO_TO_TRIGGER_POINT;
				// Fallthrough by design
			}

			case STATE_GO_TO_TRIGGER_POINT:
			{
				if (qsf::game::DistanceHelper::get2dDistance(entity, mTriggerPoint.getSafe()) > 5.0f)
				{
					actionComponent.pushAction<MoveAction>().init(new qsf::ai::ReachObjectGoal(entity, mTriggerPoint.getSafe(), GotoObjectTargetPointProvider::TARGET_POINT_ID), MovementModes::MOVEMENT_MODE_RUN);
					break;
				}

				actionComponent.pushAction<qsf::WaitAction>().init(mTimeToWaitForExplosion);
				mState = STATE_WAIT_AT_TRIGGER_POINT;
				break;
			}

			case STATE_WAIT_AT_TRIGGER_POINT:
			{
				// Trigger the bomb now
				BombComponent& bombComponent = mBombEntity->getOrCreateComponentSafe<BombComponent>();
				if (bombComponent.isBombActive())	// Already defused?
				{
					bombComponent.explodeBomb();
				}

				// Run away!
				escape(EscapeType::ESCAPE_FOOT_SHORT);
				mState = STATE_ESCAPE;
				break;
			}
		}
	}

	void GangsterBlackoutMini::onUnitsSpotted(std::vector<SpottedUnit>& sightedPoliceUnits)
	{
		// Nothing here
	}

	void GangsterBlackoutMini::onNoUnitsSpotted()
	{
		// Nothing here
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

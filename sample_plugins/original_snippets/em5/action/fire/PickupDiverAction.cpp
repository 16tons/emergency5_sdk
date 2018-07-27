// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/action/fire/PickupDiverAction.h"
#include "em5/action/move/MoveAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/action/fire/EnterBoatAction.h"
#include "em5/action/move/TurnToAction.h"
#include "em5/game/targetpoint/EnterBoatTargetPointProvider.h"
#include "em5/logic/local/firefighters/DiverLogic.h"
#include "em5/map/EntityHelper.h"

#include <qsf_ai/navigation/goal/ReachObjectGoal.h>

#include <qsf/map/Map.h>
#include <qsf/component/link/LinkComponent.h>
#include <qsf/logic/action/ActionComponent.h>
#include <qsf/logic/gamelogic/GameLogicComponent.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const qsf::NamedIdentifier PickupDiverAction::ACTION_ID = "em5::PickupDiverAction";


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	PickupDiverAction::PickupDiverAction() :
		Action(ACTION_ID),
		mCurrentState(STATE_INIT)
	{
		// Nothing here
	}

	PickupDiverAction::~PickupDiverAction()
	{
		// Nothing here
	}

	void PickupDiverAction::init(qsf::Entity& targetEntity)
	{
		mDiverEntity = &targetEntity;
	}

	uint64 PickupDiverAction::getTargetId() const
	{
		return (mDiverEntity.valid() ? mDiverEntity->getId() : qsf::getUninitialized<uint64>());
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Actions methods                   ]
	//[-------------------------------------------------------]
	void PickupDiverAction::serialize(qsf::BinarySerializer& serializer)
	{
		// TODO(mk) Serialize this class
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	bool PickupDiverAction::onStartup()
	{
		if (!mDiverEntity.valid())
			return false;

		// Some diver carry drowning persons
		const uint64 carryEntityId = EntityHelper(mDiverEntity.getSafe()).getCarryTarget(EntityHelper::CARRY_PERSON);
		mCarryTarget = getMap().getEntityById(carryEntityId);

		return true;
	}

	qsf::action::Result PickupDiverAction::updateAction(const qsf::Clock&)
	{
		if (mDiverEntity.valid())
		{
			switch (mCurrentState)
			{
				case STATE_INIT:
				{
					qsf::ActionComponent& targetActionComponent = mDiverEntity->getOrCreateComponentSafe<qsf::ActionComponent>();

					targetActionComponent.clearPlan();
					targetActionComponent.pushAction<MoveAction>(action::BLOCKING).init(new qsf::ai::ReachObjectGoal(mDiverEntity.getSafe(), getEntity(), EnterBoatTargetPointProvider::TARGET_POINT_ID));
					targetActionComponent.pushAction<TurnToAction>(action::BLOCKING).init(getEntityId());

					// Use this delay here to have the optimal position in case the diver places his carry target
					mCurrentState = STATE_MOVE_TO_OPTIMAL_POSITION;
					return qsf::action::RESULT_CONTINUE;
				}

				case STATE_MOVE_TO_OPTIMAL_POSITION:
				{
					if (mCarryTarget.valid())
					{
						// Prepare the diver
						mDiverEntity->destroyComponent<qsf::LinkComponent>();
						const qsf::GameLogicComponent* gameLogicComponent = mDiverEntity->getComponent<qsf::GameLogicComponent>();
						if (nullptr != gameLogicComponent)
						{
							DiverLogic* diverLogic = gameLogicComponent->getGameLogic<DiverLogic>();
							if (nullptr != diverLogic)
							{
								diverLogic->setCarryPerson(qsf::getUninitialized<uint64>());
							}
						}

						// The carry target is optional and handled first
						qsf::ActionComponent& carryTargetActionComponent = mCarryTarget->getOrCreateComponentSafe<qsf::ActionComponent>();
						carryTargetActionComponent.pushAction<EnterBoatAction>(action::BLOCKING, qsf::action::INSERT_AT_FRONT).init(getEntity());	// Insert be for the (possible) injured action
						mCurrentState = STATE_PLACE_CARRY_PERSON;
					}
					else
					{
						// Pick up the diver
						qsf::ActionComponent& targetActionComponent = mDiverEntity->getOrCreateComponentSafe<qsf::ActionComponent>();
						targetActionComponent.pushAction<EnterBoatAction>(action::BLOCKING).init(getEntity());
						mCurrentState = STATE_ENTER_BOAT;
					}

					return qsf::action::RESULT_CONTINUE;
				}

				case STATE_PLACE_CARRY_PERSON:
				{
					qsf::ActionComponent& carryTargetActionComponent = mCarryTarget->getOrCreateComponentSafe<qsf::ActionComponent>();
					if (nullptr != carryTargetActionComponent.getAction<EnterBoatAction>())
					{
						// Wait till the carry target is entered the boat
						return qsf::action::RESULT_CONTINUE;
					}

					// Pick up the diver
					qsf::ActionComponent& targetActionComponent = mDiverEntity->getOrCreateComponentSafe<qsf::ActionComponent>();
					targetActionComponent.pushAction<EnterBoatAction>(action::BLOCKING).init(getEntity());

					mCurrentState = STATE_ENTER_BOAT;
					return qsf::action::RESULT_CONTINUE;
				}

				case STATE_ENTER_BOAT:
				{
					const qsf::ActionComponent& targetActionComponent = mDiverEntity->getOrCreateComponentSafe<qsf::ActionComponent>();
					if (nullptr != targetActionComponent.getAction<EnterBoatAction>())
					{
						return qsf::action::RESULT_CONTINUE;
					}
				}
			}
		}
		return qsf::action::RESULT_DONE;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

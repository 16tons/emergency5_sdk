// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/action/fire/FireHoseCheckAction.h"
#include "em5/action/fire/DisconnectHydrantAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/action/move/MoveAction.h"
#include "em5/action/move/TurnToAction.h"
#include "em5/fire/FireHelper.h"
#include "em5/logic/local/firefighters/FiremanLogic.h"
#include "em5/game/targetpoint/HydrantTargetPointProvider.h"
#include "em5/game/units/UnitHelper.h"
#include "em5/specs/FireSpecsGroup.h"

#include <qsf_ai/navigation/goal/ReachObjectGoal.h>
#include <qsf_ai/navigation/goal/ReachSinglePointGoal.h>

#include <qsf/logic/gamelogic/GameLogicComponent.h>
#include <qsf/logic/action/ActionComponent.h>
#include <qsf/map/Map.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const qsf::NamedIdentifier FireHoseCheckAction::ACTION_ID = "em5::FireHoseCheckAction";


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	FireHoseCheckAction::FireHoseCheckAction() :
		Action(ACTION_ID),
		mTargetEntityId(qsf::getUninitialized<uint64>()),
		mDisconnect(false)
	{
		// Nothing to do in here
	}

	FireHoseCheckAction::~FireHoseCheckAction()
	{
		// Nothing to do in here
	}

	void FireHoseCheckAction::init(const qsf::Entity& targetEntity, bool disconnect)
	{
		mDisconnect = disconnect;
		mTargetEntityId = targetEntity.getId();
	}

	void FireHoseCheckAction::init(const glm::vec3& targetPosition, bool disconnect)
	{
		mDisconnect = disconnect;
		mTargetPosition = targetPosition;
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Actions methods                   ]
	//[-------------------------------------------------------]
	void FireHoseCheckAction::serialize(qsf::BinarySerializer& serializer)
	{
		serializer.serialize(mDisconnect);
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	bool FireHoseCheckAction::onStartup()
	{
		// Check first if the fireman is connected to the hydrant

		// Get the game logic component
		const qsf::GameLogicComponent* gameLogicComponent = getEntity().getComponent<qsf::GameLogicComponent>();
		if (nullptr != gameLogicComponent)
		{
			// Get the fireman logic
			const FiremanLogic* firemanLogic = gameLogicComponent->getGameLogic<FiremanLogic>();
			return (nullptr != firemanLogic && firemanLogic->isConnectedToHydrant());
		}

		// There is no hydrant connection to care about
		return false;
	}

	qsf::action::Result FireHoseCheckAction::updateAction(const qsf::Clock&)
	{
		qsf::ActionComponent* actionComponent = getEntity().getComponent<qsf::ActionComponent>();
		if (nullptr == actionComponent)
		{
			return qsf::action::RESULT_DONE;
		}

		// Get the game logic component
		const qsf::GameLogicComponent* gameLogicComponent = getEntity().getComponent<qsf::GameLogicComponent>();
		if (nullptr == gameLogicComponent)
		{
			return qsf::action::RESULT_DONE;
		}

		// Get the fireman logic
		FiremanLogic* firemanLogic = gameLogicComponent->getGameLogic<FiremanLogic>();
		if (nullptr == firemanLogic)
		{
			return qsf::action::RESULT_DONE;
		}

		// Get the hydrant entity
		const qsf::Entity* hydrantEntity = firemanLogic->getConnectedHydrant();
		if (nullptr == hydrantEntity)
		{
			return qsf::action::RESULT_DONE;
		}

		// Proof the range of the target
		if (!mDisconnect)
		{
			if (qsf::isInitialized(mTargetEntityId))
			{
				// Get the target entity
				const qsf::Entity* targetEntity = getMap().getEntityById(mTargetEntityId);
				if (nullptr == targetEntity)
				{
					return qsf::action::RESULT_DONE;
				}

				// Check if the new target is in range
				if (!FireHelper::isHydrantInRange(*hydrantEntity, *targetEntity, FireSpecsGroup::getInstanceSafe().getRangeHydrant()))
				{
					mDisconnect = true;
				}
			}
			else
			{
				// In medival setting we have wells as hydrants, with a hose range of 1.0 (only minor movement is allowed)
				float rangeHydrant = FireSpecsGroup::getInstanceSafe().getRangeHydrant();
				if (UnitHelper(getEntity()).isMedievalGuardsman())
				{
					rangeHydrant = 1.0f;
				}

				// Check if the new target is in range
				if (!FireHelper::isHydrantInRange(*hydrantEntity, mTargetPosition, rangeHydrant))
				{
					mDisconnect = true;
				}
			}
		}

		// Check if it's necessary to disconnect
		if (mDisconnect)
		{
			// Reverse order of action stack
			actionComponent->pushAction<DisconnectHydrantAction>(action::BLOCKING, qsf::action::INSERT_AT_FRONT).init(*hydrantEntity);
			actionComponent->pushAction<TurnToAction>(action::BLOCKING, qsf::action::INSERT_AT_FRONT).init(hydrantEntity->getId());
			actionComponent->pushAction<MoveAction>(action::COMMAND_LOW, qsf::action::INSERT_AT_FRONT).init(new qsf::ai::ReachObjectGoal(getEntity(), *hydrantEntity, HydrantTargetPointProvider::TARGET_POINT_ID));

			// Get the fire hose
			SimpleFireHoseComponent* firehoseComponent = firemanLogic->getFireHose();
			if (nullptr != firehoseComponent)
			{
				std::vector<glm::vec3> waypointPositions;
				firehoseComponent->getWaypoints(waypointPositions);

				for (const glm::vec3& waypointPosition : waypointPositions)
				{
					actionComponent->pushAction<MoveAction>(action::COMMAND_LOW, qsf::action::INSERT_AT_FRONT).init(new qsf::ai::ReachSinglePointGoal(qsf::logic::TargetPoint(waypointPosition, false, 0.0f, 1.0f)));
				}
			}
		}

		// Done
		return qsf::action::RESULT_DONE;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

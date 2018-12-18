// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/action/police/SEK/LoadRobotToTransporterAction.h"
#include "em5/action/police/SEK/WaitForRobotAction.h"
#include "em5/action/base/EnterVehicleAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/component/vehicle/VehicleComponent.h"
#include "em5/map/EntityHelper.h"

#include <qsf/math/Math.h>
#include <qsf/component/base/TransformComponent.h>
#include <qsf/component/move/MovableComponent.h>
#include <qsf/link/link/prototype/ContainerLink.h>
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
	const qsf::NamedIdentifier LoadRobotToTransporterAction::ACTION_ID = "em5::LoadRobotToTransporterAction";


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	LoadRobotToTransporterAction::LoadRobotToTransporterAction() :
		MoveAction(),
		mCurrentState(STATE_INIT),
		mTargetEntityId(qsf::getUninitialized<uint64>())
	{
		// Nothing here
	}

	LoadRobotToTransporterAction::~LoadRobotToTransporterAction()
	{
		// Nothing here
	}

	void LoadRobotToTransporterAction::init(const qsf::Entity& targetEntity, qsf::ai::NavigationGoal* goal, uint32 movementModeId, qsf::ai::Path* path)
	{
		// Call base implementation
		MoveAction::init(goal, movementModeId, path);

		mTargetEntityId = targetEntity.getId();
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Actions methods                   ]
	//[-------------------------------------------------------]
	void LoadRobotToTransporterAction::serialize(qsf::BinarySerializer& serializer)
	{
		// Call base implementation
		MoveAction::serialize(serializer);

		serializer.serializeAs<uint16>(mCurrentState);
		serializer.serializeAs<uint64>(mTargetEntityId);
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	bool LoadRobotToTransporterAction::onStartup()
	{
		// Call base class implementation
		if (!MoveAction::onStartup())
			return false;

		return true;
	}

	void LoadRobotToTransporterAction::onShutdown()
	{
		// Call base class implementation
		MoveAction::onShutdown();

		// If interrupted
		if (mCurrentState != STATE_DONE)
		{
			// Push waiting action
			getEntity().getComponentSafe<qsf::ActionComponent>().pushAction<WaitForRobotAction>(action::Priority::BLOCKING).init(mTargetEntityId);
		}

		return;
	}

	qsf::action::Result LoadRobotToTransporterAction::updateAction(const qsf::Clock& clock)
	{

		// Get the most important objects first
		VehicleComponent* vehicleComponent = getEntity().getComponent<VehicleComponent>();
		if (nullptr == vehicleComponent)
			return qsf::action::RESULT_DONE;

		// Handle the current state
		switch (mCurrentState)
		{
			case STATE_INIT:
			{
				// Nothing to do

				// Immediately go to the next state; no break by intent
				mCurrentState = STATE_MOVE;
			}

			case STATE_MOVE:
			{
				if (MoveAction::updateAction(clock) == qsf::action::RESULT_DONE)
				{
					mCurrentState = STATE_LOAD;
				}
				else
				{
					return qsf::action::RESULT_CONTINUE;
				}

				// No break here by intent
			}

			case STATE_LOAD:
			{
				// Push action enter vehicle
				qsf::Entity* targetEntity = getMap().getEntityById(mTargetEntityId);
				if (nullptr != targetEntity)
				{
					targetEntity->getComponentSafe<qsf::ActionComponent>().pushAction<EnterVehicleAction>(action::Priority::BLOCKING).init(getEntity(), DoorComponent::DOORTYPE_TRUNK);
				}

				// Immediately go to the next state; no break by intent
				mCurrentState = STATE_DONE;
			}

			case STATE_DONE:
			{
				// Done
				return qsf::action::RESULT_DONE;
			}
		}

		return qsf::action::RESULT_DONE;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

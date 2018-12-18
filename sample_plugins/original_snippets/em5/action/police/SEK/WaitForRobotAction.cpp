// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/action/base/EnterVehicleAction.h"
#include "em5/action/police/SEK/WaitForRobotAction.h"
#include "em5/plugin/Messages.h"

#include <qsf/math/Math.h>
#include <qsf/component/link/LinkComponent.h>
#include <qsf/component/rotation/LinearRotationComponent.h>
#include <qsf/logic/action/ActionComponent.h>
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
	const qsf::NamedIdentifier WaitForRobotAction::ACTION_ID = "em5::WaitForRobotAction";


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	WaitForRobotAction::WaitForRobotAction() :
		Action(ACTION_ID),
		mIsRobotAlive(true),
		mCurrentState(STATE_INIT),
		mTargetEntityId(qsf::getUninitialized<uint64>())
	{
		// Nothing here
	}

	WaitForRobotAction::~WaitForRobotAction()
	{
		// Nothing here
	}

	void WaitForRobotAction::init(uint64 entityId)
	{
		mTargetEntityId = entityId;
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Actions methods                   ]
	//[-------------------------------------------------------]
	void WaitForRobotAction::serialize(qsf::BinarySerializer& serializer)
	{
		serializer.serializeAs<bool>(mIsRobotAlive);
		serializer.serializeAs<uint16>(mCurrentState);
		serializer.serializeAs<uint16>(mTargetEntityId);
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	bool WaitForRobotAction::onStartup()
	{
		qsf::Entity* targetEntity = getMap().getEntityById(mTargetEntityId);
		if (nullptr == targetEntity)
			return false;

		qsf::MessageConfiguration messageVehicleDestroyed(Messages::EM5_VEHICLE_DESTROYED, mTargetEntityId);
		mMessageProxyVehicleDestroyed.registerAt(messageVehicleDestroyed, boost::bind(&WaitForRobotAction::onRobotDestroyed, this, _1));

		qsf::LinkComponent& linkComponent = getEntity().getComponentSafe<qsf::LinkComponent>();

		// Start rotating antenna
		qsf::LinearRotationComponent* linearRotationComponent = linkComponent.getComponentFromEntityOrLinkedChild<qsf::LinearRotationComponent>();
		if (nullptr != linearRotationComponent)
		{
			linearRotationComponent->setActive(true);
		}

		return true;
	}

	void WaitForRobotAction::onShutdown()
	{
		qsf::LinkComponent& linkComponent = getEntity().getComponentSafe<qsf::LinkComponent>();

		// Stop rotating antenna
		qsf::LinearRotationComponent* linearRotationComponent = linkComponent.getComponentFromEntityOrLinkedChild<qsf::LinearRotationComponent>();
		if (nullptr != linearRotationComponent)
		{
			linearRotationComponent->setActive(false);
		}
	}

	qsf::action::Result WaitForRobotAction::updateAction(const qsf::Clock&)
	{
		// Handle the current state
		switch (mCurrentState)
		{
			case STATE_INIT:
			{
				mCurrentState = STATE_WAITING;
			}

			case STATE_WAITING:
			{
				// Check if SEK robot entity exists
				qsf::Entity* targetEntity = getMap().getEntityById(mTargetEntityId);
				if (nullptr == targetEntity)
					return qsf::action::RESULT_CONTINUE;

				// Check if SEK robot is intact
				if (mIsRobotAlive)
					return qsf::action::RESULT_CONTINUE;

				// If target has enter vehicle action
				if (checkTargetActionPlan())
					return qsf::action::RESULT_DONE;

				mCurrentState = STATE_DONE;
			}

			case STATE_DONE:
			{
				// TODO(db): Remove ControlComponent
				// Return to Base Action

				// Done
				return qsf::action::RESULT_DONE;
			}
		}
		return qsf::action::RESULT_DONE;
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void WaitForRobotAction::onRobotDestroyed(const qsf::MessageParameters&)
	{
		mIsRobotAlive = false;
	}

	bool WaitForRobotAction::checkTargetActionPlan()
	{
		// Access the targets's action plan
		qsf::ActionComponent* actionComponent = getMap().getEntityById(mTargetEntityId)->getComponent<qsf::ActionComponent>();

		if (nullptr == actionComponent)
			return false;

		// Check if the target has the right enter vehicle action
		const EnterVehicleAction* enterVehicleAction = actionComponent->getAction<EnterVehicleAction>();
		if (nullptr != enterVehicleAction && enterVehicleAction->getTargetId() == getEntityId() && enterVehicleAction->getTargetDoorType() == DoorComponent::DOORTYPE_TRUNK)
		{
			// Don't push the same command twice
			return true;
		}

		return false;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

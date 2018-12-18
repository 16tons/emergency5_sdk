// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/action/base/WaitForEnterVehicleAction.h"
#include "em5/action/base/EnterVehicleAction.h"
#include "em5/base/ContainerCategory.h"
#include "em5/component/vehicle/VehicleComponent.h"
#include "em5/game/selection/SelectionManager.h"
#include "em5/game/Game.h"
#include "em5/map/EntityHelper.h"
#include "em5/plugin/Messages.h"
#include "em5/EM5Helper.h"

#include <qsf/map/Map.h>
#include <qsf/logic/action/ActionComponent.h>
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
	const qsf::NamedIdentifier WaitForEnterVehicleAction::ACTION_ID = "em5::WaitForEnterVehicleAction";


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	WaitForEnterVehicleAction::WaitForEnterVehicleAction() :
		Action(ACTION_ID),
		mTargetEntityId(qsf::getUninitialized<uint64>()),
		mBeamInVehicle(false)
	{
		// Nothing here
	}

	WaitForEnterVehicleAction::~WaitForEnterVehicleAction()
	{
		// Nothing to do in here
	}

	void WaitForEnterVehicleAction::init(uint64 entityId, qsf::Time maxWaitTime, bool beamInVehicle)
	{
		mTargetEntityId = entityId;
		mMaxWaitTime = maxWaitTime;
		mBeamInVehicle = beamInVehicle;
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Actions methods                   ]
	//[-------------------------------------------------------]
	void WaitForEnterVehicleAction::serialize(qsf::BinarySerializer& serializer)
	{
		serializer.serialize(mTargetEntityId);
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	bool WaitForEnterVehicleAction::onStartup()
	{
		// Get the caller and target entity
		qsf::Entity& callerEntity = getEntity();

		// Check target
		if (!EntityHelper(callerEntity).isVehicle())
		{
			return false;
		}

		// TODO(mk) Check number of caller seats

		// Check target
		qsf::Entity* targetEntity = callerEntity.getMap().getEntityById(mTargetEntityId);
		if (nullptr == targetEntity)
		{
			return false;
		}
		if (!EntityHelper(*targetEntity).isEntityPerson())
		{
			return false;
		}

		if (mMaxWaitTime < qsf::Time::ZERO)
		{
			mMaxWaitTime = qsf::Time::ZERO;
		}

		return true;
	}

	qsf::action::Result WaitForEnterVehicleAction::updateAction(const qsf::Clock& clock)
	{
		if (mMaxWaitTime > qsf::Time::ZERO)
		{
			mMaxWaitTime -= clock.getTimePassed();
			if (mMaxWaitTime <= qsf::Time::ZERO)
			{
				beamPersonInVehicle();
				return qsf::action::RESULT_DONE;
			}
		}

		// Get the target entity instance
		qsf::Entity* targetEntity = getMap().getEntityById(mTargetEntityId);
		if (nullptr != targetEntity)
		{
			const EntityHelper targetEntityHelper(*targetEntity);
			if (targetEntityHelper.isEntityInVehicle() && targetEntityHelper.getContainerEntity() == &getEntity())
			{
				// Action done
				return qsf::action::RESULT_DONE;
			}

			if (nullptr == targetEntity->getComponentSafe<qsf::ActionComponent>().getAction<EnterVehicleAction>())
			{
				// Target don't want to enter the vehicle, abort action
				return qsf::action::RESULT_DONE;
			}

			// Wait for finishing
			return qsf::action::RESULT_CONTINUE;
		}

		return qsf::action::RESULT_DONE;
	}

	void WaitForEnterVehicleAction::beamPersonInVehicle() const
	{
		if (mBeamInVehicle)
		{
			qsf::Entity* targetEntity = getMap().getEntityById(mTargetEntityId);
			if (nullptr == targetEntity)
				return;	// Error

			// Clear the actions of the target (person)
			targetEntity->getOrCreateComponentSafe<qsf::ActionComponent>().clearPlan();

			// Via entering container we lose the selection
			container::ContainerTypes containerType = (EntityHelper(*targetEntity).isCommandable() ? container::CONTAINERTYPE_SQUAD : container::CONTAINERTYPE_PASSENGER);	// The robot is a squad road vehicle
			EntityHelper(*targetEntity).enterContainer(getEntity(), containerType);

			// Because we are an vehicle and we may stuck in very uncomfortable situations, close all doors for safeties
			VehicleComponent* vehicleComponent = getEntity().getComponent<VehicleComponent>();
			if (nullptr != vehicleComponent)
			{
				vehicleComponent->closeAllDoors();
			}

			// Handling for selection, in case only the entering person is selected, select the vehicle after this
			SelectionManager& selectionManager = EM5_GAME.getSelectionManager();
			const bool selectVehicleAfterThis = (selectionManager.isIdSelected(mTargetEntityId) && selectionManager.getSelectionSize() == 1);
			if (selectVehicleAfterThis)
			{
				selectionManager.addIdToSelection(mTargetEntityId);
				QSF_MESSAGE.emitMessage(qsf::MessageConfiguration(Messages::EM5_CHANGE_SELECTION));
			}

			QSF_MESSAGE.emitMessage(qsf::MessageConfiguration(Messages::EM5_ACTION_PERSON_ENTER_VEHICLE, mTargetEntityId, getEntityId()));
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

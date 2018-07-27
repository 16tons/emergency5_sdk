// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/action/fire/DecontaminatePersonHelperAction.h"
#include "em5/action/fire/DecontaminatePersonAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/specs/RoadVehicleSpecsGroup.h"

#include <qsf/map/Map.h>
#include <qsf/logic/action/ActionComponent.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const qsf::NamedIdentifier DecontaminatePersonHelperAction::ACTION_ID = "em5::DecontaminatePersonHelperAction";


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	DecontaminatePersonHelperAction::DecontaminatePersonHelperAction() :
		Action(ACTION_ID),
		mVehicleEntityId(qsf::getUninitialized<uint64>())
	{
		// Nothing here
	}

	void DecontaminatePersonHelperAction::init(uint64 vehicleEntityId)
	{
		mVehicleEntityId = vehicleEntityId;
	}

	uint64 DecontaminatePersonHelperAction::getVehicleEntityId() const
	{
		return mVehicleEntityId;
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Actions methods                   ]
	//[-------------------------------------------------------]
	void DecontaminatePersonHelperAction::serialize(qsf::BinarySerializer& serializer)
	{
		serializer.serialize(mVehicleEntityId);
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	qsf::action::Result DecontaminatePersonHelperAction::updateAction(const qsf::Clock&)
	{
		// Get the target entity instance
		const qsf::Entity* vehicleEntity = getMap().getEntityById(mVehicleEntityId);
		QSF_CHECK(nullptr != vehicleEntity, "The EM5 decontaminate person helper action was unable to retrieve the vehicle entity instance", return qsf::action::RESULT_DONE);

		qsf::ActionComponent* actionComponentVehicle = vehicleEntity->getComponent<qsf::ActionComponent>();
		if (nullptr != actionComponentVehicle)
		{
			const RoadVehicleSpecsGroup& roadVehicleSpecsGroup = RoadVehicleSpecsGroup::getInstanceSafe();
			const qsf::Time decontaminationTime = qsf::Time::fromSeconds(roadVehicleSpecsGroup.getDecontaminationTime());

			actionComponentVehicle->pushAction<DecontaminatePersonAction>(action::BLOCKING, qsf::action::APPEND_TO_BACK).init(getEntity(), decontaminationTime);
		}

		return qsf::action::RESULT_DONE;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

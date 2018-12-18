// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/event/standard/medical/ComplexMedicalEmergencyEvent.h"
#include "em5/freeplay/objective/ObjectiveHelper.h"
#include "em5/logic/ObserverHelper.h"
#include "em5/logic/observer/PersonDiedObserver.h"

#include <qsf/map/Entity.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 ComplexMedicalEmergencyEvent::FREEPLAY_EVENT_ID = qsf::StringHash("em5::ComplexMedicalEmergencyEvent");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	ComplexMedicalEmergencyEvent::ComplexMedicalEmergencyEvent() :
		mMaxPersons(0)
	{
		// Nothing here
	}

	ComplexMedicalEmergencyEvent::~ComplexMedicalEmergencyEvent()
	{
		// Nothing here
	}

	uint32 ComplexMedicalEmergencyEvent::getMaxPersons() const
	{
		return mMaxPersons;
	}

	void ComplexMedicalEmergencyEvent::setMaxPersons(uint32 maxPersons)
	{
		mMaxPersons = maxPersons;
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::FreeplayEvent methods             ]
	//[-------------------------------------------------------]
	bool ComplexMedicalEmergencyEvent::onStartup()
	{
		setGreatJob(true);

		return MedicalEmergencyEvent::onStartup();
	}

	void ComplexMedicalEmergencyEvent::updateFreeplayEvent(const qsf::Time& timePassed)
	{
		ObjectiveHelper objectiveHelper(*this);

		// If there is a person to treat its no great job anymore
		const Objective* treatPersons = objectiveHelper.getObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_NEED_TREATPERSONS);
		if (nullptr != treatPersons)
		{
			setGreatJob(false);
		}

		// Check objectives for success or failure
		checkObjectivesState();
	}

	bool ComplexMedicalEmergencyEvent::addEntityToEvent(qsf::Entity& targetEntity, eventspreadreason::Reason eventSpreadReason, bool newReason)
	{
		ObserverHelper observerHelper(*this);
		if (eventspreadreason::INJURY == eventSpreadReason)
		{
			// Get or create fail conditions
			Objective& failConditionPersonDiedObjective = ObjectiveHelper(*this).getOrCreateObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_FAIL_DEADPERSONS);

			// Create and connect observers
			PersonDiedObserver& observerDied = observerHelper.createObserver<PersonDiedObserver>(targetEntity.getId());
			observerDied.connectToObjective(failConditionPersonDiedObjective);

			// Call base implementation
			FreeplayEvent::addEntityToEvent(targetEntity, eventSpreadReason, newReason);

			return true;
		}

		// Call base implementation
		return FreeplayEvent::addEntityToEvent(targetEntity, eventSpreadReason, newReason);
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::MedicalEmergencyEvent methods     ]
	//[-------------------------------------------------------]
	void ComplexMedicalEmergencyEvent::serialize(qsf::BinarySerializer& serializer)
	{
		// Call base Implementation
		MedicalEmergencyEvent::serialize(serializer);

		serializer.serialize(mMaxPersons);
		serializer.serialize(mInfectiveEntities);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

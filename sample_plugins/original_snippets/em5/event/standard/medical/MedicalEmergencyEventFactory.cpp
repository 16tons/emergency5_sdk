// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/event/standard/medical/MedicalEmergencyEventFactory.h"
#include "em5/event/standard/medical/MedicalEmergencyEvent.h"
#include "em5/event/EventHelper.h"
#include "em5/base/ComponentsRandomEnumerator.h"
#include "em5/map/EntityHelper.h"

#include <qsf/QsfHelper.h>
#include <qsf/map/Entity.h>
#include <qsf/map/query/ComponentMapQuery.h>
#include <qsf/math/Random.h>
#include <qsf/reflection/CampHelper.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	namespace detail
	{
		bool checkCandidateForMedicalEmergency(qsf::Entity& entity, const MedicalEmergencyEventFactory::AgeBitmask& ageBitmask, bool insideBuilding)
		{
			EntityHelper entityHelper(entity);

			if (!entityHelper.isEntityValidEventTarget())
				return false;

			// Check if entity is a correctly initialized person
			if (!entityHelper.isCivilPerson())
				return false;

			// Check if person is still healthy and not injured
			if (entityHelper.isPersonInjured())
				return false;

			// Check if person has a spawn group component (all spawned civilists have one; this excludes pre-placed persons like construction site workers)
			if (nullptr == entity.getComponentById("em5::SpawnGroupComponent"))
				return false;

			// Check person's age
			if (!ageBitmask.isSet(entity.getComponentSafe<PersonComponent>().getAge()))
				return false;

			if (insideBuilding)
			{
				// Check if container is a building
				if (!entityHelper.isEntityInBuilding())
					return false;
			}
			else
			{
				// There is no container allowed!
				if (nullptr != entityHelper.getContainerEntity())
					return false;

				// Is target hardlinked (e.g picked up by paramedic)
				if (entityHelper.isEntityHardLinked())
					return false;
			}

			// Accepted
			return true;
		}

	}


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	qsf::Entity* MedicalEmergencyEventFactory::getRandomCandidatePerson(const AgeBitmask* ageBitmask, bool insideBuilding)
	{
		// Setup age bitmask
		AgeBitmask myAgeBitmask;
		if (nullptr != ageBitmask)
		{
			myAgeBitmask = *ageBitmask;
		}
		else
		{
			// No children by default
			myAgeBitmask.set(PersonComponent::Age::AGE_YOUTH);
			myAgeBitmask.set(PersonComponent::Age::AGE_ADULT);
			myAgeBitmask.set(PersonComponent::Age::AGE_SENIOR);
		}

		ComponentsRandomEnumerator<PersonComponent> randomEnumerator(40);	// Max. 40 tries
		while (PersonComponent* personComponent = randomEnumerator.getNext())
		{
			// Check if the person is appropriate
			qsf::Entity& entity = personComponent->getEntity();
			if (detail::checkCandidateForMedicalEmergency(entity, myAgeBitmask, insideBuilding))
			{
				return &entity;
			}
		}

		// None found
		return nullptr;
	}


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	MedicalEmergencyEventFactory::MedicalEmergencyEventFactory() :
		mInsideBuilding(false)
	{
		// Nothing here
	}

	MedicalEmergencyEventFactory::~MedicalEmergencyEventFactory()
	{
		// Nothing here
	}

	std::string MedicalEmergencyEventFactory::getAgeAsString() const
	{
		return qsf::CampHelper::enumBitmaskToString(mAgeBitmask);
	}

	void MedicalEmergencyEventFactory::setAgeAsString(const std::string& jsonAge)
	{
		qsf::CampHelper::enumBitmaskFromString(jsonAge, mAgeBitmask);
	}

	std::string MedicalEmergencyEventFactory::getWeatherAsString() const
	{
		return qsf::CampHelper::enumBitmaskToString(mWeatherBitmask);
	}

	void MedicalEmergencyEventFactory::setWeatherAsString(const std::string& jsonWeather)
	{
		qsf::CampHelper::enumBitmaskFromString(jsonWeather, mWeatherBitmask);
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::FreeplayEventFactory methods      ]
	//[-------------------------------------------------------]
	FreeplayEvent* MedicalEmergencyEventFactory::tryTriggerEvent()
	{
		// Wrong weather early out
		if (!checkWeatherConditions())
		{
			setTriggerFailReason("Wrong weather condition");
			return nullptr;
		}

		qsf::Entity* targetEntity = nullptr;
		qsf::Entity* spawnPointEntity = nullptr;

		// Check if there is an event tag set. Use spawn point
		if (!getEventTag().empty())
		{
			// Find spawn point
			spawnPointEntity = EventHelper::findSpawnPoint(qsf::StringHash(getEventTag()));
			if (nullptr == spawnPointEntity)
			{
				setTriggerFailReason("Could not find spawn point by event tag " + getEventTag());
				return nullptr;
			}

			// Spawn a candidate entity
			targetEntity = EventHelper::spawnEntity(*spawnPointEntity);
			if (nullptr == targetEntity)
			{
				setTriggerFailReason("Could not find spawn entity from spawn point with event tag " + getEventTag());
				return nullptr;
			}
		}

		// Otherwise search an existing person
		else
		{
			// Find a candidate entity
			targetEntity = findCandidate();
			if (nullptr == targetEntity)
			{
				setTriggerFailReason("No valid victim candidate found");
				return nullptr;
			}
		}

		// Create the event instance
		MedicalEmergencyEvent& freeplayEvent = createAndInitializeEvent<MedicalEmergencyEvent>();
		deserializeRandomFreeplayVariant(freeplayEvent);

		// TODO(co) Why is this commented without a commented why it's commented?
		// Lock the spawnpoint with eventid
		//if (nullptr != spawnPointEntity)
		//{
		//	EventIdComponent::addToEvent(*spawnPointEntity, freeplayEvent, eventspreadreason::Reason::EVENT_SPECIFIC);
		//}

		// Initialize the event
		freeplayEvent.setTargetPerson(*targetEntity);

		// Done
		return &freeplayEvent;
	}


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	bool MedicalEmergencyEventFactory::checkWeatherConditions() const
	{
#if 1
		// TODO(fw): For fixed order of events, weather dependency is quite a problem, so I had to completely disable this
		return true;

#else
		// Check for weather conditions
		const qsf::game::WeatherComponent* qsfGameWeatherComponent = QSF_MAINMAP.getCoreEntity().getComponent<qsf::game::WeatherComponent>();
		QSF_CHECK(qsfGameWeatherComponent, "Can't find weather component class; MedicalEmergencyEventFactory method: tryTriggerEvent", QSF_REACT_NONE);
		if (nullptr != qsfGameWeatherComponent)
		{
			// Get event weather from weather component
			const WeatherComponent* weatherComponent = static_cast<const WeatherComponent*>(qsfGameWeatherComponent);
			WeatherComponent::WeatherBitmask eventWeather = weatherComponent->getEventWeather();

			// Is any of the allowed weather states set?
			return mWeatherBitmask.anySet(eventWeather.getStorage());
		}

		// Wrong weather
		return false;
#endif
	}

	qsf::Entity* MedicalEmergencyEventFactory::findCandidate() const
	{
		return getRandomCandidatePerson(&mAgeBitmask, mInsideBuilding);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

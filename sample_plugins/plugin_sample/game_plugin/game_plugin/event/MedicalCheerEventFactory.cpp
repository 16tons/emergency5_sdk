// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "game_plugin/PrecompiledHeader.h"
#include "game_plugin/event/MedicalCheerEventFactory.h"
#include "game_plugin/event/MedicalCheerEvent.h"

#include <em5/component/event/EventIdComponent.h>
#include <em5/event/EventHelper.h>
#include <em5/map/EntityHelper.h>

#include <qsf/QsfHelper.h>
#include <qsf/map/Map.h>
#include <qsf/map/Entity.h>
#include <qsf/map/query/ComponentMapQuery.h>
#include <qsf/math/Random.h>
#include <qsf/reflection/CampHelper.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace user
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	MedicalCheerEventFactory::MedicalCheerEventFactory() :
		mInsideBuilding(false)
	{
		// Nothing here
	}

	MedicalCheerEventFactory::~MedicalCheerEventFactory()
	{
		// Nothing here
	}

	std::string MedicalCheerEventFactory::getAgeAsString() const
	{
		return qsf::CampHelper::enumBitmaskToString(mAgeBitmask);
	}

	void MedicalCheerEventFactory::setAgeAsString(const std::string& jsonAge)
	{
		qsf::CampHelper::enumBitmaskFromString(jsonAge, mAgeBitmask);
	}

	std::string MedicalCheerEventFactory::getWeatherAsString() const
	{
		return qsf::CampHelper::enumBitmaskToString(mWeatherBitmask);
	}

	void MedicalCheerEventFactory::setWeatherAsString(const std::string& jsonWeather)
	{
		qsf::CampHelper::enumBitmaskFromString(jsonWeather, mWeatherBitmask);
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::FreeplayEventFactory methods      ]
	//[-------------------------------------------------------]
	em5::FreeplayEvent* MedicalCheerEventFactory::tryTriggerEvent()
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
			spawnPointEntity = em5::EventHelper::findSpawnPoint(qsf::StringHash(getEventTag()));
			if (nullptr == spawnPointEntity)
			{
				setTriggerFailReason("Could not find spawn point by event tag " + getEventTag());
				return nullptr;
			}

			// Spawn a candidate entity
			targetEntity = em5::EventHelper::spawnEntity(*spawnPointEntity);
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
		MedicalCheerEvent& freeplayEvent = createAndInitializeEvent<MedicalCheerEvent>();
		deserializeRandomFreeplayVariant(freeplayEvent);

		// Initialize the event
		freeplayEvent.setTargetPerson(*targetEntity);

		// Done
		return &freeplayEvent;
	}


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	bool MedicalCheerEventFactory::checkWeatherConditions() const
	{
		return true;
	}

	qsf::Entity* MedicalCheerEventFactory::findCandidate() const
	{
		// Get a listing of all person components
		const qsf::ComponentCollection::ComponentList<em5::PersonComponent>& personComponentsInMap = qsf::ComponentMapQuery(QSF_MAINMAP).getAllInstances<em5::PersonComponent>();
		if (personComponentsInMap.empty())
		{
			// No person there... early out
			return nullptr;
		}

		// Try to find a person we can injure
		std::vector<em5::PersonComponent*> openPersonComponents = personComponentsInMap.copyToVector();
		const size_t numberOfPersonsInMap = openPersonComponents.size();
		const size_t totalTries = 40;
		for (size_t tries = 0; tries < numberOfPersonsInMap; ++tries)
		{
			// Choose a random person component
			qsf::Entity& entity = qsf::Random::getPickOutAnyOf(openPersonComponents)->getEntity();

			// Check if the person is appropriate
			if (checkCandidate(entity))
			{
				return &entity;
			}

			if (tries > totalTries)
				return nullptr;
		}

		// None found
		return nullptr;
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	bool MedicalCheerEventFactory::checkCandidate(qsf::Entity& entity) const
	{
		em5::EntityHelper entityHelper(entity);

		if (!entityHelper.isEntityValidEventTarget())
			return false;

		// Check if entity is a correctly initialized person
		if (!entityHelper.isCivilPerson())
			return false;

		// Check if person is still healthy and not injured
		if (entityHelper.isPersonInjured())
			return false;

		// Check person's age
		if (!mAgeBitmask.isSet(entity.getComponentSafe<em5::PersonComponent>().getAge()))
			return false;

		if (mInsideBuilding)
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


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // user

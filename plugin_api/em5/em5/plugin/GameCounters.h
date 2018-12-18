// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"

#include <qsf/base/NamedIdentifier.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace game
	{
		class GameCounterManager;
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Static EMERGENCY 5 game counters collection class
	*/
	class EM5_API_EXPORT GameCounters
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		// Buildings
		static const qsf::NamedIdentifier BUILDINGS_BURNING;

		// Persons incl. animals
		static const qsf::NamedIdentifier PERSONS_INJURED;
		static const qsf::NamedIdentifier PERSONS_INFECTED;
		static const qsf::NamedIdentifier PERSONS_CONTAMINATED;
		static const qsf::NamedIdentifier PERSONS_DROWNING;
		static const qsf::NamedIdentifier PERSONS_DEAD;
		static const qsf::NamedIdentifier PERSONS_MISSING;
		static const qsf::NamedIdentifier GANGSTERS;
		static const qsf::NamedIdentifier HOSTAGES;
		static const qsf::NamedIdentifier ATTACKING_ANIMALS;

		// Vehicles
		static const qsf::NamedIdentifier VEHICLES_DAMAGED;
		static const qsf::NamedIdentifier VEHICLES_DESTROYED;
		static const qsf::NamedIdentifier VEHICLES_BURNING;

		// Other objects
		static const qsf::NamedIdentifier OBJECTS_BURNING;
		static const qsf::NamedIdentifier OBJECTS_DEFECTIVE;
		static const qsf::NamedIdentifier BOMBS;


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	public:
		static void registerGameCountersAtManager(qsf::game::GameCounterManager& gameCounterManager);
		static void unregisterGameCountersAtManager(qsf::game::GameCounterManager& gameCounterManager);

		static void incrementGameCounter(const qsf::StringHash& id);
		static void decrementGameCounter(const qsf::StringHash& id);
		static int32 getGameCounterValue(const qsf::StringHash& id);


	//[-------------------------------------------------------]
	//[ Private static data                                   ]
	//[-------------------------------------------------------]
	private:
		static bool	mCountersActive;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Default constructor
		*/
		GameCounters();

		/**
		*  @brief
		*    Destructor
		*
		*  @brief
		*    - Not virtual by intent
		*/
		~GameCounters();


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

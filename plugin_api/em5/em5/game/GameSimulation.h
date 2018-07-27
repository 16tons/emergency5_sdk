// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"

#include <qsf/time/Time.h>
#include <qsf/message/MessageProxy.h>

#include <boost/noncopyable.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Clock;
}
namespace em5
{
	class SpawnPoolManager;
	class SpawnGroupManager;
	class SpawnInhabitants;
	class UnitPoolManager;
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
	*    EMERGENCY 5 game simulation class
	*/
	class EM5_API_EXPORT GameSimulation : public boost::noncopyable
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		GameSimulation();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~GameSimulation();

		/**
		*  @brief
		*    Startup the game simulation instance
		*/
		void startup();

		/**
		*  @brief
		*    Shutdown the game simulation instance
		*/
		void shutdown();

		/**
		*  @brief
		*    Update the game simulation clock
		*
		*  @param[in] baseClock
		*    Clock to take time progress from
		*/
		void updateClock(const qsf::Clock& baseClock);

		/**
		*  @brief
		*    Return the game simulation clock
		*/
		const qsf::Clock& getClock() const;

		/**
		*  @brief
		*    Returns the global spawn pool manager instance; do not destroy
		*/
		const SpawnPoolManager& getSpawnPoolManager() const;

		/**
		*  @brief
		*    Returns the global spawn group manager instance; do not destroy
		*/
		SpawnGroupManager& getSpawnGroupManager() const;

		/**
		*  @brief
		*    Returns the global unit pool manager instance; do not destroy
		*/
		const UnitPoolManager& getUnitPoolManager() const;

		//[-------------------------------------------------------]
		//[ Internal                                              ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Update AI navigation component update rates depending on entity type and visibility
		*
		*  @note
		*    - Don't call this method until you really know what you're doing
		*/
		void updateAiLod();


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Update the game simulation
		*/
		void updateSimulation(const qsf::Clock& clock);

		/**
		*  @brief
		*    Handle all important settings at the begin of an new day
		*/
		void updateBeginOfDay();

		/**
		*  @brief
		*    React to game speed change
		*/
		void onGameSpeedChange(const qsf::MessageParameters& parameters);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		qsf::Clock*			mSimulationClock;			///< Game simulation clock; always valid, we have to destroy the instance in case we no longer need it
		qsf::MessageProxy	mGameSpeedChangeListener;	///< Listen to game speed changes
		SpawnPoolManager*	mSpawnPoolManager;			///< Keeps all the spawn pools, i.e. lists for spawning entities
		SpawnGroupManager*	mSpawnGroupManager;			///< Keeps all the spawn groups, i.e. categories for spawned entities
		SpawnInhabitants*   mSpawnInhabitants;			///< Spawn point logic for civil inhabitants
		UnitPoolManager*	mUnitPoolManager;			///< Keeps all the unit pools, i.e. lists of available units to order
		bool				mResetUnitPoolThisDay;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/job/JobProxy.h>
#include <qsf/debug/DebugDrawProxy.h>

#include <boost/noncopyable.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace game
	{
		class WeatherState;
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
	*     Base class for spawn controllers
	*
	*  @todo
	*     - TODO(fw): Get rid of this class, we actually only still need it because of "SpawnInhabitants"
	*/
	class SpawnEntities : public boost::noncopyable
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		SpawnEntities(uint32 jobManagerId);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~SpawnEntities();

		/**
		*  @brief
		*    Enable/disable debug output
		*/
		void setDebug(bool debug);


	//[-------------------------------------------------------]
	//[ Public virtual em5::SpawnEntities methods             ]
	//[-------------------------------------------------------]
	public:
		virtual void createStartupPopulation();


	//[-------------------------------------------------------]
	//[ Protected virtual em5::SpawnEntities methods          ]
	//[-------------------------------------------------------]
	protected:
		virtual uint32 getCurrentEntities() { return 0; }
		virtual uint32 getMaximumEntities() { return 0; }
		virtual void updateSpawning() {}


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		float getCurrentHourOfDay() const;
		const qsf::game::WeatherState* getCurrentWeatherState() const;

		void getStartupPopulationPoints(uint32 count, uint32 worldIndex, uint32 laneTypeId, std::vector<qsf::Transform>& outPoints);


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void updateSpawnPoints();


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		qsf::JobProxy		mSpawnJobProxy;		///< Regular simulation job proxy
		qsf::DebugDrawProxy	mDebugDrawProxy;	///< Debug draw proxy
		bool				mDebugEnabled;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

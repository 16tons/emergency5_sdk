// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf_ai/base/SpatialConfiguration.h>

#include <qsf/logic/gamelogic/GameLogic.h>
#include <qsf/job/JobProxy.h>

#include <boost/optional.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class JobArguments;
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
	*    A game logic that lets entity drive to the side of the road when units on emergency operations ("Alarmfahrt") approach
	*/
	class EvadePlayerUnitsLogic : public qsf::GameLogic
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 GAMELOGIC_TYPE_ID;	// "em5::CarsEvadesPlayerUnitsLogic" game logic type ID


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		EvadePlayerUnitsLogic();

		/**
		*  @brief
		*    Register an approaching vehicle on emergency operations
		*
		*  @param approachTime The relative time from now when the approaching vehicle would be in the vicinity.
		*  The AI tracks whether vehicles come close to each other so that they might need to drive to the side of the road.
		*  There is no exact definition how far they may be apart because this depends on the street editing.
		*/
		void registerApproachFromEmergencyVehicle(qsf::Time approachTime, const qsf::ai::SpatialConfiguration3D& approachConfig);

		/**
		*  @brief
		*    Returns whether there was an approaching entity registered during the last update that was close enough so it should be evaded.
		*/
		bool hasActiveApproach() const;

		/**
		*  @brief
		*    Returns the spatial AI configuration of the last registered approaching player unit on emergency operations.
		*    The configuration is made up of a position and a direction (velocity or facing) on the ground plane.
		*    If hasActiveApproach is not true, the data is usually deprecated or downright invalid.
		*/
		const qsf::ai::SpatialConfiguration3D& getLastApproachConfiguration() const;


	//[-------------------------------------------------------]
	//[ Public virtual qsf::GameLogic methods                 ]
	//[-------------------------------------------------------]
	public:
		virtual bool onStartup() override;
		virtual void onShutdown() override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		// Job updates
		void updateSimulation(const qsf::JobArguments& jobArguments);

		// Core logic that checks whether we should evade right now
		void evadeIfNecessary(const qsf::Time& now);


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		static const uint32 ACTION_PRIORITY;	///< The priority of the actions generated from this logic


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		qsf::JobProxy					mSimulationJobProxy;				///< Regular simulation job proxy; for updates once a game tick
		boost::optional<qsf::Time>		mNearestApproachRegistered;			///< The closest approach by a vehicle on emergency operations registered. This variable is reset after each update.
		qsf::ai::SpatialConfiguration3D mNearestApproachConfig;				///< The position and direction of a vehicle on emergency operations registered. This variable is valid only if mNearestApproachRegistered is set.
		bool							mHadRelevantApproachInLastUpdate;	///< Track whether there was a relevant approach during the last update. This assumes that steering and this game logic have the same update frequency currently.


	//[-------------------------------------------------------]
	//[ CAMP reflection system                                ]
	//[-------------------------------------------------------]
		QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(em5::EvadePlayerUnitsLogic)

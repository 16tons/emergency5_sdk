// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/job/JobProxy.h>
#include <qsf/message/MessageProxy.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace user
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    The GameListener is the main instance in the mod. It gets continuous updates for simulation and animation and handles general game logic for the user mod
	*
	*  @remarks
	*    Listen to the em5 message "GAME_STARTUP_FINISHED" and register update functions on timer
	*    Don't forget to shutdown all initialized classes
	*/
	class GameListener
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		GameListener();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~GameListener();

		void init();


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		//[-------------------------------------------------------]
		//[ Lifecycle                                             ]
		//[-------------------------------------------------------]
		void startup(const qsf::MessageParameters& parameters);
		void shutdown(const qsf::MessageParameters& parameters);

		// Called as job
		void updateSimulation(const qsf::JobArguments& jobArguments);
		void updateAnimationVehicle(const qsf::JobArguments& jobArguments);
		
		
	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Proxy object for scheduling the update process
		qsf::JobProxy	mSimulationJobProxy;
		qsf::JobProxy	mAnimationVehicleJobProxy;

		// Messages for reciving the global life cycle messages
		qsf::MessageProxy		mStartupMessageProxy;
		qsf::MessageProxy		mShutdownMessageProxy;

		// Data
		qsf::Time	mRunningTime;	// The time the mod is active

		// Game logic data
		bool		mCanStartCheeringEvent;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // user

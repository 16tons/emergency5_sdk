// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "game_plugin/PrecompiledHeader.h"
#include "game_plugin/game/GameListener.h"

#include <em5/debug/GameDebugGroup.h>
#include <em5/freeplay/FreeplaySystem.h>
#include <em5/freeplay/factory/FreeplayEventTriggerInfo.h>
#include <em5/plugin/Messages.h>
#include <em5/plugin/Jobs.h>
#include <em5/EM5Helper.h>

#include <qsf/job/JobArguments.h>
#include <qsf/message/MessageConfiguration.h>

#include <boost/bind.hpp>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace user
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	GameListener::GameListener() :
		mCanStartCheeringEvent(true)
	{
		// Nothing here
	}

	GameListener::~GameListener()
	{
		// Unregister the lifetime messages
		mStartupMessageProxy.unregister();
		mShutdownMessageProxy.unregister();
	}

	void GameListener::init()
	{
		// Register the message proxy to the life cycle messages. It is important to have the right time. Otherwise we would start our gamelogic in the main menu
		mStartupMessageProxy.registerAt(qsf::MessageConfiguration(em5::Messages::GAME_STARTUP_FINISHED), boost::bind(&GameListener::startup, this, _1));
		mShutdownMessageProxy.registerAt(qsf::MessageConfiguration(em5::Messages::GAME_SHUTDOWN_STARTING), boost::bind(&GameListener::shutdown, this, _1));
	}


	//[-------------------------------------------------------]
	//[ Lifecycle                                             ]
	//[-------------------------------------------------------]
	void GameListener::startup(const qsf::MessageParameters& parameters)
	{
		// The game is started. Activate our update jobs to listen to game updates
		// Be sure to have the best fitting job to avoid memory overhead. 
		mSimulationJobProxy.registerAt(em5::Jobs::SIMULATION_LOGIC, boost::bind(&GameListener::updateSimulation, this, _1));
		mAnimationVehicleJobProxy.registerAt(em5::Jobs::ANIMATION_VEHICLE, boost::bind(&GameListener::updateAnimationVehicle, this, _1));

		// Deactivate the freeplay system (means no automatic triggering of event)
		em5::GameDebugGroup::getInstanceSafe().setActiveFreeplay(false);
		mRunningTime = qsf::Time::ZERO;
	}

	void GameListener::shutdown(const qsf::MessageParameters& parameters)
	{
		// Free all resources and unregister all job or messages proxies
		mSimulationJobProxy.unregister();
		mAnimationVehicleJobProxy.unregister();

		// We keep the lifetime message proxies alive to register the new game start.
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void GameListener::updateSimulation(const qsf::JobArguments& jobArguments)
	{
		mRunningTime += jobArguments.getTimePassed();

		qsf::Time TIMEFORCHEERINGEVENTSTART(qsf::Time::fromSeconds(10.0f));
		if (mRunningTime > TIMEFORCHEERINGEVENTSTART && mCanStartCheeringEvent)
		{
			// Activate the freeplay system
			em5::GameDebugGroup::getInstanceSafe().setActiveFreeplay(true);
			
			// Manually start a event. The name "CheeringEvent_Definitions/CheeringEvent" the definition of the event in the group and the specific event name
			EM5_FREEPLAY.triggerEventByPath("CheeringEvent_Definitions/CheeringEvent", em5::FreeplayEventTriggerInfo());

			// Do this only once
			mCanStartCheeringEvent = false;
		}
	}
	
	void GameListener::updateAnimationVehicle(const qsf::JobArguments& jobArguments)
	{
		// This is an example function for using other timing.
		// Nothing here
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // user

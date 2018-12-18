// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/freeplay/event/FreeplayEvent.h"
#include "em5/game/gamemode/GameModeType.h"

#include <qsf/base/System.h>
#include <qsf/time/Time.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class BinarySerializer;
}
namespace em5
{
	class FreeplayEventManager;
	class FreeplayEventFactoryManager;
	class FreeplayEventGenerator;
	class FreeplayEventPoolManager;
	class FreeplayEventTriggerInfo;
	class CleanupManager;
	class ObjectiveManager;
	class DensityCalculator;
	class GameMode;
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
	*    EMERGENCY 5 freeplay system
	*/
	class EM5_API_EXPORT FreeplaySystem : public qsf::System
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		typedef std::vector<std::string> EventPoolArray;
		typedef std::unordered_map<std::string, std::string> GameModeClassNameMapping;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		FreeplaySystem();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~FreeplaySystem();

		//[-------------------------------------------------------]
		//[ Lifecycle                                             ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Load freeplay event pool assets (in case you want to do it before the startup already)
		*/
		void loadEventPoolAssets();

		/**
		*  @brief
		*    Initialize and start a new freeplay
		*/
		void startupFreeplay();

		/**
		*  @brief
		*    Shutdown the current freeplay
		*/
		void shutdownFreeplay();

		/**
		*  @brief
		*    Update the running freeplay
		*/
		void updateFreeplay(const qsf::Time& timePassed);

		/**
		*  @brief
		*    Return the game mode class name mapping
		*
		*  @note
		*    - EMERGENCY 5 modifications can use this to change e.g. "em5::MultiplayPvPGameMode" into "my::MyMultiplayGameMode" when creating the game mode instance
		*/
		GameModeClassNameMapping& getGameModeClassNameMapping();

		/**
		*  @brief
		*    Return the game mode instance, which can be a null pointer
		*/
		GameMode* getGameMode() const;

		/**
		*  @brief
		*    Send a signal to the game mode instance, if there is one
		*/
		void sendGameModeSignal(const std::string& gameModeSignal);

		//[-------------------------------------------------------]
		//[ Manager getters                                       ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Get the freeplay event manager instance
		*/
		inline FreeplayEventManager& getEventManager() const;

		/**
		*  @brief
		*    Get the freeplay event factory manager instance
		*/
		inline FreeplayEventFactoryManager& getFactoryManager() const;

		/**
		*  @brief
		*    Get the freeplay event generator instance
		*/
		inline FreeplayEventGenerator& getEventGenerator() const;

		/**
		*  @brief
		*    Get the freeplay event pool manager instance
		*/
		inline FreeplayEventPoolManager& getEventPoolManager() const;

		/**
		*  @brief
		*    Get the objective manager instance
		*/
		inline ObjectiveManager& getObjectiveManager() const;

		/**
		*  @brief
		*    Get the density calculator instance
		*/
		inline CleanupManager& getCleanupManager() const;

		/**
		*  @brief
		*    Get the density calculator instance
		*/
		inline DensityCalculator& getDensityCalculator() const;

		//[-------------------------------------------------------]
		//[ Event management                                      ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Insert a new freeplay event from outside
		*/
		void insertFreeplayEvent(FreeplayEvent& freeplayEvent);

		/**
		*  @brief
		*    Trigger an event by full path to the event
		*
		*  @param[in] eventPath
		*    The event pool name and the event name divided by '/', e.g. "EM5_Simple_MedicalEmergency_Event_Definitions/HeartAttack"
		*
		*  @return
		*    The triggered freeplay event instance if the event could be triggered, null pointer if the event definition was not found or trigger conditions were not met
		*/
		FreeplayEvent* triggerEventByPath(const std::string& eventPath, const FreeplayEventTriggerInfo& triggerInfo);

		/**
		*  @brief
		*    Trigger an event by using the factory from the last triggered event.
		*/
		void triggerLastEvent();

		/**
		*  @brief
		*    Clear current events in the system to set a new pool of events
		*
		*  @param[in] poolsName
		*    A string filled with the keys of used event pools (separator: ",")
		*
		*  @return
		*    "True" when at least one event pool exists otherwise false
		*/
		bool setEventPoolsByName(const std::string& poolsName);

		/**
		*  @brief
		*    Add pool of events to the current events in the system
		*
		*  @param[in] poolsName
		*    A string filled with the keys of used event pools (separator: ",")
		*
		*  @return
		*    "True" when at least one event pool exists otherwise false
		*/
		bool addEventPoolsByName(const std::string& poolsName);

		/**
		*  @brief
		*    Removes pool of events from the current events in the system
		*
		*  @param[in] poolsName
		*    A string filled with the keys of used event pools (separator: ",")
		*
		*  @return
		*    "True" when at least one event pool exists otherwise false
		*/
		bool removeEventPoolsByName(const std::string& poolsName);

		/**
		*  @brief
		*    React to the finishing of an event
		*/
		void onEventFinished(FreeplayEvent& freeplayEvent, FreeplayEvent::EventResult& eventResult);

		void onObjectiveGetsPoints(FreeplayEvent& freeplayEvent, FreeplayEvent::ObjectiveResult& objectiveResult);

		/**
		*  @brief
		*    Break an current event
		*/
		void onAbortEvent(FreeplayEvent& freeplayEvent);

		//[-------------------------------------------------------]
		//[ Serialization                                         ]
		//[-------------------------------------------------------]
		void serialize(qsf::BinarySerializer& serializer);


	//[-------------------------------------------------------]
	//[ Public virtual qsf::System methods                    ]
	//[-------------------------------------------------------]
	public:
		virtual const char* getName() const override;

		//[-------------------------------------------------------]
		//[ Lifecycle                                             ]
		//[-------------------------------------------------------]
		virtual bool onStartup(bool serverMode) override;
		virtual void onShutdown() override;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		EventPoolArray				 mCurrentEventPool;			///< Current event pool
		FreeplayEventManager&		 mEventManager;				///< Event manager
		FreeplayEventFactoryManager& mFactoryManager;			///< Event factory manager
		FreeplayEventGenerator&		 mEventGenerator;			///< Freeplay event generation controller
		FreeplayEventPoolManager&	 mEventPoolManager;			///< Event pool manager
		CleanupManager&				 mCleanupManager;			///< Cleanup manager
		ObjectiveManager&			 mObjectiveManager;			///< Manager for all objectives inside the current freeplay events
		DensityCalculator&			 mDensityCalculator;		///< Calculates the event density

		// Game mode
		GameModeClassNameMapping	 mGameModeClassNameMapping;	///< Game mode class name mapping
		GameMode*					 mGameMode;					///< Game mode instance


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "em5/freeplay/FreeplaySystem-inl.h"

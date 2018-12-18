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
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Static EMERGENCY 5 messages collection class
	*/
	class EM5_API_EXPORT Messages
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		//[-------------------------------------------------------]
		//[ Application                                           ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    "ApplicationStartupSettings" message, before the application finishes the startup; Use this to start modding relevant classes here
		*
		*  @remarks
		*    - No filters
		*    - Startup setting as parameter
		*    - No response
		*/
		static const qsf::NamedIdentifier APPLICATION_STARTUP_SETTINGS;

		/**
		*  @brief
		*    "ApplicationStartupFinished" message, after the application finished the startup; Use this to start modding relevant classes here
		*
		*  @remarks
		*    - No filters
		*    - No parameters
		*    - No response
		*/
		static const qsf::NamedIdentifier APPLICATION_STARTUP_FINISHED;

		/**
		*  @brief
		*    "ApplicationIconSet" message, after the application has set the icon; Use this to start modding relevant classes here
		*
		*  @remarks
		*    - No filters
		*    - No parameters
		*    - No response
		*/
		static const qsf::NamedIdentifier APPLICATION_ICON_SET;

		/**
		*  @brief
		*    "ApplicationShutdownStarting" message, after the application shutdown started; Use this to start modding relevant classes here
		*
		*  @remarks
		*    - No filters
		*    - No parameters
		*    - No response
		*/
		static const qsf::NamedIdentifier APPLICATION_SHUTDOWN_STARTING;

		//[-------------------------------------------------------]
		//[ Main menu                                             ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    "MainMenuShow" message, after the everything relevant to showing the main menu has been done, Use this to start modding relevant classes here
		*
		*  @remarks
		*    - No filters
		*    - No parameters
		*    - No response
		*/
		static const qsf::NamedIdentifier MAIN_MENU_SHOW;

		/**
		*  @brief
		*    "MainMenuHide" message, after the everything relevant to hiding the main menu has been done, Use this to start modding relevant classes here
		*
		*  @remarks
		*    - No filters
		*    - No parameters
		*    - No response
		*/
		static const qsf::NamedIdentifier MAIN_MENU_HIDE;

		//[-------------------------------------------------------]
		//[ Game                                                  ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    "GameStartupFinished" message, after the game finished the startup, Use this to start modding relevant classes here
		*
		*  @remarks
		*    - No filters
		*    - No parameters
		*    - No response
		*/
		static const qsf::NamedIdentifier GAME_STARTUP_FINISHED;

		/**
		*  @brief
		*    "GameShutdownStarting" message, before the game shutdown starts. Use this to deactivate modding relevant classes here
		*
		*  @remarks
		*    - No filters
		*    - No parameters
		*    - No response
		*/
		static const qsf::NamedIdentifier GAME_SHUTDOWN_STARTING;

		//[-------------------------------------------------------]
		//[ Command system                                        ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    "PreCommandExecute" message, emitted just before a command context gets executed
		*
		*  @remarks
		*    - Filters:
		*        (1) Command ID
		*        (2) Caller entity ID (could be uninitialized)
		*        (3) Target entity ID (could be uninitialized)
		*    - Parameters:
		*        "context": The command context, a "qsf::game::CommandContext" pointer
		*    - Response:
		*        Optional bool response; if "true", the command won't get executed
		*/
		static const qsf::NamedIdentifier EM5_PRE_COMMAND_EXECUTE;

		/**
		*  @brief
		*    "PostCommandExecute" message, emitted after a command context got executed
		*
		*  @remarks
		*    - Filters:
		*        (1) Command ID
		*        (2) Caller entity ID (could be uninitialized)
		*        (3) Target entity ID (could be uninitialized)
		*    - No parameters
		*    - No response
		*/
		static const qsf::NamedIdentifier EM5_POST_COMMAND_EXECUTE;

		/**
		*  @brief
		*    "OnCommandExecute" message, emitted before the actions of the command are set
		*
		*  @remarks
		*    - Filters:
		*        (1) Command ID
		*        (2) Caller entity ID (could be uninitialized)
		*        (3) Target entity ID (could be uninitialized)
		*    - No parameters
		*    - Response:
		*        Optional uint32 response; the value of the em5::action::Priority that is to set in the command
		*
		*  @todo
		*    - TODO(jm): Not every command uses these messages!
		*/
		static const qsf::NamedIdentifier EM5_ON_COMMAND_EXECUTE;

		//[-------------------------------------------------------]
		//[ Movement                                              ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    "MovementBlockedToPosition" message, emitted when a unit is ordered to a specific point
		*
		*  @remarks
		*    - Filters:
		*        (1) Caller entity ID
		*    - Parameters:
		*        "caller": The calling entity ID
		*        "position": The target position
		*    - Response:
		*        Optional bool response; if "true", the entity will not move to the target position. If not receiver responds, the target is considered valid
		*/
		static const qsf::NamedIdentifier EM5_MOVEMENT_BLOCKED_TO_POSITION;

		/**
		*  @brief
		*    "ReturnToBaseStart" message, emitted when a unit is returning to base
		*
		*  @remarks
		*    - Filters:
		*        (1) Caller entity ID
		*    - No parameters
		*    - No response
		*/
		static const qsf::NamedIdentifier EM5_MOVEMENT_RETURN_TO_BASE_START;

		//[-------------------------------------------------------]
		//[ Selection                                             ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    "ChangeSelection" message, emitted after the selection has change
		*
		*  @remarks
		*    - No filters
		*    - No parameters
		*    - No response
		*/
		static const qsf::NamedIdentifier EM5_CHANGE_SELECTION;

		/**
		*  @brief
		*    "AddToSelection" message, emitted after one or more entities are added to selection
		*
		*  @remarks
		*    - No filters
		*    - No parameters
		*    - No response
		*/
		static const qsf::NamedIdentifier EM5_ADDTO_SELECTION;

		/**
		*  @brief
		*    "RemoveFromSelection" message, emitted after one or more entities are removed from selection
		*
		*  @remarks
		*    - No filters
		*    - No parameters
		*    - No response
		*/
		static const qsf::NamedIdentifier EM5_REMOVEFROM_SELECTION;

		//[-------------------------------------------------------]
		//[ Player                                                ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    "PlayerCreditsChanged" message, emitted whenever the credits of a player change
		*
		*  @remarks
		*    - No filters
		*    - no parameters
		*    - No response
		*/
		static const qsf::NamedIdentifier EM5_PLAYER_CREDITS_CHANGED;

		/**
		*  @brief
		*    "PlayerScoreChanged" message, emitted whenever the score of a player changes
		*
		*  @remarks
		*    - No filters
		*    - no parameters
		*    - No response
		*/
		static const qsf::NamedIdentifier EM5_PLAYER_SCORE_CHANGED;

		/**
		*  @brief
		*    "PlayerTotalScoreChanged" message, emitted whenever the total score of a player changes
		*
		*  @remarks
		*    - No filters
		*    - No parameters
		*    - No response
		*/
		static const qsf::NamedIdentifier EM5_PLAYER_TOTAL_SCORE_CHANGED;

		/**
		*  @brief
		*    "PlayerMedalUnlocked" message, emitted whenever the player unlocks a new/better medal
		*
		*  @remarks
		*    - Filters:
		*        (1): The medal ID, which is its hashed name
		*    - no parameters
		*    - No response
		*/
		static const qsf::NamedIdentifier EM5_PLAYER_MEDAL_UNLOCKED;

		/**
		*  @brief
		*    "PlayerOnlineAccountDataChanged" message, emitted whenever the online account data of a player changes
		*
		*  @remarks
		*    - No filters
		*    - No parameters
		*    - No response
		*/
		static const qsf::NamedIdentifier EM5_PLAYER_ONLINE_ACCOUNT_DATA_CHANGED;

		//[-------------------------------------------------------]
		//[ Cutscene & Video                                      ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    "CutsceneFinished" message, emitted after a cutscene was finished
		*
		*  @remarks
		*    - No filters
		*    - No parameters
		*    - No response
		*/
		static const qsf::NamedIdentifier EM5_CUTSCENE_FINISHED;

		/**
		*  @brief
		*    "ScreenFadingFinished" message, emitted after an automatic screen fading by the cutscene player was finished
		*
		*  @remarks
		*    - No filters
		*    - No parameters
		*    - No response
		*/
		static const qsf::NamedIdentifier EM5_SCREEN_FADING_FINISHED;

		/**
		*  @brief
		*    "VideoPlaybackFinished" message, emitted when a fullscreen video finished playing
		*
		*  @remarks
		*    - No filters
		*    - No parameters
		*    - No response
		*/
		static const qsf::NamedIdentifier EM5_VIDEO_PLAYBACK_FINISHED;

		/**
		*  @brief
		*    "CutsceneCameraFlightFinished" message, emitted when a cutscene camera flight is done
		*
		*  @remarks
		*    - No filters
		*    - No parameters
		*    - No response
		*/
		static const qsf::NamedIdentifier EM5_CUTSCENE_CAMERA_FLIGHT_FINISHED;

		//[-------------------------------------------------------]
		//[ Freeplay                                              ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    "FreeplayEventTriggered" message, emitted after a event is triggered
		*
		*  @remarks
		*    - No filters
		*    - Parameters
		*        "EventId": ID of the freeplay event, uint32
		*    - No response
		*/
		static const qsf::NamedIdentifier EM5_FREEPLAY_EVENT_TRIGGERED;

		/**
		*  @brief
		*    "FreeplayEventSucceeded" message, after event was successful
		*
		*  @remarks
		*    - No filters
		*    - Parameters
		*        "EventId": ID of the freeplay event, uint32
		*        "EventResult": An pointer to the event result, only valid inside the message handler!
		*        "EventFactory": Name of the original event factory that triggered the event, std::string
		*    - No response
		*/
		static const qsf::NamedIdentifier EM5_FREEPLAY_EVENT_SUCCEEDED;

		/**
		*  @brief
		*    "FreeplayEventFailed" message, after event has failed
		*
		*  @remarks
		*    - No filters
		*    - Parameters
		*        "EventId": ID of the freeplay event, uint32
		*        "EventResult": An pointer to the event result, only valid inside the message handler!
		*        "EventFactory": Name of the original event factory that triggered the event, std::string
		*    - No response
		*/
		static const qsf::NamedIdentifier EM5_FREEPLAY_EVENT_FAILED;

		/**
		*  @brief
		*    "FreeplayEventChanged" message, after event has changed and debug must be updated!
		*
		*  @remarks
		*    - No filters
		*    - Parameters (Not all might be present when message gets emitted):
		*		 "ObjectiveId": Unique ID of the changed objective, a uint32
		*		 "ObjectiveTypeId": Type ID of the changed objective, a uint32
		*		 "EventId": Unique ID of the parent event, a uint32
		*    - No response
		*/
		static const qsf::NamedIdentifier EM5_FREEPLAY_UPDATE_EVENT;

		/**
		*  @brief
		*    "ObjectiveFinished" message, after event has finished an objective
		*
		*  @remarks
		*    - Filters
		*        (1): The ID of the event that the event belongs to
		*		 (2): Type ID of the changed objective
		*    - No parameters
		*    - No response
		*/
		static const qsf::NamedIdentifier EM5_FREEPLAY_OBJECTIVE_FINISHED;

		/**
		*  @brief
		*    "FreeplayReceiveObjectivePoints" message, send if a player finished an objective and receives points for this
		*
		*  @remarks
		*    - No filters
		*    - Parameters:
		*		 "ObjectiveId": Unique ID of the changed objective, a uint32
		*		 "ObjectivePoints": Amount of Points the objective gives, a int
		*		 "EventId": Unique ID of the freeplay event, a uint32
		*    - No response
		*/
		static const qsf::NamedIdentifier EM5_FREEPLAY_RECEIVE_OBJECTIVEPOINTS;

		/**
		*  @brief
		*    "FreeplayEventAssigned" message, after event has assigned to the player!
		*
		*  @remarks
		*    - No filters
		*    - Parameters:
		*		 "PlayerIndex": Index of the player, a unit32
		*		 "EventId": Unique ID of the freeplay event, a uint32
		*    - No response
		*/
		static const qsf::NamedIdentifier EM5_FREEPLAY_EVENT_ASSIGNED;

		/**
		*  @brief
		*    "FreeplayEventLimitChanged" message, after the number of maximum number of possible active event has possible changed.
		*
		*  @remarks
		*    - No filters
		*    - Parameters
		*		 "FreeplayEventLimit": the maximum number of the currently active freeplay events
		*    - No response
		*/
		static const qsf::NamedIdentifier EM5_FREEPLAY_EVENTLIMIT_CHANGED;

		//[-------------------------------------------------------]
		//[ Unit ordering and spawn                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    "SpawnUnit" message, emitted after a unit was spawned and initialized
		*
		*  @remarks
		*    - Filters:
		*        (1) Unit entity ID
		*        (2) Spawnpoint entity ID
		*        (3) Optional user data, "qsf::getUninitialized<uint64>()" if there's no user data
		*    - No parameters
		*    - No response
		*/
		static const qsf::NamedIdentifier EM5_SPAWN_UNIT;

		/**
		*  @brief
		*    "PreOrderUnit" message, emitted before a unit is spawned and initialized
		*
		*  @remarks
		*    - No filters
		*    - Parameters:
		*        "commandContext": The command context
		*        "orderInfoId": The order info ID for the ordered unit
		*		 "orderPairList": The order pair list (contains how much personnel the unit vehicle should have on spawn)
		*    - Response:
		*        Optional bool response; if "true", the ordering won't get executed
		*/
		static const qsf::NamedIdentifier EM5_PRE_ORDER_UNIT;

		/**
		*  @brief
		*    "OrderUnit" message, emitted after a unit was spawned and initialized
		*
		*  @remarks
		*    - Filters:
		*        (1) Unit type
		*    - No parameters
		*    - No response
		*/
		static const qsf::NamedIdentifier EM5_ORDER_UNIT;

		/**
		*  @brief
		*    "UnitSpawnedPlayerUpdateSelection" message, emitted after a unit was spawned and initialized
		*
		*  @remarks
		*    - Filters:
		*        (1) PlayerId
		*        (2) EntityId
		*    - No parameters
		*    - No response
		*/
		static const qsf::NamedIdentifier EM5_UNIT_SPAWNED_PLAYER_UPDATE_SELECTION;

		/**
		*  @brief
		*    "UnitReachedDestionationAfterSpaw" message, emitted after a unit was spawned and reached their destination position
		*
		*  @remarks
		*    - Filters:
		*        (1) Unit type
		*    - No parameters
		*    - No response
		*/
		static const qsf::NamedIdentifier EM5_UNIT_REACHED_DESTINATION_AFTER_SPAWN;

		/**
		*  @brief
		*    "UnitUnregistered" message, emitted after a unit is unregister
		*
		*  @remarks
		*    - No filters
		*    - Parameters:
		*       "EntityId": The ID of the entity
		*    - No response
		*/
		static const qsf::NamedIdentifier EM5_UNIT_UNREGISTERED;

		/**
		*  @brief
		*    "UnitSetup" message, emitted after a unit is setup;
		*    this is called after spawning of units, for both vehicles and their passengers
		*
		*  @remarks
		*    - Filters:
		*        (1) Unit type ID = order info ID
		*        (2) Entity ID
		*    - No parameters
		*    - No response
		*/
		static const qsf::NamedIdentifier EM5_UNIT_SETUP;

		//[-------------------------------------------------------]
		//[ Spawning in general                                   ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    "SpawnEntity" message, emitted after an entity was spawned and initialized
		*
		*  @remarks
		*    - Filters:
		*        (1) Entity ID
		*    - No parameters
		*    - No response
		*/
		static const qsf::NamedIdentifier EM5_SPAWN_ENTITY;

		//[-------------------------------------------------------]
		//[ SEK Drone                                             ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    "SekDroneInvalidTargetRequest" message, emitted when a SEK drone is looking for potential targets. This works like kind of a black list,
		*    so by default any entity (with some preconditions) is considered as a target, and any entity that is blacklisted by a responder of this
		*    message is not targeted by the SEK drone.
		*
		*  @remarks
		*    - Filters:
		*        (1) ID of the drone entity
		*        (2) ID of the potential target entity
		*    - No parameters
		*    - Response:
		*        Optional bool response; if "true", the entity will not be considered as drone target. If not receiver responds, the target is considered valid
		*/
		static const qsf::NamedIdentifier EM5_SEK_DRONE_INVALID_TARGET_REQUEST;

		//[-------------------------------------------------------]
		//[ Fire                                                  ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    "StartBurning" message, emitted after a entity with fire component or complex fire component starts burning
		*
		*  @remarks
		*    - Filters:
		*        (1) ID of the entity that started burning
		*    - Parameters:
		*        "SenderID": ID of the sender entity
		*    - No response
		*/
		static const qsf::NamedIdentifier EM5_OBJECT_START_BURNING;

		/**
		*  @brief
		*    "StopBurning" message, emitted after a entity with fire component or complex fire component stops burning
		*
		*  @remarks
		*    - Filters:
		*        (1) ID of the entity that stopped burning
		*    - No parameters
		*    - No response
		*/
		static const qsf::NamedIdentifier EM5_OBJECT_STOP_BURNING;

		/**
		*  @brief
		*    "FireComponentStartBurning" message, emitted after a fire component starts burning
		*
		*  @remarks
		*    - Filters:
		*        (1) ID of the entity holding the fire component
		*    - No parameters
		*    - No response
		*/
		static const qsf::NamedIdentifier EM5_FIRECOMPONENT_START_BURNING;

		/**
		*  @brief
		*    "FireComponentStopBurning" message, emitted after a fire component stops burning
		*
		*  @remarks
		*    - Filters:
		*        (1) ID of the entity holding the fire component
		*    - No parameters
		*    - No response
		*/
		static const qsf::NamedIdentifier EM5_FIRECOMPONENT_STOP_BURNING;

		/**
		*  @brief
		*    "FireReceiverCoolingSourcesChange" message, emitted after a cooling source was added
		*  @remarks
		*    - Filters:
		*        (1) ID of the fire receiver component entity
		*        (2) "true" if the source was added "false" otherwise
		*        (3) ID cooling energy source
		*    - No Parameters
		*    - No response
		*/
		static const qsf::NamedIdentifier EM5_FIRERECEIVERCOMPONENT_COOLINGSOURCES_CHANGE;

		/**
		*  @brief
		*    "FireBuildingStartedBurning" message, emitted after the fire reaction of a building
		*
		*  @remarks
		*    - Filters:
		*        (1) ID of the freeplay event
		*    - Parameters:
		*        (1) "PersonsInside": Are there persons inside the building, a bool
		*        (2) "WavingPerson": Is there a person waving out of the window, a bool
		*    - No response
		*/
		static const qsf::NamedIdentifier EM5_BUILDING_STARTED_BURNING;

		/**
		*  @brief
		*    "BuildingStoppedBurning" message, emitted after the fire reaction of a building
		*
		*  @remarks
		*    - Filters:
		*        (1) ID of the freeplay event
		*        (2) ID of the building entity
		*    - No parameters
		*    - No response
		*/
		static const qsf::NamedIdentifier EM5_BUILDING_STOPPED_BURNING;

		/**
		*  @brief
		*    "BuildingReceivedFireDamage" message, emitted after the fire reaction of a building
		*
		*  @remarks
		*    - Filters:
		*        (1) ID of the building entity
		*    - No parameters
		*    - No response
		*/
		static const qsf::NamedIdentifier EM5_BUILDING_RECEIVED_FIRE_DAMAGE;

		//[-------------------------------------------------------]
		//[ Person health                                         ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    "PersonPreInjured" message, emitted before a person applied an injury
		*
		*  @remarks
		*    - Filters:
		*        (1) injured person's entity ID
		*    - Parameters:
		*        "injuryId" the ID of the injury
		*        "freeplayEventId" the ID of the event
		*    - Response:
		*        Optional bool response; "true" if paramedic team get injured
		*/
		static const qsf::NamedIdentifier EM5_HEALTH_PERSON_PRE_INJURED;

		/**
		*  @brief
		*    "PersonHealed" message, after person was successfully healed
		*
		*  @remarks
		*    - Filters:
		*        (1) entity that was healed entity ID
		*    - No parameters
		*    - No response
		*/
		static const qsf::NamedIdentifier EM5_HEALTH_HEALED_PERSON;

		/**
		*  @brief
		*    "PersonTreated" message, after person was successfully treated
		*
		*  @remarks
		*    - Filters:
		*        (1) ID of the entity that was treated
		*    - No parameters
		*    - No response
		*/
		static const qsf::NamedIdentifier EM5_HEALTH_TREATED_PERSON;

		/**
		*  @brief
		*    "PersonDied" message, after a person died
		*
		*  @remarks
		*    - Filters:
		*        (1) "EntityId": ID of the target entity, a uint64
		*    - No parameters
		*    - No response
		*/
		static const qsf::NamedIdentifier EM5_HEALTH_PERSON_DIED;

		//[-------------------------------------------------------]
		//[ Person magnet                                         ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    "PreStartFollowPersonMagnet" message, right before a person starts following a person magnet
		*
		*  @remarks
		*    - Filters:
		*        (1) "EntityId": ID of the target entity, a uint64
		*    - No parameters
		*    - No response
		*/
		static const qsf::NamedIdentifier EM5_MAGNET_PRE_START_FOLLOW;

		/**
		*  @brief
		*    "StopFollowPersonMagnet" message, after a person stopped following a person magnet
		*
		*  @remarks
		*    - Filters:
		*        (1) "EntityId": ID of the target entity, a uint64
		*    - No parameters
		*    - No response
		*/
		static const qsf::NamedIdentifier EM5_MAGNET_STOP_FOLLOW;

		//[-------------------------------------------------------]
		//[ Timer signals                                         ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    "HintTimerSignal" message, emitted after a timer reached its threshold
		*
		*  @remarks
		*    - Filters:
		*        (1) Observed entity ID
		*        (2) Event ID
		*    - No parameters
		*    - No response
		*/
		static const qsf::NamedIdentifier EM5_HINT_TIMER_SIGNAL;

		/**
		*  @brief
		*    "EventTimerSignal" message, emitted after a timer reached its threshold
		*
		*  @remarks
		*    - Filters:
		*        (1) Event ID
		*        (2) Entity ID
		*    - No Parameters
		*    - No response
		*/
		static const qsf::NamedIdentifier EM5_EVENT_TIMER_SIGNAL;

		/**
		*  @brief
		*    "ActionTimerSignal" message, emitted after a timer reached its threshold
		*
		*  @remarks
		*    - Filters:
		*        (1) Entity ID
		*        (2) Action ID
		*    - No parameters
		*    - No response
		*/
		static const qsf::NamedIdentifier EM5_ACTION_TIMER_SIGNAL;

		/**
		*  @brief
		*    "EntityTimerSignal" message, emitted after a timer reached its threshold
		*
		*  @remarks
		*    - Filters:
		*        (1) Entity ID
		*    - No parameters
		*    - No response
		*/
		static const qsf::NamedIdentifier EM5_ENTITY_TIMER_SIGNAL;

		/**
		*  @brief
		*    "DebugTimerSignal" message, emitted after a timer reached its threshold
		*
		*  @remarks
		*    - Filters:
		*        (1) entity ID
		*    - No parameters
		*    - No response
		*/
		static const qsf::NamedIdentifier EM5_DEBUG_TIMER_SIGNAL;

		/**
		*  @brief
		*    "InfectedTimerSignal" message, emitted after a timer reached its threshold
		*
		*  @remarks
		*    - Filters:
		*        (1) entity ID
		*        (2) injury ID
		*        (3) event ID
		*    - No parameters
		*    - No response
		*/
		static const qsf::NamedIdentifier EM5_INFECTED_TIMER_SIGNAL;

		//[-------------------------------------------------------]
		//[ Observer                                              ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    "InternalObserverMessage" message from an observer, used as an internal message between observers
		*
		*  @remarks
		*    - Filters:
		*        (1) Observer ID
		*    - No parameters
		*    - No response
		*/
		static const qsf::NamedIdentifier EM5_OBSERVER_MESSAGE;

		//[-------------------------------------------------------]
		//[ Command                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    "CommandExtinguishByBoatCheckTarget" message from "em5::ExtinguishWithCannonCommand" for boats, to filter out certain targets
		*
		*  @remarks
		*    - Filters:
		*        (1) Caller entity ID
		*        (2) Target entity ID
		*    - No parameters
		*    - Response:
		*        Optional bool response; "true" if the target should be filtered out
		*/
		static const qsf::NamedIdentifier EM5_COMMAND_EXTINGUISH_BY_BOAT_CHECK_TARGET;

		/**
		*  @brief
		*    "CommandGuidePersonCheckTarget" message from "em5::GuidePlagueVictimsCommand", to allow certain targets
		*
		*  @remarks
		*    - Filters:
		*        (1) Caller entity ID
		*        (2) Target entity ID
		*    - No parameters
		*    - Response:
		*        Optional bool response; "true" if the target should be filtered out
		*/
		static const qsf::NamedIdentifier EM5_COMMAND_GUIDE_PERSON_CHECK_TARGET;


		//[-------------------------------------------------------]
		//[ Exiting and entering                                  ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    "PersonExitVehicle" message, after person exited a vehicle
		*
		*  @remarks
		*    - Filters:
		*        (1) ID of the entity that exited the vehicle
		*        (2) Vehicle entity ID
		*    - No parameters
		*    - No response
		*/
		static const qsf::NamedIdentifier EM5_ACTION_PERSON_EXIT_VEHICLE;

		/**
		*  @brief
		*    "PersonEnterVehicle" message, after person entered a vehicle
		*
		*  @remarks
		*    - Filters:
		*        (1) ID of the entity that entered the vehicle
		*        (2) Vehicle entity ID
		*    - No parameters
		*    - No response
		*/
		static const qsf::NamedIdentifier EM5_ACTION_PERSON_ENTER_VEHICLE;

		/**
		*  @brief
		*    "EnterBoatAction" message, after diver entered a boat
		*
		*  @remarks
		*    - Filters:
		*        (1) boat entity ID
		*        (2) ID of the entity that entered the vehicle
		*    - Parameters:
		*        "boatId": boat ID where, a uint64
		*        "diverId": Diver entity ID, a uint64
		*    - No response
		*/
		static const qsf::NamedIdentifier EM5_ACTION_PERSON_ENTER_BOAT;

		/**
		*  @brief
		*    "PersonPlacedInAmbulance" message, after person was placed in an ambulance
		*
		*  @remarks
		*    - Filters:
		*        (1) ID of the entity that was placed
		*    - Parameters:
		*        "personId": Injured entity ID, a uint64
		*        "vehicleId": Vehicle ID where the gangster is put in, a uint64
		*        "actorId": Entity ID of the ambulance person, a uint64
		*    - No response
		*/
		static const qsf::NamedIdentifier EM5_ACTION_PERSON_PLACED_IN_AMBULANCE;

		/**
		*  @brief
		*    "PersonPlacedInPolice" message, after person was placed in a police vehicle
		*
		*  @remarks
		*    - Filters:
		*		 (1) Policeman entity ID
		*        (2) Road vehicle entity ID
		*    - Parameters:
		*        "personId": Gangster entity ID, a uint64
		*        "vehicleId": Vehicle ID where the gangster is put in, a uint64
		*        "actorId": Policeman entity ID, a uint64
		*    - No response
		*/
		static const qsf::NamedIdentifier EM5_ACTION_PERSON_PLACED_IN_POLICE;

		/**
		*  @brief
		*    "PoliceVehicleDepart" message, after person was placed in a police vehicle
		*
		*  @remarks
		*    - Filters:
		*        (1) Road vehicle entity ID
		*    - No response
		*/
		static const qsf::NamedIdentifier EM5_ACTION_POLICE_VEHICLE_READY_TO_DEPART;

		/**
		*  @brief
		*    "UsedPepperSpray" message, after police man used the pepper spray
		*
		*  @remarks
		*    - Filters:
		*        (1) Target entity ID
		*    - No parameters
		*    - No response
		*/
		static const qsf::NamedIdentifier EM5_ACTION_POLICE_USED_PEPPER_SPRAY;

		/**
		*  @brief
		*    "SquadExitVehicle" message, after squad person exited a vehicle
		*
		*  @remarks
		*    - Filters:
		*        (1) CommandableComponent Unittag
		*    - No parameters
		*    - No response
		*/
		static const qsf::NamedIdentifier EM5_ACTION_SQUAD_EXIT_VEHICLE;

		//[-------------------------------------------------------]
		//[ Actions and events                                    ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    "ExtinguishFireStarted" message, after extinguish the fire of an burning entity was started
		*
		*  @remarks
		*    - Filters:
		*        (1) Burning entity ID
		*    - Parameters
		*        (1) ID of the entity which started extinguishing the fire
		*    - No response
		*/
		static const qsf::NamedIdentifier EM5_ACTION_EXTINGUISH_FIRE_STARTED;

		/**
		*  @brief
		*    "FindHiddenPerson" message, person is hidden (missing)
		*
		*  @remarks
		*    - Filters:
		*        (1) target entity ID
		*        (2) searcher entity ID
		*    - No parameters
		*    - No response
		*/
		static const qsf::NamedIdentifier EM5_ACTION_FIND_HIDDEN_PERSON_WITH_SEK_DRONE;

		/**
		*  @brief
		*    "ReachedHighPriorityTarget" message
		*
		*  @remarks
		*    - Filters:
		*        (1) target entity ID
		*        (2) searcher entity ID
		*    - No parameters
		*    - No response
		*/
		static const qsf::NamedIdentifier EM5_ACTION_REACHED_HIGH_PRIORITY_TARGET_WITH_SEK_DRONE;

		/**
		*  @brief
		*    "FindEntityFromDivingspot" message, person or vehicle is found by diver in divingspot
		*
		*  @remarks
		*    - Filters:
		*        (1) Divingspot entity ID
		*        (2) Divingspot target entity ID
		*    - No parameters
		*    - No response
		*/
		static const qsf::NamedIdentifier EM5_ACTION_FIND_ENTITY_FROM_DIVINGSPOT;

		/**
		*  @brief
		*    "BreakBuildingDoorOpenStart" message, the firefighter begins to break the door of the building
		*
		*  @remarks
		*    - Filters:
		*        (1) Building entity ID
		*        (2) Firefighter entity ID
		*    - No parameters
		*    - No response
		*/
		static const qsf::NamedIdentifier EM5_ACTION_BREAK_BUILDING_DOOR_OPEN_START;

		/**
		*  @brief
		*    "PickupPersonWithBoatAction" message, the motorboat picks up a person from the water
		*
		*  @remarks
		*    - Filters:
		*        (1) Target person entity ID
		*        (2) Motorboat entity ID
		*    - No parameters
		*    - No response
		*/
		static const qsf::NamedIdentifier EM5_ACTION_PICKUP_PERSON_WITH_BOAT_ACTION;

		/**
		*  @brief
		*    "PickupPersonWithRescueHelicopter" message, the rescue helicopter picks up a person. Can be injured or not.
		*
		*  @remarks
		*    - Filters:
		*        (1) Target person entity ID
		*        (2) Rescue helicopter entity ID
		*    - Parameters:
		*        "IsInjured" "true" if person is injured, in this case its hanging on the robe and blocks the helicopter
		*    - No response
		*/
		static const qsf::NamedIdentifier EM5_ACTION_PICKUP_PERSON_WITH_RESCUEHELICOPTER;

		/**
		*  @brief
		*    "PickupPersonByParamedic" message, the paramedic picks up a person on the stretcher.
		*
		*  @remarks
		*    - Filters:
		*        (1) Target person entity ID
		*        (2) Paramedics entity ID
		*    - No parameters
		*    - No response
		*/
		static const qsf::NamedIdentifier EM5_ACTION_PICKUP_PERSON_BY_PARAMEDIC;

		/**
		*  @brief
		*    "BreakBuildingDoorOpenEnd" message, the firefighter has successfully broken the door
		*
		*  @remarks
		*    - Filters:
		*        (1) Building Entity ID
		*        (2) Firefighter Entity ID
		*    - No parameters
		*    - No response
		*/
		static const qsf::NamedIdentifier EM5_ACTION_BREAK_BUILDING_DOOR_OPEN_END;

		/**
		*  @brief
		*    "PlacePersonFromBoatAction" message, the motorboat puts down a person onto a boat drop target point
		*
		*  @remarks
		*    - Filters:
		*        (1) Boat drop target point entity ID
		*        (2) Boat entity ID
		*        (3) Placed person entity ID
		*    - No parameters
		*    - No response
		*/
		static const qsf::NamedIdentifier EM5_ACTION_PLACE_PERSON_FROM_BOAT_ACTION;

		/**
		*  @brief
		*    "PlacePersonFromDiverAction" message, the diver places a person to the shore
		*
		*  @remarks
		*    - Filters:
		*        (1) Diver ID
		*        (2) Placed person ID
		*    - No parameters
		*    - No response
		*/
		static const qsf::NamedIdentifier EM5_ACTION_PLACE_PERSON_FROM_DIVER_ACTION;

		/**
		*  @brief
		*    "PlacePersonFromFlyingHeliAction" message, the heli places a person back to the ground
		*
		*  @remarks
		*    - Filters:
		*        (1) Person Entity ID
		*        (2) Helicopter Entity ID
		*    - No parameters
		*    - No response
		*/
		static const qsf::NamedIdentifier EM5_ACTION_PLACE_PERSON_FROM_FLYING_HELI_ACTION;

		/**
		*  @brief
		*    "LiftEntityWithCrane" message, when the crane lifts an entity from the ground
		*
		*  @remarks
		*    - Filters:
		*        (1) ID of entity being lifted
		*        (2) ID of crane entity
		*    - No parameters
		*    - No response
		*/
		static const qsf::NamedIdentifier EM5_ACTION_LIFT_ENTITY_WITH_CRANE;

		/**
		*  @brief
		*    "PlaceEntityFromCraneAction" message, when the crane places an entity back to the ground
		*
		*  @remarks
		*    - Filters:
		*        (1) ID of entity being placed
		*        (2) ID of crane entity
		*    - No parameters
		*    - No response
		*/
		static const qsf::NamedIdentifier EM5_ACTION_PLACE_ENTITY_FROM_CRANE_ACTION;

		/**
		*  @brief
		*    "CutVegetation" message, a tree or hedge has been cut
		*
		*  @remarks
		*    - Filters:
		*        (1) Tree/hedge entity ID
		*    - No parameters
		*    - No response
		*/
		static const qsf::NamedIdentifier EM5_ACTION_CUT_VEGETATION;

		/**
		*  @brief
		*    "CarCutOpenAction" message, a car has been cut open
		*
		*  @remarks
		*    - Filters:
		*        (1) Car entity ID
		*    - No parameters
		*    - No response
		*/
		static const qsf::NamedIdentifier EM5_ACTION_CAR_CUT_OPEN_ACTION;

		/**
		*  @brief
		*    "PutPersonInBoat" message, the person was put into the boat
		*
		*  @remarks
		*    - Filters:
		*        (1) Boat entity ID
		*		 (2) Diver entity ID
		*        (3) Target person entity ID
		*    - Parameters:
		*        "boatId": boat entity ID, a uint64
		*        "diverId": Diver entity ID, a uint64
		*        "targetId": Target person entity ID, a uint64
		*    - No response
		*/
		static const qsf::NamedIdentifier EM5_ACTION_PUT_PERSON_IN_BOAT_ACTION;

		/**
		*  @brief
		*    "FreePersonFromBuilding" message, after person was freed from building
		*
		*  @remarks
		*    - Filters:
		*        (1) ID of the entity that was frees
		*    - Parameters:
		*        "buildingId": entity ID of the building, the person is fleeing, uint64
		*    - Response:
		*        Optional bool response; "true" if the normal logic after this message should not be executed.
		*        NOTE: When an handler sets an response an message observer for e.g. "Rescue person from building" objective might not work then
		*/
		static const qsf::NamedIdentifier EM5_ACTION_FREE_PERSON_FROM_BUILDING;

		/**
		*  @brief
		*    "FreeHostageFromBuilding" message, after hostage was freed from building
		*
		*  @remarks
		*    - Filters:
		*        (1) ID of the entity that was frees
		*    - Parameters:
		*        "buildingId": entity ID of the building, the person is fleeing, uint64
		*    - No Response
		*/
		static const qsf::NamedIdentifier EM5_ACTION_FREE_HOSTAGE_FROM_BUILDING;

		/**
		*  @brief
		*    "TookPersonOutside" message, second step for DLK loading a person out of building. Send after "DLKSavePersonAction" is finished
		*
		*  @remarks
		*    - Filters:
		*        (1) ID of the entity that was placed
		*    - No parameters
		*    - No response
		*/
		static const qsf::NamedIdentifier EM5_ACTION_TOOKOUTSIDE_PERSON;

		/**
		*  @brief
		*    "LoadPersonOnDLK" message, first step for DLK loading a person out of building. Send after person was took outside and placed on the DLK ladder
		*
		*  @remarks
		*    - Filters:
		*        (1) ID of the entity that was placed
		*    - No parameters
		*    - No response
		*/
		static const qsf::NamedIdentifier EM5_ACTION_LOAD_PERSON_ON_DLK;

		/**
		*  @brief
		*    "LoadVehiceleToTransporter" message, after vehicle was load
		*
		*  @remarks
		*    - Filters:
		*        (1) ID of the vehicle entity that was loaded
		*    - No parameters
		*    - No response
		*/
		static const qsf::NamedIdentifier EM5_ACTION_LOAD_VEHICLE_TO_TRANSPORTER;

		/**
		*  @brief
		*    "UnloadVehicleFromTransporter" message, after vehicle was unload
		*
		*  @remarks
		*    - Filters:
		*        (1) ID of the vehicle entity that was unloaded
		*    - No parameters
		*    - No response
		*/
		static const qsf::NamedIdentifier EM5_ACTION_UNLOAD_VEHICLE_FROM_TRANSPORTER;

		/**
		*  @brief
		*    "EnterVehicleWithoutEquipment" message, after specific entities putting equipment in the back door of an vehicle and moving back to enter the vehicle
		*
		*  @remarks
		*    - Filters:
		*        (1) ID of the entity that is moving
		*	- Parameters:
		*        "actorId": Actor entity ID, a uint64
		*        "vehicleId": Diver entity ID, a uint64
		*    - No response
		*/
		static const qsf::NamedIdentifier EM5_LOGIC_ENTER_VEHICLE_WITHOUT_EQUIPMENT;

		/**
		*  @brief
		*    "FinishMoveAction" message, after unit is finishing a move action
		*
		*  @remarks
		*    - Filters:
		*        (1) ID of the entity that is arrived
		*    - No parameters
		*    - No response
		*/
		static const qsf::NamedIdentifier EM5_ACTION_FINISH_MOVE_ACTION;

		/**
		*  @brief
		*    "PersonJumpedOutOfWindow" message, after person jumped outside of window
		*
		*  @remarks
		*    - Filters:
		*        (1) ID of the entity that jumped
		*    - No parameters
		*    - No response
		*/
		static const qsf::NamedIdentifier EM5_ACTION_JUMPEDOUTOFWINDOW_PERSON;

		/**
		*  @brief
		*    "TookPersonOutOfWindow" message, after person was took outside of window
		*
		*  @remarks
		*    - Filters:
		*        (1) ID of the entity that was placed
		*    - No parameters
		*    - No response
		*/
		static const qsf::NamedIdentifier EM5_ACTION_TOOKOUTOFWINDOW_PERSON;

		/**
		*  @brief
		*    "TransportedToHq" message, after entity was transported to HQ
		*
		*  @remarks
		*    - Filters:
		*        (1) ID of the entity that was transported
		*    - Parameters:
		*        "EntityId": ID of the entity that was transported
		*    - No response
		*/
		static const qsf::NamedIdentifier EM5_ACTION_TRANSPORTEDTOHQ;

		/**
		*  @brief
		*    "Gunshot" message, after a gun was fired
		*
		*  @remarks
		*    - Filters:
		*        (1) ID of the entity that shot the gun
		*		 (2) ID of the entity that was shot at
		*    - Parameters:
		*        "ShooterId": ID of the entity that shot the gun
		*        "ShooteeId": ID of the entity that was shot at
		*		 "GunType" : weapon::PISTOL or weapon::RIFLE (see shoot action)
		*    - No response
		*/
		static const qsf::NamedIdentifier EM5_ACTION_GUNSHOT;

		/**
		*  @brief
		*    "Arrest" message, after a gangster was arrested
		*
		*  @remarks
		*    - Filters:
		*        (1) ID of the gangster that was arrested
		*		 (2) ID of the unit that executed the arrest
		*    - Parameters:
		*        "GangsterId": ID of the entity that was arrested
		*        "PoliceId": ID of the unit that executed the arrest
		*    - No response
		*/
		static const qsf::NamedIdentifier EM5_ACTION_ARREST;

		/**
		*  @brief
		*    "Arrest start" message, when officer a gangster start to arrest
		*
		*  @remarks
		*    - Filters:
		*        (1) ID of the gangster that start to arrest
		*    - Parameters:
		*        "GangsterId": ID of the entity that start tp arrest
		*    - No response
		*/
		static const qsf::NamedIdentifier EM5_ACTION_ARREST_STARTED;

		/**
		*  @brief
		*    "ActionStarted" message, after a action starts its work
		*
		*  @remarks
		*    - Filters:
		*        (1) ID of the action which has started
		*    - No parameters
		*    - No response
		*/
		static const qsf::NamedIdentifier EM5_ACTION_STARTED;

		/**
		*  @brief
		*    "ActionSignal" message, after a signal action was started
		*
		*  @remarks
		*    - Filters:
		*        (1) ID of the command where the SignalAction was started
		*    - No parameters
		*    - No response
		*/
		static const qsf::NamedIdentifier EM5_ACTION_SIGNAL;

		/**
		*  @brief
		*    "CarPickedUpByTowCar" message, after a car was picked up by a towcar
		*
		*  @remarks
		*    - Filters:
		*        (1) ID of target car entity
		*    - No parameters
		*    - No response
		*/
		static const qsf::NamedIdentifier EM5_ACTION_CAR_PICKED_UP_BY_TOWCAR;

		/**
		*  @brief
		*    "RepairedOrUsed" message, after a vehicle was repaired or an object was used by an engineer
		*
		*  @remarks
		*    - Filters:
		*        (1) ID of the target entity which was repaired or used
		*		 (2) ID of the unit executing the action
		*    - No parameters
		*    - No response
		*/
		static const qsf::NamedIdentifier EM5_ACTION_REPAIRED_OR_USED;

		/**
		*  @brief
		*    The police unit has finished the attack building action and brings a gangster out of the building
		*
		*  @remarks
		*    - Filters:
		*        (1) ID of the building entity
		*    - Parameters:
		*       "PolicemanId": entity ID of the policeman who finished his action, uint64
		*       "GangsterId": entity ID of the gangster being arrested and connected with the policeman, uint64
		*       "BuildingId": entity ID of the building, uint64
		*    - No Response
		*/
		static const qsf::NamedIdentifier EM5_ACTION_ATTACK_BUILDING_FINISH;

		/**
		*  @brief
		*    The police unit has finished the attack building action but is inside a fake building
		*
		*  @remarks
		*    - Filters:
		*        (1) ID of the building entity
		*    - Parameters:
		*       "PolicemanId": entity ID of the policeman who finished his action, uint64
		*       "BuildingId": entity ID of the building, uint64
		*    - No Response
		*/
		static const qsf::NamedIdentifier EM5_ACTION_ATTACK_FAKEBUILDING_FINISH;

		/**
		*  @brief
		*    The Guardsman has finished the rescue witch action
		*
		*  @remarks
		*    - Filters:
		*        (1) ID of the pyre entity
		*    - Parameters:
		*       "GuardsmanId": entity ID of the guardsman who finished his action, uint64
		*		"WitchId": entity ID of the rescued witch, uint64
		*       "PyreId": entity ID of the pyre being used, uint64
		*    - No Response
		*/
		static const qsf::NamedIdentifier EM5_ACTION_RESCUE_WITCH_FINISH;

		/**
		*  @brief
		*    The police unit has finished searching a building for bomb
		*
		*  @remarks
		*    - Filters:
		*        (1) ID of the building entity
		*    - Parameters:
		*       "PolicemanId": entity ID of the policeman who finished his action, uint64
		*       "BuildingId": entity ID of the building, uint64
		*    - No Response
		*/
		static const qsf::NamedIdentifier EM5_ACTION_SEARCH_BOMB_IN_BUILDING_FINISH;

		/**
		*  @brief
		*    The police unit has finished the speak to person action
		*
		*  @remarks
		*    - Filters:
		*        (1) ID of the speaking person entity
		*    - Parameters:
		*       "PolicemanId": entity ID of the policeman who finished his action, uint64
		*       "PersonId": entity ID of the person who was target of the policeman, uint64
		*    - No Response
		*/
		static const qsf::NamedIdentifier EM5_ACTION_SPEAK_TO_PERSON;

		/**
		*  @brief
		*    "BuildingGetsDamaged" message, building gets damaged
		*
		*  @remarks
		*    - Filters:
		*        (1) ID of the entity that was damaged
		*    - Parameters:
		*		 EntityId: The entity ID for which the event was sent, uint64
		*    - No response
		*/
		static const qsf::NamedIdentifier EM5_COMPONENT_GETSDAMAGED_BUILDING;

		/**
		*  @brief
		*    "PersonGetsInjured" message, person gets injured
		*
		*  @remarks
		*    - Filters:
		*        (1) The injured entity's ID
		*    - parameters
		*        "EventId": ID of the event that injured the entity
		*    - No response
		*/
		static const qsf::NamedIdentifier EM5_COMPONENT_GETSINJURED_PERSON;

		/**
		*  @brief
		*    "PersonGetsHeatEnergy" message, person gets heat energy
		*
		*  @remarks
		*    - Filters:
		*        (1) The injured entity's ID
		*        (2) Indicates if the person is civilian (value = 1) or is unit (value = 0)
		*    - No parameters
		*    - No response
		*/
		static const qsf::NamedIdentifier EM5_COMPONENT_GETSHEATENERGY_PERSON;

		/**
		*  @brief
		*    "VehicleGetsHeatEnergy" message, vehicle gets heat energy
		*
		*  @remarks
		*    - Filters:
		*        (1) The injured entity's ID
		*        (2) Indicates if the vehicle is civilian (value = 1) or is unit (value = 0)
		*    - No parameters
		*    - No response
		*/
		static const qsf::NamedIdentifier EM5_COMPONENT_GETSHEATENERGY_VEHICLE;

		/**
		*  @brief
		*    "VegetationGetsDamaged" message, vegetation gets damaged
		*
		*  @remarks
		*    - Filters:
		*        (1) ID of the entity that was damaged
		*    - Parameters:
		*        "EntityId": The entity ID for which the event was sent, uint64
		*    - No response
		*/
		static const qsf::NamedIdentifier EM5_COMPONENT_GETSDAMAGED_VEGETATION;

		/**
		*  @brief
		*    "VehicleGetsDamaged" message, vehicle gets damaged
		*
		*  @remarks
		*    - Filters:
		*        (1) Event that has damaged the vehicle
		*        (2) The entity ID for which the event was sent, uint64
		*    - Parameters:
		*		 "EntityId": The entity ID for which the event was sent, uint64
		*    - No response
		*/
		static const qsf::NamedIdentifier EM5_COMPONENT_GETSDAMAGED_VEHICLE;

		/**
		*  @brief
		*    "PersonGetsShoot" message, a person gets damaged by shooting weapons. Target can be injured after this
		*
		*  @remarks
		*    - Filters:
		*        (1) ID of the entity that was shooted
		*    - Parameters:
		*		 "EntityId": The entity ID for which the event was sent (shooting target), uint64
		*		 "AttackerId": The entity ID who shoots at the target, uint64
		*		 "DamageAmount": The total given damage to the target, int
		*    - No response
		*/
		static const qsf::NamedIdentifier EM5_COMPONENT_PERSON_GETS_SHOOT;

		/**
		*  @brief
		*    "VehicleDestroyed" message, vehicle gets destroyed and explodes
		*
		*  @remarks
		*    - Filters:
		*        (1) ID of the entity that was destroyed
		*    - No parameters
		*    - No response
		*/
		static const qsf::NamedIdentifier EM5_VEHICLE_DESTROYED;

		/**
		*  @brief
		*    "BoatDestroyed" message, boat gets destroyed and sinks
		*
		*  @remarks
		*    - Filters:
		*        (1) ID of the entity that was destroyed
		*    - No parameters
		*    - No response
		*/
		static const qsf::NamedIdentifier EM5_BOAT_DESTROYED;

		/**
		*  @brief
		*    "PersonInfectedByEvent" message, emitted after a entity was infected
		*
		*  @remarks
		*    - Filters:
		*        (1) Event ID
		*    - Parameters:
		*		 "EntityId": ID of the target entity, a uint64
		*		 "InjuryDelay": Delay of the hint after the infection, a float
		*    - No response
		*/
		// TODO(mk) Rename to EM5_HEALTH_PERSON_INFECTED
		static const qsf::NamedIdentifier EM5_HINT_PERSON_INFECTED;

		/**
		*  @brief
		*    "PersonContaminated" message, person gets contaminated
		*	  This is not yet injured (meaning they still can stand and only lose health as long as they stay in the contaminated area)
		*
		*  @remarks
		*    - Filters:
		*        (1) The contaminated entity's ID
		*    - parameters
		*        "EventId": ID of the event that contaminated the entity
		*    - No response
		*/
		static const qsf::NamedIdentifier EM5_COMPONENT_HEALTH_PERSON_CONTAMINATED;

		/**
		*  @brief
		*    "PersonDecontaminated" message, emitted right after a person was decontaminated (still in decontamination vehicle)
		*
		*  @remarks
		*    - Filters:
		*        (1) The decontaminated entity's ID
		*    - No parameters
		*    - No response
		*/
		static const qsf::NamedIdentifier EM5_PERSON_DECONTAMINATED;

		/**
		*  @brief
		*    "PersonExitsAfterDecontamination" message, emitted after a person was decontaminated and is leaving the decontamination vehicle now
		*
		*  @remarks
		*    - Filters:
		*        (1) The decontaminated entity's ID
		*    - No parameters
		*    - Response:
		*        Optional bool response; if "false", then the fireman does not automatically stop pulling or guiding the victim
		*/
		static const qsf::NamedIdentifier EM5_PERSON_EXITS_AFTER_DECONTAMINATION;

		/**
		*  @brief
		*    "PersonExitedAfterDecontamination" message, emitted after a person was decontaminated and has just left the decontamination vehicle
		*
		*  @remarks
		*    - Filters:
		*        (1) The decontaminated entity's ID
		*    - No parameters
		*    - No response
		*/
		static const qsf::NamedIdentifier EM5_PERSON_EXITED_AFTER_DECONTAMINATION;

		/**
		*  @brief
		*    "FirefighterInsideContaminationArea" message. Currently emitted constantly while firefighters are inside the contamination area.
		*    May be made more general (for all persons inside) when necessary.
		*
		*  @remarks
		*    - Filters:
		*        (1) Person entity ID (firefighter)
		*        (2) Contamination entity ID (for the owner of the contamination cloud component)
		*    - No parameters
		*    - No response
		*/
		static const qsf::NamedIdentifier EM5_FIREFIGHTER_INSIDE_CONTAMINATION_AREA;

		/**
		*  @brief
		*    "PlagueQuickTest" message. Emitted when the plague quicktest is done
		*
		*  @remarks
		*    - Filters:
		*        (1) Person entity ID (victim)
		*    - Parameters:
		*        "isInfectedByPlague" "true" if the person has the plague following injury
		*    - No response
		*/
		static const qsf::NamedIdentifier EM5_PERSON_PLAGUEQUICKTEST;

		/**
		*  @brief
		*    Unit order box was opened
		*
		*  @remarks
		*    - No filters
		*    - No parameters
		*    - No response
		*/
		static const qsf::NamedIdentifier EM5_OPENED_UNIT_ORDER_BOX;

		/**
		*  @brief
		*    Diagnosis of a medic is completed
		*
		*  @remarks
		*    - Filters:
		*        (1) ID of the entity that was treated
		*    - No parameters
		*    - No response
		*/
		static const qsf::NamedIdentifier EM5_DIAGNOSIS_COMPLETE;

		/**
		*  @brief
		*    A new entity from type "divingspot" is created
		*
		*  @remarks
		*    - Filters:
		*        (1) ID of the entity that was created
		*    - Parameters:
		*        - "DivingSpotId": ID of the new created diving spot entity
		*    - No response
		*/
		static const qsf::NamedIdentifier EM5_CREATED_DIVINGSPOT;

		/**
		*  @brief
		*    A bomb is defused
		*
		*  @remarks
		*    - Filters:
		*        (1) ID of the bomb entity that was defused
		*    - Parameters:
		*        - "DefusedBomb": ID of the bomb entity that was defused
		*    - No response
		*/
		static const qsf::NamedIdentifier EM5_ENTITY_BOMB_DEFUSED;

		/**
		*  @brief
		*    A bomb exploded
		*
		*  @remarks
		*    - Filters:
		*        (1) ID of the bomb entity that exploded
		*    - Parameters:
		*        - "ExplodedBomb": ID of the bomb entity that exploded
		*    - No response
		*/
		static const qsf::NamedIdentifier EM5_ENTITY_BOMB_EXPLODED;

		/**
		*  @brief
		*    A right mouse click occurred
		*
		*  @remarks
		*    - No filters
		*    - Parameters:
		*        - "TargetId": ID of the entity on which a right click was done by the player
		*    - No response
		*/
		static const qsf::NamedIdentifier EM5_MOUSE_RIGHT_CLICK;

		/**
		*  @brief
		*    A gangster appeared
		*
		*  @remarks
		*    - Filters:
		*        (1) Entity ID of the gangster who appeared
		*    - No response
		*/
		static const qsf::NamedIdentifier EM5_GANGSTER_APPEARED;

		/**
		*  @brief
		*    A gangster starts to flee
		*
		*  @remarks
		*    - Filters:
		*        (1) Entity ID of the gangster who flees
		*    - No response
		*/
		static const qsf::NamedIdentifier EM5_GANGSTER_FLEES;

		/**
		*  @brief
		*    A gangster escaped
		*
		*  @remarks
		*    - Filters:
		*        (1) Entity ID of the gangster who escaped
		*    - Parameters:
		*        - "GangsterId": ID of the gangster who escaped
		*    - No response
		*/
		static const qsf::NamedIdentifier EM5_GANGSTER_ESCAPED;

		/**
		*  @brief
		*    A gangster reached one of his waypoints
		*
		*  @remarks
		*    - Filters:
		*        (1) Entity ID of the gangster who escaped
		*        (2) Entity ID of the waypoint (only if it is an entity at all)
		*    - No response
		*/
		static const qsf::NamedIdentifier EM5_GANGSTER_REACHED_WAYPOINT;

		/**
		*  @brief
		*    BuryComponent does unhide a buried entity
		*
		*  @remarks
		*    - Filters:
		*        (1) ID of the entity with the BuryComponent (usually debris)
		*		 (2) ID of the entity which is no longer buried now
		*    - Parameters:
		*        - "DebrisId": ID of the entity with the BuryComponent (usually debris)
		*        - "UnburiedId": ID of the entity which is no longer buried now
		*    - No response
		*/
		static const qsf::NamedIdentifier EM5_UNHIDE_BURIED;

		/**
		*  @brief
		*    RescueDogLocateVictimAction has found and unhidden a person
		*
		*  @remarks
		*    - Filters:
		*        (1) ID of the entity which is found
		*    - Parameters:
		*        - "FoundId": ID of the entity which is no longer in need for finding
		*    - No response
		*/
		static const qsf::NamedIdentifier EM5_RESCUE_DOG_FOUND_PERSON;

		/**
		*  @brief
		*    PoliceDogLocateObjectAction has found an object
		*
		*  @remarks
		*    - Filters:
		*        (1) ID of the entity which is found
		*    - Parameters:
		*        - "FoundId": ID of the entity which is no longer in need for finding
		*    - No response
		*/
		static const qsf::NamedIdentifier EM5_POLICE_DOG_FOUND_OBJECT;

		/**
		*  @brief
		*    Contamination cloud starts spreading
		*    CAREFUL: This is send per cloud - not per building. So you can get it more than once.
		*
		*  @remarks
		*    - Filters:
		*		 (1) ID of the parent entity (usually an industrial building)
		*        (2) ID of the contamination cloud doing the spreading
		*    - No response
		*/
		static const qsf::NamedIdentifier EM5_COMPONENT_CONTAMINATION_START_SPREADING;

		/**
		*  @brief
		*    "InternalObserverMessage" message from an observer, used as an internal message between observers
		*
		*  @remarks
		*    - Filters:
		*        (1) Hint ID
		*    - No parameters
		*    - No response
		*/
		static const qsf::NamedIdentifier EM5_HINT_PLAYED;

		/**
		*  @brief
		*    The Camera following mode has changed this target, can be valid or invalid
		*
		*  @remarks
		*    - No filters:
		*    - Parameters:
		*        - "TargetId": ID of the entity the camera mode is following, uninitialized in case the mode is turned off
		*    - No Response
		*/
		static const qsf::NamedIdentifier EM5_CAMERA_FOLLOW_ENTITY_CHANGED;

		/**
		*  @brief
		*    The Entity has reached end of path which it was following with help of the PathMovementComponent
		*
		*  @remarks
		*    - Filters:
		*        (1) ID of the entity
		*    - No parameters
		*    - No Response
		*/
		static const qsf::NamedIdentifier EM5_FOLLOW_PATH_REACHED_END;

		/**
		*  @brief
		*    The trapped persons in the building died due time ran out
		*
		*  @remarks
		*    - Filters:
		*        (1) ID of the building entity
		*    - No parameters
		*    - Response:
		*        Optional bool response; if "true", then the persons are only removed and not killed
		*/
		static const qsf::NamedIdentifier EM5_BUILDING_TRAPPED_PERSONS_DIED;

		//[-------------------------------------------------------]
		//[ Plague                                                ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    "PlagueBuildingChecked" The check plague building check was finished
		*
		*  @remarks
		*    - Filters:
		*        (1) ID of the building entity
		*    - No parameters
		*    - No response
		*/
		static const qsf::NamedIdentifier EM5_PLAGUEBUILDING_CHECKED;

		/**
		*  @brief
		*    "PlaguePersonSentBack" The plague person was sent back to their home
		*
		*  @remarks
		*    - Filters:
		*        (1) ID of the plague person entity
		*    - No parameters
		*    - No response
		*/
		static const qsf::NamedIdentifier EM5_PLAGUEPERSON_SENTBACK;

		/**
		*  @brief
		*    "PlaguePersonRemoved" The plague person was removed
		*
		*  @remarks
		*    - Filters:
		*        (1) "true" if the plague person returned home "false" if it was removed somewhere else
		*        (2) ID of the plague person entity
		*    - No parameters
		*    - No response
		*/
		static const qsf::NamedIdentifier EM5_PLAGUEPERSON_REMOVED;

		//[-------------------------------------------------------]
		//[ Entity creation/destruction                           ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    A entity was created from prefab
		*
		*  @remarks
		*    - Filters:
		*        (1) ID of the created entity
		*    - Parameters:
		*        - "CreatedEntity": An pointer to the created entity (only valid inside the handler of this message)
		*        - "PrefabHash": The string hash of the prefab local asset ID
		*    - No response
		*/
		static const qsf::NamedIdentifier EM5_ENTITY_CREATED_FROM_PREFAB;

		//[-------------------------------------------------------]
		//[ Minimap-related messages                              ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Create a minimap icon for an entity
		*
		*  @remarks
		*    - Filters:
		*        (1) ID of the entity for which an icon should be added
		*    - Parameters:
		*        - "IconType": The MiniMapIconType of the icon to be added
		*        - "IconScale": The scale of the icon to be added
		*    - No response
		*/
		static const qsf::NamedIdentifier EM5_MINIMAP_ADD_ENTITY_ICON;

		/**
		*  @brief
		*    Create a minimap circle polygon for an entity
		*
		*  @remarks
		*    - Filters:
		*        (1) ID of the entity for which an polygon should be added
		*    - Parameters:
		*        - "Radius": The radius of the circle polygon to be added
		*        - "Points": The number of edges/angles of the circle polygon to be added
		*        - "Color": The color of the circle polygon to be added
		*        - "UpdatePosition": A boolean indicating whether the circle polygon to be added should update its position from the entity or not
		*    - No response
		*/
		static const qsf::NamedIdentifier EM5_MINIMAP_ADD_ENTITY_CIRCLE_POLYGON;

		/**
		*  @brief
		*    Removes a minimap circle polygon for an entity
		*
		*  @remarks
		*    - Filters:
		*        (1) ID of the entity for which an icon should be added
		*    - No Parameters
		*    - No response
		*/
		static const qsf::NamedIdentifier EM5_MINIMAP_REMOVE_ENTITY_CIRCLE_POLYGON;

		/**
		*  @brief
		*    Destroy a minimap icon
		*
		*  @remarks
		*    - Filters:
		*        (1) ID of the entity for which the icon should be removed
		*    - No parameters
		*    - No response
		*/
		static const qsf::NamedIdentifier EM5_MINIMAP_REMOVE_ENTITY_ICON;

		//[-------------------------------------------------------]
		//[ Warbler feed-related messages                         ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    "PostWarblerShotRendering" message, after warbler shot rendering has been finished
		*
		*  @remarks
		*    - No filters
		*    - Parameters
		*        "TextureResourceName": Name of the OGRE texture resource, std::string
		*    - No response
		*/
		static const qsf::NamedIdentifier EM5_POST_WARBLER_SHOT_RENDERING;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Default constructor
		*/
		Messages();

		/**
		*  @brief
		*    Destructor
		*
		*  @brief
		*    - Not virtual by intent
		*/
		~Messages();


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/freeplay/event/FreeplayEvent.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace em5
{
	class Objective;
}
namespace qsf
{
	class Entity;
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
	*    EMERGENCY 5 objective helper class
	*/
	class EM5_API_EXPORT ObjectiveHelper : public boost::noncopyable
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		// Objectives
		static const uint32 OBJECTIVE_NEED_TREATPERSONS;				///< "TreatPersons": Player needs to treat injured persons
		static const uint32 OBJECTIVE_NEED_TRANSPORTINJURED;			///< "TransportPersons": Player needs to transport treated persons to the head quarters
		static const uint32 OBJECTIVE_NEED_TRANSPORTPLAGUEPERSON;		///< "TransportPlaguePersons": Player needs to transport all plague infected or injured persons to the head quarters
		static const uint32 OBJECTIVE_NEED_EXTINGUISHFIRES;				///< "ExtinguishFires": Player needs to extinguish all fires
		static const uint32 OBJECTIVE_NEED_CLEANUPVEHICLES;				///< "CleanupVehicles": Player needs to clean up all damaged or destroyed cars
		static const uint32 OBJECTIVE_NEED_SAVEWAVINGPERSON;			///< "SaveWavingPerson": Player needs to save person with the DLK
		static const uint32 OBJECTIVE_NEED_DECONTAMINATEPERSONS;		///< "DecontaminatePersons": Player needs to decontaminate all contaminated persons
		static const uint32 OBJECTIVE_NEED_ARRESTPERSONS;				///< "ArrestPersons": Player needs to arrest and transport the criminal persons
		static const uint32 OBJECTIVE_NEED_TREATCRIMINALPERSON;			///< "TreatCriminalPerson": Player needs to treat injured criminal persons
		static const uint32 OBJECTIVE_NEED_TRANSPORTCRIMINALPERSON;		///< "TransportCriminalPerson": Player needs to transport treated criminal persons to the head quarters
		static const uint32 OBJECTIVE_NEED_STOPCARTHIEF;				///< "StopCarThief": Player needs to chase all stolen cars with police helicopter and stop them
		static const uint32 OBJECTIVE_NEED_RETRIEVEVEHICLE;				///< "RetrieveVehicle": Player needs to move a stolen vehicle to the HQ
		static const uint32 OBJECTIVE_NEED_DISPOSEBOMB;					///< "DisposeBomb": Player needs to dispose bomb with SEK robot
		static const uint32 OBJECTIVE_NEED_FINDKILLERWITHSEKDRONE;		///< "FindKillerWithSEKDrone": PLayer needs to find the killer with the SEK drone

		static const uint32 OBJECTIVE_NEED_GETPERSONOUTOFCAR;			///< "GetPersonOutOfCar": Player needs to take injured persons out of vehicle
		static const uint32 OBJECTIVE_NEED_HEALPERSONS;					///< "HealPersons": Person was healed and has no injury anymore
		static const uint32 OBJECTIVE_NEED_REPAIREBOXES;				///< "RepairEBoxes": Repair all defect EBoxes
		static const uint32 OBJECTIVE_NEED_EXTINGUISHEBOXES;			///< "ExtinguishEBoxes": Extinguish all burning eboxes
		static const uint32 OBJECTIVE_NEED_RESCUEDROWNING;				///< "RescueDrowning": Player needs to dive and rescue the drowning person
		static const uint32 OBJECTIVE_NEED_BRINGPERSONTOLAND;			///< "BringPersonToLand": Player needs to bring the rescued person to land
		static const uint32 OBJECTIVE_NEED_PERSONHANGONTRAINTRACK;		///< "PersonHangOnTrainTrack": Player need to bring hanging persons from train tracks to land
		static const uint32 OBJECTIVE_NEED_PERSONFROMTRAINTRACK;		///< "PersonFromTrainTrack": Player need to bring persons from train tracks to land
		static const uint32 OBJECTIVE_NEED_FINDPERSON;					///< "FindPerson": Player needs to find the hidden person
		static const uint32 OBJECTIVE_NEED_CHASEAWAYPROTESTER;			///< "ChaseAwayProtester": Chase away protester
		static const uint32 OBJECTIVE_NEED_FREEPERSONSFROMBUILDING;		///< "FreePersonsFromBuilding": Player needs to take injured persons out of building
		static const uint32 OBJECTIVE_NEED_PERSONSBURIEDLIGHTDEBRIS;	///< "PersonsBuriedLightDebris": Save all injured persons under debris which needs a fireman or a crane
		static const uint32 OBJECTIVE_NEED_PERSONSBURIEDHEAVYDEBRIS;	///< "PersonsBuriedHeavyDebris": Save all injured persons under debris which needs a crane
		static const uint32 OBJECTIVE_NEED_PERSONSBURIEDHEAVYDEBRIS2;	///< "PersonsBuriedHeavyDebris2": Save all injured persons under debris which needs a crane
		static const uint32 OBJECTIVE_NEED_RESCUEPERSONWITHHELI;		///< "RescuePersonWithHeli": Rescue injures persons with heli
		static const uint32 OBJECTIVE_NEED_RESCUEPERSONSFROMTRAINTRACK;	///< "RescuePersonsFromTrainTrack": Rescue injured persons from (electirc) train track
		static const uint32 OBJECTIVE_NEED_QUICKTESTPERSONS;			///< "QuicktestPersons": Player needs a plague quick test for the contaminated persons
		static const uint32 OBJECTIVE_NEED_REPAIRBUILDING;				///< "RepairBuilding": Send engineer in damaged house
		static const uint32 OBJECTIVE_NEED_REPAIRTRACKELECTRIC;			///< "RepairTrackElectric": Send engineer to disable ebox for train tracks
		static const uint32 OBJECTIVE_NEED_CHASEAWAYHOTELPROTESTER;		///< "ChaseAwayHotelProtester": Chase away hotel protester
		static const uint32 OBJECTIVE_NEED_SAWBURYINGTREE;				///< "SawBuryingTree": Remove a cuttable tree that is burying a person or vehicle
		static const uint32 OBJECTIVE_NEED_TAKEGANGSTEROUTSIDE_POLICE;	///< "TakeGangsterOutsidePolice": Send police in building with gangster
		static const uint32 OBJECTIVE_NEED_TAKEGANGSTEROUTSIDE_SEK;		///< "TakeGangsterOutsideSEK": Send police in building with gangster
		static const uint32 OBJECTIVE_NEED_TAKETERRORISTOUTSIDE_SEK;	///< "TakeTerroristOutsideSEK": Send police in building with gangster (terrorist)
		static const uint32 OBJECTIVE_NEED_SPEAKTO_HOSTAGETAKER;		///< "SpeakToHostageTaker": Send police to talk with gangster in building
		static const uint32 OBJECTIVE_NEED_TALK_TO_PERSON;				///< "TalkToPerson": Send police to talk with person
		static const uint32 OBJECTIVE_NEED_TAKEHOSTAGEOUTSIDE;			///< "TakeHostageOutside": Send police in building with hostage
		static const uint32 OBJECTIVE_NEED_TAKEINJUREDHOSTAGEOUTSIDE;	///< "TakeInjuredHostageOutside": Send Firefighter in building with injured hostage
		static const uint32 OBJECTIVE_NEED_FINDBOMBINCASE;				///< "FindBombInCase": Send police dog leader to search cases with bomb
		static const uint32 OBJECTIVE_NEED_RESCUEWITCH;					///< "RescueWitch": Use guardsman to rescue witch from pyre

		static const uint32 OBJECTIVE_FAIL_TIMER;						///< "Timer": The player must finish the event in a given amount of time
		static const uint32 OBJECTIVE_FAIL_CONTAMINATEDPERSONS;			///< "NoContaminationPersons": Player must prevent persons from being contaminated (squad + civil)
		static const uint32 OBJECTIVE_FAIL_DEADPERSONS;					///< "NoDead": Player must prevent persons from dying
		static const uint32 OBJECTIVE_FAIL_DEADSQUADPERSONS;			///< "NoDeadSquad": Player must prevent squad persons from dying
		static const uint32 OBJECTIVE_FAIL_DESTROYEDVEHICLES;			///< "NoDestroyedVehicles": Player must prevent vehicles from being destroyed
		static const uint32 OBJECTIVE_FAIL_DESTROYEDOBJECTS;			///< "NoDestroyedObjects": Player must prevent objects from being destroyed
		static const uint32 OBJECTIVE_FAIL_INJUREDCIVILS;				///< "NoInjuredCivilPersons": Player must prevent civil persons from being injured
		static const uint32 OBJECTIVE_FAIL_INJUREDSQUAD;				///< "NoInjuredSquad": Player must prevent squad persons from being injured
		static const uint32 OBJECTIVE_FAIL_INJUREDPERSONS;				///< "NoInjuredPersons": Player must prevent persons from being injured (squad + civil)
		static const uint32 OBJECTIVE_FAIL_UNITS_LOST;					///< "UnitsLost": Player must prevent units from being injured (persons) or damaged (vehicles)
		static const uint32 OBJECTIVE_FAIL_DAMAGESQUADVEHICLES;			///< "NoDamagedSquadVehicles": Player must prevent squad vehicles from being damaged
		static const uint32 OBJECTIVE_FAIL_BURNING_BUILDINGS;			///< "NoBurningBuildings": Player must extinguish burning buildings
		static const uint32 OBJECTIVE_FAIL_BURNING_OBJECTS;				///< "NoObjectIsBurning": Player must extinguish burning objects
		static const uint32 OBJECTIVE_FAIL_BURNING_OBJECTANDBUILDING;	///< "NoObjectAndBuildingIsBurning": Player must extinguish burning object AND buildings
		static const uint32 OBJECTIVE_FAIL_EXPLODED_BOMBS;				///< "NoBombsExploded": Player must not let bombs explode
		static const uint32 OBJECTIVE_FAIL_PERSONSESCAPE;				///< "PersonsEscape": Criminal persons escape from the map
		static const uint32 OBJECTIVE_FAIL_GANGSTERNOTFOUND;			///< "NoFindGangster": Player must find gangster in given time
		static const uint32 OBJECTIVE_FAIL_TOOMANYPROTESTER;			///< "TooMayProtester": Player must keep a demonstration under a certain number of protester


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Initialize objective by ID
		*
		*  @param[in] objectiveTypeId
		*    Named identifier of freeplay event as hashed string
		*  @param[in] objective
		*    Objective that has to be initialized
		*
		*  @return
		*    Returns "true" if everything went fine, else return "false"
		*/
		static bool initializeObjectiveByKey(uint32 objectiveTypeId, Objective& objective);


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		explicit ObjectiveHelper(FreeplayEvent& freeplayEvent);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~ObjectiveHelper();

		/**
		*  @brief
		*    Return map of objectives
		*/
		const FreeplayEvent::ObjectiveInstanceMap& getObjectives() const;

		/**
		*  @brief
		*    Return objective with the given type ID
		*
		*  @return
		*    Objective with type ID or nullptr if no objective is there
		*/
		Objective* getObjectiveByTypeId(uint32 typeId) const;

		/**
		*  @brief
		*    Return objective with the given type ID
		*
		*  @return
		*    Objective with type ID
		*/
		Objective& getObjectiveByTypeIdSafe(uint32 typeId) const;

		/**
		*  @brief
		*    Create and insert a new objective initialized by type ID
		*/
		Objective& getOrCreateObjectiveByTypeId(uint32 typeId) const;

		/**
		*  @brief
		*    Create and insert a new objective
		*
		*  @note
		*    - New created objectives are not initialized
		*/
		Objective& getOrCreateObjective(uint32 typeId, const Objective::ObjectiveType type, const std::string& text = "", uint32 orderPriority = 100) const;

		/**
		*  @brief
		*    Remove an objective initialized by type ID
		*/
		void removeObjectiveByTypeId(uint32 typeId) const;

		/**
		*  @brief
		*    Clear all objectives
		*/
		void clearObjectives() const;

		/**
		*  @brief
		*    Add a fail condition for a limit of dead persons
		*/
		Objective& addDeadCivilPersonsFailCondition(uint32 limit);
		Objective& addDeadUnitPersonsFailCondition(uint32 limit);
		Objective& addDeadPersonsFailCondition(uint32 limit);

		/**
		*  @brief
		*    Add a fail condition for a limit of injured persons
		*/
		Objective& addInjuredCivilPersonsFailCondition(uint32 limit);
		Objective& addInjuredUnitPersonsFailCondition(uint32 limit);
		Objective& addInjuredPersonsFailCondition(uint32 limit);

		/**
		*  @brief
		*    Add a fail condition for a limit of both injured unit persons and damaged unit vehicles
		*/
		Objective& addUnitsLostFailCondition(uint32 limit);

		/**
		*  @brief
		*    Add a fail condition for a limit of escaped gangsters
		*/
		Objective& addEscapedGangstersFailCondition(uint32 limit);

		/**
		*  @brief
		*    Add a fail condition for a limit of burning objects
		*/
		Objective& addBurningObjectsFailCondition(uint32 limit);
		Objective& addBurningBuildingsFailCondition(uint32 limit);
		Objective& addBurningObjectsAndBuildingsFailCondition(uint32 limit);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		FreeplayEvent& mEvent;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

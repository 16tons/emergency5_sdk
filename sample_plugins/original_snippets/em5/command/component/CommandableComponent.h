// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"

#include <qsf_game/command/component/CommandableComponent.h>

#include <qsf/base/NamedIdentifier.h>
#include <qsf/job/JobProxy.h>

#include <boost/container/flat_set.hpp>


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
	*    Commandable component class
	*/
	class EM5_API_EXPORT CommandableComponent : public qsf::game::CommandableComponent
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 COMPONENT_ID;				///< "em5::CommandableComponent" unique component ID

		// Departments
		static const qsf::NamedIdentifier UNITTAG_DEPARTMENT_FIREFIGHTERS;			// "Firefighter" tag
		static const qsf::NamedIdentifier UNITTAG_DEPARTMENT_POLICE;				// "Police" tag
		static const qsf::NamedIdentifier UNITTAG_DEPARTMENT_TECH;					// "Tech" tag
		static const qsf::NamedIdentifier UNITTAG_DEPARTMENT_AMBULANCE;				// "Ambulance" tag

		// Unit categories
		static const qsf::NamedIdentifier UNITTAG_CATEGORY_PERSON;					// "Person" tag for all persons
		static const qsf::NamedIdentifier UNITTAG_CATEGORY_VEHICLE;					// "Vehicle" tag for all kinds of vehicles
		static const qsf::NamedIdentifier UNITTAG_CATEGORY_ROAD;					// "Road" tag for road vehicles
		static const qsf::NamedIdentifier UNITTAG_CATEGORY_BOAT;					// "Boat" tag for ships and boats
		static const qsf::NamedIdentifier UNITTAG_CATEGORY_HELI;					// "Heli" tag for helicopters, drones and planes

		// Firefighter units
		static const qsf::NamedIdentifier UNITTAG_FIREFIGHTERS_RW;					// "RW" tag
		static const qsf::NamedIdentifier UNITTAG_FIREFIGHTERS_TLF;					// "TLF" tag
		static const qsf::NamedIdentifier UNITTAG_FIREFIGHTERS_DEKONP;				// "DeKon-P" tag
		static const qsf::NamedIdentifier UNITTAG_FIREFIGHTERS_DLK;					// "DLK" tag
		static const qsf::NamedIdentifier UNITTAG_FIREFIGHTERS_FLB;					// "FLB" tag
		static const qsf::NamedIdentifier UNITTAG_FIREFIGHTERS_BOAT_TRANSPORT;		// "TFMB" tag
		static const qsf::NamedIdentifier UNITTAG_FIREFIGHTERS_BOAT;				// "FirefightersMotorboat" tag
		static const qsf::NamedIdentifier UNITTAG_FIREFIGHTERS_COMMANDOCAR;			// "KF" tag
		static const qsf::NamedIdentifier UNITTAG_FIREFIGHTERS_FIREPLANE;			// "LF" tag
		static const qsf::NamedIdentifier UNITTAG_FIREFIGHTERS_DRONE_TRANSPORT;		// "TFD" tag
		static const qsf::NamedIdentifier UNITTAG_FIREFIGHTERS_DRONE;				// "FirefightersDrone" tag
		static const qsf::NamedIdentifier UNITTAG_FIREFIGHTERS_DIVER_TRANSPORT;		// "TFT" tag
		static const qsf::NamedIdentifier UNITTAG_FIREFIGHTERS_RW_HISTORIC;			// "RW_Historic" tag

		// Firefighter units
		static const qsf::NamedIdentifier UNITTAG_FIREFIGHTERS_FIREFIGHTER;			// "Firefighter" tag
		static const qsf::NamedIdentifier UNITTAG_FIREFIGHTERS_FIREFIGHTER_ABC;		// "Firefighter_ABC" tag
		static const qsf::NamedIdentifier UNITTAG_FIREFIGHTERS_DIVER;				// "Diver" tag
		static const qsf::NamedIdentifier UNITTAG_FIREFIGHTERS_FIREFIGHTER_HISTORIC;// "Firefighter_Historic" tag

		// Police cars
		static const qsf::NamedIdentifier UNITTAG_POLICE_MTW;						// "MTW" tag
		static const qsf::NamedIdentifier UNITTAG_POLICE_HELI;						// "PHC" tag
		static const qsf::NamedIdentifier UNITTAG_POLICE_SEK_HELI;					// "SEKHC" tag
		static const qsf::NamedIdentifier UNITTAG_POLICE_STW;						// "STW" tag
		static const qsf::NamedIdentifier UNITTAG_POLICE_DRONE_TRANSPORT;			// "TFSEKD" tag
		static const qsf::NamedIdentifier UNITTAG_POLICE_DRONE;						// "SEKD" tag
		static const qsf::NamedIdentifier UNITTAG_POLICE_ROBOT_TRANSPORT;			// "TSEKR" tag
		static const qsf::NamedIdentifier UNITTAG_POLICE_ROBOT;						// "SEKR" tag
		static const qsf::NamedIdentifier UNITTAG_POLICE_DOGCAR;					// "PHF" tag

		// Police units
		static const qsf::NamedIdentifier UNITTAG_POLICE_POLICE;					// "Police" tag
		static const qsf::NamedIdentifier UNITTAG_POLICE_SEK;						// "SEKPolice" tag
		static const qsf::NamedIdentifier UNITTAG_POLICE_POLICEDOG_LEADER;			// "PoliceDogLeader" tag
		static const qsf::NamedIdentifier UNITTAG_POLICE_POLICEDOG;					// "PoliceDog" tag
		static const qsf::NamedIdentifier UNITTAG_POLICE_GUARDSMAN_HISTORIC;		// "Guardsman_Historic" tag

		// Tech cars
		static const qsf::NamedIdentifier UNITTAG_TECH_TOWCAR;						// "ASF" tag
		static const qsf::NamedIdentifier UNITTAG_TECH_ENGINEERCAR;					// "EKW" tag
		static const qsf::NamedIdentifier UNITTAG_TECH_CRANE;						// "BFZ" tag
		static const qsf::NamedIdentifier UNITTAG_TECH_HELI;						// "BHC" tag

		// Tech units
		static const qsf::NamedIdentifier UNITTAG_TECH_ENGINEER;					// "Engineer" tag

		// Ambulance cars
		static const qsf::NamedIdentifier UNITTAG_AMBULANCE_NEF;					// "NEF" tag
		static const qsf::NamedIdentifier UNITTAG_AMBULANCE_RTW;					// "RTW" tag
		static const qsf::NamedIdentifier UNITTAG_AMBULANCE_HELI;					// "RHC" tag
		static const qsf::NamedIdentifier UNITTAG_AMBULANCE_RESCUEDOGCAR;			// "RHF" tag
		static const qsf::NamedIdentifier UNITTAG_AMBULANCE_IMT;					// "IMT" tag

		// Ambulance units
		static const qsf::NamedIdentifier UNITTAG_AMBULANCE_DOCTOR;					// "Doctor" tag
		static const qsf::NamedIdentifier UNITTAG_AMBULANCE_PARAMEDICS;				// "Paramedics" tag
		static const qsf::NamedIdentifier UNITTAG_AMBULANCE_RESCUEDOG_LEADER;		// "RescueDogLeader" tag
		static const qsf::NamedIdentifier UNITTAG_AMBULANCE_RESCUEDOG;				// "RescueDog" tag
		static const qsf::NamedIdentifier UNITTAG_AMBULANCE_DOCTOR_HISTORIC;		// "Doctor_Historic" tag

		// Misc units
		static const qsf::NamedIdentifier UNITTAG_MISC_CONVOITRUCK;					// "ConvoiTruck" tag for ME104
		static const qsf::NamedIdentifier UNITTAG_MISC_AMBULANCEBUS;				// "AmbulanceBus" tag for ME310
		static const qsf::NamedIdentifier UNITTAG_MISC_EVACBUS;						// "EvacBus" tag for ME310

		// Special unit tags
		static const qsf::NamedIdentifier UNITTAG_CAN_TRANSPORT_INJUREDS;			// "CanTransportInjureds" for modded ambulance vehicles
		static const qsf::NamedIdentifier UNITTAG_CAN_TRANSPORT_GANGSTERS;			// "CanTransportGangsters" for modded police vehicles

		// Minimap icons
		static const qsf::NamedIdentifier MINIMAP_ICON_UNIT_AMBULANCE;				// "Ambulance" minimap icon identifier
		static const qsf::NamedIdentifier MINIMAP_ICON_UNIT_FIREFIGHTER;			// "Firefighter" minimap icon identifier
		static const qsf::NamedIdentifier MINIMAP_ICON_UNIT_POLICE;					// "Police" minimap icon identifier
		static const qsf::NamedIdentifier MINIMAP_ICON_UNIT_TECH;					// "Tech" minimap icon identifier
		static const qsf::NamedIdentifier MINIMAP_ICON_UNIT_AMBULANCE_DOG;			// "AmbulanceDog" minimap icon identifier
		static const qsf::NamedIdentifier MINIMAP_ICON_UNIT_POLICE_DOG;				// "PoliceDog" minimap icon identifier
		static const qsf::NamedIdentifier MINIMAP_ICON_UNIT_MEDIEVAL;				// "Medieval" minimap icon identifier

		static const qsf::NamedIdentifier MINIMAP_ICON_UNIT_VEHICLE_AMBULANCE;		// "AmbulanceVehicle" minimap icon identifier
		static const qsf::NamedIdentifier MINIMAP_ICON_UNIT_VEHICLE_FIREFIGHTER;	// "FirefighterVehicle" minimap icon identifier
		static const qsf::NamedIdentifier MINIMAP_ICON_UNIT_VEHICLE_POLICE;			// "PoliceVehicle" minimap icon identifier
		static const qsf::NamedIdentifier MINIMAP_ICON_UNIT_VEHICLE_TECH;			// "TechVehicle" minimap icon identifier


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] prototype
		*    The prototype this component is in, no null pointer allowed
		*/
		explicit CommandableComponent(qsf::Prototype* prototype);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~CommandableComponent();

		/**
		*  @brief
		*    Get the ID of the player who owns this unit
		*/
		uint32 getPlayerId() const;

		/**
		*  @brief
		*    Set the ID of the player who owns this unit, can be uninitialized
		*
		*  @note
		*    - Changing player ID after ordering / creating the unit is not supported
		*/
		void setPlayerId(uint32 playerId);

		/**
		*  @brief
		*    Get the ID of the team to which this unit belongs
		*/
		uint16 getTeamId() const;

		/**
		*  @brief
		*    Set the ID of the team to which the unit belongs, can be uninitialized
		*
		*  @note
		*    Changing team ID after ordering / creating the unit is not supported
		*/
		void setTeamId(uint16 teamId);

		/**
		*  @brief
		*    Set the speaker index for command and selection feedback
		*/
		uint32 getSpeakerIndex() const;

		/**
		*  @brief
		*    Set the speaker index for command and selection feedback
		*/
		void setSpeakerIndex(uint32 speakerIndex);

		/**
		*  @brief
		*    Get the tags of the player
		*/
		const std::string& getUnitTagsAsString() const;

		/**
		*  @brief
		*    Set tags of an unit, eg "Group:Ambulance;Vehicle;RTW"
		*/
		void setUnitTagsAsString(const std::string& unitTags);

		/**
		*  @brief
		*    Check for a specific unit tag
		*/
		bool hasUnitTag(const qsf::StringHash& tag) const;

		/**
		*  @brief
		*    Get the icon type for the minimap
		*
		*  @note
		*    - To get the actual value of enum type "MiniMapIconType", use "em5::MiniMapIconRegistrationLogic::translateCommandableMiniMapIconType"
		*/
		const std::string& getMiniMapIconType() const;

		/**
		*  @brief
		*    Set the icon type for the minimap
		*/
		void setMiniMapIconType(const std::string& miniMapIconType);

		/**
		*  @brief
		*    Shine through will be delayed for new units
		*/
		void delayedShineThrough();

		bool isUnitType(const qsf::StringHash& unitTypeId) const;
		bool isAmbulanceNEF() const;
		bool isAmbulanceRTW() const;
		bool isAmbulanceIMT() const;
		bool isAmbulanceHeli() const;
		bool isAmbulanceDoctor() const;
		bool isAmbulanceParamedics() const;
		bool isAmbulanceRescueDogLeader() const;
		bool isAmbulanceRescueDog() const;

		bool isPoliceSTW() const;
		bool isPoliceMTW() const;
		bool isPoliceHeli() const;
		bool isPoliceSEKHeli() const;
		bool isPolicePerson() const;
		bool isPoliceSEKPerson() const;
		bool isPoliceTSEKR() const;
		bool isPoliceSEKR() const;
		bool isPoliceSEKDrone() const;
		bool isPoliceDogCar() const;
		bool isPoliceDogLeader() const;
		bool isPoliceDog() const;
		bool isPoliceGuardsmanHistoric() const;

		bool isFireFighterRW() const;
		bool isFireFighterDECONP() const;
		bool isFireFighterPerson() const;
		bool isFireFighterPersonHistoric() const;
		bool isFireFighterDLK() const;
		bool isFireFighterDiver() const;
		bool isFireFighterBoat() const;
		bool isFireFighterBoatTransport() const;
		bool isFireFighterFLB() const;
		bool isFireFighterDrone() const;

		bool isTechEngineer() const;
		bool isTechEngineerCar() const;
		bool isTechTowcar() const;
		bool isTechHeli() const;
		bool isTechCrane() const;

		bool canTransportGangster() const;
		bool canTransportInjured() const;

		bool canSwim() const;


	//[-------------------------------------------------------]
	//[ Public virtual qsf::game::CommandableComponent methods]
	//[-------------------------------------------------------]
	public:
		virtual void setUnitType(uint32 unitType) override;


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Component methods                 ]
	//[-------------------------------------------------------]
	public:
		virtual void serialize(qsf::BinarySerializer& serializer) override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void updateDelayedJob(const qsf::JobArguments& jobArguments);
		const std::string& getUnitTagName() const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Connected to CAMP
		uint32		  mPlayerId;			///< Player identifier
		std::string   mUnitTagsString;		///< List with the abilities of a unit as a single string
		std::string	  mMiniMapIconType;		///< Name of the icon type for the minimap

		// Internal
		boost::container::flat_set<uint32> mUnitTags;	///< Single unit tags' string hashes
		uint32		  mSpeakerIndex;		///< Speaker index (usually 1 or 2) for command and selection feedback
		uint16		  mPlayerTeamId;
		qsf::JobProxy mDelayedJobProxy;		///< Job proxy for delayed job
		float		  mSecondsPassed;


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::CommandableComponent)

// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/command/component/CommandableComponent.h"
#include "em5/plugin/Jobs.h"
#include "em5/map/EntityHelper.h"

#include <qsf_game/component/event/EventTagComponent.h>

#include <qsf/map/Entity.h>
#include <qsf/job/JobArguments.h>
#include <qsf/base/StringParser.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 CommandableComponent::COMPONENT_ID = qsf::StringHash("em5::CommandableComponent");

	// Departments
	const qsf::NamedIdentifier CommandableComponent::UNITTAG_DEPARTMENT_FIREFIGHTERS = "Firefighters";
	const qsf::NamedIdentifier CommandableComponent::UNITTAG_DEPARTMENT_POLICE		 = "Police";
	const qsf::NamedIdentifier CommandableComponent::UNITTAG_DEPARTMENT_TECH		 = "Tech";
	const qsf::NamedIdentifier CommandableComponent::UNITTAG_DEPARTMENT_AMBULANCE	 = "Ambulance";

	// Unit categories
	const qsf::NamedIdentifier CommandableComponent::UNITTAG_CATEGORY_PERSON  = "Person";
	const qsf::NamedIdentifier CommandableComponent::UNITTAG_CATEGORY_VEHICLE = "Vehicle";
	const qsf::NamedIdentifier CommandableComponent::UNITTAG_CATEGORY_ROAD	  = "Road";
	const qsf::NamedIdentifier CommandableComponent::UNITTAG_CATEGORY_BOAT	  = "Boat";
	const qsf::NamedIdentifier CommandableComponent::UNITTAG_CATEGORY_HELI	  = "Heli";

	// Firefighter cars
	const qsf::NamedIdentifier CommandableComponent::UNITTAG_FIREFIGHTERS_RW			  = "RW";			// de: Rüstwagen
	const qsf::NamedIdentifier CommandableComponent::UNITTAG_FIREFIGHTERS_TLF			  = "TLF";			// de: Tanklöschfahrzeug
	const qsf::NamedIdentifier CommandableComponent::UNITTAG_FIREFIGHTERS_DEKONP		  = "DeKon-P";		// de: Dekontaminationsfahrzeug
	const qsf::NamedIdentifier CommandableComponent::UNITTAG_FIREFIGHTERS_DLK			  = "DLK";			// de: Drehleiterfahrzeug
	const qsf::NamedIdentifier CommandableComponent::UNITTAG_FIREFIGHTERS_FLB			  = "FLB";			// de: Feuerlöschboot
	const qsf::NamedIdentifier CommandableComponent::UNITTAG_FIREFIGHTERS_BOAT_TRANSPORT  = "TFMB";			// de: Transportfahrzeug Motorboot
	const qsf::NamedIdentifier CommandableComponent::UNITTAG_FIREFIGHTERS_BOAT			  = "FirefightersMotorboat";	// de: Motorboot
	const qsf::NamedIdentifier CommandableComponent::UNITTAG_FIREFIGHTERS_COMMANDOCAR	  = "KF";			// de: Löschflugzeug-Kommandofahrzeug
	const qsf::NamedIdentifier CommandableComponent::UNITTAG_FIREFIGHTERS_FIREPLANE		  = "LF";			// de: Löschflugzeug
	const qsf::NamedIdentifier CommandableComponent::UNITTAG_FIREFIGHTERS_DRONE_TRANSPORT = "TFD";			// de: Transportfahrzeug Feuerwehrdrohne
	const qsf::NamedIdentifier CommandableComponent::UNITTAG_FIREFIGHTERS_DRONE			  = "FirefightersDrone";	// de: Feuerwehrdrohne
	const qsf::NamedIdentifier CommandableComponent::UNITTAG_FIREFIGHTERS_DIVER_TRANSPORT = "TFT";			// de: Transportfahrzeug Taucher
	const qsf::NamedIdentifier CommandableComponent::UNITTAG_FIREFIGHTERS_RW_HISTORIC	  = "RW_Historic";	// de: Historisches Tanklöschfahrzeug

	// Firefighter units
	const qsf::NamedIdentifier CommandableComponent::UNITTAG_FIREFIGHTERS_FIREFIGHTER		   = "Firefighter";		// de: Feuerwehrmann
	const qsf::NamedIdentifier CommandableComponent::UNITTAG_FIREFIGHTERS_FIREFIGHTER_ABC	   = "FirefighterABC";	// de: ABC-Feuerwehrmann
	const qsf::NamedIdentifier CommandableComponent::UNITTAG_FIREFIGHTERS_DIVER				   = "Diver";			// de: Feuerwehrtaucher
	const qsf::NamedIdentifier CommandableComponent::UNITTAG_FIREFIGHTERS_FIREFIGHTER_HISTORIC = "Firefighter_Historic"; // de: Historischer Feuerwehrmann

	// Police cars
	const qsf::NamedIdentifier CommandableComponent::UNITTAG_POLICE_MTW				= "MTW";	// de: Mannschaftstransportwagen
	const qsf::NamedIdentifier CommandableComponent::UNITTAG_POLICE_HELI			= "PHC";	// de: Polizeihelikopter
	const qsf::NamedIdentifier CommandableComponent::UNITTAG_POLICE_SEK_HELI		= "SEKHC";	// de: Sondereinsatz-Helikopter
	const qsf::NamedIdentifier CommandableComponent::UNITTAG_POLICE_STW				= "STW";	// de: Streifenwagen
	const qsf::NamedIdentifier CommandableComponent::UNITTAG_POLICE_DRONE_TRANSPORT = "TFSEKD";	// de: Transportfahrzeug SEK-Drohne
	const qsf::NamedIdentifier CommandableComponent::UNITTAG_POLICE_DRONE			= "PoliceDrone";	// de: SEK-Drohne
	const qsf::NamedIdentifier CommandableComponent::UNITTAG_POLICE_ROBOT_TRANSPORT = "TSEKR";	// de: Transportfahrzeug SEK-Roboter
	const qsf::NamedIdentifier CommandableComponent::UNITTAG_POLICE_ROBOT			= "SEKR";	// de: SEK-Roboter
	const qsf::NamedIdentifier CommandableComponent::UNITTAG_POLICE_DOGCAR			= "PHF";	// de: Polizeihund-Fahrzeug

	// Police units
	const qsf::NamedIdentifier CommandableComponent::UNITTAG_POLICE_POLICE = "Policeman";		// de: Polizist / Polizistin
	const qsf::NamedIdentifier CommandableComponent::UNITTAG_POLICE_SEK	   = "PoliceSekman";	// de: SEK-Spezialist
	const qsf::NamedIdentifier CommandableComponent::UNITTAG_POLICE_POLICEDOG_LEADER = "PoliceDogLeader";	// de: Polizeihundeführer
	const qsf::NamedIdentifier CommandableComponent::UNITTAG_POLICE_POLICEDOG = "PoliceDog";	// de: Polizeihund
	const qsf::NamedIdentifier CommandableComponent::UNITTAG_POLICE_GUARDSMAN_HISTORIC	= "Guardsman_historic";	// de: Gardist

	// Tech cars
	const qsf::NamedIdentifier CommandableComponent::UNITTAG_TECH_TOWCAR	  = "ASF";	// de: Abschleppfahrzeug
	const qsf::NamedIdentifier CommandableComponent::UNITTAG_TECH_ENGINEERCAR = "EKW";	// de: Einsatzkraftwagen
	const qsf::NamedIdentifier CommandableComponent::UNITTAG_TECH_CRANE		  = "BFZ";	// de: Bergefahrzeug (Schwerer Kran)
	const qsf::NamedIdentifier CommandableComponent::UNITTAG_TECH_HELI		  = "BHC";	// de: Bergungshelikopter

	// Tech units
	const qsf::NamedIdentifier CommandableComponent::UNITTAG_TECH_ENGINEER = "Engineer";	// de: Ingenieur

	// Ambulance cars
	const qsf::NamedIdentifier CommandableComponent::UNITTAG_AMBULANCE_NEF			= "NEF"; // de: Notarzteinsatzfahrzeug
	const qsf::NamedIdentifier CommandableComponent::UNITTAG_AMBULANCE_RTW			= "RTW"; // de: Rettungstransportwagen
	const qsf::NamedIdentifier CommandableComponent::UNITTAG_AMBULANCE_HELI			= "RHC"; // de: Rettungshelikopter
	const qsf::NamedIdentifier CommandableComponent::UNITTAG_AMBULANCE_RESCUEDOGCAR = "RHF"; // de: Rettungshundefahrzeug
	const qsf::NamedIdentifier CommandableComponent::UNITTAG_AMBULANCE_IMT			= "IMT"; // de: Intensiv medizinischer Transporter / Rettungsbus

	// Ambulance units
	const qsf::NamedIdentifier CommandableComponent::UNITTAG_AMBULANCE_DOCTOR			= "Doctor";				// de: Notarzt / Notärztin
	const qsf::NamedIdentifier CommandableComponent::UNITTAG_AMBULANCE_PARAMEDICS		= "Paramedics";			// de: Sanitäterteam
	const qsf::NamedIdentifier CommandableComponent::UNITTAG_AMBULANCE_RESCUEDOG_LEADER = "RescueDogLeader";	// de: Rettungshundeführer
	const qsf::NamedIdentifier CommandableComponent::UNITTAG_AMBULANCE_RESCUEDOG		= "RescueDog";			// de: Rettungshund
	const qsf::NamedIdentifier CommandableComponent::UNITTAG_AMBULANCE_DOCTOR_HISTORIC	= "Doctor_Historic";	// de: Pestarzt

	// Misc units
	const qsf::NamedIdentifier CommandableComponent::UNITTAG_MISC_CONVOITRUCK			= "ConvoiTruck";
	const qsf::NamedIdentifier CommandableComponent::UNITTAG_MISC_AMBULANCEBUS			= "AmbulanceBus";
	const qsf::NamedIdentifier CommandableComponent::UNITTAG_MISC_EVACBUS				= "EvacBus";

	// Special unit tags
	const qsf::NamedIdentifier CommandableComponent::UNITTAG_CAN_TRANSPORT_INJUREDS		= "CanTransportInjureds";
	const qsf::NamedIdentifier CommandableComponent::UNITTAG_CAN_TRANSPORT_GANGSTERS	= "CanTransportGangsters";

	// Minimap icons
	const qsf::NamedIdentifier CommandableComponent::MINIMAP_ICON_UNIT_AMBULANCE	= "Ambulance";
	const qsf::NamedIdentifier CommandableComponent::MINIMAP_ICON_UNIT_FIREFIGHTER	= "Firefighter";
	const qsf::NamedIdentifier CommandableComponent::MINIMAP_ICON_UNIT_POLICE		= "Police";
	const qsf::NamedIdentifier CommandableComponent::MINIMAP_ICON_UNIT_TECH			= "Tech";
	const qsf::NamedIdentifier CommandableComponent::MINIMAP_ICON_UNIT_AMBULANCE_DOG= "AmbulanceDog";
	const qsf::NamedIdentifier CommandableComponent::MINIMAP_ICON_UNIT_POLICE_DOG	= "PoliceDog";
	const qsf::NamedIdentifier CommandableComponent::MINIMAP_ICON_UNIT_MEDIEVAL		= "Medieval";

	const qsf::NamedIdentifier CommandableComponent::MINIMAP_ICON_UNIT_VEHICLE_AMBULANCE	= "AmbulanceVehicle";
	const qsf::NamedIdentifier CommandableComponent::MINIMAP_ICON_UNIT_VEHICLE_FIREFIGHTER	= "FirefighterVehicle";
	const qsf::NamedIdentifier CommandableComponent::MINIMAP_ICON_UNIT_VEHICLE_POLICE		= "PoliceVehicle";
	const qsf::NamedIdentifier CommandableComponent::MINIMAP_ICON_UNIT_VEHICLE_TECH			= "TechVehicle";


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	CommandableComponent::CommandableComponent(qsf::Prototype* prototype) :
		qsf::game::CommandableComponent(prototype),
		mPlayerId(qsf::getUninitialized<uint32>()),
		mSpeakerIndex(0),		// Default value, this is no valid speaker index
		mSecondsPassed(0.0f),
		mPlayerTeamId(qsf::getUninitialized<uint16>())
	{
		// Nothing to do in here
	}

	CommandableComponent::~CommandableComponent()
	{
		// Nothing to do in here
	}

	uint32 CommandableComponent::getPlayerId() const
	{
		return mPlayerId;
	}

	void CommandableComponent::setPlayerId(uint32 playerId)
	{
		mPlayerId = playerId;
	}

	uint16 CommandableComponent::getTeamId() const
	{
		return mPlayerTeamId;
	}

	void CommandableComponent::setTeamId(uint16 teamId)
	{
		mPlayerTeamId = teamId;
	}

	uint32 CommandableComponent::getSpeakerIndex() const
	{
		return mSpeakerIndex;
	}

	void CommandableComponent::setSpeakerIndex(uint32 speakerIndex)
	{
		mSpeakerIndex = speakerIndex;
	}

	const std::string& CommandableComponent::getUnitTagsAsString() const
	{
		return mUnitTagsString;
	}

	void CommandableComponent::setUnitTagsAsString(const std::string& unitTags)
	{
		// TODO(mk) add promote property change?
		mUnitTagsString = unitTags;

		// Update internal unit tags set
		mUnitTags.clear();
		if (!unitTags.empty())
		{
			static std::vector<std::string> parts;
			parts.clear();
			qsf::StringParser::splitString(unitTags, parts, " ");

			mUnitTags.reserve(parts.size());
			for (const std::string& part : parts)
			{
				if (!part.empty())
				{
					mUnitTags.emplace(qsf::StringHash(part));
				}
			}
		}
	}

	bool CommandableComponent::hasUnitTag(const qsf::StringHash& tag) const
	{
		return (mUnitTags.count(tag) > 0);
	}

	const std::string& CommandableComponent::getMiniMapIconType() const
	{
		return mMiniMapIconType;
	}

	void CommandableComponent::setMiniMapIconType(const std::string& miniMapIconType)
	{
		mMiniMapIconType = miniMapIconType;
	}

	bool CommandableComponent::isUnitType(const qsf::StringHash& unitTypeId) const
	{
		return (mUnitType == unitTypeId);
	}

	bool CommandableComponent::isAmbulanceNEF() const
	{
		return isUnitType(UNITTAG_AMBULANCE_NEF);
	}

	bool CommandableComponent::isAmbulanceRTW() const
	{
		return isUnitType(UNITTAG_AMBULANCE_RTW);
	}

	bool CommandableComponent::isAmbulanceIMT() const
	{
		return isUnitType(UNITTAG_AMBULANCE_IMT);
	}

	bool CommandableComponent::isAmbulanceHeli() const
	{
		return isUnitType(UNITTAG_AMBULANCE_HELI);
	}

	bool CommandableComponent::isAmbulanceDoctor() const
	{
		return isUnitType(UNITTAG_AMBULANCE_DOCTOR);
	}

	bool CommandableComponent::isAmbulanceParamedics() const
	{
		return isUnitType(UNITTAG_AMBULANCE_PARAMEDICS);
	}

	bool CommandableComponent::isAmbulanceRescueDogLeader() const
	{
		return isUnitType(UNITTAG_AMBULANCE_RESCUEDOG_LEADER);
	}

	bool CommandableComponent::isAmbulanceRescueDog() const
	{
		return isUnitType(UNITTAG_AMBULANCE_RESCUEDOG);
	}

	bool CommandableComponent::isPoliceSTW() const
	{
		return isUnitType(UNITTAG_POLICE_STW);
	}

	bool CommandableComponent::isPoliceMTW() const
	{
		return isUnitType(UNITTAG_POLICE_MTW);
	}

	bool CommandableComponent::isPoliceHeli() const
	{
		return isUnitType(UNITTAG_POLICE_HELI);
	}

	bool CommandableComponent::isPoliceSEKHeli() const
	{
		return isUnitType(UNITTAG_POLICE_SEK_HELI);
	}

	bool CommandableComponent::isPolicePerson() const
	{
		return isUnitType(UNITTAG_POLICE_POLICE);
	}

	bool CommandableComponent::isPoliceSEKPerson() const
	{
		return isUnitType(UNITTAG_POLICE_SEK);
	}

	bool CommandableComponent::isPoliceTSEKR() const
	{
		return isUnitType(UNITTAG_POLICE_ROBOT_TRANSPORT);
	}

	bool CommandableComponent::isPoliceSEKR() const
	{
		return isUnitType(UNITTAG_POLICE_ROBOT);
	}

	bool CommandableComponent::isPoliceSEKDrone() const
	{
		return isUnitType(UNITTAG_POLICE_DRONE);
	}

	bool CommandableComponent::isPoliceDogCar() const
	{
		return isUnitType(UNITTAG_POLICE_DOGCAR);
	}

	bool CommandableComponent::isPoliceDogLeader() const
	{
		return isUnitType(UNITTAG_POLICE_POLICEDOG_LEADER);
	}

	bool CommandableComponent::isPoliceDog() const
	{
		return isUnitType(UNITTAG_POLICE_POLICEDOG);
	}

	bool CommandableComponent::isPoliceGuardsmanHistoric() const
	{
		return isUnitType(UNITTAG_POLICE_GUARDSMAN_HISTORIC);
	}

	bool CommandableComponent::isFireFighterRW() const
	{
		return isUnitType(UNITTAG_FIREFIGHTERS_RW);
	}

	bool CommandableComponent::isFireFighterDECONP() const
	{
		return isUnitType(UNITTAG_FIREFIGHTERS_DEKONP);
	}

	bool CommandableComponent::isFireFighterPerson() const
	{
		return isUnitType(UNITTAG_FIREFIGHTERS_FIREFIGHTER) || isUnitType(UNITTAG_FIREFIGHTERS_FIREFIGHTER_ABC);
	}

	bool CommandableComponent::isFireFighterPersonHistoric() const
	{
		return isUnitType(UNITTAG_FIREFIGHTERS_FIREFIGHTER_HISTORIC);
	}

	bool CommandableComponent::isFireFighterDLK() const
	{
		return isUnitType(UNITTAG_FIREFIGHTERS_DLK);
	}

	bool CommandableComponent::isFireFighterDiver() const
	{
		return isUnitType(UNITTAG_FIREFIGHTERS_DIVER);
	}

	bool CommandableComponent::isFireFighterBoat() const
	{
		return isUnitType(UNITTAG_FIREFIGHTERS_BOAT);
	}

	bool CommandableComponent::isFireFighterBoatTransport() const
	{
		return isUnitType(UNITTAG_FIREFIGHTERS_BOAT_TRANSPORT);
	}

	bool CommandableComponent::isFireFighterFLB() const
	{
		return isUnitType(UNITTAG_FIREFIGHTERS_FLB);
	}

	bool CommandableComponent::isFireFighterDrone() const
	{
		return isUnitType(UNITTAG_FIREFIGHTERS_DRONE);
	}

	bool CommandableComponent::isTechEngineer() const
	{
		return isUnitType(UNITTAG_TECH_ENGINEER);
	}

	bool CommandableComponent::isTechEngineerCar() const
	{
		return isUnitType(UNITTAG_TECH_ENGINEERCAR);
	}

	bool CommandableComponent::isTechTowcar() const
	{
		return isUnitType(UNITTAG_TECH_TOWCAR);
	}

	bool CommandableComponent::isTechHeli() const
	{
		return isUnitType(UNITTAG_TECH_HELI);
	}

	bool CommandableComponent::isTechCrane() const
	{
		return isUnitType(UNITTAG_TECH_CRANE);
	}

	bool CommandableComponent::canTransportGangster() const
	{
		// Should usually be STW, MTW and both police helicopters (PHC and SWAT helicopter)
		return hasUnitTag(UNITTAG_CAN_TRANSPORT_GANGSTERS);
	}

	bool CommandableComponent::canTransportInjured() const
	{
		// Should usually be RTW, IMT and ambulance helicopter
		return hasUnitTag(UNITTAG_CAN_TRANSPORT_INJUREDS);
	}

	bool CommandableComponent::canSwim() const
	{
		return (isFireFighterBoat() || isFireFighterDiver() || isFireFighterFLB());
	}

	void CommandableComponent::delayedShineThrough()
	{
		EntityHelper(getEntity()).enableShineThroughEffect(false);

		mSecondsPassed = 0.0f;
		mDelayedJobProxy.registerAt(Jobs::ANIMATION_VEHICLE, boost::bind(&CommandableComponent::updateDelayedJob, this, _1));
	}

	void CommandableComponent::setUnitType(uint32 unitType)
	{
		// Unregister old unit type from event tag component
		if (isSimulating() && qsf::isInitialized(mUnitType))
		{
			qsf::game::EventTagComponent& eventTagComponent = getEntity().getOrCreateComponentSafe<qsf::game::EventTagComponent>();
			if (eventTagComponent.hasTag(mUnitType))
			{
				eventTagComponent.removeEventTag(mUnitType);
			}
		}

		qsf::game::CommandableComponent::setUnitType(unitType);

		// Register new unit type to event tag component
		if (isSimulating() && qsf::isInitialized(unitType))
		{
			qsf::game::EventTagComponent& eventTagComponent = getEntity().getOrCreateComponentSafe<qsf::game::EventTagComponent>();
			if (!eventTagComponent.hasTag(mUnitType))
			{
				const std::string& unitTagName = getUnitTagName();
				if (unitTagName.empty())
				{
#ifndef ENDUSER
					QSF_WARN("Invalid unittype in a commandable component found", QSF_REACT_NONE);
#endif
				}
				else
				{
					eventTagComponent.addEventTag(unitTagName);
				}
			}
		}
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Component methods                 ]
	//[-------------------------------------------------------]
	void CommandableComponent::serialize(qsf::BinarySerializer& serializer)
	{
		// TODO(fr): This method causes runtime errors. Check it!
		serializer.serialize(mPlayerId);
		serializer.serialize(mUnitTagsString);
		// TODO(fw): Update unit tags on reading

		// Call the base implementation
		qsf::game::CommandableComponent::serialize(serializer);
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void CommandableComponent::updateDelayedJob(const qsf::JobArguments& jobArguments)
	{
		mSecondsPassed += jobArguments.getSecondsPassed();
		if (mSecondsPassed > 5.0f)
		{
			EntityHelper(getEntity()).enableShineThroughEffect();

			mDelayedJobProxy.unregister();
		}
	}

	const std::string& CommandableComponent::getUnitTagName() const
	{
		// TODO(tl): Bad solution for now, but we need the string to put the unit tag into eventTagComponent
		// TODO(mk): It looks like changing the mUnitType to named Identifer and return mUnitType.getName() would solve this problem

		static std::map<uint32, const qsf::NamedIdentifier&> unitTagMap;
		if (unitTagMap.empty())
		{
			// Initialization of unit tag map
			const qsf::NamedIdentifier* unitTags[] =
			{
				&UNITTAG_DEPARTMENT_FIREFIGHTERS,
				&UNITTAG_DEPARTMENT_POLICE,
				&UNITTAG_DEPARTMENT_TECH,
				&UNITTAG_DEPARTMENT_AMBULANCE,
				&UNITTAG_CATEGORY_PERSON,
				&UNITTAG_CATEGORY_VEHICLE,
				&UNITTAG_CATEGORY_ROAD,
				&UNITTAG_CATEGORY_BOAT,
				&UNITTAG_CATEGORY_HELI,
				&UNITTAG_FIREFIGHTERS_RW,
				&UNITTAG_FIREFIGHTERS_TLF,
				&UNITTAG_FIREFIGHTERS_DEKONP,
				&UNITTAG_FIREFIGHTERS_DLK,
				&UNITTAG_FIREFIGHTERS_FLB,
				&UNITTAG_FIREFIGHTERS_BOAT_TRANSPORT,
				&UNITTAG_FIREFIGHTERS_BOAT,
				&UNITTAG_FIREFIGHTERS_COMMANDOCAR,
				&UNITTAG_FIREFIGHTERS_FIREPLANE,
				&UNITTAG_FIREFIGHTERS_DRONE_TRANSPORT,
				&UNITTAG_FIREFIGHTERS_DRONE,
				&UNITTAG_FIREFIGHTERS_DIVER_TRANSPORT,
				&UNITTAG_FIREFIGHTERS_RW_HISTORIC,
				&UNITTAG_FIREFIGHTERS_FIREFIGHTER,
				&UNITTAG_FIREFIGHTERS_FIREFIGHTER_ABC,
				&UNITTAG_FIREFIGHTERS_DIVER,
				&UNITTAG_FIREFIGHTERS_FIREFIGHTER_HISTORIC,
				&UNITTAG_POLICE_MTW,
				&UNITTAG_POLICE_HELI,
				&UNITTAG_POLICE_SEK_HELI,
				&UNITTAG_POLICE_STW,
				&UNITTAG_POLICE_DRONE_TRANSPORT,
				&UNITTAG_POLICE_DRONE,
				&UNITTAG_POLICE_ROBOT_TRANSPORT,
				&UNITTAG_POLICE_ROBOT,
				&UNITTAG_POLICE_DOGCAR,
				&UNITTAG_POLICE_POLICE,
				&UNITTAG_POLICE_POLICEDOG_LEADER,
				&UNITTAG_POLICE_POLICEDOG,
				&UNITTAG_POLICE_SEK,
				&UNITTAG_POLICE_GUARDSMAN_HISTORIC,
				&UNITTAG_TECH_TOWCAR,
				&UNITTAG_TECH_ENGINEERCAR,
				&UNITTAG_TECH_CRANE,
				&UNITTAG_TECH_HELI,
				&UNITTAG_TECH_ENGINEER,
				&UNITTAG_AMBULANCE_NEF,
				&UNITTAG_AMBULANCE_RTW,
				&UNITTAG_AMBULANCE_HELI,
				&UNITTAG_AMBULANCE_RESCUEDOGCAR,
				&UNITTAG_AMBULANCE_IMT,
				&UNITTAG_AMBULANCE_DOCTOR,
				&UNITTAG_AMBULANCE_DOCTOR_HISTORIC,
				&UNITTAG_AMBULANCE_PARAMEDICS,
				&UNITTAG_AMBULANCE_RESCUEDOG_LEADER,
				&UNITTAG_AMBULANCE_RESCUEDOG,
				&UNITTAG_MISC_CONVOITRUCK,
				&UNITTAG_MISC_AMBULANCEBUS,
				&UNITTAG_MISC_EVACBUS
			};

			for (const qsf::NamedIdentifier* tag : unitTags)
			{
				unitTagMap.emplace(tag->getHash(), *tag);
			}
		}

		// Find unit type
		const auto iterator = unitTagMap.find(mUnitType);
		if (iterator != unitTagMap.end())
		{
			return iterator->second.getName();
		}

		// Not found
		static const std::string emptyString;
		return emptyString;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

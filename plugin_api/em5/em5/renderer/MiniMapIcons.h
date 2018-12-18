// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/reflection/CampClass.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Definitions                                           ]
	//[-------------------------------------------------------]
	enum class MiniMapIconType // CAMP type exported in GeneralMetaclasses.cpp
	{
		UNDEFINED = -1,

		// BEGIN of fixed texture atlas icons: These icons have fixed positions in the texture atlas, don't move them
		// Fire
		FIRE = 0,
		// Buildings
		BUILDING_TRAPPED,			// TODO(fw): Thought for buildings with trapped persons where you need the fire axe
		BUILDING_ON_FIRE,
		// Victims
		CONTAMINATED_BIOLOGICAL,	// Use this for biological-contaminated persons
		VICTIM_INJURED_RED,			// Use this for infected (standing) injured persons
		VICTIM_TRANSPORT,			// TODO(fw): Unused
		// Units
		UNIT_AMBULANCE,
		UNIT_FIREFIGHTER,
		UNIT_POLICE,
		UNIT_TECH,
		UNIT_AMBULANCE_DOG,
		// Unit vehicles
		UNIT_VEHICLE_AMBULANCE,
		UNIT_VEHICLE_FIREFIGHTER,
		UNIT_VEHICLE_POLICE,
		UNIT_VEHICLE_TECH,
		UNIT_VEHICLE_FIREPLANE,
		// Other
		HARBOR,
		BRIDGE,
		DEFECT_VEHICLE,
		BROKEN_TREE,
		DANGEROUS_ANIMAL,
		BOMB_OLD,				// Not used any more
		HQ,
		GANGSTER,
		SEARCH,					// Displays an magnifying glass on yellow background
		PEST_BUILDING,
		BARRIER,
		QUESTION_MARK,
		EVENT,					// Generic exclamation mark icon
		UNIT_MEDIEVAL,
		UNIT_VEHICLE_OTHER_PLAYER,
		UNIT_OTHER_PLAYER,
		POSSIBLY_PEST_BUILDING,
		WELL,					// For EM2017 historic map
		GARDIST,				// For EM2017 historic map
		STAKE,					// For EM2017 historic map
		WARBLERTWEET,
		ARSONIST,				// For EM2017 historic map
		FANATIC,				// For EM2017 historic map
		WITCH,					// For EM2017 historic map
		VICTIM_INJURED,			// Use this for injured persons. Green version of VICTIM_INJURED_RED.
		VICTIM_RESCUE_HELI,		// For injured to be rescued by helicopter
		VICTIM_RESCUE_DLK,		// For injured to be rescued by DLK
		CONTAMINATION_CHEMICAL,
		CONTAMINATION_NUCLEAR,
		RESCUE_AXE,				// For buildings where the fireman's axe is needed to open the door
		GANGSTER_IN_CAR,
		UNIT_POLICE_DOG,
		SEARCH_VARIOUS,			// Question mark on red background
		PATH_DESTINATION,		// White bull's eye
		DEAD,
		BOMB,					// Icon marking bombs
		DEFECT,					// Repair icon for broken eboxes
		GAPER,					// Marks gapers to lead away
		HELICOPTER,				// TODO(co) What is this icon used for?
		MISSING_PERSON,			// Missing person
		CUSTOM_1,				// Free for custom usage
		CUSTOM_2,				// Free for custom usage
		// END of fixed icons from texture atlas

		// Signals
		PING,
		PING_PIN,
		BLINK,

		// That's all folks
		_MAX
	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE(em5::MiniMapIconType);

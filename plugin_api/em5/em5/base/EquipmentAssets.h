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


	struct EM5_API_EXPORT equipment
	{


		// Name of fire fighting equipments
		static const std::string FIRE_EXTINGUISHER;
		static const std::string HOSE_NOZZLE;
		static const std::string HOSE_ROLLED;
		static const std::string CHAIN_SAW;
		static const std::string AXE;
		static const std::string RESCUE_CUTTER;
		static const std::string RESCUE_COMPRESSION_PUMP;
		static const std::string HOSE_PART;
		static const std::string DUST_MASK;

		// Historical fire fighting equipment
		static const std::string FIRE_EXTINGUISHER_HISTORICAL;
		static const std::string HOSE_NOZZLE_HISTORICAL;
		static const std::string HOSE_ROLLED_HISTORICAL;
		static const std::string CHAIN_SAW_HISTORICAL;

		// Squad police equipment
		static const std::string POLICE_PEPPERSPRAY;
		static const std::string POLICE_HANDCUFFS;
		static const std::string POLICE_HANDCUFFS_OPEN;
		static const std::string POLICE_SIGNALLINGDISC;
		static const std::string POLICE_MEGAPHONE;
		static const std::string POLICE_PISTOL;
		static const std::string POLICE_SEK_PISTOL;
		static const std::string POLICE_SNIPERRIFLE;
		static const std::string POLICE_STUN_GRENADE;
		static const std::string POLICE_BARRIER_TAPE_POLE;
		static const std::string POLICE_BARRIER_TAPE;
		static const std::string POLICE_BARRIER_TAPE_ROLL;
		static const std::string POLICE_SWORD;

		// Name of gangster equipments
		static const std::string GANGSTER_PISTOL;
		static const std::string BASEBALL_BAT;
		static const std::string KNIFE;
		static const std::string STONE;
		static const std::string MOLOTOV_COCKTAIL;
		static const std::string FANATIC_TORCH;

		// Name of ambulance equipment
		static const std::string AMBULANCE_MEDIKIT;
		static const std::string AMBULANCE_DEFIBRILLATOR;
		static const std::string AMBULANCE_DEFIBRILLATOR_PAD_LEFT;
		static const std::string AMBULANCE_DEFIBRILLATOR_PAD_RIGHT;
		static const std::string AMBULANCE_MEDICATION;
		static const std::string AMBULANCE_BOWL;

		// Name of tech equipment
		static const std::string TECH_STRETCHER;
		static const std::string TECH_HELICOPTER_ROBE;
		static const std::string TECH_TOOLBOX;
		static const qsf::NamedIdentifier MESH_TECH_TOOLBOX_OPEN;
		static const qsf::NamedIdentifier MESH_TECH_TOOLBOX_CLOSED;

		// Name of civil equipment
		static const std::string MOBILEPHONE;
		static const std::string BOTTLE;
		static const std::string UMBRELLA;
		static const std::string UMBRELLA_CLOSED;
		static const std::string DEMO_SIGN_1;
		static const std::string DEMO_SIGN_2;
		static const std::string DEMO_SIGN_3;
		static const std::string GUITAR;

		// Name of meshes
		static const qsf::NamedIdentifier MESH_MEDIKIT_OPEN;
		static const qsf::NamedIdentifier MESH_MEDIKIT_CLOSE;

		// Name of human bones
		static const std::string STANDARD_BONE_RIGHT_HAND;
		static const std::string STANDARD_BONE_LEFT_HAND;
		static const std::string STANDARD_BONE_RIGHT_ARM;
		static const std::string STANDARD_BONE_LEFT_ARM;
		static const std::string STANDARD_BONE_RIGHT_UPPERARM;
		static const std::string STANDARD_BONE_LEFT_UPPERARM;
		static const std::string STANDARD_BONE_RIGHT_LEG;
		static const std::string STANDARD_BONE_LEFT_LEG;
		static const std::string STANDARD_BONE_BODY;
		static const std::string STANDARD_BONE_HEAD;
		static const std::string STANDARD_BONE_RIGHT_TOE;
		static const std::string STANDARD_BONE_LEFT_TOE;
		static const std::string STANDARD_BONE_BACK;
		static const std::string STANDARD_BONE_PELVIS;

		static const std::string STANDARD_BONE_EFFECT;

		static const std::string EQUIPMENT_BONE_STRETCHER;
		static const std::string EQUIPMENT_BONE_TECH_STRETCHER;
		static const std::string EQUIPMENT_BONE_RIGHTHAND;
		static const std::string EQUIPMENT_BONE_LEFTHAND;

		static const std::string VEHICLE_BONE_BOATTRANSPORTER_CRANE_HEEL;

		// Helper objects
		static const qsf::NamedIdentifier PREFAB_DUMMY_BONE;

		// Name of assets
		static const qsf::NamedIdentifier PREFAB_FIRE_EXTINGUISHER_EFFECT;
		static const qsf::NamedIdentifier PREFAB_POLICE_PEPPERSPRAY_EFFECT;
		static const qsf::NamedIdentifier PREFAB_FIRE_BUOY;
		static const qsf::NamedIdentifier PREFAB_FIRE_DIVINGSPOT;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

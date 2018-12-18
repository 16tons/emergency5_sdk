// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"

#include <qsf/base/StringHash.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	struct EM5_API_EXPORT assets
	{


		// Prefabs
		static const qsf::StringHash PREFAB_RESCUEDOG;
		static const qsf::StringHash PREFAB_POLICEDOG;
		static const qsf::StringHash PREFAB_SINGLE_PARAMEDIC;
		static const qsf::StringHash PREFAB_AMBULANCE_STRETCHER;
		static const qsf::StringHash PREFAB_FIREFIGHTING_AIRPLANE;
		static const qsf::StringHash PREFAB_FIREFIGHTING_AIRPLANE_AIREFFECT;
		static const qsf::StringHash PREFAB_FIREFIGHTING_AIRPLANE_GROUNDEFFECT;
		static const qsf::StringHash PREFAB_FIREFIGHTING_AIRPLANE_GROUNDMARKER;
		static const qsf::StringHash PREFAB_FIREFIGHTING_CUTTER_PUMP;
		static const qsf::StringHash PREFAB_FIREFIGHTING_GROUNDHYDRANT;
		static const qsf::StringHash PREFAB_BUILDING_SMOKE;
		static const qsf::StringHash PREFAB_CHAINSAW_EFFECT;
		static const qsf::StringHash PREFAB_CHAINSAW_EFFECT_SMOKE;
		static const qsf::StringHash PREFAB_BREAK_DOOR_EFFECT;
		static const qsf::StringHash PREFAB_BARRIER_TAPE_PLACEMENT;

		static const qsf::StringHash PREFAB_FIRE_WATERJET_EFFECT;
		static const qsf::StringHash PREFAB_SHOOT_BLOOD_EFFECT;
		static const qsf::StringHash PREFAB_SHOOT_WEAPON_EFFECT;
		static const qsf::StringHash PARTICLE_WATERSPRAY;
		static const qsf::StringHash PARTICLE_WATERSPRAY2;
		static const qsf::StringHash PARTICLE_WATERDROPS;

		static const qsf::StringHash PREFAB_SPOTLIGHT;

		// Texture
		static const std::string TEXTURE_FIREFIGHTING_DRONESCAN_GROUND;

		// Events
		static const qsf::StringHash PREFAB_EVENT_POSITION_HELPER_ENTITY;

		// Spawn pools
		static const std::string SPAWNPOOL_DEFAULT_PERSON;
		static const std::string SPAWNPOOL_MEDIEVAL_PERSON;
		static const std::string SPAWNPOOL_DEFAULT_VEHICLE;

		static const std::string SPANWPOOL_GANGSTER;
		static const std::string SPANWPOOL_GANGSTER_EXTENDED;
		static const std::string SPANWPOOL_GANGSTER_TERRORIST;
		static const std::string SPANWPOOL_GANGSTER_CONVICT;

		// Medieval
		static const qsf::StringHash PREFAB_MEDIEVAL_DEAD_BODY1;
		static const qsf::StringHash PREFAB_MEDIEVAL_DEAD_BODY2;
		static const qsf::StringHash PREFAB_MEDIEVAL_DEAD_BODY3;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

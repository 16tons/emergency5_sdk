// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <boost/noncopyable.hpp>


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
	*    EMERGENCY 5 ground map definitions
	*/
	class GroundMaps : public boost::noncopyable
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		// Ground map IDs
		//  -> All IDs are basic bitmasks (just single bits) that can be combined to define filters for ground map queries
		enum GroundMapId
		{
			GROUNDMAP_TERRAIN			= 0x00000001,	///< Terrain + walkables level 0
			GROUNDMAP_LIQUIDS			= 0x00000100,	///< Liquids (e.g. water surfaces)
			GROUNDMAP_DYNAMIC_LIQUIDS	= 0x00000200,	///< Liquids (e.g. water surfaces) created at runtime
			GROUNDMAP_WALKABLES			= 0x00010000,	///< Walkables of levels 1..3
			GROUNDMAP_DYNAMIC_WALKABLES	= 0x00020000	///< Event-specific walkables created at runtime
		};

		// Filter masks
		//  -> Predefined bitmasks for ground map query filtering
		enum FilterMasks
		{
			// Basic filter bitmasks
			FILTER_MASK_ALL				= 0xffffffff,

			// Filter bitmasks for the most common use-cases
			FILTER_DEFAULT				= 0xffffffff,	///< Terrain + all walkable levels + liquids (e.g. water surfaces) surface
			FILTER_TERRAIN_ONLY			= 0x000000ff,	///< Terrain + walkables level 0
			FILTER_LIQUIDS_ONLY			= 0x0000ff00,	///< Liquids (e.g. water surfaces)
			FILTER_NO_LIQUIDS			= 0xffff00ff,	///< Terrain + all walkable levels
			FILTER_TERRAIN_LIQUIDS		= 0x0000ffff,	///< Terrain + walkables level 0 + liquids (e.g. water surfaces)
			FILTER_HELICOPTER_GROUND	= 0x0000ffff	///< Terrain + walkables level 0 + liquids (e.g. water surfaces)
		};


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	public:
		static void setupGroundMaps();
		static bool areGroundMapsValid();
		static void updateAllGroundMaps();


	//[-------------------------------------------------------]
	//[ Private static methods                                ]
	//[-------------------------------------------------------]
	private:
		static void updateFreeGroundAreaNavigationMap();


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

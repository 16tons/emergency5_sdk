// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/map/Map.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Map backup abstract base class
	*/
	class MapBackup
	{


	//[-------------------------------------------------------]
	//[ Public virtual qsf::MapBackup methods                 ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Destructor
		*/
		virtual ~MapBackup() {}

		/**
		*  @brief
		*    Create a backup of the map
		*
		*  @param[in] map
		*    The map instance to save
		*  @param[in] serializationOptions
		*    Configuration for the serialization
		*
		*  @return
		*    "true" if all went fine, else "false"
		*/
		virtual bool backupMap(const Map& map, const Map::SerializationOptions& serializationOptions) = 0;

		/**
		*  @brief
		*    Load a map from the backup
		*
		*  @param[in] map
		*    The map instance to load
		*  @param[in] serializationOptions
		*    Configuration for the serialization
		*
		*  @return
		*    "true" if all went fine, else "false"
		*/
		virtual bool restoreMap(Map& map, const Map::SerializationOptions& serializationOptions) const = 0;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf

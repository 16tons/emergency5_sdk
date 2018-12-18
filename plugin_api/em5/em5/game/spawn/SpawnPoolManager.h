// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"

#include <qsf/base/manager/Manager.h>

#include <boost/container/flat_map.hpp>

#include <unordered_map>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	class EM5_API_EXPORT SpawnPoolManager : public qsf::Manager
	{


	//[-------------------------------------------------------]
	//[ Public data                                           ]
	//[-------------------------------------------------------]
	public:
		struct SpawnInstanceData
		{
			float spawnRate;
			boost::container::flat_map<std::string, float> equipment;	///< Key is the name of the equipment prefab the entity is spawning, value is the percent of spawning rate
			std::string activity;	///< The name of the activity the entity gets at start
		};
		typedef const SpawnInstanceData* SpawnInstanceDataPointer;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		SpawnPoolManager();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~SpawnPoolManager();

		/**
		*  @brief
		*    Load the assets
		*/
		void loadAssets();

		/**
		*  @brief
		*    Load order information JSON file
		*
		*  @param[in] filename
		*    Path of the file to load
		*
		*  @return
		*    "true" if all went fine, "false" on error
		*/
		bool loadByFilename(const std::string& filename);

		/**
		*  @brief
		*    Get a random prefab local asset name from the given spawn pool
		*
		*  @param[in] spawnPoolName
		*    Name of the spawn pool to choose a random prefab from
		*
		*  @return
		*    Randomly chosen prefab local asset name
		*/
		const std::string& getRandomNameFromSpawnPool(const std::string& spawnPoolName, SpawnInstanceDataPointer* instanceData = nullptr) const;

		const std::string& getRandomEquipmentName(const SpawnInstanceData& instanceData) const;

		bool getSpawnpoolInstance(const std::string& spawnPoolName, const std::string& instanceName, SpawnInstanceDataPointer* instanceData = nullptr) const;


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		struct SpawnPoolData
		{
			boost::container::flat_map<std::string, SpawnInstanceData> data;	// Key is the name of the entity for spawning, value is the percent of spawning rate
		};


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		float calculateRandomTriggerValueFromSpawnPool(const SpawnPoolData& spawnPoolData) const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// TODO(mk) To improve performance use uint32 (hashed name) as key
		std::unordered_map<std::string, SpawnPoolData> mSpawnPools;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

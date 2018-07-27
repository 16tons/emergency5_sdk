// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/map/backup/MapBackup.h"
#include "qsf/component/base/MetadataComponent.h"

#include <boost/property_tree/ptree.hpp>	// Needed for definitions of "boost::property_tree::ptree::const_iterator"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Layer;
}


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
	*    Map backup implementation using a boost property tree
	*/
	class QSF_API_EXPORT BoostPTreeMapBackup : public MapBackup
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 FORMAT_VERSION;		///< Current map backup format version


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		BoostPTreeMapBackup();

		/**
		*  @brief
		*    Destructor
		*/
		~BoostPTreeMapBackup();

		/**
		*  @brief
		*    Check if the map backup is empty
		*
		*  @return
		*    "true" if the map backup is empty, "false" if it has any content
		*/
		bool isEmpty() const;

		/**
		*  @brief
		*    Clear the map backup's contents
		*/
		void clear();

		/**
		*  @brief
		*    Get a reference to the internal Boost property tree structure
		*
		*  @return
		*    The Boost property tree, do not destroy the instance
		*/
		boost::property_tree::ptree& getBoostPTree();

		/**
		*  @brief
		*    Write out the map backup as JSON formatted UTF-8 string
		*
		*  @param[out] outJsonString
		*    JSON string to fill
		*/
		void getBackupAsJsonString(std::string& outJsonString) const;


	//[-------------------------------------------------------]
	//[ Public virtual qsf::MapBackup methods                 ]
	//[-------------------------------------------------------]
	public:
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
		virtual bool backupMap(const Map& map, const Map::SerializationOptions& serializationOptions) override;

		/**
		*  @brief
		*    Load a map from the backup
		*
		*  @param[out] map
		*    The map instance to load
		*  @param[in] serializationOptions
		*    Configuration for the serialization
		*
		*  @return
		*    "true" if all went fine, else "false"
		*/
		virtual bool restoreMap(Map& map, const Map::SerializationOptions& serializationOptions) const override;


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		struct EntityDescriptor
		{
			MetadataComponent::Quality		   mQuality;
			uint64							   mId;
			Entity*							   mEntity;
			std::vector<EntityDescriptor*>	   mChildren;
			const boost::property_tree::ptree* mSourcePTree;
		};

		struct LayerDescriptor
		{
			uint32 mLayerId;
			std::vector<EntityDescriptor*> mEntities;	///< Only top-level entities, no children
		};


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Load layers
		*
		*  @param[in] map
		*    Map instance to fill
		*  @param[in] rootPTree
		*    Boost root ptree
		*  @param[in] serializationOptions
		*    Configuration for the serialization
		*
		*  @exception
		*    Throws "std::exception" in case on an error
		*/
		static void loadLayers(Map& map, const boost::property_tree::ptree& rootPTree, const Map::SerializationOptions& serializationOptions);

		/**
		*  @brief
		*    Load layer
		*
		*  @param[in] layer
		*    Layer to fill
		*  @param[in] layerIterator
		*    Boost layer ptree iterator
		*  @param[in] serializationOptions
		*    Configuration for the serialization
		*
		*  @exception
		*    Throws "std::exception" in case on an error
		*/
		static void loadLayer(Layer& layer, const boost::property_tree::ptree::const_iterator& layerIterator, const Map::SerializationOptions& serializationOptions);

		/**
		*  @brief
		*    Load entities
		*
		*  @param[in] map
		*    Map instance to fill
		*  @param[in] rootPTree
		*    Boost root ptree
		*  @param[in] serializationOptions
		*    Configuration for the serialization
		*
		*  @exception
		*    Throws "std::exception" in case on an error
		*/
		static void loadEntities(Map& map, const boost::property_tree::ptree& rootPTree, const Map::SerializationOptions& serializationOptions);

		static void createEntitiesRecursive(const std::vector<EntityDescriptor*>& entityDescriptors, Map& map);
		static void loadEntitiesRecursive(const std::vector<EntityDescriptor*>& entityDescriptors, std::vector<EntityDescriptor*>& failedEntities, const Map::SerializationOptions& serializationOptions);

		/**
		*  @brief
		*    Save layers
		*
		*  @param[in] layerManager
		*    Layer manager instance so save
		*  @param[in] rootPTree
		*    Boost root ptree
		*
		*  @exception
		*    Throws "std::exception" in case on an error
		*/
		static void saveLayers(const LayerManager& layerManager, boost::property_tree::ptree& rootPTree);

		/**
		*  @brief
		*    Save layer
		*
		*  @param[in] layer
		*    Layer instance so save
		*  @param[in] layerPTree
		*    Boost layer ptree
		*
		*  @exception
		*    Throws "std::exception" in case on an error
		*/
		static void saveLayer(const Layer& layer, boost::property_tree::ptree& layerPTree);

		/**
		*  @brief
		*    Save entities
		*
		*  @param[in] map
		*    Map instance so save
		*  @param[in] rootPTree
		*    Boost root ptree
		*  @param[in] serializationOptions
		*    Configuration for the serialization
		*
		*  @exception
		*    Throws "std::exception" in case on an error
		*/
		static void saveEntities(const Map& map, boost::property_tree::ptree& rootPTree, const Map::SerializationOptions& serializationOptions);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		boost::property_tree::ptree mRootPTree;		///< Boost property tree used for storing the backup


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf

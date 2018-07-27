// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/map/Map.h"
#include "qsf/prototype/serializer/BinaryPrototypeSerializationHelper.h"


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
	*    Binary map serialization helper
	*/
	class BinaryMapSerializationHelper : public BinaryPrototypeSerializationHelper
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const std::string FORMAT_TYPE;		///< Map format type string "Map"
		static const uint32		 FORMAT_VERSION;	///< Current map format version


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		BinaryMapSerializationHelper();

		/**
		*  @brief
		*    Destructor
		*/
		~BinaryMapSerializationHelper();

		/**
		*  @brief
		*    Serialize or deserialize a map
		*/
		bool serializeMap(BinarySerializer& serializer, Map& map, Map::SerializationOptions serializationOptions);


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		struct LayerDescriptor
		{
			Layer* mLayer;
			std::vector<PrototypeDescriptor> mEntities;   ///< Only top-level entities, no children
		};

		struct MyObjectSerializationOptions : public ObjectSerializationOptions
		{
			inline MyObjectSerializationOptions(const Map::SerializationOptions& mapSerializationOptions) :
				ObjectSerializationOptions(mapSerializationOptions.mDifferenceToDefault ? Object::SERIALIZE_DIFFERENTIAL : Object::SERIALIZE_COMPLETE,
										   mapSerializationOptions.mCompatibleMode ? Object::MODE_COMPATIBLE : Object::MODE_MINIMAL,
										   mapSerializationOptions.mFilterLoadInGame,
										   mapSerializationOptions.mSetPropertyOverrideFlags)
			{
				mComponentIdFilterCallback = mapSerializationOptions.mComponentIdFilterCallback;
			}
		};


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void buildLayerDescriptors(std::vector<LayerDescriptor>& layerDescriptors, Map& map);

		void serializeLayers(BinarySerializer& serializer, std::vector<LayerDescriptor>& layerDescriptors, Map& map, const ObjectSerializationOptions& serializationOptions);
		void deserializeLayers(BinarySerializer& serializer, std::vector<LayerDescriptor>& layerDescriptors, Map& map, const ObjectSerializationOptions& serializationOptions);

		void deserializePrototypeDescriptorRecursive(BinarySerializer& serializer, PrototypeDescriptor& prototypeDescriptor, const ObjectSerializationOptions& serializationOptions, Map& map, uint8 mapQuality, bool parentExists);

		void serializeEntities(BinarySerializer& serializer, const std::vector<LayerDescriptor>& layerDescriptors, const ObjectSerializationOptions& serializationOptions);
		void deserializeEntities(BinarySerializer& serializer, const std::vector<LayerDescriptor>& layerDescriptors, const ObjectSerializationOptions& serializationOptions);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		uint32 mTotalNumberOfEntities;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf

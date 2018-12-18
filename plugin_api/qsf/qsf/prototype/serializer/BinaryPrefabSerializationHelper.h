// Copyright (C) 2012-2018 Promotion Software GmbH


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
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Binary prefab serialization helper
	*/
	class BinaryPrefabSerializationHelper : public BinaryPrototypeSerializationHelper
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const std::string FORMAT_TYPE;		///< Map format type string "Prefab"
		static const uint32		 FORMAT_VERSION;	///< Current prefab format version


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		BinaryPrefabSerializationHelper();

		/**
		*  @brief
		*    Destructor
		*/
		~BinaryPrefabSerializationHelper();

		/**
		*  @brief
		*    Deserialize a prefab
		*/
		Prototype* deserializePrefab(BinarySerializer& serializer, PrototypeManager& prototypeManager);

		/**
		*  @brief
		*    Serialize a prefab
		*/
		bool serializePrefab(BinarySerializer& serializer, Prototype& prototype);


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void deserializePrototypeDescriptorRecursive(BinarySerializer& serializer, PrototypeDescriptor& prototypeDescriptor, const ObjectSerializationOptions& serializationOptions, PrototypeManager& manager);


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf

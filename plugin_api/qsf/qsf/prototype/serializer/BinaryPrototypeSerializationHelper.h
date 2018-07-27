// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/reflection/object/Object.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class BinarySerializer;
	class Component;
	class Prototype;
	class PrototypeManager;
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
	*    Binary prototype serialization helper
	*
	*  @remarks
	*    This class holds some shared functionality for map and prefab serialization
	*/
	class BinaryPrototypeSerializationHelper
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		struct PrototypeDescriptor
		{
			Prototype*						 mPrototype;
			std::vector<Component*>			 mComponents;
			std::vector<PrototypeDescriptor> mChildren;
		};

		struct ObjectSerializationOptions
		{
			Object::SerializationMethod   mMethod;
			Object::SerializationMode     mMode;
			bool						  mSetPropertyOverrideFlags;
			bool						  mFilterLoadInGame;
			boost::function<bool(uint32)> mComponentIdFilterCallback;

			inline ObjectSerializationOptions(Object::SerializationMethod method, Object::SerializationMode mode, bool filterLoadInGame = false, bool setPropertyOverrideFlags = true) :
				mMethod(method),
				mMode(mode),
				mSetPropertyOverrideFlags(setPropertyOverrideFlags),
				mFilterLoadInGame(filterLoadInGame)
			{}
		};


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		BinaryPrototypeSerializationHelper();

		/**
		*  @brief
		*    Destructor
		*/
		~BinaryPrototypeSerializationHelper();

		void addAndFillPrototypeDescriptorRecursive(std::vector<PrototypeDescriptor>& prototypeDescriptors, Prototype& prototype);

		void serializePrototypeDescriptorRecursive(BinarySerializer& serializer, const PrototypeDescriptor& prototypeDescriptor, const ObjectSerializationOptions& serializationOptions, bool writeMetadataQuality);
		// There is no "deserializePrototypeDescriptorRecursive", as this is specific to either maps or prefabs, as it has to create the instances

		void serializePrototypeRecursive(BinarySerializer& serializer, const PrototypeDescriptor& prototypeDescriptor, const ObjectSerializationOptions& serializationOptions);
		void deserializePrototypeRecursive(BinarySerializer& serializer, const PrototypeDescriptor& prototypeDescriptor, const ObjectSerializationOptions& serializationOptions, std::vector<Prototype*>& failedEntities);


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		PrototypeManager& mPrototypeManager;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf

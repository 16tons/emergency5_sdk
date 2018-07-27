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
	class Prototype;
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
	*    Static prototype serialization helper class for serialization from and to a Boost property tree
	*/
	class BoostPTreePrototypeSerializationHelper
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		// TODO(fw): Add descriptions
		static void serializePrototypeToPTree(const Prototype& prototype, boost::property_tree::ptree& pTree, Object::SerializationMethod serializationMethod);

		// Returns "false" in case the prototype could not be loaded and should be destroyed again
		static bool deserializePrototypeFromPTree(Prototype& prototype, const boost::property_tree::ptree& pTree, Object::SerializationMethod serializationMethod, const std::string& prefabAssetFilename = "");


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf

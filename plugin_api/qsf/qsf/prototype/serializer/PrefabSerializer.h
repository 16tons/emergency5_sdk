// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/asset/AssetDataSerializer.h"

#include <ostream>
#include <istream>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
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
	*    Abstract prefab serializer interface
	*
	*  @note
	*    - Must work with exceptions in order to be able to interact with standard C++ streams
	*    - A prefab can be identical to a prototype, or consist of multiple prototypes linked together in order to form a more complex structure
	*/
	class PrefabSerializer : public AssetDataSerializer
	{


	//[-------------------------------------------------------]
	//[ Public virtual qsf::PrefabSerializer methods          ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Deserialize a prototype by using a given input stream
		*
		*  @param[in] prototypeManager
		*    Prototype manager to add the prefab to
		*  @param[in] rootPTree
		*    Boost root ptree
		*  @param[in] prefabAssetFilename
		*    Prefab asset filename for better log error message in case of problems
		*
		*  @return
		*    Pointer to the prefab's main prototype, or a null pointer if loading failed
		*
		*  @exception
		*    Throws "std::exception" in case on an error
		*/
		virtual Prototype* deserialize(PrototypeManager& prototypeManager, std::istream& istream, const std::string& prefabAssetFilename) const = 0;

		/**
		*  @brief
		*    Serialize the prototype into a given output stream
		*
		*  @param[in] prototype
		*   Prototype instance to serialize
		*  @param[in] ostream
		*    Output stream to serialize the prototype into
		*  @param[in] differenceToDefault
		*    Do not serialize properties which have the default value (meaning we already know its value without having to read it when deserializing)
		*
		*  @exception
		*    Throws "std::exception" in case on an error
		*/
		virtual void serialize(Prototype& prototype, std::ostream& ostream, bool differenceToDefault = true) const = 0;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf

// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/prototype/serializer/PrefabSerializer.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace boost
{
	namespace property_tree
	{
		template <typename U, typename V, typename W>
		class basic_ptree;
		typedef basic_ptree<std::string, std::string, std::less<std::string> > ptree;
	}
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
	*    JSON prefab serializer
	*/
	class JsonPrefabSerializer : public PrefabSerializer
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 FORMAT_VERSION;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		inline JsonPrefabSerializer();

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~JsonPrefabSerializer();

		/**
		*  @brief
		*    Load a JSON prefab
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
		*    Throws "std::exception" in case of an error
		*/
		Prototype* loadJson(PrototypeManager& prototypeManager, const boost::property_tree::ptree& rootPTree, const std::string& prefabAssetFilename) const;


	//[-------------------------------------------------------]
	//[ Public virtual qsf::PrefabSerializer methods          ]
	//[-------------------------------------------------------]
	public:
		virtual Prototype* deserialize(PrototypeManager& prototypeManager, std::istream& istream, const std::string& prefabAssetFilename) const override;
		virtual void serialize(Prototype& prototype, std::ostream& ostream, bool differenceToDefault = true) const override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Load all prototypes of the prefab
		*
		*  @param[in] prototype
		*    Main prototype instance to fill
		*  @param[in] parentPTree
		*    Boost parent ptree
		*  @param[in] prefabAssetFilename
		*    Prefab asset filename for better log error message in case of problems
		*
		*  @exception
		*    Throws "std::exception" in case on an error
		*/
		Prototype* loadPrefab(PrototypeManager& prototypeManager, const boost::property_tree::ptree& parentPTree, const std::string& prefabAssetFilename) const;

		/**
		*  @brief
		*    Load a prototype
		*
		*  @param[in] prototype
		*    Prototype instance to fill
		*  @param[in] parentPTree
		*    Boost parent ptree
		*
		*  @exception
		*    Throws "std::exception" in case on an error
		*
		*  @note
		*    - Legacy method for loading format version 1 prefabs
		*/
		void loadPrototype(Prototype& prototype, const boost::property_tree::ptree& parentPTree) const;

		/**
		*  @brief
		*    Load prototype components
		*
		*  @param[in] prototype
		*    Prototype instance to fill
		*  @param[in] parentPTree
		*    Boost parent ptree
		*
		*  @exception
		*    Throws "std::exception" in case on an error
		*
		*  @note
		*    - Legacy method for loading format version 1 prefabs
		*/
		void loadComponents(Prototype& prototype, const boost::property_tree::ptree& parentPTree) const;

		/**
		*  @brief
		*    Save prototype
		*
		*  @param[in] prototype
		*    Prototype to save
		*  @param[in] differenceToDefault
		*    Do not save properties which have the default value (meaning we already know its value without having to read it when loading)
		*  @param[in] parentPTree
		*    Parent PTree
		*
		*  @exception
		*    Throws "std::exception" in case on an error
		*/
		void savePrototype(const Prototype& prototype, bool differenceToDefault, boost::property_tree::ptree& parentPTree) const;

		/**
		*  @brief
		*    Write an error message for the situation that a requested prototype ID is already used and hence a prefab could not be loaded
		*
		*  @param[in] prototype
		*    Prototype instance which is using the prototype ID we wanted to use
		*
		*  @exception
		*    Throws "std::exception" in case on an error
		*/
		void prototypeIdAlreadyUsed(const Prototype& existingPrototype) const;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/prototype/serializer/JsonPrefabSerializer-inl.h"

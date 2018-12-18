// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/Export.h"
#include "qsf/base/GetUninitialized.h"
#include "qsf/asset/AssetSystemTypes.h"

#include <boost/container/flat_set.hpp>

#include <vector>
#include <unordered_map>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace camp
{
	class UserObject;
}
namespace qsf
{
	class Prototype;
	class PrototypeManager;
	class Entity;
	class AssetProxy;
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
	*    Prefab content class
	*
	*  @note
	*    - This is a simple helper class for the QSF editor's asset import manager, and should be used with care
	*/
	class QSF_API_EXPORT PrefabContent
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		typedef std::vector<Prototype*>			   PrototypeArray;
		typedef std::unordered_map<uint64, uint64> UniqueIdMap;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		PrefabContent();

		/**
		*  @brief
		*    Destructor
		*/
		~PrefabContent();

		/**
		*  @brief
		*    Return the prototype manager of the prefab content
		*
		*  @return
		*    The prototype manager of the prefab content, do not destroy the instance
		*/
		inline PrototypeManager& getPrototypeManager() const;

		/**
		*  @brief
		*    Return the main prototype
		*
		*  @return
		*    The prototype on top of the parent-child prototype hierarchy
		*/
		inline Prototype* getMainPrototype() const;

		/**
		*  @brief
		*    Check if there is any prototype in this prefab
		*
		*  @return
		*    "true" if there is no prototype, "false" if there is at least one
		*/
		inline bool empty() const;

		/**
		*  @brief
		*    Return the prototypes of this prefab
		*
		*  @return
		*    The array of prototypes, sorted in a way that parent prototypes are listed before their children
		*/
		inline const PrototypeArray& getPrototypes() const;

		/**
		*  @brief
		*    Return the prefab name
		*
		*  @return
		*    The UTF-8 prefab name
		*/
		std::string getPrefabName() const;

		//[-------------------------------------------------------]
		//[ Fill the prefab content                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Insert a prototype
		*
		*  @param[in] prototype
		*    The prototype to add; should not be part of this prefab already and has to be registered at the prefab's prototype manager
		*/
		void insertPrototype(Prototype& prototype);

		/**
		*  @brief
		*    Create clones of a prototype and its child prototypes inside this prefab content
		*
		*  @param[in] originalPrototype
		*    The prototype to clone
		*  @param[in] uniqueIdMap
		*    Maps prototype IDs inside the original prototype to new prototype IDs that should be used as a replacement
		*  @param[in] copyOverrideFlags
		*    If "true", the property override flags are copied as well
		*  @param[in] followAllLinks
		*    If "true", all kinds of child links are followed when gathering the prototype children to clone; otherwise only hard-links are used
		*  @param[out] copiedOriginalPrototypes
		*    Optional list of the original prototypes that were actually cloned
		*
		*  @remarks
		*    The unique ID map has to contain IDs for all prototypes to be cloned as keys; all others will be ignored.
		*    The entries in copiedOriginalPrototypes are congruent to the internal prototypes list (see "getPrototypes()"), i.e. original and clone share the same index in the arrays
		*/
		void cloneFromPrototype(const Prototype& originalPrototype, const UniqueIdMap& uniqueIdMap, bool copyOverrideFlags, bool followAllLinks, std::vector<const Prototype*>* copiedOriginalPrototypes = nullptr);

		/**
		*  @brief
		*    Add base prefab references to the given original prototypes and prefab, and optionally apply a name and position
		*
		*  @param[in] originalPrototypes
		*    Original prototypes this prefab content was cloned from; must be congruent to the prefab content's internal list of prototypes
		*  @param[in] globalPrefabAssetId
		*    The global asset ID of the prefab this was cloned from; can be uninitialized when no base prefab references should be set
		*  @param[in] name
		*    Optional name to set for the main prototype in this prefab content
		*  @param[in] position
		*    Optional position to set for the main prototype in this prefab content
		*
		*  @remarks
		*    The unique ID map has to contain IDs for all prototypes to be cloned as keys; all others will be ignored.
		*    The entries in copiedOriginalPrototypes are congruent to the internal prototypes list (see "getPrototypes()"), i.e. original and clone share the same index in the arrays
		*/
		void processForEntityInstancing(const std::vector<const Prototype*>& originalPrototypes, GlobalAssetId globalPrefabAssetId, const std::string* name = nullptr, const glm::vec3* position = nullptr);

		/**
		*  @brief
		*    Copy a prefab and merge in local changes of the given entity
		*
		*  @param[in] entityToUpdate
		*    The entity to replace, and from which local modifications should be taken
		*  @param[in] prefabMainPrototype
		*    The main prototype of the prefab to be copied as a base
		*
		*  @remarks
		*    The prefab content's prototypes will use the same IDs as the entity, as far as possible
		*/
		void copyEntityWithUpdateFromPrefab(Entity& entityToUpdate, Prototype& prefabMainPrototype, bool forBasePrefabChange = false);

		/**
		*  @brief
		*    Clone prototypes from the given prototype and add them to the prefab
		*/
		Prototype* fillPrefabWithPrototypeToSave(const Prototype& prototype, UniqueIdMap& uniqueIdMap, GlobalAssetId globalPrefabAssetId = getUninitialized<GlobalAssetId>(), bool removeInheritance = true, bool preserveExistingIds = false);

		/**
		*  @brief
		*    Remove base prefab and prototype references to the given prefab asset
		*
		*  @remarks
		*    References to other bases are not modified.
		*    Also this removes only one level of inheritance, i.e. prototypes refer to the removed base's base afterwards.
		*/
		void removeBasePrefabInheritance(GlobalAssetId basePrefabAssetId);

		//[-------------------------------------------------------]
		//[ Slot link references                                  ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Use the given mapping to replace all unique IDs in links and slots inside this prefab
		*/
		void translateUniqueIds(const UniqueIdMap& uniqueIdMap);

		/**
		*  @brief
		*    Remove the given unique IDs from links and slots inside this prefab
		*/
		void removeUniqueIdsFromSlots(const boost::container::flat_set<uint64>& idsToRemove);

		//[-------------------------------------------------------]
		//[ Dependencies                                          ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Gather direct global asset ID dependencies
		*
		*  @param[out] directGlobalAssetIdDependencies
		*    Receives the direct global asset ID dependencies, list is not cleared before new entries are added
		*/
		void gatherDirectGlobalAssetIdDependencies(boost::container::flat_set<GlobalAssetId>& directGlobalAssetIdDependencies) const;

		/**
		*  @brief
		*    Check if the prefab content would introduce a cyclic dependency in prefab inheritance
		*/
		bool performCyclicDependencyCheck(const AssetProxy& assetProxy) const;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Gather direct global asset ID dependencies by searching for asset proxy properties inside a given CAMP user object
		*
		*  @param[in] campUserObject
		*    CAMP user object to search for asset proxy properties
		*  @param[out] directGlobalAssetIdDependencies
		*    Receives the direct global asset ID dependencies, list is not cleared before new entries are added
		*/
		void gatherDirectGlobalAssetIdDependenciesFromCampUserObject(const camp::UserObject& campUserObject, boost::container::flat_set<GlobalAssetId>& directGlobalAssetIdDependencies) const;

		std::string choosePrototypeName(const Prototype& originalPrototype, bool clone) const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		PrototypeManager& mPrototypeManager;	///< Prototype manager used especially for storing this prefab
		PrototypeArray	  mPrototypes;			///< Array of prototypes that are part of this prefab; don't destroy the instances


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/prototype/helper/PrefabContent-inl.h"

// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/prototype/BasePrototypeManager.h"
#include "qsf/asset/AssetSystemTypes.h"
#include "qsf/base/StringHash.h"

#include <boost/container/flat_map.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Prototype;
	class CachedAsset;
	class AssetSystem;
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
	*    Prototype manager class
	*
	*  @remarks
	*    The following example shows how to get the instance of a certain prefab main prototype:
	*    @code
	*    qsf::Prototype* prototype = QSF_MAINPROTOTYPE.getPrefabByLocalAssetId("sample/prefab/vehicle/ambulance");
	*    @endcode
	*
	*  @note
	*    - The order of the prototype instances inside the prototype manager does not matter
	*
	*  @see
	*    - "qsf::Prototype" class documentation for information about the prototype concept, e.g. for how to get component instances
	*/
	class QSF_API_EXPORT PrototypeManager : public BasePrototypeManager
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class PrototypeSystem;	// Only the prototype system is allowed to create/destroy prototype manager instances


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		//[-------------------------------------------------------]
		//[ Prototype (the atomic thing)                          ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Create a new and empty prototype instance
		*
		*  @param[in] id
		*    The ID to assign to the new prototype
		*
		*  @return
		*    The created prototype instance, null pointer on error (unique prototype identifier already used?), do no destroy the returned
		*    instance and do need keep a reference to it outside the C runtime stack (use the unique prototype ID instead)
		*
		*  @note
		*    - The new prototype instance has no components at all. There's e.g. no "qsf::MetadataComponent"-instance holding the human readable
		*      unique prefab name. We have to be able to use customized "qsf::MetadataComponent"-versions, so we can't build something like that into the QSF core.
		*/
		inline Prototype* createPrototypeById(uint64 id);

		/**
		*  @brief
		*    Create a new prototype instance by cloning from another prototype
		*
		*  @return
		*    The created prototype instance; you are responsible for destroying the instance
		*
		*  @note
		*    - Use this method only in case you know exactly what you are doing
		*/
		Prototype* createPrototypeClone(const Prototype& originalPrototype, uint64 prototypeId, bool copyOverrideState);

		/**
		*  @brief
		*    Destroy all prototypes within this prototype manager
		*/
		void destroyAllPrototypes();

		/**
		*  @brief
		*    Return a prototype name that is not used by another prototype
		*
		*  @param[in] firstPart
		*    The first part of the desired prototype name, it will be expanded by a number
		*
		*  @return
		*    A free name that can be used for a new prototype
		*/
		std::string generateUnusedPrototypeName(const std::string& firstPart);

		//[-------------------------------------------------------]
		//[ Prefab (the constructed thing)                        ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Create a new prefab instance by cloning from another prefab
		*
		*  @return
		*    The created prototype instance; you are responsible for destroying the instance
		*
		*  @note
		*    - Use this method only in case you know exactly what you are doing
		*/
		Prototype* createPrefabCloneByGlobalAssetId(const Prototype& originalPrototype, uint64 prototypeId, bool copyOverrideState, GlobalAssetId globalPrefabAssetId);

		/**
		*  @brief
		*    Return the global asset ID by using a given prototype ID
		*
		*  @param[in] id
		*    The prototype unique ID
		*
		*  @return
		*    The requested global asset ID, "qsf::getUninitialized<GlobalAssetId>()" on error
		*/
		GlobalAssetId getPrefabGlobalAssetIdByPrototypeId(uint64 id) const;

		/**
		*  @brief
		*    Return the local asset ID by using a given prototype ID
		*
		*  @param[in] id
		*    The prototype unique ID
		*
		*  @return
		*    The requested local asset ID, "qsf::getUninitialized<LocalAssetId>()" on error
		*/
		LocalAssetId getPrefabLocalAssetIdByPrototypeId(uint64 id) const;

		/**
		*  @brief
		*    Return a prefab's main prototype instance by using its global asset ID
		*
		*  @param[in] globalAssetId
		*    The global prefab asset ID
		*  @param[in] load
		*    In case the prefab is not loaded, yet, do automatically load it?
		*
		*  @return
		*    The requested main prototype instance, null pointer on error
		*
		*  @note
		*    - In case there's no instance of the given asset, yet, an instance will be created on-the-fly
		*/
		Prototype* getPrefabByGlobalAssetId(GlobalAssetId globalAssetId, bool load = true);

		/**
		*  @brief
		*    Return a prefab's main prototype instance by using its local asset ID
		*
		*  @param[in] stringHash
		*    The local prefab asset ID generated by "qsf::StringHash" (example: "sample/prefab/vehicle/ambulance")
		*  @param[in] load
		*    In case the prefab is not loaded, yet, do automatically load it?
		*
		*  @return
		*    The requested main prototype instance, null pointer on error
		*
		*  @note
		*    - In case there's no instance of the given asset, yet, an instance will be created on-the-fly
		*/
		Prototype* getPrefabByLocalAssetId(const StringHash& stringHash, bool load = true);

		/**
		*  @brief
		*    Destroy a prefab instance by using its main prototype unique ID
		*
		*  @param[in] id
		*    The prototype unique ID
		*
		*  @return
		*    "true" if all went fine, else "false" (unknown prototype ID?)
		*/
		bool destroyPrefabById(uint64 id);

		/**
		*  @brief
		*    Destroy a prefab instance by using its global asset identifier
		*
		*  @param[in] globalPrefabAssetId
		*    The global prefab asset identifier
		*
		*  @return
		*    "true" if all went fine, else "false" (unknown prefab identifier?)
		*
		*  @note
		*    - Does not destroy linked prefabs
		*/
		bool destroyPrefabByGlobalAssetId(GlobalAssetId globalPrefabAssetId);

		/**
		*  @brief
		*    Save a prefab by using a given filename
		*
		*  @param[in] mainPrototype
		*    Main prototype of the prefab to save
		*  @param[in] filename
		*    UTF-8 filename in platform-independent notation of the prototype to save
		*  @param[in] differenceToDefault
		*    Do not save properties which have the default value (meaning we already know its value without having to read it when loading)
		*
		*  @return
		*    "true" if all went fine, else "false"
		*
		*  @todo
		*    - TODO(co) For now, we only support JSON
		*/
		bool savePrefabByFilename(Prototype& mainPrototype, const std::string& filename, bool differenceToDefault = true) const;


	//[-------------------------------------------------------]
	//[ Public virtual qsf::BasePrototypeManager methods      ]
	//[-------------------------------------------------------]
	public:
		virtual bool destroyPrototypeById(uint64 id) override;
		virtual bool changePrototypeId(uint64 oldId, uint64 newId) override;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::BasePrototypeManager methods   ]
	//[-------------------------------------------------------]
	protected:
		virtual Prototype* createPrototypeInstance(uint64 id) override;
		virtual uint64 generatePrototypeIdInternal() override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] id
		*    The unique prototype manager identifier
		*/
		explicit PrototypeManager(uint32 id);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~PrototypeManager();

		void onAssetsMounted(const Assets& assets);
		void onAssetsUnmounted(const Assets& assets);

		void registerPrefabByGlobalAssetId(Prototype& prototype, GlobalAssetId globalAssetId);
		Prototype* unregisterPrototypeById(uint64 id, GlobalAssetId* globalAssetId = nullptr);
		void removePrefabsDependingOnBasePrefab(GlobalAssetId globalAssetId);


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		Prototype* loadPrefabByCachedAsset(const CachedAsset& cachedAsset, bool noCheckInMainPrototypeMap);


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		typedef boost::container::flat_map<uint64, GlobalAssetId>	  PrefabIdMap;
		typedef boost::container::flat_map<GlobalAssetId, Prototype*> MainPrototypeMap;
		typedef std::vector<const CachedAsset*>						  PrefabAssetArray;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		AssetSystem&	 mAssetSystem;			///< Asset system instance, do not destroy the instance
		PrefabIdMap		 mPrefabIdMap;			///< Prefab map with unique prototype identifier as key and prefab global asset ID as value
		MainPrototypeMap mMainPrototypeMap;		///< Prefab map with prefab global asset ID as key and prototype instance as value
		PrefabAssetArray mLoadingPrefabStack;	///< Stack of currently loading prefab cached assets


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/prototype/PrototypeManager-inl.h"

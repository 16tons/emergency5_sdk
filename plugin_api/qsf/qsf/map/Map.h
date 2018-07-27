// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/prototype/BasePrototypeManager.h"
#include "qsf/base/StringHash.h"	// No forward declaration by intent
#include "qsf/math/EnumBitmask.h"
#include "qsf/asset/AssetSystemTypes.h"
#include "qsf/base/manager/PagedAllocator.h"

#include <boost/signals2.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Ogre
{
	class SceneManager;
}
namespace qsf
{
	class Object;
	class Entity;
	class Prototype;
	class Component;
	class LayerManager;
	class GroundMapManager;
	class CameraComponent;
	class ComponentManager;
	class DebugDrawManager;
	class MapBackup;
	class BinarySerializer;
	class GridSceneManager;
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
	*    Map class
	*
	*  @remarks
	*    Usually, you create entity instances by using a prototype
	*    @code
	*    // "QSF_MAINMAP" stands for "qsf::Qsf::instance()->getMapSystem().getMainMap()"
	*    qsf::Entity* qsfEntity = QSF_MAINMAP.createObjectByLocalPrefabAssetId("sample/prefab/vehicle/ambulance");
	*    // ... entity null pointer check would be a good idea ...
	*    uint64 myPersonId = qsfEntity->getId();	// "qsf::getUninitialized<uint64>()" if invalid entity ID
	*    @endcode
	*
	*    The following example shows how to get the instance of a certain entity:
	*    @code
	*    // uint64 myPersonId ...
	*    qsf::Entity* qsfEntity = QSF_MAINMAP.getEntityById(myPersonId);
	*    @endcode
	*
	*  @note
	*    - In QSF terminology we're talking about "maps", not "scenes" or "worlds" (don't mix up a QSF map with "std::map")
	*    - On the QSF abstraction level, a map is not scene graph based, it's just a list of entities
	*    - An entity is a concrete prototype instance
	*    - Everything within a map is an entity
	*    - "Objects" are formed by linking entities together, prefab instances generate objects (linked entities)
	*    - A map system is a collection of maps
	*    - By concept there can be only one current map instance at one and the same time
	*    - The order of the entity instances inside the map does not matter
	*    - The map is responsible for the creation and destruction of entity instance, no one else
	*    - For performance and simplicity reasons, during runtime entities should only be addressed by using the unique entity identifiers
	*    - Depends on the renderer system ("qsf::RendererSystem")
	*    - Depends on the prototype system ("qsf::PrototypeSystem")
	*
	*  @todo
	*    - TODO(co) Is the current implementation sufficient, or do we need something more sophisticated like the system described
	*      within the article "Managing Decoupling Part 4 — The ID Lookup Table" from Niklas Frykholm? ( http://bitsquid.blogspot.de/2011/09/managing-decoupling-part-4-id-lookup.html )
	*/
	class QSF_API_EXPORT Map : public BasePrototypeManager
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class Layer;
		friend class Prototype;
		friend class MapSystem;	// Only the map system is allowed to create/destroy map instances


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		typedef void (*CreateEntityIdMapCallback)(const std::vector<const Prototype*>&, UniqueIdMap&); // Callback to fill the unique id map from outside

		struct SerializationOptions
		{
			bool						  mDifferenceToDefault;			///< (Saving only) If "true", save as difference to default values, by using the property override flags
			bool						  mFilterLoadInGame;			///< (Loading only) If "true", only load layers marked as "LoadInGame" == "true" and entities which are not rejected by the map quality setting, otherwise do not filter at all
			bool						  mSetPropertyOverrideFlags;	///< (Loading only) If "false", property override flags are not necessarily set during load (for performance only, this is not guaranteed for all properties)
			bool						  mCompatibleMode;				///< If "true", the map is stored in a way that does not require the very same CAMP classes (for permanent saves, when the binaries may change)
			boost::function<bool(uint32)> mComponentIdFilterCallback;	///< Component ID filter callback, should return "false" to filter out components by ID
			boost::function<void(float)>  mProgressCallback;			///< Serialization progress callback (0...1 -> start...finished)

			SerializationOptions() :
				mDifferenceToDefault(true),
				mFilterLoadInGame(false),
				mSetPropertyOverrideFlags(true),
				mCompatibleMode(true)
			{}

			SerializationOptions(boost::function<void(float)> progressCallback) :
				mDifferenceToDefault(true),
				mFilterLoadInGame(false),
				mSetPropertyOverrideFlags(true),
				mCompatibleMode(true),
				mProgressCallback(progressCallback)
			{}
		};

		class EntityHashMapWrapper
		{
		public:
			struct iterator : public PrototypeHashMap::iterator
			{
				inline iterator(PrototypeHashMap::Entry* element = nullptr) : PrototypeHashMap::iterator(element) {}
				inline Entity* operator*()  { return reinterpret_cast<Entity*>(mEntry->mPrototype); }
			};

		public:
			inline EntityHashMapWrapper(const PrototypeHashMap& prototypeHashMap) : mPrototypeHashMap(prototypeHashMap) {}
			inline bool empty() const		{ return mPrototypeHashMap.empty(); }
			inline size_t size() const		{ return mPrototypeHashMap.size(); }
			inline Entity* front() const	{ return reinterpret_cast<Entity*>(mPrototypeHashMap.front()); }
			inline Entity* back() const		{ return reinterpret_cast<Entity*>(mPrototypeHashMap.back()); }

			inline iterator begin() const	{ return iterator(const_cast<PrototypeHashMap::Entry*>(mPrototypeHashMap.getFirstEntry())); }
			inline iterator end() const		{ return iterator(); }
			inline iterator rbegin() const	{ return iterator(const_cast<PrototypeHashMap::Entry*>(mPrototypeHashMap.getLastEntry())); }
			inline iterator rend() const	{ return iterator(); }

		private:
			const PrototypeHashMap& mPrototypeHashMap;
		};


	//[-------------------------------------------------------]
	//[ Public Boost signals                                  ]
	//[-------------------------------------------------------]
	public:
		boost::signals2::signal<void (const CameraComponent&)> PreCompositorWorkspaceUpdate;	///< This Boost signal is emitted before a compositor workspace gets updated; parameter: used camera component which is inside this map
		boost::signals2::signal<void ()>					   PreMapSave;						///< This Boost signal is emitted before a map is going to be saved, don't do work before the map is saved until you really have to (in case of a crash the artist will lose the work while saving, total fail)
		boost::signals2::signal<void ()>					   PostMapSave;						///< This Boost signal is emitted after the map has been be saved

		// TODO(co) This was added in the emergency EMERGENCY 5 development phase. Please don't use it if you can avoid it so we're able to get rid of it later on (constant overhead at the bottom).
		boost::signals2::signal<void (uint64)>				   ObjectDestroyed;		///< This Boost signal is emitted when an object gets destroyed (via destroyObjectById)


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		//[-------------------------------------------------------]
		//[ Map state                                             ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Return whether or not the map is currently up-and-running
		*
		*  @return
		*    "true" if the map is currently up-and-running (meaning "qsf::Map::startup()" was called successfully), else "false"
		*
		*  @remarks
		*    Maps which are currently up-and-running have e.g. audio visual representation as well as at least basic logic needed for the editor client.
		*    Not running maps are basically data storage.
		*/
		inline bool isRunning() const;

		/**
		*  @brief
		*    Return whether or not the map is currently in simulating mode
		*
		*  @return
		*    "true" if the map is currently in simulating mode, else "false"
		*
		*  @remarks
		*    Maps which are currently in simulating mode have e.g. active AI and game logic.
		*    If a map is in simulating mode, this implies it is also up-and-running, see "isRunning()".
		*/
		inline bool isSimulating() const;

		/**
		*  @brief
		*    Set the map and all running components inside to simulating mode
		*
		*  @remarks
		*    Maps which are currently in simulating mode have e.g. active AI and game logic.
		*    If a map is in simulating mode, this implies it is also up-and-running, see "isRunning()".
		*/
		void setSimulatingMode(bool simulatingMode);

		/**
		*  @brief
		*    Return whether or not the map is currently in server mode
		*
		*  @return
		*    "true" if the map is currently in server mode, else "false"
		*
		*  @remarks
		*    If the application is running in server mode, each map has this flag set.
		*/
		inline bool isInServerMode() const;

		/**
		*  @brief
		*    Set the map and all running components inside to server mode
		*
		*  @remarks
		*    If the application is running in server mode, each map has this flag set.
		*/
		void setServerMode(bool serverMode);

		//[-------------------------------------------------------]
		//[ Clear & garbage collection                            ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Clear the map
		*
		*  @note
		*    - Resets the map filename
		*    - Destroys all entities within this map
		*    - Destroys all layers within this map, except root and base layer
		*/
		void clear();

		/**
		*  @brief
		*    Perform garbage collection
		*
		*  @remarks
		*    In order to avoid destruction related issues, entities and entity components are not destroyed immediately.
		*    Instead, they are removed from the "alife" data structures and are added to a "destroyed" list. Call this
		*    garbage collection method after important actions or for example once per frame.
		*/
		void performGarbageCollection();

		//[-------------------------------------------------------]
		//[ Load & save                                           ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Load a map by using a global asset ID
		*
		*  @param[in] globalAssetId
		*    Global asset ID of the map to load
		*  @param[in] serializationOptions
		*    Configuration for the serialization
		*
		*  @return
		*    "true" if all went fine, else "false" (asset not found?)
		*
		*  @note
		*    - The current map will get lost (same effect as "qsf::Map::clear()"), so, you might want to save a manipulated map before loading a new one
		*    - On success, automatically makes the given global asset ID to the internally stored map global asset ID
		*
		*  @todo
		*    - TODO(co) For now, we only support JSON, BIN and "OgreMax Scene Exporter" (www.ogremax.com) XML scene support
		*/
		bool loadByGlobalAssetId(GlobalAssetId globalAssetId, const SerializationOptions& serializationOptions);

		/**
		*  @brief
		*    Load a map by using a given filename
		*
		*  @param[in] filename
		*    UTF-8 filename in platform-independent notation of the map to load
		*  @param[in] serializationOptions
		*    Configuration for the serialization
		*  @param[in] globalAssetId
		*    Global asset ID of the map to load (only stored internally, not really used for loading)
		*
		*  @return
		*    "true" if all went fine, else "false" (file not found?)
		*
		*  @note
		*    - The current map will get lost (same effect as "qsf::Map::clear()"), so, you might want to save a manipulated map before loading a new one
		*    - On success, automatically makes the given filename to the internally stored map filename
		*
		*  @todo
		*    - TODO(co) For now, we only support JSON, BIN and "OgreMax Scene Exporter" (www.ogremax.com) XML scene support
		*    - TODO(co) depreciated, don't use this method anymore, use "loadByGlobalAssetId()" instead
		*/
		bool loadByFilename(const std::string& filename, const SerializationOptions& serializationOptions, GlobalAssetId globalAssetId);

		/**
		*  @brief
		*    Save the map by using a given filename
		*
		*  @param[in] filename
		*    UTF-8 filename in platform-independent notation of the map to save
		*  @param[in] serializationOptions
		*    Configuration for the serialization
		*
		*  @return
		*    "true" if all went fine, else "false"
		*
		*  @todo
		*    - TODO(co) For now, we only support JSON and BIN
		*/
		bool saveByFilename(const std::string& filename, const SerializationOptions& serializationOptions);

		//[-------------------------------------------------------]
		//[ Map meta information                                  ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Return the global asset ID of the map
		*
		*  @return
		*    The global asset ID of the map
		*/
		inline GlobalAssetId getGlobalAssetId() const;

		/**
		*  @brief
		*    Set the global asset ID of the map (gets overridden on every load and save so this is only useful in those cases where the map is directly deserialized from a buffer)
		*
		*  @param[in] globalAssetId
		*    The global asset ID of the map
		*/
		inline void setGlobalAssetId(GlobalAssetId globalAssetId);

		/**
		*  @brief
		*    Return the version string of the map
		*
		*  @return
		*    The version string of the map
		*/
		inline const std::string& getVersionString() const;

		/**
		*  @brief
		*    Set the version string of the map
		*
		*  @param[in] versionString
		*    The version string of the map
		*/
		inline void setVersionString(const std::string& versionString);

		/**
		*  @brief
		*    Set the version string of the map
		*
		*  @param[in] versionString
		*    The version string of the map
		*
		*  @note
		*    - Adds "retail_" if not enduser build
		*/
		void setBuildDependentVersionString(const std::string& versionString);

		//[-------------------------------------------------------]
		//[ Serialization                                         ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Serialize the map to a backup
		*
		*  @param[in] mapBackup
		*    Map backup to save the map to
		*  @param[in] serializationOptions
		*    Configuration for the serialization
		*
		*  @return
		*    "true" if all went fine, else "false"
		*/
		bool createMapBackup(MapBackup& mapBackup, const SerializationOptions& serializationOptions) const;

		/**
		*  @brief
		*    Deserialize the map from a backup
		*
		*  @param[in] mapBackup
		*    Map backup tree to load the map from
		*  @param[in] serializationOptions
		*    Configuration for the serialization
		*
		*  @return
		*    "true" if all went fine, else "false"
		*/
		bool restoreMapBackup(const MapBackup& mapBackup, const SerializationOptions& serializationOptions);

		/**
		*  @brief
		*    Serialize or deserialize the map using a binary serializer
		*
		*  @param[in] serializer
		*    The serializer, which can be either in read or write mode
		*/
		void serialize(BinarySerializer& serializer);

		//[-------------------------------------------------------]
		//[ Manager                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Return the layer manager
		*
		*  @return
		*    The layer manager, do no destroy the returned instance
		*/
		inline LayerManager& getLayerManager() const;

		/**
		*  @brief
		*    Return the ground map manager
		*
		*  @return
		*    The ground map manager, do no destroy the returned instance
		*/
		inline GroundMapManager& getGroundMapManager() const;

		/**
		*  @brief
		*    Return the debug draw manager instance
		*
		*  @return
		*    The debug draw manager instance, don't destroy the returned instance
		*/
		inline DebugDrawManager& getDebugDrawManager() const;

		//[-------------------------------------------------------]
		//[ Entity                                                ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Return the core entity
		*
		*  @return
		*    Reference to the core entity, do not destroy the instance
		*
		*  @remarks
		*    In order to be as flexible and reusable as possible, QSF is extensively using
		*    the entity/component architecture. As a result certain features, which other
		*    projects would hardcode into a framework/engine, are realized as entity components
		*    in QSF. This way new core features can be plugged into a map or be exchanged without
		*    rewriting QSF. Here are some examples for such core components which usually only have
		*    a single instance per map:
		*    - Map properties component
		*    - Sky component
		*    - Water plane component
		*    - Time of day component
		*    - Weather component
		*
		*    Beside the flexibility, other advantages of this approach are support for edit/undo/redo/cooperative work
		*    inside the QSF editor or automatic serialization just to mention a few. This support comes without the need
		*    to explicitly implement it for the core components listed above (or any other components, of course).
		*
		*    This "qsf::Map::getCoreEntity()"-method exists to have a common central entity for
		*    the core components and to provide comfortable access to it.
		*
		*  @note
		*    - It's guaranteed that there's a core entity, even if it means that it has to be created during the method call
		*/
		Entity& getCoreEntity();
		const Entity& getCoreEntity() const;

		/**
		*  @brief
		*    Reserve entities
		*
		*  @param[in] numberOfEntities
		*    Number of entities to reserve
		*
		*  @note
		*    - You might want to reserve entities before starting an insertion of entity masses (performance)
		*/
		inline void reserveEntities(size_t numberOfEntities);

		/**
		*  @brief
		*    Create a new entity instance
		*
		*  @return
		*    The created entity instance, null pointer on error, do no destroy the returned
		*    instance and do need keep a reference to it outside the C runtime stack (use the unique entity ID instead)
		*
		*  @note
		*    - Use "createObjectByLocalPrefabAssetId()" instead of "createEntity()" whenever possible
		*/
		Entity* createEntity();

		/**
		*  @brief
		*    Create a new entity instance with an enforced given unique entity identifier
		*
		*  @param[in] id
		*    The unique (inside the map the entity is in) entity identifier
		*
		*  @return
		*    The created entity instance, null pointer on error (e.g. ID already used), do no destroy the returned
		*    instance and do need keep a reference to it outside the C runtime stack (use the unique entity ID instead)
		*
		*  @note
		*    - Use "createObjectByLocalPrefabAssetId()" instead of "createEntityById()" whenever possible
		*    - Use "createEntity()" instead of "createEntityById()" whenever possible, do not enforce unique entity identifier if you can avoid it
		*/
		Entity* createEntityById(uint64 id);

		/**
		*  @brief
		*    Create a new entity instance by cloning a prototype given by reference
		*
		*  @param[in] prototype
		*    Prototype instance to use
		*
		*  @return
		*    The created entity instance, null pointer on error, do no destroy the returned
		*    instance and do need keep a reference to it outside the C runtime stack (use the unique entity ID instead)
		*
		*  @todo
		*    - TODO(co) Provide such methods or just enforce ID only. Might make it more complicated in case one already has the concrete reference.
		*/
		Entity* createEntityByPrototypeReference(const Prototype& prototype);

		/**
		*  @brief
		*    Create a new entity instance by cloning a prototype given by ID generated by using the local prefab asset name
		*
		*  @param[in] stringHash
		*    The local prefab asset name of the prototype to use, generated by "qsf::StringHash" (example: "sample/prefab/vehicle/ambulance")
		*  @param[in] createEntityIdMapCallback
		*    Optional callback which fills the map of prototype IDs to IDs of the created entities
		*
		*  @return
		*    The created entity instance, null pointer on error, do no destroy the returned
		*    instance and do need keep a reference to it outside the C runtime stack (use the unique entity ID instead)
		*/
		Entity* createObjectByLocalPrefabAssetId(const StringHash& stringHash, CreateEntityIdMapCallback createEntityIdMapCallback = nullptr);

		/**
		*  @brief
		*    Destroy an entity instance by using its unique identifier
		*
		*  @param[in] id
		*    The unique (inside the map the entity is in) entity identifier
		*
		*  @return
		*    "true" if all went fine, else "false" (unknown entity identifier?)
		*/
		bool destroyEntityById(uint64 id);

		/**
		*  @brief
		*    Destroy an object instance by using its unique identifier
		*
		*  @param[in] id
		*    The unique (inside the map the entity is in) entity identifier
		*
		*  @return
		*    "true" if all went fine, else "false" (unknown entity identifier?)
		*/
		bool destroyObjectById(uint64 id);

		/**
		*  @brief
		*    Destroy all entities within this map
		*
		*  @note
		*    - Performs garbage collection at once after destroying all entities by calling "qsf::Map::performGarbageCollection()"
		*/
		void destroyAllEntities();

		/**
		*  @brief
		*    Return an entity instance by using its unique identifier
		*
		*  @param[in] id
		*    The unique (inside the map the entity is in) entity identifier
		*
		*  @return
		*    The requested entity instance, null pointer on error (maybe the ID is unknown?), do not destroy the instance
		*/
		inline Entity* getEntityById(uint64 id) const;

		/**
		*  @brief
		*    Return an entity instance by using its unique identifier
		*
		*  @param[in] id
		*    The unique (inside the map the entity is in) entity identifier
		*
		*  @return
		*    The requested entity instance, do not destroy the instance
		*
		*  @note
		*    - In case the entity does not exist, this method will throw an exception
		*/
		inline Entity& getEntityByIdSafe(uint64 id) const;

		/**
		*  @brief
		*    Return the entity list
		*
		*  @return
		*    Reference to the internal entity list, do not manipulate the list or destroy the entities
		*/
		inline const EntityHashMapWrapper& getEntities() const;

		/**
		*  @brief
		*    Get an array of entity instance pointers from an array of entity IDs
		*
		*  @param[in] entityIds
		*    Array of entity IDs
		*  @param[out] outEntities
		*    Array of entity instances, may contain null pointers; the array is automatically cleared
		*/
		void getEntitiesByIdArray(const std::vector<uint64>& entityIds, std::vector<Entity*>& outEntities) const;

		/**
		*  @brief
		*    Get an array of entity instance pointers from a Vector/list/set (or other iteratable container) of entity IDs
		*
		*  @param[in] entityIds
		*    Entity IDs
		*  @param[out] outEntities
		*    Array of entity instances; the array is automatically cleared
		*  @param[in] filterNullPointers
		*    If "true", all invalid entity IDs are ignored; if "false", the output will contains null pointers for those
		*/
		template<typename T>
		void getEntitiesByIds(const T& entityIds, std::vector<Entity*>& outEntities, bool filterNullPointers = false) const;

		//[-------------------------------------------------------]
		//[ Internal                                              ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Return the encapsulated OGRE scene manager instance
		*
		*  @return
		*    The encapsulated OGRE scene manager instance, can be a null pointer, do no destroy the returned instance
		*
		*  @note
		*    - Don't access the encapsulated OGRE scene manager instance if you don't have to
		*/
		inline Ogre::SceneManager* getOgreSceneManager() const;


	//[-------------------------------------------------------]
	//[ Public virtual qsf::BasePrototypeManager methods      ]
	//[-------------------------------------------------------]
	public:
		inline virtual bool containsEntities() const override  { return true; }


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
		*    The unique map identifier
		*/
		explicit Map(uint32 id);

		/**
		*  @brief
		*    Destructor
		*
		*  @note
		*    - Not virtual by intent
		*/
		~Map();

		/**
		*  @brief
		*    Clear the map
		*
		*  @note
		*    - Destroys all entities within this map
		*    - Destroys all layers within this map, except root and base layer
		*/
		void internalClear();

		//[-------------------------------------------------------]
		//[ Lifecycle                                             ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Startup the map
		*
		*  @param[in] numberOfWorkerThreads
		*    Number of worker threads to use, 0 for automatic detection, 1 for single-threaded processing, n for multi-threaded processing
		*
		*  @return
		*    "true" if all went fine, else "false"
		*
		*  @remarks
		*    The implementation does the following tasks:
		*    - Create the OGRE scene manager
		*/
		bool startup(uint8 numberOfWorkerThreads);

		/**
		*  @brief
		*    Shutdown the map
		*
		*  @note
		*    - This method should only called case "startup()" returned successfully
		*
		*  @remarks
		*    The implementation does the following tasks:
		*    - Destroy all entities within this map
		*    - Destroy the OGRE scene manager
		*/
		void shutdown();

		//[-------------------------------------------------------]
		//[ Helper methods                                        ]
		//[-------------------------------------------------------]
		void markEntityForDestruction(Entity& entity);
		void markComponentForDestruction(Component& component);


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		enum InternalFlags
		{
			SIMULATING_MODE,	///< If "true", the map is in simulating mode, which is a hint for components that we're not in the editor right now
			SERVER_MODE,		///< If "true", the map is in server mode, i.e. the application is running on the server
		};
		typedef EnumBitmask<uint8, InternalFlags> InternalFlagsSet;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// OGRE related
		GridSceneManager*		mOgreSceneManager;			///< OGRE scene manager, can be a null pointer, we're responsible for destroying the instance in case we no longer need it

		// Map meta information
		GlobalAssetId			mGlobalAssetId;				///< Global asset ID of the map
		std::string				mVersionString;				///< Version string of the map

		// Map internal
		std::vector<Object*>	mDestroyedObjectList;		///< List of all destroyed objects (without entities and components), will get deleted at the next "safe" point in time
		std::vector<Entity*>	mDestroyedEntityList;		///< List of all destroyed entities, will get deleted at the next "safe" point in time
		std::vector<uint64>		mDestroyedEntityIdList;		///< List of all destroyed entity IDs, will get deleted at the next "safe" point in time
		std::vector<Component*>	mDestroyedComponentList;	///< List of all destroyed components, will get deleted at the next "safe" point in time
		Entity*					mCoreEntity;				///< The core entity instance, can be a null pointer, we're responsible for destroying the instance in case we no longer need it
		InternalFlagsSet		mInternalFlags;				///< Internal map flags
		PagedAllocator<Entity>	mEntityAllocator;			///< Allocator for qsf::Entity instances
		EntityHashMapWrapper	mEntityHashMapWrapper;

		// Managers
		LayerManager*			mLayerManager;				///< Layer manager instance, always valid, we're responsible for destroying the instance in case we no longer need it
		GroundMapManager*		mGroundMapManager;			///< Ground map manager instance, always valid, we're responsible for destroying the instance in case we no longer need it
		DebugDrawManager*		mDebugDrawManager;			///< Debug draw manager instance, always valid


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/map/Map-inl.h"

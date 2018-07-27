// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/base/NamedIdentifier.h"
#include "qsf/math/EnumBitmask.h"
#include "qsf/reflection/object/Object.h"

#include <unordered_set>
#include <vector>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class LayerManager;
	class MetadataComponent;
	class Map;
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
	*    Layer class
	*
	*  @note
	*    - The metadata component automatically registers/unregisters the owner entity within the correct layer
	*    - Entities without a metadata component are not registered inside any layer, this should be an exception
	*    - Destroying a layer is a brutal act: All registered entities are now inside no layer; it's up to the user of the
	*      layers to e.g. move the entities to another layer or destroying the entities before destroying the layer
	*    - Derived from "qsf::Object" by intent in order to make it easier to do certain implementations inside the QSF editor
	*
	*  @todo
	*    - TODO(fw): The buffer map mechanism could be optimized; this map has a layer manager with three layers, which is pretty useless
	*/
	class QSF_API_EXPORT Layer : public Object
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class LayerManager;
		friend class MetadataComponent;


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 NAME;			///< "Name" unique class property ID inside the class
		static const uint32 LOAD_IN_GAME;	///< "LoadInGame" unique class property ID inside the class
		typedef std::unordered_set<uint64> EntityIdSet;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Destructor
		*
		*  @note
		*    - Do never destroy a layer manually, use "qsf::Layer::destroyLayerById()" instead
		*    - All registered entities are now inside no layer
		*    - Not virtual by intent
		*/
		~Layer();

		/**
		*  @brief
		*    Return the owner layer manager
		*
		*  @return
		*    The owner layer manager, do not destroy the instance
		*/
		inline LayerManager& getLayerManager() const;

		/**
		*  @brief
		*    Get the parent layer the layer is in
		*
		*  @return
		*    The parent layer the layer is in, null pointer if there's no parent (meaning this layer is the root)
		*/
		inline Layer* getParentLayer() const;

		/**
		*  @brief
		*    Set the parent layer the layer is in
		*
		*  @param[in] layer
		*    New parent layer
		*
		*  @note
		*    - Please note that changing the parent of the root layer is not allowed, such requests will be ignored
		*/
		void setParentLayer(Layer& layer);

		//[-------------------------------------------------------]
		//[ Properties                                            ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Return the unique layer identifier
		*
		*  @return
		*    The unique layer identifier, the base layer has ID 0
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline uint32 getId() const;

		/**
		*  @brief
		*    Return the layer name
		*
		*  @return
		*    The not unique layer name
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline const std::string& getName() const;

		/**
		*  @brief
		*    Return the layer name's hash value
		*
		*  @return
		*    The not unique layer name hash
		*/
		inline uint32 getNameHash() const;

		/**
		*  @brief
		*    Set the layer name
		*
		*  @param[in] name
		*    The not unique layer name
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline void setName(const std::string& name);

		/**
		*  @brief
		*    Return whether or not the layer is loaded at game startup
		*
		*  @return
		*    "true" if the layer will be flagged to be loaded in the game, else "false"
		*/
		inline bool getLoadInGame() const;

		/**
		*  @brief
		*    Return whether or not the layer is loaded at game startup
		*
		*  @return
		*    "true" if the layer will be flagged to be loaded in the game, else "false"
		*/
		bool getLoadInGameRecursive() const;

		/**
		*  @brief
		*    Set whether or not the layer is loaded at game startup
		*
		*  @param[in] loadInGame
		*    "true" if the layer is flagged to be loaded in the game, else "false"
		*/
		inline void setLoadInGame(bool loadInGame);

		/**
		*  @brief
		*    Copy properties from another layer into this layer
		*
		*  @param[in] layer
		*    Layer to copy the properties from
		*
		*  @note
		*    - This is only about serialized and hence none local properties
		*/
		void copyPropertiesFromLayer(const Layer& layer);

		//[-------------------------------------------------------]
		//[ Local properties                                      ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Return whether or not the layer is hidden
		*
		*  @return
		*    "true" if the layer is hidden, else "false"
		*
		*  @remarks
		*    "Hidden" is a passive special layer feature. While the content of hidden layers
		*    is identical to the content of not hidden layers, the content of hidden layers
		*    will not be visible inside the render viewport. This special property will
		*    not be serialized and e.g. inside the QSF editor will have no influence on the
		*    layer hidden state of other QSF editor users.
		*
		*  @note
		*    - Not connected to the CAMP reflection system by intent
		*/
		inline bool isHidden() const;

		/**
		*  @brief
		*    Set whether or not the layer is hidden
		*
		*  @param[in] hidden
		*   "true" if the layer is hidden, else "false"
		*
		*  @see
		*    - "qsf::Layer::isHidden()"
		*/
		void setHidden(bool hidden);

		/**
		*  @brief
		*    Return whether or not the layer is globally hidden
		*
		*  @return
		*    "true" if the layer is globally hidden, else "false"
		*
		*  @remarks
		*    Even in case a layer is not hidden, it might be globally hidden because
		*    the parent layer is hidden.
		*
		*  @note
		*    - Not connected to the CAMP reflection system by intent
		*/
		bool isGloballyHidden() const;

		//[-------------------------------------------------------]
		//[ Layers                                                ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Create a new layer
		*
		*  @return
		*    The created layer, null pointer on error, do not destroy the instance and
		*    do need keep a reference to it outside the C runtime stack (use the unique layer ID instead)
		*/
		Layer* createLayer();

		/**
		*  @brief
		*    Create a new layer instance with an enforced given unique layer identifier
		*
		*  @param[in] id
		*    The unique (inside the layer manager the layer is in) layer identifier
		*
		*  @return
		*    The created layer instance, null pointer on error (e.g. ID already used), do no destroy the returned
		*    instance and do need keep a reference to it outside the C runtime stack (use the unique layer ID instead)
		*
		*  @note
		*    - Use "createLayer()" instead of "createLayerWithId()" whenever possible, do not enforce unique layer identifier if you can avoid it
		*/
		Layer* createLayerWithId(uint32 id);

		/**
		*  @brief
		*    Destroy a layer instance by using its unique identifier
		*
		*  @param[in] id
		*    The unique (inside the layer manager the layer is in) layer identifier
		*
		*  @return
		*    "true" if all went fine, else "false" (unknown layer identifier? invincible base layer which has the ID 0?)
		*
		*  @note
		*    - All registered entities of the destroyed layer are now inside no layer
		*/
		bool destroyLayerById(uint32 id);

		/**
		*  @brief
		*    Destroy all layers within this layer
		*
		*  @note
		*    - All registered entities of the destroyed layers are now inside no layer
		*/
		void destroyAllLayers();

		/**
		*  @brief
		*    Return the layer list
		*
		*  @return
		*    Reference to the internal layer list, do not manipulate the list or destroy the layers
		*/
		inline const std::vector<Layer*>& getLayers() const;

		/**
		*  @brief
		*    Return a list of layers and its child layers
		*
		*  @param[out] layers
		*    Reference to the layer list which will receive the layers and its child layers, do not destroy the layer instances
		*
		*  @note
		*    - Ease-of-use method
		*/
		void getLayersRecursive(std::vector<Layer*>& layers) const;

		//[-------------------------------------------------------]
		//[ Entities                                              ]
		//[-------------------------------------------------------]
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
		*    Register an entity inside this layer by using the ID of the entity
		*
		*  @param[in] id
		*    ID of the entity to register inside this layer
		*
		*  @return
		*    "true" if all went fine, else "false" (invalid entity ID?)
		*
		*  @remarks
		*    There's no public "qsf::Layer::unregisterEntityById()" method by intent, entities with metadata components must always be inside a layer.
		*    Register an entity inside the base layer which has the ID 0 in order to unregister it from another layer.
		*    It's no error when the given entity is already registered inside this layer, hence the method will return "true".
		*    The entity is automatically unregistered from the previous layer before it's registered inside the new layer.
		*
		*  @note
		*    - In case the given entity ID is valid, this method automatically creates a metadata component in case there's no instance, yet
		*/
		bool registerEntityById(uint64 id);

		/**
		*  @brief
		*    Register entities inside this layer by using the IDs of the entities
		*
		*  @param[in] ids
		*    IDs of the entities to register inside this layer
		*
		*  @return
		*    "true" if all went fine, else "false" (at least a single invalid entity ID?)
		*
		*  @see
		*    - "qsf::Layer::registerEntityById()"
		*
		*  @note
		*    - Ease-of-use method
		*/
		bool registerEntitiesById(const EntityIdSet& ids);

		/**
		*  @brief
		*    Register an entity inside this layer by using the metadata component of the entity
		*
		*  @param[in] metadataComponent
		*    Metadata component of the entity to register inside this layer
		*
		*  @note
		*    - Use "qsf::Layer::registerEntityById()" instead of "qsf::Layer::registerEntityByMetadataComponent()" whenever possible
		*
		*  @see
		*    - "qsf::Layer::registerEntityById()" for details
		*/
		void registerEntityByMetadataComponent(MetadataComponent& metadataComponent);

		/**
		*  @brief
		*    Return the list of IDs of the entities registered inside this layer
		*
		*  @return
		*    Reference to the internal list of IDs of the entities registered inside this layer, do not manipulate the list
		*/
		inline const EntityIdSet& getEntityIds() const;

		/**
		*  @brief
		*    For internal purposes only: Set entity ID set directly
		*
		*  @param[in] ids
		*    IDs of the entities to register inside this layer
		*
		*  @note
		*    - Lookout! This method is for internal purposes only, do not use it if you don't know what this is about.
		*    - Can be used e.g. for internal temporary layers (handy e.g. inside the editor)
		*/
		inline void unsafeSetEntityIds(const EntityIdSet& ids);

		/**
		*  @brief
		*    Return a list of IDs of the entities registered inside this layer and its child layers
		*
		*  @param[out] entityIds
		*    Reference to the list which will receive the IDs of the entities registered inside this layer and its child layers,
		*    list is not cleared before adding new IDs
		*
		*  @note
		*    - Ease-of-use method
		*/
		void getEntityIdsRecursive(EntityIdSet& entityIds) const;

		//[-------------------------------------------------------]
		//[ Active                                                ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Get or create the internal buffer map for unloaded entities
		*
		*  @return
		*    The buffer map instance
		*/
		Map& getInternalBufferMap();
		const Map& getInternalBufferMap() const;

		/**
		*  @brief
		*    Return whether this layer is activated
		*
		*  @return
		*    "true" if this layer was activated, "false" otherwise
		*
		*  @remarks
		*    By default, a layer is always active
		*/
		inline bool isActive() const;

		/**
		*  @brief
		*    Activate the layer
		*
		*  @remarks
		*    Activating a layer will instantiate the layer's entities into the map
		*/
		void activateLayer();

		/**
		*  @brief
		*    Deactivate the layer
		*
		*  @param[in] copyIntoBufferMap
		*    If "true", the layer's current entities are copied into the internal map buffer as a backup, overwriting the existing buffer map's contents
		*/
		void deactivateLayer(bool copyIntoBufferMap = false);


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] layerManager
		*    Layer manager, must stay valid as long as this layer instance exists
		*  @param[in] parentLayer
		*    The parent layer the layer is in, null pointer if there's no parent
		*  @param[in] id
		*    Unique layer identifier
		*/
		Layer(LayerManager& layerManager, Layer* parentLayer, uint32 id);

		/**
		*  @brief
		*    Unregister an entity from this layer by using the ID of the entity
		*
		*  @param[in] id
		*    ID of the entity to register from this layer
		*
		*  @note
		*    - Not public by intent, see "qsf::Layer::registerEntityById()" comment
		*/
		void unregisterEntityById(uint64 id);

		/**
		*  @brief
		*    Unregister this layer from the layer manager
		*/
		void unregisterFromLayerManager();

		/**
		*  @brief
		*    Set whether or not the layer is hidden, recursive part
		*
		*  @param[in] hidden
		*   "true" if the layer is hidden, else "false"
		*
		*  @see
		*    - "qsf::Layer::isHidden()"
		*/
		void setHiddenRecursive(bool hidden);


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		enum class Flags
		{
			ACTIVE,			///< "true" if the layer is active, meaning that its entities are instantiated inside the layer's owning map
			HIDDEN,			///< "true" if the layer is hidden, else "false"
			LOAD_IN_GAME	///< "true" if the layer itself is marked as it should be loaded at game startup, else "false"
		};
		typedef EnumBitmask<uint8, Flags> FlagsSet;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Connected to CAMP
		NamedIdentifier		mName;			///< The not unique layer name, and its hashed value
		FlagsSet			mFlags;			///< Internal flags (only partly connected to CAMP), see enum "Flags"

		// Internal
		LayerManager*		mLayerManager;	///< Owner layer manager, always valid, do not destroy the instance
		Layer*				mParentLayer;	///< The parent layer the layer is in, null pointer if there's no parent, do not destroy the instance
		uint32				mId;			///< Unique layer identifier
		std::vector<Layer*> mLayers;		///< List of layer instances, destroy the instances if you no longer need them
		EntityIdSet			mEntityIds;		///< List of IDs of the entities registered inside this layer
		Map*				mBufferMap;		///< Map instance as buffer for entities currently not instanced in the layer's owning map, while the layer is unloaded; may be a null pointer, we're responsible for destroying it


	//[-------------------------------------------------------]
	//[ CAMP reflection system                                ]
	//[-------------------------------------------------------]
	QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/map/layer/Layer-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::Layer)

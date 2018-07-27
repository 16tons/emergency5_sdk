// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/base/manager/Manager.h"

#include <boost/container/flat_map.hpp>

#include <vector>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Map;
	class Layer;
	class StringHash;
	class BinarySerializer;
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
	*    Layer manager class
	*
	*  @remarks
	*    The layer manager only consists of the root layer as well as ease-of-use access to layers of interest.
	*    Layers can be hierarchical without technical restrictions. Meaning a layer can contain not only entities
	*    but only contain more layers. An entity can only be inside a single layer at one and the same time. On
	*    the other hand, an entity must always be inside a layer. By default entities are inside the base layer
	*    which has the layer ID 0.
	*
	*    The follow example shows the intended standard organization of the layer manager:
	*    @verbatim
	*    - Root Layer
	*        - Base Layer (has always the ID 0, is always there)
	*            - House Folder
	*                - Entity 0
	*                - Entity n
	*            - Person Folder
	*                - Entity 0
	*                - Entity n
	*            - Entity 0
	*            - Entity n
	*        - Event X Layer
	*            - House Folder
	*                - Entity 0
	*                - Entity n
	*            - Person Folder
	*                - Entity 0
	*                - Entity n
	*            - Entity 0
	*            - Entity n
	*    @endverbatim
	*
	*   Our conventions inside the QSF editor:
	*   - The root layer is invisible to the user, the user only sees a linear list of layers
	*   - Layers which are inside another layer, which is not the root layer, are called "Folder"
	*   - A folder inside a folder is allowed
	*   - An entity can be inside a folder or be directly inside a layer
	*   - Folders (the terminology) or entities inside the root layer are not allowed
	*   - The base layer can not be destroyed explicitly
	*
	*  @note
	*    - The layer manager can be seen as a special view onto the map,
	*      meaning it's optional and not every system needs to be aware of layers
	*/
	class QSF_API_EXPORT LayerManager : public Manager
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class Layer;


	//[-------------------------------------------------------]
	//[ Public declarations                                   ]
	//[-------------------------------------------------------]
	public:
		typedef boost::container::flat_map<uint32, Layer*> LayerMap;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] map
		*    Owner map, must stay valid as long as this layer manager instance exists
		*/
		explicit LayerManager(Map& map);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~LayerManager();

		/**
		*  @brief
		*    Return the map this layer manager is in
		*
		*  @return
		*    The map this layer manager is in, do not destroy the returned instance
		*/
		inline Map& getMap() const;

		/**
		*  @brief
		*    Return the root layer instance
		*
		*  @return
		*    The root layer instance, do not destroy the returned instance
		*
		*  @note
		*    - Although it's technically possible, please do not add entities to the root layer
		*/
		inline Layer& getRootLayer() const;

		/**
		*  @brief
		*    Return the base layer
		*
		*  @return
		*    The requested base layer which has the ID 0, do not destroy the instance
		*
		*  @note
		*    - Ease-of-use method
		*    - Has the same effect as using "qsf::LayerManager::getLayerById(0)",
		*      but due to the importance of the base layer use this method when possible
		*    - The base layer is inside the root layer
		*/
		inline Layer& getBaseLayer() const;

		/**
		*  @brief
		*    Return a layer by using its ID
		*
		*  @param[in] id
		*    ID of the layer to return, 0 is the base layer
		*
		*  @return
		*    The requested layer, null pointer if there's no such layer, do not destroy the instance
		*
		*  @note
		*    - Ease-of-use method
		*/
		inline Layer* getLayerById(uint32 id) const;

		/**
		*  @brief
		*    Return a layer by using its name
		*
		*  @param[in] name
		*    Name of the layer to return
		*
		*  @return
		*    The requested layer, null pointer if there's no such layer, do not destroy the instance
		*
		*  @note
		*    - A layer's name is not unique, so this method will only find the first layer of that name
		*    - This method is relatively slow, only use it when you have to
		*/
		Layer* getLayerByName(const StringHash& name) const;

		/**
		*  @brief
		*    Generate a new unique layer ID
		*
		*  @return
		*    New layer ID which is unique inside this layer manager
		*
		*  @note
		*    - Do only need this method in case you really need a new layer ID
		*/
		uint32 generateId();

		/**
		*  @brief
		*    Destroy all layers, except base and root layer
		*
		*  @note
		*    - Base layer sub-layers are also destroyed
		*    - All registered entities of the destroyed layers are now inside no layer
		*/
		void destroyAllLayers();

		/**
		*  @brief
		*    Get map of layers by their IDs
		*
		*  @return
		*    Reference to the internal map of layers, with their ID as key
		*/
		inline const LayerMap& getLayerMap() const;

		//[-------------------------------------------------------]
		//[ Serialization                                         ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Serialize or deserialize the layer manager using a binary serializer
		*
		*  @param[in] serializer
		*    The serializer, which can be either in read or write mode
		*/
		void serialize(BinarySerializer& serializer);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		Map*	 mMap;			///< Owner map, always valid, do not destroy the instance
		uint32	 mNextId;		///< Layer ID which will be returned when "qsf::LayerManager::generateId()" is called the next time
		Layer*	 mRootLayer;	///< Root layer, always valid, destroy the instance if you no longer need it
		Layer*	 mBaseLayer;	///< The base layer (ID = 0) which is inside the root layer, always valid, destroy the instance if you no longer need it
		LayerMap mLayerMap;		///< Map of layer instances, unique layer ID as key and layer instance as value, do not destroy the instances because they are managed by the layer they are in


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/map/layer/LayerManager-inl.h"

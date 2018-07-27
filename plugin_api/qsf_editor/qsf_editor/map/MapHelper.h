// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/asset/AssetSystemTypes.h>

#include "qsf_editor/selection/entity/EntitySelectionManager.h"

#include <boost/noncopyable.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
QT_BEGIN_NAMESPACE
	class QWidget;
QT_END_NAMESPACE
namespace qsf
{
	class Map;
	class Layer;
	class LayerManager;
	namespace editor
	{
		namespace base
		{
			class CompoundOperation;
		}
	}
}
namespace Ogre
{
	class Image;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace editor
	{


		//[-------------------------------------------------------]
		//[ Classes                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Map helper class
		*
		*  @remarks
		*    Usage example:
		*    @code
		*    qsf::editor::MapHelper(QSF_MAINMAP).createEmptyMap(1500, 1500, 2048);
		*    @endcode
		*/
		class QSF_EDITOR_API_EXPORT MapHelper : public boost::noncopyable
		{


		//[-------------------------------------------------------]
		//[ Public static methods                                 ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Return whether or not the main map asset is writable
			*
			*  @return
			*    "true" if the main map asset is writable, else "false"
			*/
			static bool isMainMapAssetWritable();

			/**
			*  @brief
			*    Return a height map size by using a given terrain world size
			*
			*  @param[in] terrainWorldSize
			*    Terrain world size to use
			*
			*  @return
			*    The calculated height map size
			*/
			static uint32 getHeightMapSizeByTerrainWorldSize(uint32 terrainWorldSize);

			/**
			*  @brief
			*    Create a new map by using a GUI to guide the user
			*
			*  @param[in] qWidgetParent
			*    Pointer to parent Qt widget, can be a null pointer (in this case you're responsible for destroying this dialog instance)
			*
			*  @return
			*    Global asset ID of the new map asset, uninitialized global asset ID in case of an error
			*
			*  @note
			*    - This method only creates the map asset, it's the responsibility of the caller to load the map on the client side or ask the server to startup a map
			*/
			static GlobalAssetId createNewMapAssetByGui(QWidget* qWidgetParent);


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Constructor
			*
			*  @param[in] map
			*    Map to use, must stay valid as long as this map helper instance exists
			*/
			explicit inline MapHelper(Map& map);

			/**
			*  @brief
			*    Destructor
			*
			*  @note
			*    - Not virtual by intent
			*/
			inline ~MapHelper();

			/**
			*  @brief
			*    Create an empty map with all necessary system components, this is the minimum number of components needed for the QSF editor
			*
			*  @param[in] width
			*    Width of the empty map in world units
			*  @param[in] height
			*    Height of the empty map in world units
			*  @param[in] heightMapSize
			*    Height map size, if uninitialized a value is derived automatically
			*  @param[in] mapName
			*    Optional map name
			*
			*  @note
			*    - Does not automatically clear the map before adding the new entities
			*    - New maps are always centered around the coordinate system origin
			*/
			void createEmptyMap(uint32 width, uint32 height, uint32 heightMapSize = getUninitialized<uint32>(), const std::string& mapName = "") const;

			/**
			*  @brief
			*    Create a mini-map snapshot from the given map
			*
			*  @param[in] image
			*    The Ogre image instance which will contain the minimap snapshot
			*  @param[in] data
			*    The image data of the minimap snapshot
			*    Note: The user has to make sure that the memory block which data points to gets freed when not needed anymore
			*  @param[in] maintainAspectRation
			*    Should the minimap image have the same aspect ratio as the map boundaries
			*/
			bool createMiniMapSnapshot(Ogre::Image& image, uint8*& data, bool maintainAspectRation) const;

			/**
			*  @brief
			*    Create a mini-map snapshot from the given map
			*
			*  @param[in] image
			*    The OGRE image instance which will contain the minimap snapshot
			*  @param[in] minimapWidth
			*    The width of the minimap image
			*  @param[in] minimapHeight
			*    The height of the minimap image
			*/
			bool createMiniMapSnapshot(Ogre::Image& image, uint8*& data, uint32 minimapWidth, uint32 minimapHeight) const;

			/**
			*  @brief
			*    Import layer
			*
			*  @param[in] absoluteFilename
			*    Absolute UTF-8 filename where to load the imported layer from
			*/
			void importLayer(const std::string& absoluteFilename) const;

			/**
			*  @brief
			*    Export layer
			*
			*  @param[in] layer
			*    Layer to export
			*  @param[in] absoluteFilename
			*    Absolute UTF-8 filename where to save the exported layer
			*/
			void exportLayer(const Layer& layer, const std::string& absoluteFilename) const;

			/**
			*  @brief
			*    Returns the size of the map covered by the map boundaries
			*
			*  @return
			*    Size of the map covered by the map boundaries
			*/
			glm::vec3 getMapBoundarySize() const;


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			/**
			*  @brief
			*    Import the given layer recursive
			*
			*  @param[in] fromLayer
			*    Layer to import
			*  @param[in] toLayerManager
			*    Layer manager to import into
			*  @param[in] toLayerId
			*    ID of the layer to import into
			*  @param[out] compoundOperation
			*    Compound operation
			*  @param[out] entityIdSet
			*    Receives the list of created entity IDs, list is not cleared before adding new entries
			*
			*  @note
			*    - Will also directly import entities
			*/
			void importLayerRecursive(const Layer& fromLayer, LayerManager& toLayerManager, uint32 toLayerId, base::CompoundOperation& compoundOperation, EntitySelectionManager::IdSet& entityIdSet) const;

			/**
			*  @brief
			*    Export the given layer recursive
			*
			*  @param[in] fromLayer
			*    Layer to export
			*  @param[out] toLayer
			*    Layer to export into
			*
			*  @note
			*    - Will also directly export entities
			*/
			void exportLayerRecursive(const Layer& fromLayer, Layer& toLayer) const;

			/**
			*  @brief
			*    Change the map ID of a given compound operation recursive
			*
			*  @param[in] compoundOperation
			*    Compound operation to change
			*  @param[in] mapId
			*    Map ID to change to
			*/
			void changeCompoundOperationMapId(base::CompoundOperation& compoundOperation, uint32 mapId) const;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			Map* mMap;	///< Map to use, always valid, do not destroy the instance


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf_editor/map/MapHelper-inl.h"

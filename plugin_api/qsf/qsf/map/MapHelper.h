// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/Export.h"

#include <boost/noncopyable.hpp>

#include <string>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Map;
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
	*    Map helper class
	*/
	class QSF_API_EXPORT MapHelper : public boost::noncopyable
	{


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
		*    Return the map instance the map helper operates on
		*
		*  @return
		*    The map instance the map helper operates on, do not destroy the instance
		*/
		inline Map& getMap() const;

		/**
		*  @brief
		*    Search through all entities in the map and create an unique name, given by the input string
		*
		*  @param[in] name
		*    UTF-8 name for an existing entity
		*
		*  @return
		*    An unique UTF-8 name, based on the given name. Suffix style is "_00x", starts with "_002".
		*/
		std::string findUniqueNameForEntity(const std::string& name) const;

		/**
		*  @brief
		*    Destroy all prototype component instances which should not be serialized (gizmo etc.)
		*
		*  @param[in] prototype
		*    Prototype instance to destroy components from
		*/
		void destroyNotSerializedPrototypeComponents(Prototype& prototype);

		/**
		*  @brief
		*    Activate the given map layer
		*
		*  @param[in] layerName
		*    Name of the layer
		*  @param[in] showError
		*    If "true" and the layer is not found,this results in a QSF_ERROR (but not an exception)
		*/
		bool activateLayerByName(const std::string& layerName, bool showError = true);

		/**
		*  @brief
		*    Deactivate the given map layer
		*
		*  @param[in] layerName
		*    Name of the layer
		*  @param[in] saveEntities
		*    If "true", the layer's current entities are copied into the internal map buffer as a backup, overwriting the existing buffer map's contents
		*  @param[in] showError
		*    If "true" and the layer is not found,this results in a QSF_ERROR (but not an exception)
		*/
		bool deactivateLayerByName(const std::string& layerName, bool saveEntities = false, bool showError = true);


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		Map& mMap;	///< Map to use, always valid, do not destroy the instance


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/map/MapHelper-inl.h"

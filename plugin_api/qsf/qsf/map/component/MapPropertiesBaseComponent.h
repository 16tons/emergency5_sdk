// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/component/Component.h"
#include "qsf/reflection/type/CampQsfAssetProxy.h"


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
	*    Abstract map properties base component interface
	*
	*  @note
	*    - Core component, see "qsf::Map::getCoreEntity()" documentation for details
	*/
	class QSF_API_EXPORT MapPropertiesBaseComponent : public Component
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 COMPONENT_ID;	///< "qsf::MapPropertiesBaseComponent" unique component ID
		static const uint32 MAP_NAME;		///< "MapName" unique component ID
		static const uint32 MINI_MAP;		///< "MiniMap" unique component ID


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~MapPropertiesBaseComponent();

		/**
		*  @brief
		*    Return the map name
		*
		*  @return
		*    The current UTF-8 map name
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline const std::string& getMapName() const;

		/**
		*  @brief
		*    Set the map name
		*
		*  @param[in] mapName
		*    The new UTF-8 map name
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline void setMapName(const std::string& mapName);

		/**
		*  @brief
		*    Return the mini-map asset
		*
		*  @return
		*    The current mini-map asset
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline const AssetProxy& getMiniMap() const;

		/**
		*  @brief
		*    Set the mini-map asset
		*
		*  @param[in] miniMap
		*    The new mini-map asset
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline void setMiniMap(const AssetProxy& miniMap);


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] prototype
		*    The prototype this component is in, no null pointer allowed
		*/
		inline explicit MapPropertiesBaseComponent(Prototype* prototype);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Connected to the camp reflection system
		std::string	mMapName;		///< UTF-8 map name, by default the same as the saved map file or "unnamed"
		AssetProxy	mMiniMap;		///< Mini-map texture asset


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
#include "qsf/map/component/MapPropertiesBaseComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::MapPropertiesBaseComponent)

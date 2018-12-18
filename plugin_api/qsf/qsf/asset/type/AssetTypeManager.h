// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/base/manager/Manager.h"
#include "qsf/base/manager/ElementManager.h"
#include "qsf/asset/type/AssetType.h"


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
	*    Asset type manager class
	*/
	class QSF_API_EXPORT AssetTypeManager : private ElementManager<AssetTypeId, AssetType>
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		typedef boost::container::flat_map<AssetTypeId, AssetType*> AssetTypeMap;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		inline AssetTypeManager();

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~AssetTypeManager();

		/**
		*  @brief
		*    Register asset type
		*
		*  @param[in] typeName
		*    UTF-8 type name, completely lower case
		*  @param[in] isSyncBeforeMapLoad
		*    Whether or not the assets of this type need to be synchronized before map load (default: "true")
		*
		*  @return
		*    Registered asset type, null pointer on error (maybe the asset type name is already used?), do not destroy the instance
		*/
		inline AssetType* registerAssetType(const std::string& typeName, bool isSyncBeforeMapLoad = true);

		/**
		*  @brief
		*    Unregister asset type
		*
		*  @param[in] typeName
		*    UTF-8 type name, completely lower case
		*
		*  @return
		*    "true" if all went fine, else "false"
		*/
		inline bool unregisterAssetType(const std::string& typeName);

		/**
		*  @brief
		*    Return the asset type map
		*
		*  @return
		*    The internal asset type map, do not destroy or manipulate the instance
		*/
		inline const AssetTypeMap& getAssetTypeMap() const;

		/**
		*  @brief
		*    Add a dependency to the DependencyTypeIdList
		*
		*  @param[in] typeId Type ID to add to the list
		*/
		inline void addAssetTypeDependency(AssetTypeId typeId, AssetTypeId dependencyTypeId);

		/**
		*  @brief
		*    Remove a dependency from the DependencyTypeIdList
		*
		*  @param[in] typeId Type ID to remove from the list
		*/
		inline void removeAssetTypeDependency(AssetTypeId typeId, AssetTypeId dependencyTypeId);


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/asset/type/AssetTypeManager-inl.h"

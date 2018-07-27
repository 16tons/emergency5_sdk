// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/base/StringHash.h"
#include "qsf/base/manager/ElementManager.h"

#include "qsf/asset/AssetSystemTypes.h"

#include <boost/noncopyable.hpp>
#include <boost/container/flat_set.hpp>

#include <string>


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
	*    Asset type class
	*/
	class QSF_API_EXPORT AssetType : public boost::noncopyable
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class AssetTypeManager;						// Only the asset type element manager is allowed to create and destroy asset type instances
		friend class detail::DirectAllocator<AssetType>;	// Only the asset type element manager is allowed to create and destroy asset type instances


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Return the type name
		*
		*  @return
		*    The UTF-8 type name (example: "mesh"), do no destroy the returned instance, completely lower case
		*/
		inline const std::string& getTypeName() const;

		/**
		*  @brief
		*    Return the type ID
		*
		*  @return
		*    The asset type ID (e.g. qsf::StringHash("mesh"))
		*
		*  @note
		*    - The asset type ID is generated by using "qsf::StringHash" on the UTF-8 asset type name
		*/
		inline AssetTypeId getTypeId() const;

		/**
		*  @brief
		*    Returns a list of types which assets of this type may depend on directly
		*
		*  @return
		*    The asset type ID list
		*
		*  @note
		*    - Asset type IDs are generated by using "qsf::StringHash" on the UTF-8 asset type name
		*/
		inline const boost::container::flat_set<AssetTypeId>& getDependencyTypeIdList() const;

		/**
		*  @brief
		*    Returns whether or not the assets of this type need to be synchronized before map load
		*
		*  @return
		*    "true if assets of this type need to be synchronized before map load
		*/
		inline bool isSyncBeforeMapLoad() const;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] typeId
		*    Type ID (e.g. qsf::StringHash("mesh"))
		*/
		inline explicit AssetType(const StringHash& typeId);

		/**
		*  @brief
		*    Destructor
		*
		*  @note
		*    - Not virtual by default
		*/
		inline ~AssetType();

		/**
		*  @brief
		*    Add a dependency to the DependencyTypeIdList
		*
		*  @param[in] typeId Type ID to add to the list
		*/
		inline void addDependencyTypeId(AssetTypeId typeId);

		/**
		*  @brief
		*    Remove a dependency from the DependencyTypeIdList
		*
		*  @param[in] typeId Type ID to remove from the list
		*/
		inline void removeDependencyTypeId(AssetTypeId typeId);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		std::string								mTypeName;				///< UTF-8 type name, completely lower case
		AssetTypeId								mTypeId;				///< Type ID (e.g. qsf::StringHash("mesh")), "qsf::getUninitialized<uint32>()" if invalid
		boost::container::flat_set<AssetTypeId> mDependencyTypeIdList;	///< Types assets of this type may depend on directly
		bool									mIsSyncBeforeMapLoad;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/asset/type/AssetType-inl.h"
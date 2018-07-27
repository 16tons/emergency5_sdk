// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/game/units/OrderInfo.h"

#include <qsf/base/manager/ElementManager.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	class EM5_API_EXPORT OrderInfoManager : public qsf::ElementManager<uint32, OrderInfo>
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		OrderInfoManager();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~OrderInfoManager();

		/**
		*  @brief
		*    Load the assets
		*/
		void loadAssets();

		/**
		*  @brief
		*    Load order information JSON file
		*
		*  @param[in] filename
		*    Path of the file to load
		*/
		bool loadByFilename(const std::string& filename);

		/**
		*  @brief
		*    Save order information JSON file
		*
		*  @param[in] filename
		*    Path of the file to save
		*/
		bool saveByFilename(const std::string& filename);

		/**
		*  @brief
		*    Find order information by ID
		*
		*  @param[in] id
		*    Unit type ID to searching for (for example qsf::StringHash("ambulance"))
		*
		*  @return
		*    Returns the found order information, can be a null pointer, do not destroy the returned instance
		*/
		const OrderInfo* getOrderInfoById(const qsf::StringHash& id) const;

		/**
		*  @brief
		*    Access order information by ID
		*
		*  @param[in] id
		*    Unit type ID to searching for (for example qsf::StringHash("ambulance"))
		*
		*  @return
		*    Returns the found order information, can be a null pointer, do not destroy the returned instance
		*/
		OrderInfo* getOrderInfoByIdForWriting(const qsf::StringHash& id);

		/**
		*  @brief
		*    Find order information by prefab
		*
		*  @param[in] assetProxy
		*    Prefab asset to searching for
		*
		*  @return
		*    Returns the found order information, can be a null pointer, do not destroy the returned instance
		*/
		const OrderInfo* getOrderInfoByPrefab(const qsf::AssetProxy& assetProxy) const;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

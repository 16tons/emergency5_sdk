// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/asset/GlobalAssetIdProvider.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace editor
	{
		class AssetStorageManager;
	}
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
		*    Remove global asset ID provider base class
		*/
		class RemoteGlobalAssetIdProvider : public GlobalAssetIdProvider
		{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Constructor
			*
			*  @param[in] assetStorageManager
			*    Asset storage manager which will provide the global asset IDs, instance must stay valid as long as this global asset ID provider instance exits
			*/
			explicit RemoteGlobalAssetIdProvider(AssetStorageManager& assetStorageManager);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~RemoteGlobalAssetIdProvider();


		//[-------------------------------------------------------]
		//[ Public virtual qsf::GlobalAssetIdProvider methods     ]
		//[-------------------------------------------------------]
		public:
			virtual GlobalAssetId acquireGlobalAssetId() override;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			AssetStorageManager* mAssetStorageManager;	///< Asset storage manager which will provide the global asset IDs, always valid, do not destroy the instance


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf

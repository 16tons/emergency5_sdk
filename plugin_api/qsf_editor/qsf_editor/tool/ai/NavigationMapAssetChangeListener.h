// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf_ai/worldModel/WorldModelManager.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace editor
	{
		class AssetEditHelper;
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
		*    Implementation of a listener for changes to navigation AI assets
		*
		*  @remarks
		*    Basically the AI triggers some changes to AI navigation map assets itself and this simple listener provides callbacks to react to the changes.
		*/
		class NavigationMapAssetChangeListener : public ai::WorldModelManager::AssetChangeListener
		{


		//[------------------------------------------------------------------------]
		//[ Public virtual qsf::ai::WorldModelManager::AssetChangeListener methods ]
		//[------------------------------------------------------------------------]
		public:
			virtual void onAssetUpdated(GlobalAssetId assetId) override;
			virtual void onAssetDeleted(GlobalAssetId assetId) override;

			void reimportAndUploadChangedAssets(AssetEditHelper& assetEditHelper);
			void reimportAndUploadChangedAsset(AssetEditHelper& assetEditHelper, GlobalAssetId globalAssetId);


		//[------------------------------------------------------------------------]
		//[ Private data                                                           ]
		//[------------------------------------------------------------------------]
		private:
			boost::container::flat_set<GlobalAssetId> mChangedAssets;


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf

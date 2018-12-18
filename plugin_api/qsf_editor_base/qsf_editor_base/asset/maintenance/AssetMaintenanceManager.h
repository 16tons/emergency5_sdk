// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor_base/Export.h"

#include <qsf/plugin/pluginable/PluginableManager.h>
#include <qsf/asset/AssetSystemTypes.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class AssetPackage;
	class CachedAsset;
	namespace editor
	{
		namespace base
		{
			class AssetIssue;
		}
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace editor
	{
		namespace base
		{


			//[-------------------------------------------------------]
			//[ Classes                                               ]
			//[-------------------------------------------------------]
			/**
			*  @brief
			*    Asset maintenance manager
			*/
			class QSF_EDITOR_BASE_API_EXPORT AssetMaintenanceManager : public PluginableManager
			{


			//[-------------------------------------------------------]
			//[ Public methods                                        ]
			//[-------------------------------------------------------]
			public:
				/**
				*  @brief
				*    Default constructor
				*/
				AssetMaintenanceManager();

				/**
				*  @brief
				*    Destructor
				*/
				virtual ~AssetMaintenanceManager();

				void onStartup();
				void onShutdown();

				void clearIssueList();

				const std::vector<std::shared_ptr<base::AssetIssue>>& getIssueList() const;

				void runAllChecks();

				void checkAssetPackage(AssetPackage& assetPackage, std::vector<std::shared_ptr<base::AssetIssue>>& assetIssues, bool runAssetChecks = false) const;
				void checkAsset(const Asset& asset, std::vector<std::shared_ptr<base::AssetIssue>>& assetIssues) const;
				void checkCachedAsset(const Asset& asset, const CachedAsset& cachedAsset, std::vector<std::shared_ptr<base::AssetIssue>>& assetIssues) const;
				void checkSourceAsset(const std::string& sourceAsset, std::vector<std::shared_ptr<base::AssetIssue>>& assetIssues) const;


			//[-------------------------------------------------------]
			//[ Protected virtual qsf::PluginableManager methods      ]
			//[-------------------------------------------------------]
			protected:
				virtual void classAdded(const camp::Class& added) override;
				virtual void createPluginableInstance(const camp::Class& added, camp::UserObject& campUserObject) override;


			//[-------------------------------------------------------]
			//[ Private data                                          ]
			//[-------------------------------------------------------]
			private:
				bool										   mIsRunning;
				std::vector<std::shared_ptr<base::AssetIssue>> mAssetIssues;


			//[-------------------------------------------------------]
			//[ CAMP reflection system                                ]
			//[-------------------------------------------------------]
			QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
		} // base
	} // editor
} // qsf


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::base::AssetMaintenanceManager)

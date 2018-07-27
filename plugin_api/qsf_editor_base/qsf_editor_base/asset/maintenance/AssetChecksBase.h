// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor_base/asset/maintenance/AssetMaintenanceManager.h"

#include <qsf/plugin/pluginable/Pluginable.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class AssetPackage;
	class AssetSystem;
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
			*    Asset issue check base class
			*/
			class QSF_EDITOR_BASE_API_EXPORT AssetChecksBase : public Pluginable<AssetMaintenanceManager>
			{


			//[-------------------------------------------------------]
			//[ Public methods                                        ]
			//[-------------------------------------------------------]
			public:
				/**
				*  @brief
				*    Constructor
				*/
				explicit AssetChecksBase(AssetMaintenanceManager* manager);

				/**
				*  @brief
				*    Destructor
				*/
				virtual ~AssetChecksBase();


			//[-------------------------------------------------------]
			//[ Public virtual qsf::editor::base::AssetChecks methods ]
			//[-------------------------------------------------------]
			public:
				virtual void checkAssetPackage(AssetPackage& assetPackage, std::vector<std::shared_ptr<base::AssetIssue>>& issueList) const;
				virtual void checkAsset(const Asset& asset, std::vector<std::shared_ptr<base::AssetIssue>>& issueList) const;
				virtual void checkCachedAsset(const Asset& asset, const CachedAsset& cachedAsset, std::vector<std::shared_ptr<base::AssetIssue>>& issueList) const;
				virtual void checkSourceAsset(const std::string& sourceAsset, std::vector<std::shared_ptr<base::AssetIssue>>& issueList) const;


			//[-------------------------------------------------------]
			//[ Protected data                                        ]
			//[-------------------------------------------------------]
			public:
				AssetSystem& mAssetSystem;


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
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::base::AssetChecksBase)

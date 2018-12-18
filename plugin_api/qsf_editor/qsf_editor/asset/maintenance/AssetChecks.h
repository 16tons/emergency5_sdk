// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf_editor_base/asset/maintenance/AssetChecksBase.h>


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
		*    Concrete QSF asset issue check base class
		*/
		class AssetChecks : public base::AssetChecksBase
		{


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			static const std::string ASSET_NAMING_CONVENTION_REGEX;	///< Regular expression for asset naming convention checks


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Constructor
			*/
			explicit AssetChecks(base::AssetMaintenanceManager* manager);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~AssetChecks();


		//[-------------------------------------------------------]
		//[ Public virtual qsf::editor::base::AssetChecks methods ]
		//[-------------------------------------------------------]
		public:
			virtual void checkAssetPackage(AssetPackage& assetPackage, std::vector<std::shared_ptr<base::AssetIssue>>& issueList) const override;
			virtual void checkAsset(const Asset& asset, std::vector<std::shared_ptr<base::AssetIssue>>& issueList) const override;
			virtual void checkCachedAsset(const Asset& asset, const CachedAsset& cachedAsset, std::vector<std::shared_ptr<base::AssetIssue>>& issueList) const override;


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		public:
			bool findCircularDependencyLeaf(const Asset& asset, boost::container::flat_set<GlobalAssetId>& currentDependencyLeaf) const;


		//[-------------------------------------------------------]
		//[ CAMP reflection system                                ]
		//[-------------------------------------------------------]
		QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::AssetChecks)

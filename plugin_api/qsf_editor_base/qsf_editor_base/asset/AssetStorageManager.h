// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor_base/Export.h"

#include <qsf/asset/AssetSystemTypes.h>
#include <qsf/base/manager/Manager.h>
#include <qsf/base/GetUninitialized.h>

#include <unordered_map>
#include <unordered_set>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class BaseAsset;
	class BaseCachedAsset;
	namespace editor
	{
		namespace base
		{
			class TextureSemanticManager;
			class AssetMaintenanceManager;
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
			*    Abstract asset storage manager class
			*/
			class QSF_EDITOR_BASE_API_EXPORT AssetStorageManager : public Manager
			{


			//[-------------------------------------------------------]
			//[ Public definitions                                    ]
			//[-------------------------------------------------------]
			public:
				struct DerivedAssetInfo
				{
					// Input
					std::string derivedAssetIdentifier;		///< Unique derived asset identifier (build this one by combining all input parameters that influence the derived asset into a NamedIdentifier)
					std::string assetTypename;
					std::string fileExtension;

					// Input & Output
					BaseCachedAsset* cachedAsset;

					// Output
					std::string   absoluteCachedAssetPath;
					GlobalAssetId globalAssetId;

					// Default constructor
					inline DerivedAssetInfo() :
						cachedAsset(nullptr),
						globalAssetId(getUninitialized<GlobalAssetId>())
					{
						// Nothing to do in here
					}
				};

				static const std::string DERIVED_ASSET_CATEGORY;


			//[-------------------------------------------------------]
			//[ Public methods                                        ]
			//[-------------------------------------------------------]
			public:
				/**
				*  @brief
				*    Destructor
				*/
				virtual ~AssetStorageManager();

				/**
				*  @brief
				*    Return the texture semantic manager instance
				*
				*  @return
				*    The texture semantic manager instance, do no destroy the returned instance
				*/
				inline TextureSemanticManager& getTextureSemanticManager();
				inline const TextureSemanticManager& getTextureSemanticManager() const;

				/**
				*  @brief
				*    Return the asset maintenance manager instance
				*
				*  @return
				*    The asset maintenance instance, do no destroy the returned instance
				*/
				inline AssetMaintenanceManager& getAssetMaintenanceManager();
				inline const AssetMaintenanceManager& getAssetMaintenanceManager() const;

				void addDependency(GlobalAssetId globalAssetId, GlobalAssetId globalDependencyAssetId);
				void removeDependency(GlobalAssetId globalAssetId, GlobalAssetId globalDependencyAssetId);
				void removeDependency(GlobalAssetId globalDependencyAssetId);
				void removeDependenant(GlobalAssetId globalAssetId);
				void clearDependenants();

				std::vector<GlobalAssetId> getAssetsDependingOn(GlobalAssetId globalAssetId) const;


			//[-------------------------------------------------------]
			//[ Public virtual qsf::editor::base::AssetStorageManager methods ]
			//[-------------------------------------------------------]
			public:
				// TODO(ca) comment these once signature and implementation are final
				virtual std::string getAssetSourcePath(GlobalAssetId globalAssetId) = 0;
				virtual AssetRevision getAssetSourceRevision(GlobalAssetId globalAssetId) const = 0;
				virtual bool getAssetsDerivedFrom(const BaseCachedAsset& derivedFrom, std::unordered_map<uint32, DerivedAssetInfo>& derivedAssetList) = 0;
				virtual bool deriveAssetsFrom(BaseCachedAsset& deriveFrom, std::unordered_map<uint32, DerivedAssetInfo>& derivedAssetList) = 0;
				virtual bool commitDerivedAssets(BaseCachedAsset& derivedFrom, std::unordered_map<uint32, DerivedAssetInfo>& derivedAssetList) = 0;
				virtual const BaseAsset* getAssetInfo(GlobalAssetId globalAssetId) const = 0;


			//[-------------------------------------------------------]
			//[ Protected methods                                     ]
			//[-------------------------------------------------------]
			protected:
				/**
				*  @brief
				*    Default constructor
				*/
				AssetStorageManager();


			//[-------------------------------------------------------]
			//[ Private data                                          ]
			//[-------------------------------------------------------]
			private:
				TextureSemanticManager*												 mTextureSemanticManager;	///< Texture semantic manager instance, always valid, we're responsible for destroying the instance in case we no longer need it
				AssetMaintenanceManager*											 mAssetMaintenanceManager;	///< Asset maintenance manager instance, always valid, we're responsible for destroying the instance in case we no longer need it
				std::unordered_map<GlobalAssetId, std::unordered_set<GlobalAssetId>> mDependants;				///< Maps global asset IDs to the assets that depend on them (one might call it "reverse dependency map")


			};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
		} // base
	} // editor
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf_editor_base/asset/AssetStorageManager-inl.h"

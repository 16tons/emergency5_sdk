// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/asset/AssetSystemTypes.h>

#include <QtWidgets/qmenu.h>

#include <vector>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class AssetPackage;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace editor
	{


		//[-------------------------------------------------------]
		//[ Global definitions                                    ]
		//[-------------------------------------------------------]
		typedef std::vector<AssetPackage*> AssetPackages;


		//[-------------------------------------------------------]
		//[ Classes                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Asset menu
		*
		*  @note
		*    - This menu is more complex and dynamic, so it's justified to have a special menu class for this task
		*/
		class AssetMenu : public QMenu
		{


		//[-------------------------------------------------------]
		//[ Qt definitions (MOC)                                  ]
		//[-------------------------------------------------------]
		Q_OBJECT	// All files using the Q_OBJECT macro need to be compiled using the Meta-Object Compiler (MOC) of Qt, else slots won't work!
					// (VisualStudio: Header file -> Right click -> Properties -> "Custom Build Tool")


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			enum Flag
			{
				NO_SHOW_IN_ASSET_BROWSER	= 1<<0,	///< The "Show in Asset Browser"-action isn't available
				NO_SHOW_IN_PREFAB_BROWSER	= 1<<1,	///< The "Show in Prefab Browser"-action isn't available
				NO_SHOW_IN_MATERIAL_BROWSER = 1<<2	///< The "Show in Material Browser"-action isn't available
			};


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Constructor
			*
			*  @param[in] selectedAssetPackages
			*    List of selected asset packages
			*  @param[in] globalAssetIds
			*    List of global asset IDs this menu should operate on
			*  @param[in] firstAssetAsTitle
			*    Use the first asset as title? (usability)
			*  @param[in] parent
			*    Optional parent
			*  @param[in] flags
			*    Flags
			*/
			AssetMenu(const AssetPackages& selectedAssetPackages, const std::vector<GlobalAssetId>& globalAssetIds, bool firstAssetAsTitle = false, QWidget* parent = nullptr, uint8 flags = 0);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~AssetMenu();


		//[-------------------------------------------------------]
		//[ Protected methods                                     ]
		//[-------------------------------------------------------]
		protected:
			/**
			*  @brief
			*    Default constructor
			*/
			AssetMenu();

			/**
			*  @brief
			*    Set global asset IDs and add the Qt actions to the Qt menu
			*
			*  @param[in] globalAssetIds
			*    List of global asset IDs this menu should operate on
			*  @param[in] selectedAssetPackages
			*    List of selected asset packages, empty to consider all asset packages
			*
			*  @note
			*    - You might not want to call this method multiple times, because the Qt actions are added to the Qt menu on each method call
			*/
			void setGlobalAssetIds(const std::vector<GlobalAssetId>& globalAssetIds, const AssetPackages& selectedAssetPackages = AssetPackages());

			bool isAnyAssetInALockedProject(const std::vector<GlobalAssetId>& globalAssetIds) const;


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::editor::AssetMenu methods      ]
		//[-------------------------------------------------------]
		protected:
			/**
			*  @brief
			*    Retrieve the local absolute cached asset filename to use in different actions
			*
			*  @param[in] index
			*    The index in the global asset ID-list the filename should be retrieved for
			*
			*  @return
			*    The local absolute cached asset filename to the specified asset
			*
			*  @note
			*    - Override this if you need to add additional data sources
			*    - The default implementation will throw an exception if out of bounds
			*/
			virtual std::string getAbsoluteCachedAssetFilename(size_t index) const;


		//[-------------------------------------------------------]
		//[ Protected data                                        ]
		//[-------------------------------------------------------]
		protected:
			std::vector<GlobalAssetId> mGlobalAssetIds;
			AssetPackages			   mSelectedAssetPackages;	///< List of selected asset packages, empty means consider all asset packages


		//[-------------------------------------------------------]
		//[ Private definitions                                   ]
		//[-------------------------------------------------------]
		private:
			/**
			*  @brief
			*    Asset identifier type
			*/
			enum AssetIdentifierType
			{
				CACHED_ASSET_PATH,
				SOURCE_ASSET_PATH,
				GLOBAL_ASSET_ID,
				LOCAL_ASSET_ID,
				LOCAL_ASSET_NAME,
				ALL_ASSET_IDENTIFIERS
			};


		//[-------------------------------------------------------]
		//[ Private Qt slots (MOC)                                ]
		//[-------------------------------------------------------]
		private Q_SLOTS:
			// Asset
			void onTriggeredShowInPrefabBrowser();
			void onTriggeredShowInMaterialBrowser();
			void onTriggeredShowInAssetBrowser();
			void onTriggeredEditAsset();
			void onTriggeredAssetHistory();
			void onAboutToShowAssetDependencies();
			void onAboutToShowAssetDependants();
			void onTriggeredCheckAssetDependencies();
			void onTriggeredMapShutdown();
			void onTriggeredMapStartup();
			// Cached asset
			void onTriggeredOpenCachedAsset();
			void onTriggeredExploreCachedAssetDirectory();
			void onTriggeredCopyCachedAssetPath();
			void onTriggeredInspectCachedAssetDynamicProperties();
			// Source asset
			void onTriggeredOpenSourceAsset();
			void onTriggeredExploreSourceAssetDirectory();
			void onTriggeredCopySourceAssetPath();
			// Asset identifier
			void onTriggeredCopyGlobalAssetId();
			void onTriggeredCopyLocalAssetId();
			void onTriggeredCopyLocalAssetName();
			void onTriggeredCopyAllAssetIdentifiers();


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			/**
			*  @brief
			*    Copy asset identifier to clipboard
			*
			*  @param[in] assetIdentifierType
			*    Asset identifier to copy to the clipboard
			*/
			void copyAssetIdentifierToClipboard(AssetIdentifierType assetIdentifierType) const;

			/**
			*  @brief
			*    Get asset identifier as string
			*
			*  @param[in] index
			*    The index in the mGlobalAssetIds-list the path should be retrieved for
			*  @param[in] assetIdentifierType
			*    Asset identifier type to use for encoding as string
			*
			*  @return
			*    Asset identifier as string
			*/
			QString getAssetIdentifierAsString(size_t index, AssetIdentifierType assetIdentifierType) const;

			/**
			*  @brief
			*    Retrieve the local absolute source asset filename to use in different actions
			*
			*  @param[in] index
			*    The index in the global asset ID-list the filename should be retrieved for
			*
			*  @return
			*    The local absolute source asset filename to the specified asset
			*/
			std::string getAbsoluteSourceAssetFilename(size_t index) const;

			/**
			*  @brief
			*    Check asset dependencies
			*
			*  @param[in] globalAssetId
			*    Global asset ID to check the asset dependencies of
			*  @param[in] history
			*    Asset dependency history
			*
			*  @return
			*    "true" if all went fine, else "false" and asset issue has been detected
			*
			*  @note
			*    - Some kind of "Asset Maintenance Lite" so the artists can figure out why they just see a "missing box" (TM)
			*/
			bool checkAssetDependencies(GlobalAssetId globalAssetId, const std::string& history) const;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			uint8  mFlags;
			QMenu* mAssetDependenciesSubmenu;
			QMenu* mAssetDependantsSubmenu;


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf

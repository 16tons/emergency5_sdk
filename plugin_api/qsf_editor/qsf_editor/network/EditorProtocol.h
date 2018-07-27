// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/Export.h"

#include <qsf_editor_base/asset/AssetSystemTypes.h>
#include <qsf_editor_base/network/EditorProtocol.h>

#include <qsf/asset/BaseCachedAsset.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class BaseProject;
	class BaseAssetPackage;
	namespace editor
	{
		class EditorClient;
		namespace base
		{
			class Operation;
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


		//[-------------------------------------------------------]
		//[ Classes                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Editor network protocol class
		*/
		class QSF_EDITOR_API_EXPORT EditorProtocol : public base::EditorProtocol
		{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Constructor
			*
			*  @param[in] parentProtocol
			*    Parent protocol instance, must be valid and must stay alive as long as this editor protocol instance exists
			*  @param[in] editorClient
			*    Editor client instance, must be valid and must stay alive as long as this editor protocol instance exists
			*/
			EditorProtocol(QsfProtocol* parentProtocol, EditorClient* editorClient);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~EditorProtocol();

			/**
			*  @brief
			*    Attempt to login at the QSF editor server
			*
			*  @param[in] login
			*    Login name to request login with
			*  @param[in] password
			*    Token to request login with
			*/
			void requestLogin(const std::string& login, const std::string& token) const;

			/**
			*  @brief
			*    Ask QSF editor server for a list of available maps
			*/
			void requestMapList() const;

			/**
			*  @brief
			*    Ask QSF editor server to open a given project map
			*
			*  @param[in] projectId
			*    Project that contains the map (only actually needed for privilege/write access checks)
			*  @param[in] globalMapAssetId
			*    Global asset ID of the actual map asset that is going to be opened
			*  @param[in] file
			*    Path to download the current map snapshot into in order to be loaded afterwards
			*
			*  @return
			*    The NetworkTransferId of the snapshot-transfer if the request was sent successfully, else uninitialized
			*/
			base::NetworkTransferId requestMapOpen(ProjectId projectId, GlobalAssetId globalMapAssetId, const std::string& file);

			base::NetworkTransferId requestDownloadAsset(ProjectId projectId, const std::string& assetPackageName, GlobalAssetId globalAssetId, AssetRevision cachedAssetRevision, const std::string& file);
			void cancelTransfer(base::NetworkTransferId transferId);
			void requestEntityIds(size_t count);
			void requestAssetUpdate(ProjectId projectId, const std::string& assetPackageName, GlobalAssetId globalAssetId) const;
			void requestAssetPackageList(ProjectId projectId) const;
			void requestProjectList() const;
			void requestStartSync() const;
			void requestShutdownMap(ProjectId projectId, GlobalAssetId globalMapAssetId);
			void requestStartupMap(ProjectId projectId, GlobalAssetId globalMapAssetId);

			/**
			*  @brief
			*    Ask QSF editor server to create a new project
			*
			*  @param[in] project
			*    The project
			*/
			void requestProjectCreate(const BaseProject& project) const;

			/**
			*  @brief
			*    Ask QSF editor server to delete an existing project
			*
			*  @param[in] projectId
			*    The project ID
			*/
			void requestProjectDelete(ProjectId projectId) const;

			/**
			*  @brief
			*    Ask QSF editor server to edit an existing project
			*
			*  @param[in] project
			*    The project
			*/
			void requestProjectEdit(const BaseProject& project) const;

			/**
			*  @brief
			*    Ask QSF editor server to create a new asset package in an existing project
			*
			*  @param[in] projectId
			*    The target project ID
			*  @param[in] assetPackage
			*    The asset package
			*/
			void requestAssetPackageCreate(ProjectId projectId, const BaseAssetPackage& assetPackage) const;

			/**
			*  @brief
			*    Ask QSF editor server to create a new unmanaged asset package in an existing project and initiate the upload of an archive containing the asset package's contents
			*
			*  @param[in] projectId
			*    The target project ID
			*  @param[in] assetPackage
			*    The asset package
			*  @param[in] absoluteFilename
			*    The absolute filename of the archive containing the new asset package
			*/
			base::NetworkTransferId requestAssetPackageCreateUnmanaged(ProjectId projectId, const BaseAssetPackage& assetPackage, const std::string& absoluteFilename);

			/**
			*  @brief
			*    Ask QSF editor server to edit an existing asset package in an existing project
			*
			*  @param[in] projectId
			*    The target project ID
			*  @param[in] assetPackageName
			*    The target asset package name
			*/
			void requestAssetPackageDelete(ProjectId projectId, const std::string& assetPackageName) const;

			/**
			*  @brief
			*    Ask QSF editor server to edit an existing asset package in an existing project
			*
			*  @param[in] projectId
			*    The target project ID
			*  @param[in] assetPackage
			*    The asset package
			*/
			void requestAssetPackageEdit(ProjectId projectId, const BaseAssetPackage& assetPackage) const;

			/**
			*  @brief
			*    Ask QSF editor server to add an asset to an existing asset package in an existing project
			*
			*  @param[in] projectId
			*    The target project ID
			*  @param[in] assetPackageName
			*    The target asset package name
			*  @param[in] globalAssetId
			*    The asset's global ID
			*/
			void requestAssetPackageAddAsset(ProjectId projectId, const std::string& assetPackageName, GlobalAssetId globalAssetId) const;

			/**
			*  @brief
			*    Ask QSF editor server to remove an asset from an existing asset package in an existing project
			*
			*  @param[in] projectId
			*    The target project ID
			*  @param[in] assetPackageName
			*    The target asset package name
			*  @param[in] globalAssetId
			*    The asset's global ID
			*/
			void requestAssetPackageRemoveAsset(ProjectId projectId, const std::string& assetPackageName, GlobalAssetId globalAssetId) const;

			/**
			*  @brief
			*    Ask QSF editor server to set all assets to the release version
			*
			*  @param[in] projectId
			*    Project ID
			*
			*  @return
			*    "true" if all went fine, else "false"
			*/
			void requestProjectRelease(ProjectId projectId) const;

			/**
			*  @brief
			*    Send operation to the QSF editor server
			*
			*  @param[in] operation
			*    Operation to send
			*  @param[in] isUndo
			*    "true" if operation is being undone, "false" otherwise
			*/
			void submitOperation(const base::Operation& operation, bool isUndo) const;

			/**
			*  @brief
			*    Enable or disable receiving operations from the QSF editor server
			*
			*  @param[in] state
			*    "true" to enable, "false" to disable
			*/
			void setReceiveOperations(bool state);

			void requestSetMaintenanceMode(bool active);
			bool isInMaintenanceMode() const;

			//[-------------------------------------------------------]
			//[ Internal                                              ]
			//[-------------------------------------------------------]
			const BaseCachedAsset& getFinishedTransferBaseCachedAsset() const;
			std::pair<std::string, std::string> popTransferPaths(base::NetworkTransferId transferId);


		//[-------------------------------------------------------]
		//[ Public virtual qsf::QsfBinaryProtocol methods         ]
		//[-------------------------------------------------------]
		public:
			virtual void handlePacket(const qsf::packet::BinaryPacketBase& packet) override;


		//[-------------------------------------------------------]
		//[ Protected methods                                     ]
		//[-------------------------------------------------------]
		protected:
			void requestOperation();


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			EditorClient* mEditorClient;	///< Editor client, always valid, do not destroy the instance
			bool mReceivingOperations;
			bool mWaitingForOperations;
			bool mIsInMaintenanceMode;
			BaseCachedAsset mFinishedTransferBaseCachedAsset;
			std::map<base::NetworkTransferId, std::pair<std::string, std::string>> mTransferPathsMap;


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf

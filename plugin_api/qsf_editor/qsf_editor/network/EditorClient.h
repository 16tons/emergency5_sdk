// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/Export.h"

#include <qsf_editor_base/network/NetworkTypes.h>

#include <qsf/asset/AssetSystemTypes.h>
#include <qsf/network/layered/client/QsfClient.h>

#include <boost/property_tree/ptree_fwd.hpp>

#include <QtCore/qobject.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class BaseProject;
	class ParameterGroup;
	class BaseAssetPackage;
	class BaseCachedAsset;
	namespace editor
	{
		class EditorProtocol;
		namespace base
		{
			class Operation;
			struct EditorMapProjectList;
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
		*    QSF editor network client class
		*/
		class QSF_EDITOR_API_EXPORT EditorClient : public QObject, public QsfClient
		{


		//[-------------------------------------------------------]
		//[ Qt definitions (MOC)                                  ]
		//[-------------------------------------------------------]
		Q_OBJECT	// All files using the Q_OBJECT macro need to be compiled using the Meta-Object Compiler (MOC) of Qt, else slots won't work!
					// (VisualStudio: Header file -> Right click -> Properties -> "Custom Build Tool")


		//[-------------------------------------------------------]
		//[ Friends                                               ]
		//[-------------------------------------------------------]
			friend class EditorProtocol;	// The editor protocol needs to tell the editor client about itself


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Project map
			*/
			struct ProjectMap
			{
				ProjectId     projectId;		///< ID of the project the map is in
				GlobalAssetId globalMapAssetId;	///< Global ID of the map asset
				std::string   file;				///< File to download the current map snapshot into

				ProjectMap();
			};


		//[-------------------------------------------------------]
		//[ Public Qt signals (MOC)                               ]
		//[-------------------------------------------------------]
		Q_SIGNALS:
			/**
			*  @brief
			*    Qt signal emitted after the QSF server connection has been established
			*/
			void connected();

			/**
			*  @brief
			*    Qt signal emitted after the QSF server connection has been closed
			*/
			void disconnected();

			/**
			*  @brief
			*    Qt signal emitted after the QSF server connection has been lost
			*/
			void connectionLost();

			/**
			*  @brief
			*    Qt signal emitted after the QSF server connection attempt failed
			*/
			void connectFailed();

			/**
			*  @brief
			*    Qt signal emitted after the QSF server login response is available
			*/
			void loginResponse(bool isSuccess, bool isServerEmpty, const std::string& serverUuid, const boost::property_tree::ptree& pluginVersions);

			/**
			*  @brief
			*    Qt signal emitted after the QSF server project map list is available (empty on failure)
			*/
			void projectMapListResponse(const base::EditorMapProjectList& mapProjectList);

			void transferStarted(base::NetworkTransferId transferId);

			void transferProgress(base::NetworkTransferId transferId, uint64 current, uint64 total);

			void transferFinished(base::NetworkTransferId transferId, const std::string& fileExtension, const BaseCachedAsset& baseCachedAsset);

			void transferCanceled(base::NetworkTransferId transferId);

			void isInMaintenanceModeChanged(bool isInMaintenanceMode);


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Default constructor
			*/
			EditorClient();

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~EditorClient();

			/**
			*  @brief
			*    Attempt to login at the QSF editor server
			*
			*  @param[in] login
			*    Login name to request login with
			*  @param[in] password
			*    Token to request login with
			*
			*  @return
			*    "true" if all went fine, else "false"
			*/
			bool requestLogin(const std::string& login, const std::string& token) const;

			/**
			*  @brief
			*    Ask QSF editor server for a list of available project maps
			*
			*  @param[out] projectMaps
			*    Receives a list of available project maps, list is not cleared before adding new entries
			*
			*  @return
			*    "true" if all went fine, else "false"
			*/
			bool requestProjectMaps() const;

			/**
			*  @brief
			*    Ask QSF editor server to open a given project map
			*
			*  @param[in] projectMap
			*    Project map to open
			*
			*  @return
			*    The NetworkTransferId of the snapshot-transfer if the request was sent successfully, else uninitialized
			*/
			base::NetworkTransferId requestOpenProjectMap(const ProjectMap& projectMap);

			base::NetworkTransferId requestDownloadAsset(ProjectId projectId, const std::string& assetPackageName, GlobalAssetId globalAssetId, AssetRevision cachedAssetRevision, const std::string& file);
			bool cancelTransfer(base::NetworkTransferId transferId);
			bool requestEntityIds(size_t count);
			bool requestAssetUpdate(ProjectId projectId, const std::string& assetPackageName, GlobalAssetId globalAssetId) const;
			bool requestAssetPackageList(ProjectId projectId) const;
			bool requestProjectList() const;
			bool requestStartSync() const;
			bool requestShutdownMap(ProjectId projectId, GlobalAssetId globalMapAssetId) const;
			bool requestStartupMap(ProjectId projectId, GlobalAssetId globalMapAssetId) const;

			/**
			*  @brief
			*    Ask QSF editor server to create a new project
			*
			*  @param[in] project
			*    The project
			*/
			bool requestProjectCreate(const BaseProject& project) const;

			/**
			*  @brief
			*    Ask QSF editor server to delete an existing project
			*
			*  @param[in] projectId
			*    The project ID
			*/
			bool requestProjectDelete(ProjectId projectId) const;

			/**
			*  @brief
			*    Ask QSF editor server to edit an existing project
			*
			*  @param[in] project
			*    The project
			*/
			bool requestProjectEdit(const BaseProject& project) const;

			/**
			*  @brief
			*    Ask QSF editor server to create a new asset package in an existing project
			*
			*  @param[in] projectId
			*    The target project ID
			*  @param[in] assetPackage
			*    The asset package
			*/
			bool requestAssetPackageCreate(ProjectId projectId, const BaseAssetPackage& assetPackage) const;

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
			bool requestAssetPackageDelete(ProjectId projectId, const std::string& assetPackageName) const;

			/**
			*  @brief
			*    Ask QSF editor server to edit an existing asset package in an existing project
			*
			*  @param[in] projectId
			*    The target project ID
			*  @param[in] assetPackage
			*    The asset package
			*/
			bool requestAssetPackageEdit(ProjectId projectId, const BaseAssetPackage& assetPackage) const;

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
			bool requestAssetPackageAddAsset(ProjectId projectId, const std::string& assetPackageName, GlobalAssetId globalAssetId) const;

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
			bool requestAssetPackageRemoveAsset(ProjectId projectId, const std::string& assetPackageName, GlobalAssetId globalAssetId) const;

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
			bool requestProjectRelease(ProjectId projectId) const;

			/**
			*  @brief
			*    Send operation to the QSF editor server
			*
			*  @param[in] operation
			*    Operation to send
			*  @param[in] isUndo
			*    "true" if operation is being undone, "false" otherwise
			*
			*  @return
			*    "true" if all went fine, else "false"
			*/
			bool submitOperation(const base::Operation& operation, bool isUndo) const;

			/**
			*  @brief
			*    Enable or disable receiving operations from the QSF editor server
			*
			*  @param[in] state
			*    "true" to enable, "false" to disable
			*/
			bool setReceiveOperations(bool state);

			bool requestSetMaintenanceMode(bool active);
			bool isInMaintenanceMode() const;


		//[-------------------------------------------------------]
		//[ Public virtual qsf::Client methods                    ]
		//[-------------------------------------------------------]
		public:
			virtual void onConnected() override;
			virtual void onUnexpectedDisconnected() override;
			virtual void onConnectFailed() override;


		//[-------------------------------------------------------]
		//[ Public virtual qsf::QsfClient methods                 ]
		//[-------------------------------------------------------]
		public:
			inline virtual const QsfProtocolList& getProtocols() const override;


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			void setEditorProtocol(EditorProtocol* editorProtocol);
			void onLoginResponse(bool isSuccess, bool isServerEmpty, const std::string& serverUuid, const boost::property_tree::ptree& pluginVersions);
			void onProjectMapListResponse(const base::EditorMapProjectList& mapProjectList);
			void onTransferStarted(base::NetworkTransferId transferId);
			void onTransferProgress(base::NetworkTransferId transferId, uint64 current, uint64 total);
			void onTransferFinished(base::NetworkTransferId transferId, const std::string& fileExtension);
			void onTransferCanceled(base::NetworkTransferId transferId);

			/**
			*  @brief
			*    Called as soon as there's a property change inside the settings group manager
			*
			*  @param[in] parameterGroup
			*    Parameter group instance the changed property is in
			*  @param[in] propertyId
			*    Unique class property ID the component property which values was changed
			*/
			void onSettingsGroupPropertyChanged(const ParameterGroup& parameterGroup, uint32 propertyId);


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			QsfProtocolList mProtocols;			///< List of protocols the editor client supports
			EditorProtocol* mEditorProtocol;	///< Editor protocol instance, can be a null pointer, do not destroy the instance, pointer is set by "qsf::editor::EditorProtocol"

			boost::signals2::scoped_connection slot_transferStarted;
			boost::signals2::scoped_connection slot_transferProgress;
			boost::signals2::scoped_connection slot_transferFinished;
			boost::signals2::scoped_connection slot_transferCanceled;


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf_editor/network/EditorClient-inl.h"

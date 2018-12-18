// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/Export.h"

#include <qsf_editor_base/network/packet/asset/AssetHistory.h>
#include <qsf_editor_base/network/EditorCampStructs.h>

#include <qsf/asset/AssetSystemTypes.h>
#include <qsf/network/layered/client/QsfClient.h>

#include <QtCore/qobject.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class CachedAsset;
	class ParameterGroup;
	namespace editor
	{
		class AssetProtocol;
		namespace base
		{
			class UserBase;
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
		class QSF_EDITOR_API_EXPORT AssetClient : public QObject, public QsfClient
		{


		//[-------------------------------------------------------]
		//[ Qt definitions (MOC)                                  ]
		//[-------------------------------------------------------]
		Q_OBJECT	// All files using the Q_OBJECT macro need to be compiled using the Meta-Object Compiler (MOC) of Qt, else slots won't work!
					// (VisualStudio: Header file -> Right click -> Properties -> "Custom Build Tool")


		//[-------------------------------------------------------]
		//[ Friends                                               ]
		//[-------------------------------------------------------]
			friend class AssetProtocol;	// The editor protocol needs to tell the editor client about itself


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
			void loginResponse(const std::string& token, uint32 userId, const base::UserBase& user);

			/**
			*  @brief
			*    Qt signal emitted after the QSF server asset history response is available
			*/
			void assetHistory(GlobalAssetId globalAssetId, const std::vector<base::packet::AssetHistory::AssetHistoryEntry>& assetHistory);


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Default constructor
			*/
			AssetClient();

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~AssetClient();

			/**
			*  @brief
			*    Attempt to login at the QSF editor server
			*
			*  @param[in] login
			*    Login name to request login with
			*  @param[in] password
			*    Password to request login with
			*
			*  @return
			*    "true" if all went fine, else "false"
			*/
			bool requestLogin(const std::string& login, const std::string& password) const;

			bool requestUserList();
			bool createUser(const base::UserBase& userTemplate, const std::string& password);
			bool deleteUser(const std::string& login);
			bool editUser(const base::UserBase& userTemplate, const std::string& password = "");
			bool requestGlobalAssetIds(size_t count);
			bool requestAssetUpdate(GlobalAssetId globalAssetId);
			bool requestAssetDelete(GlobalAssetId globalAssetId);
			bool requestAssetHistory(GlobalAssetId globalAssetId);
			bool requestLockAssetForEditing(GlobalAssetId globalAssetId);
			bool unlockAssetForEditing(GlobalAssetId globalAssetId);
			bool deleteAssetSourceData(GlobalAssetId globalAssetId);
			bool submitAssetRevision(const Asset& asset, const CachedAsset* cachedAsset = nullptr);
			base::NetworkTransferId submitAssetRevision(const Asset& asset, const std::string& file, const CachedAsset* cachedAsset = nullptr);
			base::NetworkTransferId requestDownloadAssetSource(GlobalAssetId globalAssetId, AssetRevision revision, const std::string& file);
			bool cancelTransfer(base::NetworkTransferId transferId);
			bool requestStartSync();
			inline AssetProtocol* getAssetProtocol();


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
			void setAssetProtocol(AssetProtocol* assetProtocol);
			void onLoginResponse(const std::string& token, uint32 userId, const base::UserBase& user);

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
			QsfProtocolList mProtocols;		///< List of protocols the editor client supports
			AssetProtocol*  mAssetProtocol;	///< Asset protocol instance, can be a null pointer, do not destroy the instance, pointer is set by "qsf::editor::EditorProtocol"


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf_editor/network/AssetClient-inl.h"

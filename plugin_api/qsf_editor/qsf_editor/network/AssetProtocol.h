// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/Export.h"

#include <qsf_editor_base/network/AssetProtocol.h>

#include <qsf/asset/AssetSystemTypes.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class CachedAsset;
	namespace editor
	{
		class AssetClient;
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
		*    Editor network protocol class
		*/
		class QSF_EDITOR_API_EXPORT AssetProtocol : public base::AssetProtocol
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
			AssetProtocol(QsfProtocol* parentProtocol, AssetClient* editorClient);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~AssetProtocol();

			/**
			*  @brief
			*    Attempt to login at the QSF editor server
			*
			*  @param[in] username
			*    Username to request login with
			*  @param[in] password
			*    Password to request login with
			*/
			void requestLogin(const std::string& login, const std::string& password);

			void requestUserList();
			void createUser(const base::UserBase& userTemplate, const std::string& password);
			void deleteUser(const std::string& login);
			void editUser(const base::UserBase& userTemplate, const std::string& password);
			void requestGlobalAssetIds(size_t count);
			void requestAssetUpdate(GlobalAssetId globalAssetId);
			void requestAssetDelete(GlobalAssetId globalAssetId);
			void requestAssetHistory(GlobalAssetId globalAssetId);
			void requestLockAssetForEditing(GlobalAssetId globalAssetId);
			void unlockAssetForEditing(GlobalAssetId globalAssetId);
			void deleteAssetSourceData(GlobalAssetId globalAssetId);
			void submitAssetRevision(const Asset& asset, const CachedAsset* cachedAsset);
			base::NetworkTransferId submitAssetRevision(const Asset& asset, const std::string& file, const CachedAsset* cachedAsset);
			base::NetworkTransferId requestDownloadAssetSource(GlobalAssetId globalAssetId, AssetRevision revision, const std::string& file);
			void cancelTransfer(base::NetworkTransferId transferId);
			void requestStartSync();


		//[-------------------------------------------------------]
		//[ Public virtual qsf::QsfBinaryProtocol methods         ]
		//[-------------------------------------------------------]
		public:
			virtual void handlePacket(const qsf::packet::BinaryPacketBase& packet) override;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			AssetClient* mAssetClient;	///< Asset client, always valid, do not destroy the instance


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf

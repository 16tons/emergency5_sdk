// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/Export.h"

#include <qsf/base/manager/Manager.h>
#include <qsf/forward/BoostPTree.h>

#include <QtCore/qobject.h>

#include <string>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class RightsManager;
	namespace editor
	{
		class AssetClient;
		class EditorClient;
		namespace base
		{
			class User;
			class UserBase;
			class UserManager;
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
		*    QSF editor network manager
		*/
		class QSF_EDITOR_API_EXPORT NetworkManager : public QObject, public Manager
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
			static const QString OFFLINE_HOSTNAME;	///< Hostname which is used for the offline mode ("offline")
			static const QString DEFAULT_HOSTNAME;	///< Name of the default host to connect to (e.g. "localhost")
			static const QString DEFAULT_PORT;		///< Default port to use (e.g. "4042")

			enum LoginResponse
			{
				LOGIN_SUCCESS,
				LOGIN_BAD_LOGIN_PASSWORD,
				LOGIN_INCOMPATIBLE_VERSION
			};


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Default constructor
			*/
			NetworkManager();

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~NetworkManager();

			/**
			*  @brief
			*    Return whether or not the QSF editor client is currently logged into a server
			*
			*  @return
			*    "true" if the QSF editor is currently logged into a server, else "false"
			*/
			bool isLoggedIn() const;

			/**
			*  @brief
			*    Disconnect asset and editor clients from the servers
			*/
			void disconnectFromServers();

			//[-------------------------------------------------------]
			//[ User                                                  ]
			//[-------------------------------------------------------]
			/**
			*  @brief
			*    Set default user settings
			*
			*  @note
			*    - Only interesting for the offline editor user, do only use this method if you know exactly what you're doing
			*    - Method is blocked in online editor mode
			*/
			void setDefaultUserSettings();

			/**
			*  @brief
			*    Return the rights manager instance
			*
			*  @return
			*    The rights manager instance, do not destroy the returned instance
			*/
			inline RightsManager& getRightsManager() const;

			/**
			*  @brief
			*    Return the user manager instance
			*
			*  @return
			*    The user manager instance, do not destroy the returned instance
			*/
			inline base::UserManager& getUserManager() const;

			/**
			*  @brief
			*    Return the QSF editor user instance
			*
			*  @return
			*    Reference to the QSF editor user instance, do not destroy the instance
			*
			*  @note
			*    - WARINING: Do not cache the returned instance - it may get invalidated at any time!
			*    - The user is the "operator" and has "the right to execute certain operations"
			*/
			inline base::User& getUser() const;

			//[-------------------------------------------------------]
			//[ Asset client                                          ]
			//[-------------------------------------------------------]
			/**
			*  @brief
			*    Return the QSF asset network client instance
			*
			*  @return
			*    Reference to the QSF asset network client instance, do not destroy the instance
			*/
			inline AssetClient& getAssetClient() const;

			/**
			*  @brief
			*    Connect the client to a asset server
			*
			*  @param[in] hostname
			*    ASCII name of the host to connect to (e.g. "localhost")
			*  @param[in] port
			*    Port to use (e.g. "4000")
			*
			*  @return
			*    "true" if all went fine, else "false"
			*
			*  @note
			*    - If the client is already connected to a server, the previous connection will be closed before opening a new one
			*/
			bool connectToAssetServer(const std::string& hostname, unsigned short port);

			/**
			*  @brief
			*    Connect the client to a asset server
			*
			*  @return
			*    "true" if all went fine, else "false"
			*
			*  @note
			*    - If the client is already connected to a server, the previous connection will be closed before opening a new one
			*    - Already existing asset hostname and asset port will be used (reconnect)
			*/
			bool connectToAssetServer();

			/**
			*  @brief
			*    Attempt to login at the QSF asset server
			*
			*  @param[in] username
			*    Username to request login with
			*  @param[in] password
			*    Password to request login with
			*
			*  @return
			*    "true" if all went fine, else "false"
			*/
			bool requestLoginOnAssetServer(const std::string& username, const std::string& password);

			//[-------------------------------------------------------]
			//[ Editor client                                         ]
			//[-------------------------------------------------------]
			/**
			*  @brief
			*    Return the QSF editor network client instance
			*
			*  @return
			*    Reference to the QSF editor network client instance, do not destroy the instance
			*/
			inline EditorClient& getEditorClient() const;

			/**
			*  @brief
			*    Connect the client to a editor server
			*
			*  @param[in] hostname
			*    ASCII name of the host to connect to (e.g. "localhost")
			*  @param[in] port
			*    Port to use (e.g. "4000")
			*
			*  @return
			*    "true" if all went fine, else "false"
			*
			*  @note
			*    - If the client is already connected to a server, the previous connection will be closed before opening a new one
			*/
			bool connectToEditorServer(const std::string& hostname, unsigned short port);

			/**
			*  @brief
			*    Connect the client to a editor server
			*
			*  @return
			*    "true" if all went fine, else "false"
			*
			*  @note
			*    - If the client is already connected to a server, the previous connection will be closed before opening a new one
			*    - In case there's no editor hostname and editor port, the asset hostname and asset port will be used instead
			*/
			bool connectToEditorServer();

			/**
			*  @brief
			*    Attempt to login at the QSF editor server
			*
			*  @return
			*    "true" if all went fine, else "false"
			*/
			bool requestLoginOnEditorServer();

			//[-------------------------------------------------------]
			//[ Internal                                              ]
			//[-------------------------------------------------------]
			void pushUsers(const std::unordered_map<uint32, base::UserBase>& users);


		//[-------------------------------------------------------]
		//[ Public Qt signals (MOC)                               ]
		//[-------------------------------------------------------]
		Q_SIGNALS:
			void userListUpdated();
			void assetClientLoginResponse(LoginResponse response, const std::string& message);
			void editorClientLoginResponse(LoginResponse response, const std::string& message, const std::string& serverUuid, bool isServerEmpty);


		//[-------------------------------------------------------]
		//[ Private Qt slots (MOC)                                ]
		//[-------------------------------------------------------]
		private Q_SLOTS:
			void onAssetDisconnected();
			void onAssetLoginResponse(const std::string& token, uint32 userId, const base::UserBase& user);
			void onEditorLoginResponse(bool isSuccess, bool isServerEmpty, const std::string& serverUuid, const boost::property_tree::ptree& pluginVersions);


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			// User
			RightsManager*		 mRightsManager;	///< Rights manager instance, always valid, we have to destroy the instance in case we no longer need it TODO(co) We probably want to have this in the operation manager base class
			base::UserManager*   mUserManager;		///< Internal user manager instance, always valid, we have to destroy the instance in case we no longer need it
			base::User*			 mUser;				///< QSF editor user instance, always valid, the user manager will destroy the instance
			std::string			 mToken;			///< Since it's not a good idea to keep sending the actual password back and forth when authenticating to different services the Asset Server provides us with a random (security-) token which gets invalidated as soon as the user logs out from the Asset Server again
			// Asset client
			std::string		mAssetHostname;
			unsigned short	mAssetPort;
			AssetClient*	mAssetClient;	///< QSF asset network client instance, always valid, we have to destroy the instance in case we no longer need it
			// Editor client
			std::string    mEditorHostname;
			unsigned short mEditorPort;
			EditorClient*  mEditorClient;	///< QSF editor network client instance, always valid, we have to destroy the instance in case we no longer need it


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf_editor/network/NetworkManager-inl.h"

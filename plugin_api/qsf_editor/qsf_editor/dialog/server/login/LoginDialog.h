// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/dialog/server/ClientStartupDialog.h"
#include "qsf_editor/network/NetworkManager.h"

#include <qsf/forward/BoostPTree.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace editor
	{
		namespace base
		{
			class UserBase;
		}
	}
}
namespace Ui
{
	class LoginDialog;
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
		*    QSF editor client login dialog
		*
		*  @note
		*    - As result, the Qt dialog returns the new stage "qsf::editor::Application::Stage" the application should switch to
		*
		*  @todo
		*    - TODO(co) This is just a first quick'n'dirty prototype, fine tune it
		*    - TODO(co) Give the user feedback about the attempt to sign in to the server and also provide information in case of a sign in failure
		*    - TODO(co) Do only store and submit encrypted passwords
		*    - TODO(co) Add history support
		*/
		class LoginDialog : public ClientStartupDialog
		{


		//[-------------------------------------------------------]
		//[ Qt definitions (MOC)                                  ]
		//[-------------------------------------------------------]
		Q_OBJECT	// All files using the Q_OBJECT macro need to be compiled using the Meta-Object Compiler (MOC) of Qt, else slots won't work!
					// (VisualStudio: Header file -> Right click -> Properties -> "Custom Build Tool")


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Constructor
			*
			*  @param[in] loginAutomatically
			*    Try to automatically login on startup?
			*  @param[in] qWidgetParent
			*    Pointer to parent Qt widget, can be a null pointer (in this case you're responsible for destroying this dialog instance)
			*/
			LoginDialog(bool loginAutomatically, QWidget* qWidgetParent = nullptr);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~LoginDialog();


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::editor::ClientStartupDialog methods ]
		//[-------------------------------------------------------]
		protected:
			virtual void retranslateUi() override;


		//[-------------------------------------------------------]
		//[ Protected virtual QWidget methods                     ]
		//[-------------------------------------------------------]
		protected:
			virtual void closeEvent(QCloseEvent* qCloseEvent) override;


		//[-------------------------------------------------------]
		//[ Private Qt slots (MOC)                                ]
		//[-------------------------------------------------------]
		private Q_SLOTS:
			void onClickedBack(bool checked);
			void onClickedOptions(bool checked);
			void onClickedSignIn(bool checked);
			void onAssetConnected();
			void onAssetConnectFailed();
			void onAssetLoginResponse(NetworkManager::LoginResponse response, const std::string& message);
			void onEditorConnected();
			void onEditorConnectFailed();
			void onEditorLoginResponse(NetworkManager::LoginResponse response, const std::string& message, const std::string& serverUuid, bool isServerEmpty);


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			Ui::LoginDialog* mUiLoginDialog;		///< UI login dialog instance, can be a null pointer, we have to destroy the instance in case we no longer need it


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf

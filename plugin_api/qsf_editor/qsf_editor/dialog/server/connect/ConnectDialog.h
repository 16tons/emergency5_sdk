// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/dialog/server/ClientStartupDialog.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Ui
{
	class ConnectDialog;
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
		*    QSF editor client connect dialog
		*
		*  @note
		*    - As result, the Qt dialog returns the new stage "qsf::editor::Application::Stage" the application should switch to
		*
		*  @todo
		*    - TODO(co) This is just a first quick'n'dirty prototype, fine tune it
		*    - TODO(co) Give the user feedback about the attempt to connect to the server and also provide information in case of a connection failure
		*    - TODO(co) Make it possible to reset to the default settings (hostname="localhost", port="4042")
		*    - TODO(co) Add history support
		*/
		class ConnectDialog : public ClientStartupDialog
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
			*  @param[in] connectAutomatically
			*    Try to automatically connect on startup?
			*  @param[in] allowDifferentHostPort
			*    Allow changing host/port in this dialog?
			*  @param[in] qWidgetParent
			*    Pointer to parent Qt widget, can be a null pointer (in this case you're responsible for destroying this dialog instance)
			*/
			ConnectDialog(bool connectAutomatically, bool allowDifferentHostPort, QWidget* qWidgetParent = nullptr);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~ConnectDialog();


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
			void onClickedOptions(bool checked);
			void onClickedConnect(bool checked);
			void onCurrentTextChangedQsfEditorServer(const QString& text);
			void onAssetConnected();
			void onAssetConnectFailed();


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			Ui::ConnectDialog* mUiConnectDialog;	///< UI connect dialog instance, can be a null pointer, we have to destroy the instance in case we no longer need it


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf

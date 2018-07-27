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
	class LoginOptionsDialog;
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
		*    QSF editor client login options dialog
		*
		*  @todo
		*    - TODO(co) This is just a first quick'n'dirty prototype, fine tune it
		*    - TODO(co) Support the other options
		*/
		class LoginOptionsDialog : public ClientStartupDialog
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
			*  @param[in] qWidgetParent
			*    Pointer to parent Qt widget, can be a null pointer (in this case you're responsible for destroying this dialog instance)
			*/
			explicit LoginOptionsDialog(QWidget* qWidgetParent = nullptr);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~LoginOptionsDialog();


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::editor::ClientStartupDialog methods ]
		//[-------------------------------------------------------]
		protected:
			virtual void retranslateUi() override;


		//[-------------------------------------------------------]
		//[ Private Qt slots (MOC)                                ]
		//[-------------------------------------------------------]
		private Q_SLOTS:
			void onClickedOk(bool checked);
			void onClickedCancel(bool checked);


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			Ui::LoginOptionsDialog* mUiLoginOptionsDialog;	///< UI login options dialog instance, can be a null pointer, we have to destroy the instance in case we no longer need it


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf

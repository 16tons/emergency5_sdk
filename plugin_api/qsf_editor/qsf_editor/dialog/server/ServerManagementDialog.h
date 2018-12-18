// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/dialog/Dialog.h"

#include <qsf_editor_base/user/UserBase.h>

#include <qsf/base/GetUninitialized.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Ui
{
	class ServerManagementDialog;
}
namespace qsf
{
	namespace editor
	{
		class AssetClient;
		namespace base
		{
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
		*    QSF editor server management dialog
		*/
		class ServerManagementDialog : public Dialog
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
			static const uint32 PLUGINABLE_ID;	///< "qsf::editor::ServerManagementDialog" unique pluginable dialog ID

			class EditedUser : public base::UserBase
			{
			public:
				EditedUser() :
					mUserId(getUninitialized<uint32>())
				{
					// Nothing to do in here
				}

				explicit EditedUser(const base::UserBase& x) :
					base::UserBase(x),
					mUserId(getUninitialized<uint32>())
				{
					// Nothing to do in here
				}

				// Connected to CAMP reflection system
				std::string mPassword;
				// Internal
				uint32 mUserId;


				//[-------------------------------------------------------]
				//[ CAMP reflection system                                ]
				//[-------------------------------------------------------]
				QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


			};


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Constructor
			*
			*  @param[in] dialogManager
			*    The dialog manager this dialog instance is in, no null pointer allowed (the CAMP reflection system does not like references in constructors, so we have to use a pointer instead)
			*  @param[in] qWidgetParent
			*    Pointer to parent Qt widget, can be a null pointer (in this case you're responsible for destroying this dialog instance)
			*/
			ServerManagementDialog(DialogManager* dialogManager, QWidget* qWidgetParent);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~ServerManagementDialog();


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::editor::Dialog methods         ]
		//[-------------------------------------------------------]
		protected:
			virtual void retranslateUi() override;
			virtual void changeVisibility(bool visible) override;


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			void rebuildListUsers();
			EditedUser* getEditedUser() const;
			bool isEditedUserModified() const;
			bool saveEditedUser();


		//[-------------------------------------------------------]
		//[ Private Qt slots (MOC)                                ]
		//[-------------------------------------------------------]
		private Q_SLOTS:
			void onClickedUserCreate(bool checked);
			void onClickedUserDelete(bool checked);
			void onClickedUserSave(bool checked);
			void onUserSelectionChanged(QListWidgetItem* current, QListWidgetItem* previous);
			void onIsInMaintenanceModeChanged(bool isInMaintenanceMode);
			void onClickedMaintenanceEnable(bool checked);
			void onClickedMaintenanceDisable(bool checked);


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			Ui::ServerManagementDialog* mUiServerManagementDialog;	///< UI server management dialog instance, can be a null pointer, we have to destroy the instance in case we no longer need it
			AssetClient&				mAssetClient;
			base::UserManager&			mUserManager;


		//[-------------------------------------------------------]
		//[ CAMP reflection system                                ]
		//[-------------------------------------------------------]
		QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::ServerManagementDialog)
QSF_CAMP_TYPE(qsf::editor::ServerManagementDialog::EditedUser)

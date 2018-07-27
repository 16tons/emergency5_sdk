// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/dialog/Dialog.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Ui
{
	class ShortcutMapperDialog;
}
namespace qsf
{
	namespace editor
	{
		class ShortcutMapperConflictDialog;
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
		*    QSF editor shortcut mapper dialog
		*/
		class ShortcutMapperDialog : public Dialog
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
			static const uint32 PLUGINABLE_ID;	///< "qsf::editor::ShortcutMapperDialog" unique pluginable dialog ID


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
			ShortcutMapperDialog(DialogManager* dialogManager, QWidget* qWidgetParent);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~ShortcutMapperDialog();


		//[-------------------------------------------------------]
		//[ Public virtual QObject methods                        ]
		//[-------------------------------------------------------]
		public:
			virtual bool event(QEvent* qEvent) override;
			virtual bool eventFilter(QObject* qObject, QEvent* qEvent) override;


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::editor::Dialog methods         ]
		//[-------------------------------------------------------]
		protected:
			virtual void retranslateUi() override;
			virtual void changeVisibility(bool visible) override;


		//[-------------------------------------------------------]
		//[ Private definitions                                   ]
		//[-------------------------------------------------------]
		private:
			static const int CATEGORY_COLUMN	= 0;	///< Column of the category
			static const int ACTION_NAME_COLUMN	= 1;	///< Column of the action name
			static const int SHORTCUT_COLUMN	= 2;	///< Column of the shortcut


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			/**
			*  @brief
			*    Checks the given QKeyEvent for a valid QKeySequence and save it as new shortcut to the current action
			*
			*  @param[in] qKeyEvent
			*    Is checked for valid shortcut and assigned to selected action
			*
			*  @TODO(fr) Looks like the return type is irrelevant, as it always returns true. Is this intended?
			*/
			bool recordAction(QKeyEvent& qKeyEvent);

			/**
			*  @brief
			*    Set the shortcut of an action
			*
			*  @parm[in] newShortcut
			*    The new Shortcut
			*  @parm[in] row
			*    Row inside the table
			*/
			void setActionShortcut(QKeySequence newShortcut, int row);

			/**
			*  @brief
			*    Check all actions for duplicates with check
			*
			*  @param[in] check
			*    The KeySequence for check for conflicts over all actions
			*  @param[out] conflictInRow
			*    The row with the conflict
			*/
			bool checkForConflicts(QKeySequence check, int& conflictInRow) const;

			// TODO(co) Comment me
			void startupEditMode();
			void shutdownEditMode();


		//[-------------------------------------------------------]
		//[ Private Qt slots (MOC)                                ]
		//[-------------------------------------------------------]
		private Q_SLOTS:
			void onSearchFieldTextChanged(const QString& searchText);
			void onClickedResetButton(bool checked);
			void onClickedResetAllButton(bool checked);
			void onClickedBindButtonKey(bool checked);
			void onClickedUnBindButtonKey(bool checked);


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			Ui::ShortcutMapperDialog*		mUiShortcutMapperDialog;		///< UI shortcut mapper dialog instance, can be a null pointer, we have to destroy the instance in case we no longer need it
			QKeySequence					mOldShortcut;					///< The old key sequence of the shortcut we are working with
			bool							mEditShortcutMode;
			ShortcutMapperConflictDialog*	mShortcutMapperConflictDialog;	///< Shortcut mapper conflict dialog. We use an own instance instead of getting it via the DialogManager because we need to set a proper parent


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
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::ShortcutMapperDialog)

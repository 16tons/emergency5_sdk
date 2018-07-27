// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <QtWidgets/qdialog.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Ui
{
	class ShortcutMapperConflictDialog;
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
		*    QSF editor shortcut mapper conflict dialog
		*
		*  @note
		*    - This is a special dialog only used by the shortcut mapper dialog -> No CAMP registration and such only a pure QDialog
		*/
		class ShortcutMapperConflictDialog : public QDialog
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
			explicit ShortcutMapperConflictDialog(QWidget* qWidgetParent);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~ShortcutMapperConflictDialog();

			/**
			*  @brief
			*    Set texts
			*
			*  @param[in] newShortcutText
			*    Shortcut text
			*  @param[in] textLeft
			*    Left text
			*  @param[in] textRight
			*    Right text
			*/
			void setTexts(const QString& newShortcutText, const QString& textLeft, const QString& textRight);

			/**
			*  @brief
			*    Gets called when the translation changes
			*/
			void retranslateUi();


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			Ui::ShortcutMapperConflictDialog* mUiShortcutMapperConflictDialog;	///< UI shortcut mapper conflict dialog instance, always valid, we have to destroy the instance in case we no longer need it


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf

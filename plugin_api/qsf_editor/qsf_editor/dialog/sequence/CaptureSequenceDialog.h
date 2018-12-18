// Copyright (C) 2012-2018 Promotion Software GmbH


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
	class CaptureSequenceDialog;
}
namespace qsf
{
	namespace editor
	{
		class SequenceEditorState;
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
		*    QSF editor capture sequence dialog
		*
		*  @note
		*    - This is a special dialog only used by the sequence dialog -> No CAMP registration and such only a pure QDialog
		*/
		class CaptureSequenceDialog : public QDialog
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
			*  @param[in] sequenceEditorState
			*    Sequence editor state to use, must stay valid as long as this dialog instance exists
			*/
			CaptureSequenceDialog(QWidget* qWidgetParent, SequenceEditorState& sequenceEditorState);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~CaptureSequenceDialog();


		//[-------------------------------------------------------]
		//[ Public virtual QDialog methods                        ]
		//[-------------------------------------------------------]
		public:
			virtual void reject() override;


		//[-------------------------------------------------------]
		//[ Protected virtual QWidget methods                     ]
		//[-------------------------------------------------------]
		protected:
			virtual void closeEvent(QCloseEvent* qCloseEvent) override;


		//[-------------------------------------------------------]
		//[ Private Qt slots (MOC)                                ]
		//[-------------------------------------------------------]
		private Q_SLOTS:
			void onCurrentIndexChangedSize(int index);
			void onClickedDirectory(bool checked);
			void onClickedCapture(bool checked);


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			SequenceEditorState&	   mSequenceEditorState;		///< Sequence editor state to use, must stay valid as long as this dialog instance exists
			Ui::CaptureSequenceDialog* mUiCaptureSequenceDialog;	///< UI capture screenshot dialog instance, always valid, we have to destroy the instance in case we no longer need it
			bool					   mCapturingInProgress;		/// "true" if sequence capturing is currently in progress, else "false"


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf

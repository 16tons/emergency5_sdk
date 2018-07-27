// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/Export.h"

#include <QtWidgets/qdialog.h>


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
		*    Abstract QSF editor client startup dialog base class
		*
		*  @remarks
		*    The client startup dialogs are outside of the normal QSF editor infrastructure. All dialogs involved in the QSF editor client
		*    startup process should be derived from "qsf::editor::ClientStartupDialog" instead of the managed "qsf::editor::Dialog" base class.
		*/
		class ClientStartupDialog : public QDialog
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
			*    Destructor
			*/
			inline virtual ~ClientStartupDialog();


		//[-------------------------------------------------------]
		//[ Protected methods                                     ]
		//[-------------------------------------------------------]
		protected:
			/**
			*  @brief
			*    Constructor
			*
			*  @param[in] qWidgetParent
			*    Pointer to parent Qt widget, can be a null pointer (in this case you're responsible for destroying this dialog instance)
			*/
			explicit ClientStartupDialog(QWidget* qWidgetParent);

			/**
			*  @brief
			*    Setup window title
			*/
			void setupWindowTitle();


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::editor::ClientStartupDialog methods ]
		//[-------------------------------------------------------]
		protected:
			/**
			*  @brief
			*    Retranslate the UI
			*
			*  @note
			*    - The default implementation is empty
			*/
			inline virtual void retranslateUi();


		//[-------------------------------------------------------]
		//[ Protected virtual QWidget methods                     ]
		//[-------------------------------------------------------]
		protected:
			virtual void changeEvent(QEvent* qEvent) override;


		//[-------------------------------------------------------]
		//[ Private Qt slots (MOC)                                ]
		//[-------------------------------------------------------]
		private Q_SLOTS:
			void onConnectionLost();


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf_editor/dialog/server/ClientStartupDialog-inl.h"

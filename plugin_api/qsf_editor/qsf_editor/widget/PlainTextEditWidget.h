// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <QtWidgets/qplaintextedit.h>


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
		*    QSF plant text edit with focus out signal
		*/
		class PlainTextEditWidget: public QPlainTextEdit
		{


			//[-------------------------------------------------------]
			//[ Qt definitions (MOC)                                  ]
			//[-------------------------------------------------------]
			Q_OBJECT	// All files using the Q_OBJECT macro need to be compiled using the Meta-Object Compiler (MOC) of Qt, else slots won't work!
						// (VisualStudio: Header file -> Right click -> Properties -> "Custom Build Tool")


			//[-------------------------------------------------------]
			//[ Public Qt signals (MOC)                               ]
			//[-------------------------------------------------------]
			Q_SIGNALS:
				/**
				*  @brief
				*    Qt signal emitted on focus out event
				*/
				void focusOut();


			//[-------------------------------------------------------]
			//[ Public methods                                        ]
			//[-------------------------------------------------------]
			public:
				explicit PlainTextEditWidget(QWidget* qWidgetParent);


			//[-------------------------------------------------------]
			//[ Protected virtual QWidget methods                     ]
			//[-------------------------------------------------------]
			protected:
				virtual void focusOutEvent(QFocusEvent* qFocusEvent) override;


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf

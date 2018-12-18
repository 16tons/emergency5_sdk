// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/Export.h"

#include <QtWidgets/qwidget.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
QT_BEGIN_NAMESPACE
	class QPlainTextEdit;
QT_END_NAMESPACE
namespace qsf
{
	struct LogMessage;
	namespace editor
	{
		class EditorSettingsGroup;
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
		*    Debug log widget class
		*
		*  @note
		*    - The UI is created via source code
		*    - Log introspection only, no manipulation
		*/
		class LogWidget : public QWidget
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
			*    Pointer to parent Qt widget, can be a null pointer (in this case you're responsible for destroying this view instance)
			*/
			explicit LogWidget(QWidget* qWidgetParent = nullptr);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~LogWidget();


		//[-------------------------------------------------------]
		//[ Public virtual QWidget methods                        ]
		//[-------------------------------------------------------]
		public:
			virtual void setVisible(bool visible) override;


		//[-------------------------------------------------------]
		//[ Private Qt slots (MOC)                                ]
		//[-------------------------------------------------------]
		private Q_SLOTS:
			void onCustomContextMenuRequested(const QPoint& qPoint);
			void onTriggeredCopy();


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			/**
			*  @brief
			*    Append a log message to the Qt plain text edit instance
			*
			*  @param[in] message
			*    The UTF-8 log message to append
			*/
			void appendLogMessage(const LogMessage& message);

			/**
			*  @brief
			*    Called when a new log message has been added
			*
			*  @param[in] message
			*    The new UTF-8 log message
			*/
			void onNewLogMessage(const LogMessage& message);

			/**
			*  @brief
			*    Perform a GUI rebuild
			*/
			void rebuildGui();


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			bool				 mVisible;				///< Current QWidget visibility
			EditorSettingsGroup& mEditorSettingsGroup;	///< Editor settings group instance; The log widget is using it quite often, so we cache the reference
			QPlainTextEdit*		 mPlainTextEdit;		///< The text edit field of the view, can be a null pointer, Qt will destroy the instance automatically


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf

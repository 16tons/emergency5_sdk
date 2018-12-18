// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/Export.h"
#include "qsf_editor/base/FilterConfiguration.h"

#include <QtWidgets/qwidget.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
QT_BEGIN_NAMESPACE
	class QToolButton;
	class QLineEdit;
QT_END_NAMESPACE


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
		*    Search widget class
		*
		*  @note
		*    - Reusable class which shows a search mask
		*
		*  @todo
		*    - TODO(sw) add signal when the search text changes or better let this widget know the filter model to which it sets the values correctly (needs a proper base class of such filter models)
		*/
		class QSF_EDITOR_API_EXPORT SearchWidget : public QWidget
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
			*    Qt signal emitted after an changing a filter setting via context menu
			*
			*  @param[in] newFilterConfiguration
			*    The changed filter configuration
			*/
			void filterChanged(const FilterConfiguration& newFilterConfiguration);
			void textChanged(const QString& searchText);


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Constructor
			*
			*  @param[in] qWidgetParent
			*    Pointer to parent Qt widget, can be a null pointer (in this case you're responsible for destroying this widget instance)
			*  @param[in] hasSettings
			*    Should there be a settings button?
			*/
			SearchWidget(QWidget* qWidgetParent, bool hasSettings = true);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~SearchWidget();

			/**
			*  @brief
			*    Return the text field used for insert the text to searching
			*/
			QLineEdit& getSearchFieldText() const;

			const FilterConfiguration& getFilterConfiguration() const;

			/**
			*  @brief
			*    Set the visibility state of the settings button
			*/
			void setSettingsButtonVisible(bool visible);

			/**
			*  @brief
			*    Retranslate the UI
			*/
			void retranslateUi();


		//[-------------------------------------------------------]
		//[ Protected virtual QWidget methods                     ]
		//[-------------------------------------------------------]
		protected:
			void focusInEvent(QFocusEvent* focusInEvent) override;


		//[-------------------------------------------------------]
		//[ Private Qt slots (MOC)                                ]
		//[-------------------------------------------------------]
		private Q_SLOTS:
			void onPressedSettings();


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			QToolButton*		mButtonSetting;			///< Pointer to QToolButton, can be a null pointer, Qt will destroy the instance automatically
			QLineEdit*			mLineEditTextField;		///< Pointer to QLineEdit, always valid, Qt will destroy the instance automatically
			FilterConfiguration	mFilterConfiguration;	///< Filter configuration


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf

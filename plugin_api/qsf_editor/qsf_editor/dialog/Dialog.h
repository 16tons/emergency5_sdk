// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/dialog/DialogManager.h"

#include <qsf/plugin/pluginable/Pluginable.h>

#include <QtWidgets/qdialog.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
QT_BEGIN_NAMESPACE
	class QSettings;
QT_END_NAMESPACE
namespace qsf
{
	namespace editor
	{
		class MainWindow;
		class Application;
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
		*    Abstract dialog class
		*
		*  @note
		*    - All available dialog classes are instanced at once by the dialog manager to be
		*      as flexible as possible
		*    - It's a good idea to perform lazy evaluation in a concrete dialog implementation:
		*      If the dialog is shown the first time, create its content
		*/
		class QSF_EDITOR_API_EXPORT Dialog : public Pluginable<DialogManager>, public QDialog
		{


		//[-------------------------------------------------------]
		//[ Friends                                               ]
		//[-------------------------------------------------------]
		friend class DialogManager;


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Destructor
			*/
			inline virtual ~Dialog();

			/**
			*  @brief
			*    Return the QSF editor application
			*
			*  @return
			*    The QSF editor application, do no destroy the returned instance
			*
			*  @note
			*    - Ease-of-use method
			*/
			Application& getApplication() const;

			/**
			*  @brief
			*    Return the main window
			*
			*  @return
			*    The main window, do no destroy the returned instance
			*
			*  @note
			*    - Ease-of-use method
			*/
			MainWindow& getMainWindow() const;


		//[-------------------------------------------------------]
		//[ Public virtual QWidget methods                        ]
		//[-------------------------------------------------------]
		public:
			virtual void setVisible(bool visible) override;


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::editor::Dialog methods         ]
		//[-------------------------------------------------------]
		protected:
			/**
			*  @brief
			*    Read settings by using Qt's "QSettings"
			*
			*  @param[in] qSettings
			*    Qt settings instance to use
			*
			*  @note
			*    - The default implementation is empty
			*/
			inline virtual void readSettings(QSettings& qSettings);

			/**
			*  @brief
			*    Write settings by using Qt's "QSettings"
			*
			*  @param[in] qSettings
			*    Qt settings instance to use
			*
			*  @note
			*    - The default implementation is empty
			*/
			inline virtual void writeSettings(QSettings& qSettings);

			/**
			*  @brief
			*    Retranslate the UI
			*
			*  @note
			*    - This method is only called in case the dialog is visible
			*    - The default implementation is empty
			*/
			inline virtual void retranslateUi();

			/**
			*  @brief
			*    Filter method called before "qsf::editor::Dialog::changeVisibility()" is called
			*
			*  @param[in] visible
			*    The requested visibility state
			*
			*  @return
			*    The final new visibility state, the dialog method is allowed to e.g. reject the new requested visibility state
			*
			*  @note
			*    - Unlike "QWidget::setVisible()" this method is only called if there was a real visibility state change
			*    - The default implementation is empty
			*    - By using this method, the dialog implementation has the opportunity to reject becoming visible, e.g. because it's unable to acquire a required entity lock
			*/
			inline virtual bool preChangeVisibility(bool visible);

			/**
			*  @brief
			*    Change visibility
			*
			*  @param[in] visible
			*    The new visibility state
			*
			*  @note
			*    - Unlike "QWidget::setVisible()" this method is only called if there was a real visibility state change
			*    - The default implementation is empty
			*/
			inline virtual void changeVisibility(bool visible);


		//[-------------------------------------------------------]
		//[ Protected methods                                     ]
		//[-------------------------------------------------------]
		protected:
			/**
			*  @brief
			*    Constructor
			*
			*  @param[in] dialogManager
			*    The dialog manager this dialog instance is in, no null pointer allowed (the CAMP reflection system does not like references in constructors, so we have to use a pointer instead)
			*  @param[in] qWidgetParent
			*    Pointer to parent Qt widget, can be a null pointer (in this case you're responsible for destroying this dialog instance)
			*/
			Dialog(DialogManager* dialogManager, QWidget* qWidgetParent);


		//[-------------------------------------------------------]
		//[ Protected virtual QWidget methods                     ]
		//[-------------------------------------------------------]
		protected:
			virtual void changeEvent(QEvent* qEvent) override;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			bool mVisible;	///< Current QWidget visibility


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
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf_editor/dialog/Dialog-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::Dialog)

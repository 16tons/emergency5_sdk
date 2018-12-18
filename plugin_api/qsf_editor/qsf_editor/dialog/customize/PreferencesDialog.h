// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/dialog/Dialog.h"

#include <boost/property_tree/ptree.hpp>

#include <vector>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Ui
{
	class PreferencesDialog;
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
		*    QSF editor preferences dialog
		*/
		class PreferencesDialog : public Dialog
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
			static const uint32 PLUGINABLE_ID;	///< "qsf::editor::PreferencesDialog" unique pluginable dialog ID

			/**
			*  @brief
			*    Dialog view mode
			*/
			enum ViewMode
			{
				SETTINGS_MODE = 0,
				DEBUG_MODE
			};


		//[-------------------------------------------------------]
		//[ Public static methods                                 ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Fill the given language combo box
			*
			*  @param[in] qComboBox
			*    Language combo box to fill
			*/
			static void fillLanguageQComboBox(QComboBox& qComboBox);


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
			PreferencesDialog(DialogManager* dialogManager, QWidget* qWidgetParent);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~PreferencesDialog();

			/**
			*  @brief
			*    Set preferences dialog view mode
			*
			*  @param[in] viewMode
			*    View mode to set
			*/
			void setViewMode(ViewMode viewMode);


		//[-------------------------------------------------------]
		//[ Public virtual QDialog methods                        ]
		//[-------------------------------------------------------]
		public:
			virtual void done(int result) override;


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
			/**
			*  @brief
			*    Destroy the tabs
			*/
			void createTabs() const;

			/**
			*  @brief
			*    Create the tabs
			*/
			void destroyTabs() const;

			/**
			*  @brief
			*    Backup the current settings
			*/
			void backupSettings();

			/**
			*  @brief
			*    Revert all changes made to the settings
			*/
			void revertAllChanges() const;

			/**
			*  @brief
			*    Reset all settings to the default values ("Reset to Factory Settings")
			*/
			void resetToDefaultSettings();


		//[-------------------------------------------------------]
		//[ Private Qt slots (MOC)                                ]
		//[-------------------------------------------------------]
		private Q_SLOTS:
			void onCurrentIndexChangedLanguage(int index);
			void onButtonOK(bool checked);
			void onButtonReset(bool checked);
			void onButtonCancel(bool checked);
			void onCurrentIndexChangedViewMode(int index);
			void onStateChangedShowHidden(int);
			void onClickedOpenSettingsDirectory(bool checked);


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			Ui::PreferencesDialog*					 mUiPreferencesDialog;	///< UI preferences dialog instance, can be a null pointer, we have to destroy the instance in case we no longer need it
			std::vector<boost::property_tree::ptree> mPTreeSettings;		///< Boost ptree instance, used to store a copy of the settings
			std::vector<boost::property_tree::ptree> mPTreeDebug;			///< Boost ptree instance, used to store a copy of the debug options


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
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::PreferencesDialog)

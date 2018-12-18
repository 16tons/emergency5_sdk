// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/dialog/Dialog.h"
#include "qsf_editor/dialog/sequence/SequenceMapBackup.h"

#include <qsf_game/environment/weather/WeatherState.h>
#include <qsf_game/environment/weather/WeatherConfiguration.h>

#include <qsf/asset/AssetProxy.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Ui
{
	class WeatherDialog;
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
		*    QSF editor weather dialog
		*/
		class WeatherDialog : public Dialog
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
			static const uint32 PLUGINABLE_ID;	///< "qsf::editor::WeatherDialog" unique pluginable dialog ID


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
			WeatherDialog(DialogManager* dialogManager, QWidget* qWidgetParent);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~WeatherDialog();


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
		//[ Private Qt slots (MOC)                                ]
		//[-------------------------------------------------------]
		private Q_SLOTS:
			void onCurrentChanged(int);
			// Weather states
			void onSelectWeatherSet(int);
			void onCreateNewWeatherSet(bool);
			void onDeleteWeatherSet(bool);
			void onWeatherSetPropertyChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight, const QVector<int>& roles);
			void onPreviewWeatherSet(bool);
			void onButtonApplyWeatherSet(bool);
			void onButtonOkWeatherSet(bool);
			void onButtonCancelWeatherSet(bool);
			// Weather configurations
			void onSelectWeatherConfigurations(int);
			void onCreateNewWeatherConfigurations(bool);
			void onDeleteWeatherConfigurations(bool);
			void onButtonApplyWeatherConfigurations(bool);
			void onButtonOkWeatherConfigurations(bool);
			void onButtonCancelWeatherConfigurations(bool);
			// qsf::editor::Application
			void onPostMapRebuild();


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			void updateTabByIndex(int index);

			// Weather states
			const game::WeatherState& getDefaultWeatherState() const;
			void updateTabWeatherStates();
			void fillComboBoxWeatherStates();
			void showWeatherState(game::WeatherState* weather);

			// Weather configurations
			void updateTabWeatherConfigurations();
			void fillComboBoxWeatherConfigurations();
			int getWeatherTransitionRow(uint32 id) const;
			void initializeWeatherTransitions();
			void showWeatherTransitions();
			void saveWeatherConfigurations();


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			Ui::WeatherDialog*  mUiWeatherDialog;		///< UI weather dialog instance, can be a null pointer, we have to destroy the instance in case we no longer need it
			bool				mUpdatingUi;

			// Weather states and configurations
			std::vector<uint32> mNewWeatherStates;			///< New created weather state IDs
			std::vector<uint32> mNewWeatherConfigurations;	///< New created weather configuration IDs
			game::WeatherState  mFallbackDefaultWeatherState;
			std::string			mDefaultWeatherStateSourceFile;
			std::string			mDefaultWeatherConfigurationSourceFile;


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
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::WeatherDialog)

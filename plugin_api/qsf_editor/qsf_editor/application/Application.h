// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/Export.h"

#include <qsf_editor_base/application/Application.h>

#include <qsf/asset/AssetSystemTypes.h>

#include <QtCore/qobject.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
QT_BEGIN_NAMESPACE
	class QSettings;
	class QApplication;
	class QProgressBar;
QT_END_NAMESPACE
namespace qsf
{
	class Clock;
	class GuiContext;
	namespace editor
	{
		class MainWindow;
		class CameraManager;
		class NetworkManager;
		class QActionsManager;
		class SelectionSystem;
		class AssetImportManager;
		class SimulationController;
		class DynamicEnumManager;
		class ValueConverterManager;
		class EditorListenerManager;
		class AssetPriorityProvider;
		class MaterialEditorManager;
		class TerrainEditManager;
		class BatchJobManager;
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
		*    QSF editor application implementation
		*
		*  @note
		*    - Qt has the full control over the main loop, so, we have to use a Qt timer to perform a frequent QSF update
		*/
		class QSF_EDITOR_API_EXPORT Application : public QObject, public base::Application
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
			static const std::string DEFAULT_LAYOUT_FILENAME;	///< Filename of the default editor layout
			static const uint32 APPLICATION_ID;					///< "qsf::editor::Application" unique ID

			/**
			*  @brief
			*    Application stage
			*/
			enum Stage
			{
				STARTUP_STAGE = 42,	///< Stage 0: Application is in the startup process
				CONNECT_STAGE,		///< Stage 1: Connect to server ("qsf::editor::ConnectDialog"-dialog, don't start with 0 in order to avoid potential conflicts with "QDialog::Accepted"/"QDialog::Rejected" return codes)
				LOGIN_STAGE,		///< Stage 2: Login to connected server ("qsf::editor::LoginDialog"-dialog)
				PROJECT_MAP_STAGE,	///< Stage 3: Open project and map ("qsf::editor::SelectProjectMapDialog"-dialog)
				MAIN_STAGE,			///< Stage 4: Run main application ("qsf::editor::MainWindow" main window)
				EXIT_STAGE			///< Stage 5: Exit application
			};


		//[-------------------------------------------------------]
		//[ Public Qt signals (MOC)                               ]
		//[-------------------------------------------------------]
		Q_SIGNALS:
			/**
			*  @brief
			*    Qt signal emitted before a map is about to be rebuild
			*
			*  @note
			*    - Do only use "qsf::editor::Application::emitPreMapRebuildSignal()" to emit this signal
			*/
			void preMapRebuild();

			/**
			*  @brief
			*    Qt signal emitted after a map has been rebuild
			*
			*  @note
			*    - Do only use "qsf::editor::Application::emitPostMapRebuildSignal()" to emit this signal
			*/
			void postMapRebuild();


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Default constructor
			*/
			Application();

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~Application();

			//[-------------------------------------------------------]
			//[ Stage independent                                     ]
			//[-------------------------------------------------------]
			/**
			*  @brief
			*    Return the Qt settings instance
			*
			*  @return
			*    The Qt settings instance, do not destroy the returned instance
			*
			*  @note
			*  - By default, Qt is using "QSettings::NativeFormat" and hence e.g. on Windows is storing settings within the registry
			*  - We don't want settings within the registry, we use the application data directory instead so everything like also the log is at one and the same place
			*/
			inline QSettings& getQSettings() const;

			/**
			*  @brief
			*    Save editor settings
			*/
			void saveSettings();

			/**
			*  @brief
			*    Automatic forced reconnect during the next application stage hopping
			*/
			void automaticForcedReconnect();

			/**
			*  @brief
			*    Emit the Qt pre map rebuild signal
			*/
			void emitPreMapRebuildSignal();

			/**
			*  @brief
			*    Map loading progress callback
			*
			*  @param[in] progress
			*    Map loading progress callback (0...1 -> start...finished)
			*
			*  @note
			*    - Call this whenever you want to tell the application about a map progress
			*    - Ensure you call the method with progress >= 1 to signal the completion on the progress
			*/
			void onMapProgress(float progress);

			/**
			*  @brief
			*    Emit the Qt post map rebuild signal
			*/
			void emitPostMapRebuildSignal();

			/**
			*  @brief
			*    Return the animation clock instance
			*
			*  @return
			*    The animation clock instance, can be a null pointer, do no destroy the returned instance
			*/
			inline Clock* getAnimationClock() const;

			/**
			*  @brief
			*    Return the simulation clock instance
			*
			*  @return
			*    The simulation clock instance, can be a null pointer, do no destroy the returned instance
			*/
			inline Clock* getSimulationClock() const;

			//[-------------------------------------------------------]
			//[ Stage dependent                                       ]
			//[-------------------------------------------------------]
			/**
			*  @brief
			*    Return the QSF editor application stage
			*
			*  @return
			*    The QSF editor application stage
			*/
			inline Stage getStage() const;

			/**
			*  @brief
			*    Return the network manager instance
			*
			*  @return
			*    Reference to the network manager, do not destroy the instance
			*/
			inline NetworkManager& getNetworkManager() const;

			/**
			*  @brief
			*    Return the QActions manager instance
			*
			*  @return
			*    The QActions manager instance, do no destroy the returned instance
			*/
			inline QActionsManager& getQActionsManager() const;

			/**
			*  @brief
			*    Return the selection system instance
			*
			*  @return
			*    The selection system instance, do no destroy the returned instance
			*/
			inline SelectionSystem& getSelectionSystem() const;

			/**
			*  @brief
			*    Return the GUI context instance
			*
			*  @return
			*    The GUI context instance, do no destroy the returned instance
			*/
			inline GuiContext& getGuiContext() const;

			/**
			*  @brief
			*    Return the simulation controller instance
			*
			*  @return
			*    The simulation controller instance, do no destroy the returned instance
			*/
			inline SimulationController& getSimulationController() const;

			/**
			*  @brief
			*    Return the camera manager instance
			*
			*  @return
			*    The camera manager instance, do no destroy the returned instance
			*/
			inline CameraManager& getCameraManager() const;

			/**
			*  @brief
			*    Return the asset import manager instance
			*
			*  @return
			*    The asset import manager instance, do no destroy the returned instance
			*/
			inline AssetImportManager& getAssetImportManager() const;

			/**
			*  @brief
			*    Return the dynamic enum manager instance
			*
			*  @return
			*    The dynamic enum manager instance, do no destroy the returned instance
			*/
			inline DynamicEnumManager& getDynamicEnumManager() const;

			/**
			*  @brief
			*    Return the value converter manager instance
			*
			*  @return
			*    The value converter manager instance, do no destroy the returned instance
			*/
			inline ValueConverterManager& getValueConverterManager() const;

			/**
			*  @brief
			*    Return the editor listener manager instance
			*
			*  @return
			*    The editor listener manager instance, do no destroy the returned instance
			*/
			inline EditorListenerManager& getEditorListenerManager() const;

			/**
			*  @brief
			*    Return the material editor manager instance
			*
			*  @return
			*    The material editor instance, do no destroy the returned instance
			*/
			inline MaterialEditorManager& getMaterialEditorManager() const;

			/**
			*  @brief
			*    Return the terrain edit manager instance
			*
			*  @return
			*    Terrain edit manager instance, do not destroy the instance
			*/
			inline TerrainEditManager& getTerrainEditManager() const;

			/**
			*  @brief
			*    Return the batch job manager instance
			*
			*  @return
			*    Batch job manager instance, do not destroy the instance
			*/
			inline BatchJobManager& getBatchJobManager() const;

			/**
			*  @brief
			*    Return the main window instance
			*
			*  @return
			*    The main window instance, can be a null pointer, do not destroy the returned instance
			*/
			inline MainWindow* getMainWindow() const;

			/**
			*  @brief
			*    Return the update interval
			*
			*  @return
			*    The update interval in milliseconds
			*/
			inline int getUpdateInterval() const;

			/**
			*  @brief
			*    Set the update interval
			*
			*  @param[in] updateInterval
			*    Update interval in milliseconds, "0" to disable updates
			*
			*  @note
			*    - By default an update interval of 10 milliseconds resulting in 100 FPS is set
			*/
			void setUpdateInterval(int updateInterval);

			/**
			*  @brief
			*    Enable or disable the update timer
			*
			*  @param[in] enabled
			*    "true" if timer should be enabled (the default), "false" to disable it
			*/
			void setUpdateTimerEnabled(bool enabled);

			/**
			*  @brief
			*    Create a backup of the main map
			*
			*  @note
			*    - The map backup will be stored in the QSF editor plugin, so it survives application switches
			*/
			void storeMapBackup();

			/**
			*  @brief
			*    Restore the last created main map backup
			*
			*  @return
			*    "true" in case a map backup has been restored, else "false"
			*
			*  @note
			*    - The map backup inside the QSF editor plugin will be restored
			*    - If the backup is empty (the default case), nothing is done; after load, the backup gets cleared
			*/
			bool restoreMapBackup();


		//[-------------------------------------------------------]
		//[ Public virtual qsf::Application methods               ]
		//[-------------------------------------------------------]
		public:
			virtual void exit() override;
			virtual uint32 getId() override;


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::Application methods            ]
		//[-------------------------------------------------------]
		protected:
			//[-------------------------------------------------------]
			//[ Lifecycle                                             ]
			//[-------------------------------------------------------]
			virtual bool onStartup() override;
			virtual int onRun() override;
			virtual void onShutdown() override;


		//[-------------------------------------------------------]
		//[ Protected virtual QObject methods                     ]
		//[-------------------------------------------------------]
		protected:
			virtual void timerEvent(QTimerEvent* qTimerEvent) override;


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			/**
			*  @brief
			*    Editor application stage hopping
			*
			*  @param[in] qApplication
			*    Qt application instance to use
			*/
			void applicationStageHopping(QApplication& qApplication);

			/**
			*  @brief
			*    Run main application stage
			*
			*  @param[in] qApplication
			*    Qt application instance to use
			*
			*  @return
			*    Application stage to switch into
			*/
			Stage runMainStage(QApplication& qApplication);


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			// Stage independent
			QSettings*	  mQSettings;									///< Qt settings instance, always valid, we have to destroy the instance in case we no longer need it
			QProgressBar* mQProgressBar;								///< Qt progress bar instance, can be a null pointer, Qt will destroy the instance automatically
			Clock*		  mAnimationClock;								///< Animation clock; always valid, we have to destroy the instance in case we no longer need it
			Clock*		  mSimulationClock;								///< Simulation clock; always valid, we have to destroy the instance in case we no longer need it
			bool		  mForceConnectAutomatically;					///< Automatic forced reconnect during the next application stage hopping
			bool		  mForceLoginAutomatically;						///< Automatic forced reconnect during the next application stage hopping
			bool		  mForceOpenSelectedProjectMapAutomatically;	///< Automatic forced reconnect during the next application stage hopping
			// Stage dependent
			Stage				   mStage;					///< The current editor application stage
			GlobalAssetId		   mGlobalMapAssetId;		///< The global asset ID of the map selected in the select project map dialog
			NetworkManager*		   mNetworkManager;			///< Network manager instance, always valid, we have to destroy the instance in case we no longer need it
			QActionsManager*	   mQActionsManager;		///< QActions manager instance, always valid while the application is running, we're responsible for destroying the instance in case we no longer need it
			SelectionSystem*	   mSelectionSystem;		///< Selection system instance, always valid while the application is running, we're responsible for destroying the instance in case we no longer need it
			GuiContext*			   mGuiContext;				///< QSF GUI context instance, always valid, we're responsible for destroying the instance in case we no longer need it
			MainWindow*			   mMainWindow;				///< Main window instance, can be a null pointer, we have to destroy the instance in case we no longer need it
			int					   mUpdateInterval;			///< Update interval in milliseconds
			int					   mUpdateQtTimerId;		///< Update Qt timer ID, "0" means no timer
			bool				   mUpdateQtTimerEnabled;	///< Usually "true", except in cases where we need to temporarily disable timer updates
			SimulationController*  mSimulationController;	///< Simulation controller instance, can be a null pointer, we have to destroy the instance in case we no longer need it
			CameraManager*		   mCameraManager;			///< Camera manager instance, can be a null pointer, we have to destroy the instance in case we no longer need it
			AssetImportManager*    mAssetImportManager;		///< Asset import manager instance, can be a null pointer, we have to destroy the instance in case we no longer need it
			DynamicEnumManager*    mDynamicEnumManager;		///< Dynamic enum manager instance, can be a null pointer, we have to destroy the instance in case we no longer need it
			ValueConverterManager* mValueConverterManager;	///< Value converter manager instance, can be a null pointer, we have to destroy the instance in case we no longer need it
			EditorListenerManager* mEditorListenerManager;	///< Editor listener manager instance, can be a null pointer, we have to destroy the instance in case we no longer need it
			AssetPriorityProvider* mAssetPriorityProvider;	///< Asset priority provider instance, can be a null pointer, we have to destroy the instance in case we no longer need it
			MaterialEditorManager* mMaterialEditorManager;	///< Material editor manager instance, can be a null pointer, we have to destroy the instance in case we no longer need it
			TerrainEditManager*	   mTerrainEditManager;		///< Terrain edit manager instance, always valid while the application is running, we have to destroy the instance in case we no longer need it
			BatchJobManager*	   mBatchJobManager;		///< Batch job manager instance, always valid while the application is running, we have to destroy the instance in case we no longer need it


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
#include "qsf_editor/application/Application-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::Application)

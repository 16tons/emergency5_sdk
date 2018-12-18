// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/base/error/BoostAssertHandler.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace editor
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		inline QSettings& Application::getQSettings() const
		{
			// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
			QSF_ASSERT(nullptr != mQSettings, "QSF editor: The Qt settings instance is invalid", QSF_REACT_THROW);
			return *mQSettings;
		}

		inline Clock* Application::getAnimationClock() const
		{
			return mAnimationClock;
		}

		inline Clock* Application::getSimulationClock() const
		{
			return mSimulationClock;
		}

		inline Application::Stage Application::getStage() const
		{
			return mStage;
		}

		inline NetworkManager& Application::getNetworkManager() const
		{
			// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
			QSF_ASSERT(nullptr != mNetworkManager, "The network manager instance is invalid", QSF_REACT_THROW);
			return *mNetworkManager;
		}

		inline QActionsManager& Application::getQActionsManager() const
		{
			// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
			QSF_ASSERT(nullptr != mQActionsManager, "QSF editor: The Qt actions manager instance is invalid", QSF_REACT_THROW);
			return *mQActionsManager;
		}

		inline SelectionSystem& Application::getSelectionSystem() const
		{
			// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
			QSF_ASSERT(nullptr != mSelectionSystem, "QSF editor: The selection system instance is invalid", QSF_REACT_THROW);
			return *mSelectionSystem;
		}

		inline GuiContext& Application::getGuiContext() const
		{
			// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
			QSF_ASSERT(nullptr != mGuiContext, "QSF editor: The GUI context instance is invalid", QSF_REACT_THROW);
			return *mGuiContext;
		}

		inline SimulationController& Application::getSimulationController() const
		{
			// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
			QSF_ASSERT(nullptr != mSimulationController, "QSF editor: The simulation controller instance is invalid", QSF_REACT_THROW);
			return *mSimulationController;
		}

		inline CameraManager& Application::getCameraManager() const
		{
			// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
			QSF_ASSERT(nullptr != mCameraManager, "QSF editor: The camera manager instance is invalid", QSF_REACT_THROW);
			return *mCameraManager;
		}

		inline AssetImportManager& Application::getAssetImportManager() const
		{
			// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
			QSF_ASSERT(nullptr != mAssetImportManager, "QSF editor: The asset import manager instance is invalid", QSF_REACT_THROW);
			return *mAssetImportManager;
		}

		inline DynamicEnumManager& Application::getDynamicEnumManager() const
		{
			// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
			QSF_ASSERT(nullptr != mDynamicEnumManager, "QSF editor: The dynamic enum manager instance is invalid", QSF_REACT_THROW);
			return *mDynamicEnumManager;
		}

		inline ValueConverterManager& Application::getValueConverterManager() const
		{
			// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
			QSF_ASSERT(nullptr != mValueConverterManager, "QSF editor: The value converter manager instance is invalid", QSF_REACT_THROW);
			return *mValueConverterManager;
		}

		inline EditorListenerManager& Application::getEditorListenerManager() const
		{
			// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
			QSF_ASSERT(nullptr != mEditorListenerManager, "QSF editor: The editor listener manager instance is invalid", QSF_REACT_THROW);
			return *mEditorListenerManager;
		}

		inline MaterialEditorManager& Application::getMaterialEditorManager() const
		{
			// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
			QSF_ASSERT(nullptr != mMaterialEditorManager, "QSF editor: The material editor manager instance is invalid", QSF_REACT_THROW);
			return *mMaterialEditorManager;
		}

		inline TerrainEditManager& Application::getTerrainEditManager() const
		{
			// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
			QSF_ASSERT(nullptr != mTerrainEditManager, "QSF editor: The terrain edit manager instance is invalid", QSF_REACT_THROW);
			return *mTerrainEditManager;
		}

		inline BatchJobManager& Application::getBatchJobManager() const
		{
			// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
			QSF_ASSERT(nullptr != mBatchJobManager, "QSF editor: The batch job manager instance is invalid", QSF_REACT_THROW);
			return *mBatchJobManager;
		}

		inline MainWindow *Application::getMainWindow() const
		{
			return mMainWindow;
		}

		inline int Application::getUpdateInterval() const
		{
			return mUpdateQtTimerId;
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf

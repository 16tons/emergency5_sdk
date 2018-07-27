// Copyright (C) 2012-2017 Promotion Software GmbH


// These helper macros only exist to make the daily life more comfortable


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/application/MainWindow.h"
#include "qsf_editor/application/Application.h"
#include "qsf_editor/operation/OperationManager.h"
#include "qsf_editor/qaction/QActions.h"
#include "qsf_editor/qaction/QActionsManager.h"
#include "qsf_editor/tool/Tool.h"
#include "qsf_editor/tool/ToolManager.h"
#include "qsf_editor/toolbar/ToolBar.h"
#include "qsf_editor/toolbar/ToolBarManager.h"
#include "qsf_editor/statusbar/StatusBar.h"
#include "qsf_editor/statusbar/StatusBarManager.h"
#include "qsf_editor/editmode/EditMode.h"
#include "qsf_editor/editmode/EditModeManager.h"
#include "qsf_editor/dialog/Dialog.h"
#include "qsf_editor/dialog/DialogManager.h"
#include "qsf_editor/view/View.h"
#include "qsf_editor/view/ViewManager.h"
#include "qsf_editor/selection/SelectionSystem.h"
#include "qsf_editor/network/NetworkManager.h"

#include <qsf/Qsf.h>


//[-------------------------------------------------------]
//[ Macros                                                ]
//[-------------------------------------------------------]
/**
*  @brief
*    Ease of use macro returning a reference to the running QSF editor application instance
*
*  @return
*    Reference to the running QSF editor application instance, do no destroy the returned instance
*
*  @note
*    - When this macro is used, it's usually safe to consider that the application pointer
*      is not null and this way this macro is in its usage identical to e.g. "QSF_LOG"
*    - The application instance pointer ("qsf::Qsf::instance()->getApplication()") is set
*      and also unset within the "qsf::Qsf::run()"-method
*/
#define QSF_EDITOR_APPLICATION static_cast<qsf::editor::Application&>(*qsf::Qsf::instance()->getApplication())

/**
*  @brief
*    Macro for checking whether or not the editor is currently up-and-running
*
*  @return
*    "true" if the editor is currently up-and-running, else "false"
*
*  @note
*    - There are only a few special use-cases for this macro, do not overuse it
*/
#define QSF_EDITOR_IS_RUNNING (nullptr != dynamic_cast<qsf::editor::Application*>(qsf::Qsf::instance()->getApplication()))


//[-------------------------------------------------------]
//[ Macros for "qsf::editor::Application" managed stuff   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Ease of use macro returning a reference to the running QSF editor operation manager instance
*
*  @return
*    Reference to the running QSF editor operation manager instance, do no destroy the returned instance
*
*  @note
*    - When this macro is used, it's usually safe to consider that the application pointer
*      is not null and this way this macro is in its usage identical to e.g. "QSF_LOG"
*    - The application instance pointer ("qsf::Qsf::instance()->getApplication()") is set
*      and also unset within the "qsf::Qsf::run()"-method
*/
#define QSF_EDITOR_OPERATION static_cast<qsf::editor::OperationManager&>(static_cast<qsf::editor::Application&>(*qsf::Qsf::instance()->getApplication()).getOperationManager())

/**
*  @brief
*    Ease of use macro returning a reference to the running QSF editor asset storage manager instance
*
*  @return
*    Reference to the running QSF editor asset storage manager instance, do no destroy the returned instance
*
*  @note
*    - When this macro is used, it's usually safe to consider that the application pointer
*      is not null and this way this macro is in its usage identical to e.g. "QSF_LOG"
*    - The application instance pointer ("qsf::Qsf::instance()->getApplication()") is set
*      and also unset within the "qsf::Qsf::run()"-method
*/
#define QSF_EDITOR_ASSET_STORAGE static_cast<qsf::editor::AssetStorageManager&>(static_cast<qsf::editor::Application&>(*qsf::Qsf::instance()->getApplication()).getAssetStorageManager())

/**
*  @brief
*    Ease of use macro returning a reference to the running QSF editor asset preview cache instance
*
*  @return
*    Reference to the running QSF editor asset preview instance, do no destroy the returned instance
*
*  @note
*    - When this macro is used, it's usually safe to consider that the application pointer
*      is not null and this way this macro is in its usage identical to e.g. "QSF_LOG"
*    - The application instance pointer ("qsf::Qsf::instance()->getApplication()") is set
*      and also unset within the "qsf::Qsf::run()"-method
*/
#define QSF_EDITOR_ASSET_PREVIEW static_cast<qsf::editor::AssetPreviewCache&>(static_cast<qsf::editor::Application&>(*qsf::Qsf::instance()->getApplication()).getMainWindow()->getAssetPreviewCache())

/**
*  @brief
*    Ease of use macro returning a reference to the running QSF editor QSettings instance
*
*  @return
*    Reference to the running QSF editor QSettings instance, do no destroy the returned instance
*
*  @note
*    - When this macro is used, it's usually safe to consider that the application pointer
*      is not null and this way this macro is in its usage identical to e.g. "QSF_LOG"
*    - The application instance pointer ("qsf::Qsf::instance()->getApplication()") is set
*      and also unset within the "qsf::Qsf::run()"-method
*/
#define QSF_EDITOR_QSETTINGS static_cast<qsf::editor::Application&>(*qsf::Qsf::instance()->getApplication()).getQSettings()

/**
*  @brief
*    Ease of use macro returning a reference to the QSF editor network manager instance of the running QSF editor application instance
*
*  @return
*    Reference to the QSF editor network manager instance of the running QSF editor application instance, do no destroy the returned instance
*
*  @note
*    - When this macro is used, it's usually safe to consider that the application pointer and the
*      main window is not null and this way this macro is in its usage identical to e.g. "QSF_LOG"
*    - The application instance pointer ("qsf::Qsf::instance()->getApplication()") is set
*      and also unset within the "qsf::Qsf::run()"-method
*/
#define QSF_EDITOR_NETWORK_MANAGER static_cast<qsf::editor::Application&>(*qsf::Qsf::instance()->getApplication()).getNetworkManager()

/**
*  @brief
*    Ease of use macro returning a reference to the QSF editor material editor manager instance of the running QSF editor application instance
*
*  @return
*    Reference to the QSF editor material editor manager instance of the running QSF editor application instance, do no destroy the returned instance
*
*  @note
*    - When this macro is used, it's usually safe to consider that the application pointer and the
*      main window is not null and this way this macro is in its usage identical to e.g. "QSF_LOG"
*    - The application instance pointer ("qsf::Qsf::instance()->getApplication()") is set
*      and also unset within the "qsf::Qsf::run()"-method
*/
#define QSF_EDITOR_MATERIAL_EDITOR_MANAGER static_cast<qsf::editor::Application&>(*qsf::Qsf::instance()->getApplication()).getMaterialEditorManager()

/**
*  @brief
*    Ease of use macro returning a reference to the QSF editor QActions manager instance of the running QSF editor application instance
*
*  @return
*    Reference to the QSF editor QActions manager instance of the running QSF editor application instance, do no destroy the returned instance
*
*  @note
*    - When this macro is used, it's usually safe to consider that the application pointer
*      is not null and this way this macro is in its usage identical to e.g. "QSF_LOG"
*    - The application instance pointer ("qsf::Qsf::instance()->getApplication()") is set
*      and also unset within the "qsf::Qsf::run()"-method
*/
#define QSF_EDITOR_QACTIONS_MANAGER static_cast<qsf::editor::Application&>(*qsf::Qsf::instance()->getApplication()).getQActionsManager()

/**
*  @brief
*    Ease of use macro returning a reference to the QSF editor selection system instance of the running QSF editor application instance
*
*  @return
*    Reference to the QSF editor selection system instance of the running QSF editor application instance, do no destroy the returned instance
*
*  @note
*    - When this macro is used, it's usually safe to consider that the application pointer and the
*      main window is not null and this way this macro is in its usage identical to e.g. "QSF_LOG"
*    - The application instance pointer ("qsf::Qsf::instance()->getApplication()") is set
*      and also unset within the "qsf::Qsf::run()"-method
*/
#define QSF_EDITOR_SELECTION_SYSTEM static_cast<qsf::editor::Application&>(*qsf::Qsf::instance()->getApplication()).getSelectionSystem()

/**
*  @brief
*    Ease of use macro returning a reference to the QSF editor simulation controller instance of the running QSF editor application instance
*
*  @return
*    Reference to the QSF editor simulation controller instance of the running QSF editor application instance, do no destroy the returned instance
*
*  @note
*    - When this macro is used, it's usually safe to consider that the application pointer
*      is not null and this way this macro is in its usage identical to e.g. "QSF_LOG"
*    - The application instance pointer ("qsf::Qsf::instance()->getApplication()") is set
*      and also unset within the "qsf::Qsf::run()"-method
*/
#define QSF_EDITOR_SIMULATION_CONTROLLER static_cast<qsf::editor::Application&>(*qsf::Qsf::instance()->getApplication()).getSimulationController()

/**
*  @brief
*    Ease of use macro returning a reference to the running QSF editor terrain edit manager instance
*
*  @return
*    Reference to the running QSF editor terrain edit manager instance, do no destroy the returned instance
*
*  @note
*    - When this macro is used, it's usually safe to consider that the application pointer
*      is not null and this way this macro is in its usage identical to e.g. "QSF_LOG"
*    - The application instance pointer ("qsf::Qsf::instance()->getApplication()") is set
*      and also unset within the "qsf::Qsf::run()"-method
*/
#define QSF_EDITOR_TERRAINEDIT static_cast<qsf::editor::Application&>(*qsf::Qsf::instance()->getApplication()).getTerrainEditManager()


//[-------------------------------------------------------]
//[ Macros for "qsf::editor::MainWindow" managed stuff    ]
//[-------------------------------------------------------]
/**
*  @brief
*    Ease of use macro returning a reference to the QSF editor dialog manager instance of the running QSF editor application instance
*
*  @return
*    Reference to the QSF editor dialog manager instance of the running QSF editor application instance, do no destroy the returned instance
*
*  @note
*    - When this macro is used, it's usually safe to consider that the application pointer and the
*      main window is not null and this way this macro is in its usage identical to e.g. "QSF_LOG"
*    - The application instance pointer ("qsf::Qsf::instance()->getApplication()") is set
*      and also unset within the "qsf::Qsf::run()"-method
*/
#define QSF_EDITOR_DIALOG_MANAGER static_cast<qsf::editor::Application&>(*qsf::Qsf::instance()->getApplication()).getMainWindow()->getDialogManager()

/**
*  @brief
*    Ease of use macro returning a reference to the QSF editor view manager instance of the running QSF editor application instance
*
*  @return
*    Reference to the QSF editor view manager instance of the running QSF editor application instance, do no destroy the returned instance
*
*  @note
*    - When this macro is used, it's usually safe to consider that the application pointer and the
*      main window is not null and this way this macro is in its usage identical to e.g. "QSF_LOG"
*    - The application instance pointer ("qsf::Qsf::instance()->getApplication()") is set
*      and also unset within the "qsf::Qsf::run()"-method
*/
#define QSF_EDITOR_VIEW_MANAGER static_cast<qsf::editor::Application&>(*qsf::Qsf::instance()->getApplication()).getMainWindow()->getViewManager()

/**
*  @brief
*    Ease of use macro returning a reference to the QSF editor tool bar manager instance of the running QSF editor application instance
*
*  @return
*    Reference to the QSF editor tool bar manager instance of the running QSF editor application instance, do no destroy the returned instance
*
*  @note
*    - When this macro is used, it's usually safe to consider that the application pointer and the
*      main window is not null and this way this macro is in its usage identical to e.g. "QSF_LOG"
*    - The application instance pointer ("qsf::Qsf::instance()->getApplication()") is set
*      and also unset within the "qsf::Qsf::run()"-method
*/
#define QSF_EDITOR_TOOLBAR_MANAGER static_cast<qsf::editor::Application&>(*qsf::Qsf::instance()->getApplication()).getMainWindow()->getToolBarManager()

/**
*  @brief
*    Ease of use macro returning a reference to the QSF editor status bar manager instance of the running QSF editor application instance
*
*  @return
*    Reference to the QSF editor status bar manager instance of the running QSF editor application instance, do no destroy the returned instance
*
*  @note
*    - When this macro is used, it's usually safe to consider that the application pointer and the
*      main window is not null and this way this macro is in its usage identical to e.g. "QSF_LOG"
*    - The application instance pointer ("qsf::Qsf::instance()->getApplication()") is set
*      and also unset within the "qsf::Qsf::run()"-method
*/
#define QSF_EDITOR_STATUSBAR_MANAGER static_cast<qsf::editor::Application&>(*qsf::Qsf::instance()->getApplication()).getMainWindow()->getStatusBarManager()

/**
*  @brief
*    Ease of use macro returning a reference to the QSF editor tool manager instance of the running QSF editor application instance
*
*  @return
*    Reference to the QSF editor tool manager instance of the running QSF editor application instance, do no destroy the returned instance
*
*  @note
*    - When this macro is used, it's usually safe to consider that the application pointer and the
*      main window is not null and this way this macro is in its usage identical to e.g. "QSF_LOG"
*    - The application instance pointer ("qsf::Qsf::instance()->getApplication()") is set
*      and also unset within the "qsf::Qsf::run()"-method
*/
#define QSF_EDITOR_TOOL_MANAGER static_cast<qsf::editor::Application&>(*qsf::Qsf::instance()->getApplication()).getMainWindow()->getToolManager()

/**
*  @brief
*    Ease of use macro returning a reference to the QSF editor edit mode manager instance of the running QSF editor application instance
*
*  @return
*    Reference to the QSF editor edit mode manager instance of the running QSF editor application instance, do no destroy the returned instance
*
*  @note
*    - When this macro is used, it's usually safe to consider that the application pointer and the
*      main window is not null and this way this macro is in its usage identical to e.g. "QSF_LOG"
*    - The application instance pointer ("qsf::Qsf::instance()->getApplication()") is set
*      and also unset within the "qsf::Qsf::run()"-method
*/
#define QSF_EDITOR_EDITMODE_MANAGER static_cast<qsf::editor::Application&>(*qsf::Qsf::instance()->getApplication()).getMainWindow()->getEditModeManager()

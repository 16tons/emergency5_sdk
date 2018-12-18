// Copyright (C) 2012-2018 Promotion Software GmbH


// These helper macros only exist to make the daily life more comfortable


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/Qsf.h"


//[-------------------------------------------------------]
//[ Macros for "qsf::Qsf" ease-of-use access              ]
//[-------------------------------------------------------]
/**
*  @brief
*    Ease of use macro returning a reference to the QSF worker system instance
*
*  @return
*    A reference to the QSF worker system instance ("qsf::Qsf::instance()->getWorkerSystem()"), do no destroy the returned instance
*/
#define QSF_WORKER qsf::Qsf::instance()->getWorkerSystem()

/**
*  @brief
*    Ease of use macro returning a reference to the QSF platform system instance
*
*  @return
*    A reference to the QSF platform system instance ("qsf::Qsf::instance()->getPlatformSystem()"), do no destroy the returned instance
*/
#define QSF_PLATFORM qsf::Qsf::instance()->getPlatformSystem()

/**
*  @brief
*    Ease of use macro returning a reference to the QSF file system instance
*
*  @return
*    A reference to the QSF file system instance ("qsf::Qsf::instance()->getFileSystem()"), do no destroy the returned instance
*/
#define QSF_FILE qsf::Qsf::instance()->getFileSystem()

/**
*  @brief
*    Ease of use macro returning a reference to the QSF log system instance
*
*  @return
*    A reference to the QSF log system instance ("qsf::Qsf::instance()->getLogSystem()"), do no destroy the returned instance
*/
#define QSF_LOG qsf::Qsf::instance()->getLogSystem()

/**
*  @brief
*    Ease of use macro returning a reference to the QSF localization system instance
*
*  @return
*    A reference to the QSF localization system instance ("qsf::Qsf::instance()->getLocalizationSystem()"), do no destroy the returned instance
*/
#define QSF_LOCALIZATION qsf::Qsf::instance()->getLocalizationSystem()

/**
*  @brief
*    Ease of use macro returning a reference to the QSF job system instance
*
*  @return
*    A reference to the QSF job system instance ("qsf::Qsf::instance()->getJobSystem()"), do no destroy the returned instance
*/
#define QSF_JOB qsf::Qsf::instance()->getJobSystem()

/**
*  @brief
*    Ease of use macro returning a reference to the QSF class system instance
*
*  @return
*    A reference to the QSF class system instance ("qsf::Qsf::instance()->getClassSystem()"), do no destroy the returned instance
*/
#define QSF_CLASS qsf::Qsf::instance()->getClassSystem()

/**
*  @brief
*    Ease of use macro returning a reference to the QSF plugin system instance
*
*  @return
*    A reference to the QSF plugin system instance ("qsf::Qsf::instance()->getPluginSystem()"), do no destroy the returned instance
*/
#define QSF_PLUGIN qsf::Qsf::instance()->getPluginSystem()

/**
*  @brief
*    Ease of use macro returning a reference to the QSF asset system instance
*
*  @return
*    A reference to the QSF asset system instance ("qsf::Qsf::instance()->getAssetSystem()"), do no destroy the returned instance
*/
#define QSF_ASSET qsf::Qsf::instance()->getAssetSystem()

/**
*  @brief
*    Ease of use macro returning a reference to the QSF window system instance
*
*  @return
*    A reference to the QSF window system instance ("qsf::Qsf::instance()->getWindowSystem()"), do no destroy the returned instance
*/
#define QSF_WINDOW qsf::Qsf::instance()->getWindowSystem()

/**
*  @brief
*    Ease of use macro returning a reference to the QSF script system instance
*
*  @return
*    A reference to the QSF script system instance ("qsf::Qsf::instance()->getScriptSystem()"), do no destroy the returned instance
*/
#define QSF_SCRIPT qsf::Qsf::instance()->getScriptSystem()

/**
*  @brief
*    Ease of use macro returning a reference to the QSF renderer system instance
*
*  @return
*    A reference to the QSF renderer system instance ("qsf::Qsf::instance()->getRendererSystem()"), do no destroy the returned instance
*/
#define QSF_RENDERER qsf::Qsf::instance()->getRendererSystem()

/**
*  @brief
*    Ease of use macro returning a reference to the QSF physics system instance
*
*  @return
*    A reference to the QSF physics system instance ("qsf::Qsf::instance()->getPhysicsSystem()"), do no destroy the returned instance
*/
#define QSF_PHYSICS qsf::Qsf::instance()->getPhysicsSystem()

/**
*  @brief
*    Ease of use macro returning a reference to the QSF input system instance
*
*  @return
*    A reference to the QSF input system instance ("qsf::Qsf::instance()->getInputSystem()"), do no destroy the returned instance
*/
#define QSF_INPUT qsf::Qsf::instance()->getInputSystem()

/**
*  @brief
*    Ease of use macro returning a reference to the QSF audio system instance
*
*  @return
*    A reference to the QSF audio system instance ("qsf::Qsf::instance()->getAudioSystem()"), do no destroy the returned instance
*/
#define QSF_AUDIO qsf::Qsf::instance()->getAudioSystem()

/**
*  @brief
*    Ease of use macro returning a reference to the QSF network system instance
*
*  @return
*    A reference to the QSF network system instance ("qsf::Qsf::instance()->getNetworkSystem()"), do no destroy the returned instance
*/
#define QSF_NETWORK qsf::Qsf::instance()->getNetworkSystem()

/**
*  @brief
*    Ease of use macro returning a reference to the QSF message system instance
*
*  @return
*    A reference to the QSF message system instance ("qsf::Qsf::instance()->getMessageSystem()"), do no destroy the returned instance
*/
#define QSF_MESSAGE qsf::Qsf::instance()->getMessageSystem()

/**
*  @brief
*    Ease of use macro returning a reference to the QSF component system instance
*
*  @return
*    A reference to the QSF component system instance ("qsf::Qsf::instance()->getComponentSystem()"), do no destroy the returned instance
*/
#define QSF_COMPONENT qsf::Qsf::instance()->getComponentSystem()

/**
*  @brief
*    Ease of use macro returning a reference to the QSF prototype system instance
*
*  @return
*    A reference to the QSF prototype system instance ("qsf::Qsf::instance()->getPrototypeSystem()"), do no destroy the returned instance
*/
#define QSF_PROTOTYPE qsf::Qsf::instance()->getPrototypeSystem()

/**
*  @brief
*    Ease of use macro returning a reference to the QSF link system instance
*
*  @return
*    A reference to the QSF link system instance ("qsf::Qsf::instance()->getLinkSystem()"), do no destroy the returned instance
*/
#define QSF_LINK qsf::Qsf::instance()->getLinkSystem()

/**
*  @brief
*    Ease of use macro returning a reference to the QSF map system instance
*
*  @return
*    A reference to the QSF map system instance ("qsf::Qsf::instance()->getMapSystem()"), do no destroy the returned instance
*/
#define QSF_MAP qsf::Qsf::instance()->getMapSystem()

/**
*  @brief
*    Ease of use macro returning a reference to the QSF GUI system instance
*
*  @return
*    A reference to the QSF GUI system instance ("qsf::Qsf::instance()->getGuiSystem()"), do no destroy the returned instance
*/
#define QSF_GUI qsf::Qsf::instance()->getGuiSystem()


/**
*  @brief
*    Ease of use macro returning a reference to the QSF plugin system instance
*
*  @return
*    A reference to the QSF plugin system instance ("qsf::Qsf::instance()->getActionExecutionSystem()"), do no destroy the returned instance
*/
#define QSF_ACTION_EXECUTION_SYSTEM qsf::Qsf::instance()->getActionExecutionSystem()

/**
*  @brief
*    Ease of use macro returning a reference to the QSF settings group manager instance
*
*  @return
*    A reference to the QSF settings group manager ("qsf::Qsf::instance()->getSettingsGroupManager()"), do no destroy the returned instance
*/
#define QSF_SETTINGSGROUP qsf::Qsf::instance()->getSettingsGroupManager()

/**
*  @brief
*    Ease of use macro returning a reference to the QSF debug group manager instance
*
*  @return
*    A reference to the QSF debug group manager ("qsf::Qsf::instance()->getDebugGroupManager()"), do no destroy the returned instance
*/
#define QSF_DEBUGGROUP qsf::Qsf::instance()->getDebugGroupManager()

/**
*  @brief
*    Ease of use macro returning a pointer to the running application instance
*
*  @return
*    Pointer to the running application instance, can be a null pointer, do no destroy the returned instance
*
*  @note
*    - The application instance pointer is set and also unset within the "qsf::Qsf::run()"-method
*/
#define QSF_APPLICATION qsf::Qsf::instance()->getApplication()


//[-------------------------------------------------------]
//[ Macros for ease-of-use access                         ]
//[-------------------------------------------------------]
/**
*  @brief
*    Ease of use macro returning a reference to the QSF main prototype manager instance
*
*  @return
*    A reference to the QSF main prototype manager ("qsf::Qsf::instance()->getMainPrototypeManager()"), do no destroy the returned instance
*
*  @note
*    -  Please do not use this macro if you can avoid it, use concrete prototype manager instances instead
*/
#define QSF_MAINPROTOTYPE qsf::Qsf::instance()->getMainPrototypeManager()

/**
*  @brief
*    Ease of use macro returning a reference to the QSF main map instance
*
*  @return
*    A reference to the QSF main map ("qsf::Qsf::instance()->getMainMap()"), do no destroy the returned instance
*
*  @note
*    -  Please do not use this macro if you can avoid it, use concrete map instances instead
*/
#define QSF_MAINMAP qsf::Qsf::instance()->getMainMap()

/**
*  @brief
*    Ease of use macro returning a reference to the QSF debug draw manager instance from the current map system
*
*  @return
*    A reference to the QSF debug draw manager ("qsf::Qsf::instance()->getDebugDrawManager()"), do no destroy the returned instance
*
*  @note
*    -  Please do not use this macro if you can avoid it, use the debug draw manager of a concrete map instance instead
*/
#define QSF_DEBUGDRAW qsf::Qsf::instance()->getDebugDrawManager()

/**
*  @brief
*    Ease of use macro returning a reference to the QSF compositing material system instance
*
*  @return
*    Reference to the QSF compositing material system instance, do no destroy the returned instance
*
*  @note
*    - When this macro is used, it's usually safe to consider that the application pointer
*      is not null and this way this macro is in its usage identical to e.g. "QSF_LOG"
*    - The plugin instance reference ("qsf::compositing::Plugin::getInstanceSafe()") is valid only
*      when the QSF compositing plugin was completely started; an exception will be thrown otherwise
*/
#define QSF_MATERIAL qsf::Qsf::instance()->getRendererSystem().getMaterialSystem()

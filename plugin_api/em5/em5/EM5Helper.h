// Copyright (C) 2012-2018 Promotion Software GmbH


// These helper macros only exist to make the daily life more comfortable


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/plugin/Plugin.h"

#include <qsf/Qsf.h>


//[-------------------------------------------------------]
//[ Macros for ease-of-use access                         ]
//[-------------------------------------------------------]
/**
*  @brief
*    Ease of use macro returning a reference to the running EMERGENCY 5 application instance
*
*  @return
*    Reference to the running EMERGENCY 5 application instance, do no destroy the returned instance
*/
#define EM5_APPLICATION em5::Application::getInstanceSafe()

/**
*  @brief
*    Ease of use macro returning a reference to the running EMERGENCY 5 game instance
*
*  @return
*    Reference to the running EMERGENCY 5 game instance, do no destroy the returned instance
*/
#define EM5_GAME em5::Game::getInstanceSafe()

/**
*  @brief
*    Ease of use macro returning a reference to the EMERGENCY 5 GUI instance
*
*  @return
*    Reference to the EMERGENCY 5 gui system instance, do no destroy the returned instance
*/
#define EM5_GUI em5::EmergencyGui::getInstanceSafe()

/**
*  @brief
*    Ease of use macro returning a reference to the EMERGENCY 5 network manager instance of the running EMERGENCY 5 application instance
*
*  @return
*    Reference to the EMERGENCY 5 network manager instance of the running EMERGENCY 5 application instance, do no destroy the returned instance
*/
#define EM5_NETWORK em5::NetworkManager::getInstanceSafe()

/**
*  @brief
*    Ease of use macro returning a reference to the EMERGENCY 5 mod system instance
*
*  @return
*    Reference to the EMERGENCY 5 mod system instance, do no destroy the returned instance
*/
#define EM5_MOD em5::Plugin::getInstanceSafe().getModSystem()

/**
*  @brief
*    Ease of use macro returning a reference to the EMERGENCY 5 freeplay system instance
*
*  @return
*    Reference to the EMERGENCY 5 freeplay system instance, do no destroy the returned instance
*/
#define EM5_FREEPLAY em5::Plugin::getInstanceSafe().getFreeplaySystem()

/**
*  @brief
*    Ease of use macro returning a reference to the EMERGENCY 5 health system instance
*
*  @return
*    Reference to the EMERGENCY 5 health system instance, do no destroy the returned instance
*/
#define EM5_HEALTH em5::Plugin::getInstanceSafe().getHealthSystem()

/**
*  @brief
*    Ease of use macro returning a reference to the EMERGENCY 5 fire system instance
*
*  @return
*    Reference to the EMERGENCY 5 fire system instance, do no destroy the returned instance
*/
#define EM5_FIRE em5::Plugin::getInstanceSafe().getFireSystem()

/**
*  @brief
*    Ease of use macro returning a reference to the EMERGENCY 5 activity system instance
*
*  @return
*    Reference to the EMERGENCY 5 activity system instance, do no destroy the returned instance
*/
#define EM5_ACTIVITY em5::Plugin::getInstanceSafe().getActivitySystem()

/**
*  @brief
*    Ease of use macro returning a reference to the EMERGENCY 5 achievement system instance
*
*  @return
*    Reference to the EMERGENCY 5 achievement system instance, do no destroy the returned instance
*/
#define EM5_ACHIEVEMENT em5::Plugin::getInstanceSafe().getAchievementSystem()

/**
*  @brief
*    Ease of use macro returning a reference to the EMERGENCY 5 player manager instance
*
*  @return
*    Reference to the EMERGENCY 5 player manager instance, do no destroy the returned instance
*/
#define EM5_PLAYERS EM5_GAME.getPlayerManager()

/**
*  @brief
*    Ease of use macro returning a reference to the EMERGENCY 5 game timer manager instance
*
*  @return
*    Reference to the EMERGENCY 5 game timer manager instance, do no destroy the returned instance
*/
#define EM5_GAMETIMERS EM5_GAME.getGameTimerManager()

/**
*  @brief
*    Ease of use macro returning a reference to the EMERGENCY 5 game counter manager instance
*
*  @return
*    Reference to the EMERGENCY 5 game counter manager instance, do no destroy the returned instance
*/
#define EM5_GAMECOUNTERS EM5_GAME.getGameCounterManager()

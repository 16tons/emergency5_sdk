// Copyright (C) 2012-2017 Promotion Software GmbH


// These helper macros only exist to make the daily life more comfortable


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_game/plugin/Plugin.h"


//[-------------------------------------------------------]
//[ Macros                                                ]
//[-------------------------------------------------------]
/**
*  @brief
*    Ease of use macro returning a reference to the QSF Game command system instance
*
*  @return
*    Reference to the QSF Game command system instance, do no destroy the returned instance
*
*  @note
*    - When this macro is used, it's usually safe to consider that the application pointer
*      is not null and this way this macro is in its usage identical to e.g. "QSF_LOG"
*    - The plugin instance reference ("qsf::game::Plugin::getInstanceSafe()") is valid only
*      when the QSF Game plugin was completely started; an exception will be thrown otherwise
*/
#define QSFGAME_COMMAND qsf::game::Plugin::getInstanceSafe().getCommandSystem()

/**
*  @brief
*    Ease of use macro returning a reference to the QSF Game weather system instance
*
*  @return
*    Reference to the QSF Game weather system instance, do no destroy the returned instance
*
*  @note
*    - When this macro is used, it's usually safe to consider that the application pointer
*      is not null and this way this macro is in its usage identical to e.g. "QSF_LOG"
*    - The plugin instance reference ("qsf::game::Plugin::getInstanceSafe()") is valid only
*      when the QSF Game plugin was completely started; an exception will be thrown otherwise
*/
#define QSFGAME_WEATHER qsf::game::Plugin::getInstanceSafe().getWeatherSystem()

/**
*  @brief
*    Ease of use macro returning a reference to the QSF Game equipment system instance
*
*  @return
*    Reference to the QSF Game equipment system instance, do no destroy the returned instance
*
*  @note
*    - When this macro is used, it's usually safe to consider that the application pointer
*      is not null and this way this macro is in its usage identical to e.g. "QSF_LOG"
*    - The plugin instance reference ("qsf::game::Plugin::getInstanceSafe()") is valid only
*      when the QSF Game plugin was completely started; an exception will be thrown otherwise
*/
#define QSFGAME_EQUIPMENT qsf::game::Plugin::getInstanceSafe().getEquipmentSystem()

/**
*  @brief
*    Ease of use macro returning a reference to the QSF Game character animation system instance
*
*  @return
*    Reference to the QSF Game character animation system instance, do no destroy the returned instance
*
*  @note
*    - When this macro is used, it's usually safe to consider that the application pointer
*      is not null and this way this macro is in its usage identical to e.g. "QSF_LOG"
*    - The plugin instance reference ("qsf::game::Plugin::getInstanceSafe()") is valid only
*      when the QSF Game plugin was completely started; an exception will be thrown otherwise
*/
#define QSFGAME_CHARACTERANIMAION qsf::game::Plugin::getInstanceSafe().getCharacterAnimationSystem()

// Copyright (C) 2012-2018 Promotion Software GmbH


// These helper macros only exist to make the daily life more comfortable


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_compositing/plugin/Plugin.h"


//[-------------------------------------------------------]
//[ Macros                                                ]
//[-------------------------------------------------------]
/**
*  @brief
*    Ease of use macro returning a reference to the QSF compositing compositor system instance
*
*  @return
*    Reference to the QSF compositing compositor system instance, do no destroy the returned instance
*
*  @note
*    - When this macro is used, it's usually safe to consider that the application pointer
*      is not null and this way this macro is in its usage identical to e.g. "QSF_LOG"
*    - The plugin instance reference ("qsf::compositing::Plugin::getInstanceSafe()") is valid only
*      when the QSF compositing plugin was completely started; an exception will be thrown otherwise
*/
#define QSFCOMPOSITING_COMPOSITOR qsf::compositing::Plugin::getInstanceSafe().getCompositorSystem()

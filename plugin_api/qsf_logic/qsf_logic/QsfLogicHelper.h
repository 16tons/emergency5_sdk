// Copyright (C) 2012-2018 Promotion Software GmbH


// These helper macros only exist to make the daily life more comfortable


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_logic/plugin/Plugin.h"


//[-------------------------------------------------------]
//[ Macros                                                ]
//[-------------------------------------------------------]
/**
*  @brief
*    Ease of use macro returning a reference to the QSF sequence system instance
*
*  @return
*    A reference to the QSF sequence system instance ("qsf::Qsf::instance()->getSequenceSystem()"), do no destroy the returned instance
*/
#define QSF_LOGIC_SEQUENCE qsf::logic::Plugin::getInstanceSafe().getSequenceSystem()

// Copyright (C) 2012-2017 Promotion Software GmbH


// These helper macros only exist to make the daily life more comfortable


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor_base/application/Application.h"

#include <qsf/Qsf.h>


//[-------------------------------------------------------]
//[ Macros                                                ]
//[-------------------------------------------------------]
/**
*  @brief
*    Ease of use macro returning a reference to the running QSF editor base application instance
*
*  @return
*    Reference to the running QSF editor base application instance, do no destroy the returned instance
*
*  @note
*    - When this macro is used, it's usually safe to consider that the application pointer
*      is not null and this way this macro is in its usage identical to e.g. "QSF_LOG"
*    - The application instance pointer ("qsf::Qsf::instance()->getApplication()") is set
*      and also unset within the "qsf::Qsf::run()"-method
*/
#define QSF_EDITOR_BASE_APPLICATION static_cast<qsf::editor::base::Application&>(*qsf::Qsf::instance()->getApplication())


//[-------------------------------------------------------]
//[ Macros for "qsf::editor::base::Application" managed stuff ]
//[-------------------------------------------------------]
/**
*  @brief
*    Ease of use macro returning a reference to the running QSF editor base operation manager instance
*
*  @return
*    Reference to the running QSF editor base operation manager instance, do no destroy the returned instance
*
*  @note
*    - When this macro is used, it's usually safe to consider that the application pointer
*      is not null and this way this macro is in its usage identical to e.g. "QSF_LOG"
*    - The application instance pointer ("qsf::Qsf::instance()->getApplication()") is set
*      and also unset within the "qsf::Qsf::run()"-method
*/
#define QSF_EDITOR_BASE_OPERATION static_cast<qsf::editor::base::Application&>(*qsf::Qsf::instance()->getApplication()).getOperationManager()

/**
*  @brief
*    Ease of use macro returning a reference to the running QSF editor base network transfer manager instance
*
*  @return
*    Reference to the running QSF editor base network transfer manager instance, do no destroy the returned instance
*
*  @note
*    - When this macro is used, it's usually safe to consider that the application pointer
*      is not null and this way this macro is in its usage identical to e.g. "QSF_LOG"
*    - The application instance pointer ("qsf::Qsf::instance()->getApplication()") is set
*      and also unset within the "qsf::Qsf::run()"-method
*/
#define QSF_EDITOR_BASE_NETWORK_TRANSFER static_cast<qsf::editor::base::Application&>(*qsf::Qsf::instance()->getApplication()).getNetworkTransferManager()

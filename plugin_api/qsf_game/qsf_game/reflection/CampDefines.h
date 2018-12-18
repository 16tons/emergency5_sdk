// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/reflection/CampDefines.h>


//[-------------------------------------------------------]
//[ Defines                                               ]
//[-------------------------------------------------------]

// Macros to streamline CAMP export

#define QSF_GAME_CAMP_IS_COMMAND \
		.base<qsf::game::Command>() \
		.constructor1<qsf::game::CommandManager*>()

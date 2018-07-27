// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Defines                                               ]
//[-------------------------------------------------------]

// Macros to streamline CAMP export

/*
 *  @brief
 *    Mark the class as a map test
 */
#define QSF_CAMP_IS_MAPTEST \
		.base<qsf::editor::MapTest>() \
		.constructor1<qsf::editor::MapTestManager*>()

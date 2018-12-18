// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace action
	{


		// Defines an enum that governs how new actions are integrated into an exisiting plan.
		enum InsertionMode
		{
			APPEND_TO_BACK,
			INSERT_AT_FRONT,
			INSERT_BEFORE_CURRENT,
			REPLACE_WHOLE_PLAN
		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // action
} // qsf

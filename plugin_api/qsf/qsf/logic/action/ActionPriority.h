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


		// Defines the action priority type, simply an uint32; the meaning of certain values may be defined by the application.
		// Actions with higher priority values can't (or at least should not) be replaced with lower priority actions.
		typedef uint32 Priority;


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // action
} // qsf

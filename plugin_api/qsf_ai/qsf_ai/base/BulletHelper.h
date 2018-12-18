// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <LinearMath/btTransform.h>


// File contains helper functionality for using bullet constructs
namespace qsf
{
	// bullet defines only equality, not inequality operator
	inline bool operator !=(const btTransform& lhs, const btTransform& rhs)
	{
		return !(lhs == rhs);
	}
}

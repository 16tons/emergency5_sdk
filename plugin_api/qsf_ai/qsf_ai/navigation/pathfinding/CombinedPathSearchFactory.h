// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/navigation/pathfinding/PathSearch.h"
#include "qsf_ai/navigation/pathfinding/PathSearchConfiguration.h"
#include "qsf_ai/base/DebugSetting.h"

#include <memory>

namespace qsf
{
	namespace ai
	{
		// Helper function to encapsulate the creation of combined path searches which requires currently a case by case analysis to avoid runtime polymorphy
		// The combined path search takes ownership on the path search configuration passed, the pointer is reset during the call
		std::auto_ptr<PathSearch> createCombinedPathSearch(unsigned int taskId, std::auto_ptr<PathSearchConfiguration> config);
	}
}

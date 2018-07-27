// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"

#include <vector>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	class EM5_API_EXPORT FreeplayEventTriggerInfo
	{
	public:
		typedef std::string LogLine;

		// TODO(fw): Only mTriggeredByDebug is currently supported and used, the rest has to be added

		bool mTriggeredByDebug;					///< "true" if triggered from debug menu; can be used to exclude several checks like weather, time of day, etc.
		std::string mEnforcedLocation;			///< When triggered by debug menu, this can be an additional string naming a certain event location where to trigger the event
		std::vector<LogLine> mTriggerFailLog;	///< Write information why triggering failed inside here

	public:
		inline explicit FreeplayEventTriggerInfo(bool debug = false) : mTriggeredByDebug(debug) {}
	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

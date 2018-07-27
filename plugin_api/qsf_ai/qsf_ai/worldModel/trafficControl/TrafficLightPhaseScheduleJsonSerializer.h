// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/worldModel/trafficControl/TrafficLightPhaseSchedule.h"

#include <qsf/serialization/Serializer.h>

#include <string>

namespace qsf
{
	namespace ai
	{
		/**
		* @brief Contains static functions for reading and writing a traffic light phase schedule from or to Json text format.
		*/
		class TrafficLightPhaseScheduleJsonSerializer : public Serializer
		{
		public:
			static const TrafficLightPhaseSchedule& deserialize(const std::string& filename);
			static void serialize(const TrafficLightPhaseSchedule& schedule, const std::string& filename);
		};
	}
}

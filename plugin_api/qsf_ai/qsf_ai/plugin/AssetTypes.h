// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/Export.h"

#include <qsf/asset/type/AssetTypes.h>
#include <qsf/base/NamedIdentifier.h>

namespace qsf
{
	namespace ai
	{

		/**
		*  @brief
		*    Static QSF AI plugin asset types collection class.
		*	Has only static members so there is actually no need to ever create instances of this class.
		*/
		class QSF_AI_API_EXPORT AssetTypes : public qsf::AssetTypes
		{
		public:
			friend class Plugin;

			// AI traffic lane world ("ai_traffic_lane_world")
			static const NamedIdentifier AI_TRAFFIC_LANE_WORLD;
			// AI traffic light schedule ("ai_traffic_light_schedule")
			static const NamedIdentifier AI_TRAFFIC_LIGHT_SCHEDULE;
			// AI area type ("ai_area_type")
			static const NamedIdentifier AI_AREA_TYPE;

			// AI sensor configurations ("ai_sensor_configurations")
			static const NamedIdentifier AI_SENSOR_CONFIGURATIONS_TYPE;

			// AI reaction table configurations, contains prioritised ReactionRules ("ai_reaction_table_configurations")
			static const NamedIdentifier AI_REACTION_TABLE_TYPE;


			static void registerAssetTypes();
			static void unregisterAssetTypes();
		};
	}
}

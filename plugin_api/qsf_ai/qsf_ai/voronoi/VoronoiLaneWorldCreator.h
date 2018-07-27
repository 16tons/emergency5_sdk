// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/Export.h"

#include <memory>

namespace qsf
{
	class Map;

	namespace ai
	{
		class TrafficLaneWorld;
		class TrafficLaneWorldCreationSettings;

		namespace voronoi
		{
			/**
			* Facade method to manage all the activities for creating a traffic lane world model via the automatic Voronoi generation.
			* This executes the complete creation algorithm consisting of the following steps:
			* 1.) Create a grid with the requested dimensions.
			* 2.) Enter all entities with a collision component attached as obstacles into the grid.
			*  The default is to use no height restriction on which obstacles to consider.
			*  Additionally the outer border cells may be marked as blocked by the core entity (to represent the map boundaries).
			* 3.) Calculate the distances and the cells with several equidistant obstacles.
			* 4.) Find the segments as connected lines of equidistant cells to the same obstacles.
			* 5.) Connect neighbouring end points of identified segments.
			* 6.) Convert to lanes and create a traffic lane world.
			* 7.) Test the turning constraint settings passed as parameter. This is only a preview version and will be handled differently in the final version.
			* 8.) Mix with another lane world and connect them with street crossings
			*
			* The height restriction can be used to filter the obstacles height value (currently hardcoded to Y).
			* The debug settings define what is shown during the creation process.
			* Each option is drawn when the data is available which means there is a delay between the single sets of data depending on data size.
			* The map needs to be provided in case the terrain heights influence the actual test positions and to access the bullet collision world via the core entity.
			* It should be const but can't be made const because the getCoreEntity call is not const.
			*/
			std::auto_ptr<TrafficLaneWorld> QSF_AI_API_EXPORT createTrafficLaneWorld(const TrafficLaneWorldCreationSettings& settings, Map& map);
		}
	}
}

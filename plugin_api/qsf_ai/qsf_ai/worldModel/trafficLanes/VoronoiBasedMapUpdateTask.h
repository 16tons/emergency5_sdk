// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/navigation/NavigationTask.h"
#include "qsf_ai/worldModel/dynamicUpdate/NavigationMapUpdateConfiguration.h"
#include "qsf_ai/voronoi/DynamicVoronoiGraph.h"
#include "qsf_ai/voronoi/VoronoiSegmentFinder.h"
#include "qsf_ai/navigation/pathfinding/CombinedSearchConnectionPoint.h"

#include <deque>


namespace qsf
{
	namespace ai
	{
		class TrafficLaneWorld;

		class VoronoiBasedMapUpdateTask : public NavigationTask
		{
		public:
			VoronoiBasedMapUpdateTask(unsigned int mapId, const NavigationMapUpdateConfiguration& updateConfig);

			// public navigation task interface implementation
			//@{
			virtual process::State getState() const override;
			virtual process::State execute() override;
			virtual void interrupt() override;
			virtual void fail() override;
			//@}

		private:
			// sub function that tweak the final nav map data
			void updateNavigationMap(const voronoi::DynamicGraph::TweakedGraphDataCollection& tweakedData, const voronoi::SegmentFinder& segFinder, std::deque<CombinedSearchConnectionPoint>& newConnections);

			process::State mState;

			unsigned int mMapId;
			TrafficLaneWorld* mTrafficLaneWorldToUpdate; // is only initialized during the execution phase, previously a nullptr
			NavigationMapUpdateConfiguration mUpdateConfig;
		};
	}
}

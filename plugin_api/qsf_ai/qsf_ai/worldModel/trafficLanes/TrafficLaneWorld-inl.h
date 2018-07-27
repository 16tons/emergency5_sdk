// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/base/Functors.h"

#include <qsf/base/error/ErrorHandling.h>

#include <algorithm>

namespace qsf
{
	namespace ai
	{
		inline TrafficLaneWorld::TrafficLaneWorld(unsigned int id) :
			WorldModel(id),
			mConstructionFinished(false),
			mUseFunnelPathSmoothing(false)
		{}

		inline std::size_t TrafficLaneWorld::getNumNodes() const
		{
			return mLaneNodes.size();
		}

		inline const LaneCollection& TrafficLaneWorld::getLanes() const
		{
			return mLanes;
		}

		inline const WorldElementStateCollection& TrafficLaneWorld::getWorldElementsState() const
		{
			return mWorldElementsState;
		}

		inline WorldElementStateCollection& TrafficLaneWorld::getWorldElementsState()
		{
			return mWorldElementsState;
		}

		inline WorldElementStateCollection* TrafficLaneWorld::tryGetWorldElementsState()
		{
			return &mWorldElementsState;
		}

		inline const WorldElementStateCollection* TrafficLaneWorld::tryGetWorldElementsState() const
		{
			return &mWorldElementsState;
		}

		inline const LaneEndNode* TrafficLaneWorld::tryGetLaneEndNode(unsigned int id) const
		{
			if (id >= mLaneNodes.size())
				return nullptr;

			return &mLaneNodes[id];
		}

		inline const LaneEndNode& TrafficLaneWorld::getLaneEndNode(unsigned int id) const
		{
			const LaneEndNode* node = tryGetLaneEndNode(id);
			QSF_CHECK(node, "index " << id << " out of bounds when accessing lane node",
				QSF_REACT_THROW);

			return *node;
		}

		inline TrafficLaneWorld::IDRange TrafficLaneWorld::getAllConnectionsForNode(unsigned int nodeIndex) const
		{
			return mNodeToLaneConnections.equal_range(nodeIndex);
		}

		inline bool TrafficLaneWorld::wasAutomaticallyCreated() const
		{
			return mAutomaticCreationSettings.get() != nullptr;
		}

		inline const TrafficLaneWorldCreationSettings& TrafficLaneWorld::getAutomaticCreationSettings() const
		{
			QSF_CHECK(mAutomaticCreationSettings.get(), "Trying to access automatic creation settings for manually edited lane world " << getName(),
				QSF_REACT_THROW);

			return *mAutomaticCreationSettings;
		}

		inline void TrafficLaneWorld::setAutomaticCreationSettings(const TrafficLaneWorldCreationSettings& settings)
		{
			// Currently connecting the funnel smoothing to the fact that the world was automatically created
			mUseFunnelPathSmoothing = true;
			mAutomaticCreationSettings = settings.clone();
		}

		inline boost::optional<unsigned int> TrafficLaneWorld::tryFindOptimizedTurningConstraintId(const TurningConstraint& constraint) const
		{
			const EntityConfigurations::const_iterator currentEntry = std::find(mOptimizedEntityConfigurations.begin(), mOptimizedEntityConfigurations.end(), constraint);
			if (currentEntry == mOptimizedEntityConfigurations.end())
				return boost::optional<unsigned int>(); // not contained

			return static_cast<unsigned int>(currentEntry - mOptimizedEntityConfigurations.begin()); // already optimized, return index
		}

		inline WorldModel::IDRange TrafficLaneWorld::getIntersectingAreaIds(unsigned int areaId) const
		{
			return mLanes.getIntersectingLaneIds(areaId);
		}

		inline void TrafficLaneWorld::setUseFunnelSmoothing(bool enable)
		{
			mUseFunnelPathSmoothing = enable;
		}

		inline bool TrafficLaneWorld::isUsingFunnelSmoothing() const
		{
			return mUseFunnelPathSmoothing;
		}

		inline bool TrafficLaneWorld::hasDynamicVoronoiGraph() const
		{
			return mDynamicVoronoiGraph.get() != nullptr;
		}

		inline void TrafficLaneWorld::setDynamicVoronoiGraph(std::auto_ptr<voronoi::DynamicGraph> voronoi)
		{
			mDynamicVoronoiGraph = voronoi;
		}

		inline voronoi::DynamicGraph& TrafficLaneWorld::getDynamicVoronoiGraph() const
		{
			QSF_CHECK(hasDynamicVoronoiGraph(), "Unable to access uninitialized dynamic voronoi graph",
				QSF_REACT_THROW);

			return *mDynamicVoronoiGraph;
		}
	}
}

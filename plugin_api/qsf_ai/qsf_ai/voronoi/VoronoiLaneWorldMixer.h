// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/Export.h"


//[-------------------------------------------------------]
//[ Forward declaration                                   ]
//[-------------------------------------------------------]
namespace qsf
{
	class Map;
	class StreetCrossingComponent;

	namespace ai
	{
		class TrafficLaneWorld;
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace ai
	{
		namespace voronoi
		{
			//[-------------------------------------------------------]
			//[ Classes                                               ]
			//[-------------------------------------------------------]
			/**
			*  @brief
			*    Voronoi lane mixer
			*
			*  @note
			*    - Mixes 2 ai map / lane worlds together
			*    - Uses street crossings as connection between the 2 worlds
			*    - We also call our "mLaneworld" (z.B. free area ai map) mapA or laneWorldA
			*    - and the "mMixLaneWorld" (bridge ai map) mapB or laneWorldB
			*/
			class QSF_AI_API_EXPORT VoronoiLaneWorldMixer : public boost::noncopyable
			{
			public:

				/*
					TODO(tl): Write me
				*/
				VoronoiLaneWorldMixer(TrafficLaneWorld& laneWorld, const TrafficLaneWorld& mixLaneWorld, Map& map);

				/*
				TODO(tl): Write me
				*/
				void execute();

			private:

				// Different steps we use in execute for better overview
				void mixLaneWorlds();
				void insertStreetCrossing(const StreetCrossingComponent& crossing);
				void insertLanesAndNodesFromStreetCrossing(const StreetCrossingComponent& crossing);
				void connectEdgeToMap(const StreetCrossingComponent& crossing, bool closeToMapA, unsigned int edgeId);
				bool findClosestConnection(const StreetCrossingComponent& crossing, bool closeToMapA, unsigned int edgeId,
					unsigned int& closestLeaf, unsigned int& closestLaneId, unsigned int&closestSegmentId);
				void connectNodeWithLane(const StreetCrossingComponent& crossing, bool closeToMapA, unsigned int leafId,
					unsigned int laneId, unsigned int segmentId);
				void connectNodeWithNode(const StreetCrossingComponent& crossing, unsigned int leafId, unsigned int laneId,
					unsigned int segmentId);

				unsigned int getEndNodeId(std::vector<unsigned int>& laneEndNodeId, const glm::vec3& segmentPosition);

				Map& mMap;

				TrafficLaneWorld& mLaneWorld;
				const TrafficLaneWorld& mMixLaneWorld;

				unsigned int mTypeId;

				unsigned int mNumberOfNodesBeforeMix;		// tracking of different parts inside the "end" node vector
				unsigned int mNumberOfNodesBeforeStreetCrossings;
				unsigned int mNumberOfNodesAfterOtherStreetCrossings;

				unsigned int mNumberOfLanesBeforeMix;		// tracking of different parts inside the lanee vector
				unsigned int mNumberOfLanesBeforeStreetCrossings;
				unsigned int mNumberOfLanesAfterOtherStreetCrossings;

				std::vector<unsigned int> mAddedLanesToA;	// we need to keep track the splitted lanes to connect to them too
				std::vector<unsigned int> mAddedLanesToB;
			};
		}
	}
}

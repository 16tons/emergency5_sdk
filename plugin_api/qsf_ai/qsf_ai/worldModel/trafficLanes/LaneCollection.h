// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/Export.h"
#include "qsf_ai/worldModel/trafficLanes/Lane.h"
#include "qsf_ai/worldModel/trafficLanes/LaneNode.h"
#include "qsf_ai/worldModel/WorldModel.h"

#include <glm/glm.hpp>

#include <boost/optional.hpp>
#include <boost/container/flat_map.hpp>

#include <deque>
#include <vector>


namespace qsf
{
	class CompoundDebugDrawRequest;
	class Color4;

	namespace ai
	{
		/**
		* @brief A collection of lanes.
		* Some parts of the lane representation are stored inside this compound structure for saving memory.
		* Because these are the basic data types for the AI world representation creating an instance for each element which would have its own dynamic containers would probably be a noticeable overhead.
		*/
		class QSF_AI_API_EXPORT LaneCollection
		{
		public:
			friend struct serialization::serializer<LaneCollection>;

			void addOrReplaceLane(const std::vector<LaneNode3D>& nodes, unsigned int laneId, unsigned int startNodeId, unsigned int endNodeId, unsigned int type);
			void eraseLane(unsigned int id);
			unsigned int getNumLanes() const;

			/** Access the data for the lane addressed by the index.
			* Additional data is written to the optional pointer parameters if these are provided.
			* The points are returned as access to an internal array if you provide a pointer parameter.
			* The try variant returns a nullptr if the lane does not exist while the getter throws an exception if the lane index is illegal.
			*
			* @param[out] nodes
			* A pointer to the first node of the lane. All other nodes are linear in memory after this one.
			*
			* @param[out] numNodes
			* The number of nodes linear in memory that make up this lane.
			*/
			//@{
			const Lane& getLane(unsigned int index) const;
			Lane& getLane(unsigned int index);
			const Lane* tryGetLane(unsigned int index) const;
			//@}

			/**
			* Returns a range of iterators for lanes that intersect with the one indicated as parameter.
			* The range may be empty.
			*/
			WorldModel::IDRange getIntersectingLaneIds(unsigned int lane) const;

			// Returns a pair of connected lane ids that represent the intersection with the given index.
			// This is only used during debugging and it refers to the linear order in memory which is not accessible otherwise.
			boost::optional<std::pair<unsigned int, unsigned int>> tryGetLaneIntersectionByIndex(unsigned int intersectionIndex) const;

			// Prepare data structure to avoid dynamic allocations
			void prepareForIntersectionUpdate(std::size_t numIntersections);

			/** Adds intersection information between lanes A and B.
			* Since it is a symmetric relation it will always be added in both directions.
			* This is a slow process because it is expected to happen only during initialization or editing time to make look up of the data fast at runtime where it is static.
			* The IfNecessary signals that it contains a duplicate check and silently ignores any attempt to create duplicate entries because our current generation creates such situations by design.
			*/
			void addIntersectionIfNecessary(unsigned int laneA, unsigned int laneB);

			// Add debug draw requests that indicate the outline of a whole lane to the compound object passed
			void drawLaneOutline(unsigned int index, CompoundDebugDrawRequest& request, const Color4& color) const;
			// Add debug drawings that indicate the ideal path of a whole lane to the compound object passed
			void drawLaneIdealPath(unsigned int index, CompoundDebugDrawRequest& request, const Color4& color) const;
			// Add debug drawings that indicate a part of a lane to the compound debug draw request.
			void drawLanePartialOutline(unsigned int laneId, unsigned int firstNode, unsigned int lastNode, CompoundDebugDrawRequest& request, const Color4& color) const;
			// Add debug drawings that indicate a part of the ideal path along a lane to the compound debug draw request.
			void drawLanePartialIdealPath(unsigned int laneId, unsigned int firstNode, unsigned int lastNode, CompoundDebugDrawRequest& request, const Color4& color) const;
			// Add debug drawing that writes the lane id as text in the middle of the lane space
			void drawLaneNumber(unsigned int laneId, CompoundDebugDrawRequest& request) const;

			// Templated visitation of all areas defined by the lane with the given index.
			// Each area will be passed as the four corners of a trapezoid that define a polygon.
			template <typename Functor>
			void visitAllAreas(unsigned int laneIndex, Functor& functor) const;

			/** This returns a pair containing the two lane nodes that define the segment closest to the lane end specified by the id of the lane and the id of the end node.
			* Throws an exception if the lane id itself is illegal or the node id is not actually one of the defining nodes for this lane.
			* It is guaranteed that the first of the pair is the lane end node with the given ID.
			*/
			std::pair<LaneNode3D, LaneNode3D> getSegmentAttachedToEnd(unsigned int laneId, unsigned int endNodeId, bool throwexception, bool& error) const;

			/** Calculates the length along the nodes that make up a part of a lane.
			* The version adds the distance from one extreme end node to the node passed.
			*/
			//@{
			UnsignedFloat calculatePartialLaneLengthFrom(unsigned int laneIndex, unsigned int fromNodeIndex) const;
			UnsignedFloat calculatePartialLaneLengthTo(unsigned int laneIndex, unsigned int toNodeIndex) const;
			UnsignedFloat calculatePartialLaneLength(unsigned int laneIndex, unsigned int fromNodeIndex, unsigned int toNodeIndex) const;
			//@}


			// Helper class for communicating a closest point result
			class SingleClosestPointOption
			{
			public:
				// Defines whether the perpendicular onto the lane actually intersects between the two nodes or not, and if not, on which end we overshoot
				enum RelationToLane
				{
					IDEAL_BETWEEN_NODES,
					BEFORE_START,
					BEHIND_END,
				};

				SingleClosestPointOption();

				bool isInsideIdealLane() const; // shortcut for querying the relation to the ideal lane

				glm::vec3 mClosestPoint;
				UnsignedFloat mFreeSpaceAtIdealLane;
				UnsignedFloat mDistanceToLane; // is zero means the point is inside the ideal area
				RelationToLane mRelationToLane;
			};

			// Calculate the closest point on the lane area defined by the two lane nodes to the point.
			// An optional value that is calculated anyway and may be a useful byproduct for clients is the position on the ideal lane.
			// It is written if the pointer is initialized to a real value.
			// Will give back false if with distanceFromCollision no valid point could be found
			static bool calculateClosestPointOnSegment(SingleClosestPointOption& result, const glm::vec3& point, const LaneNode3D& startLane,
				const LaneNode3D& end, glm::vec3& closestPointOnIdealLane, bool isDistanceOnPlane, float distanceFromCollision = 0.0f);

			// Options where to change lanes are stored here with starting and arriving lane id as keys and values
			typedef boost::container::flat_multimap<unsigned int, unsigned int> LaneChangeOptionContainer;
			void setLaneChangeOptions(const LaneChangeOptionContainer& options);
			const LaneChangeOptionContainer& getLaneChangeOptions() const;

			std::size_t calculateMemoryConsumption() const;

		private:
			// Helper function to encapsulate the repeated task of accessing the start index of the intersecting lane ids
			unsigned int getIntersectingLanesStart(unsigned int lane) const;

			// Adds the information that A is intersected by B.
			// The IfNecessary signals that it contains a check for duplicates and silently ignores an attempt to add an entry twice because the current creation process generates this situation by design.
			void addIntersectionUnidirectionalIfNecessary(unsigned int laneA, unsigned int laneB);

			// Helper to exchange intersection data for a lane, possibly erasing it in case the vector is empty or a nullptr is passed
			void replaceLaneIntersections(unsigned int laneId, const std::vector<unsigned int>* newConnections);


			// A vector containing all the lane instances.
			std::deque<Lane> mLanes;

			// The vector contains sets of ids of intersecting lanes linear in memory.
			// To save memory instead having one (often empty) dynamic collection stored with each lane they are defined in one vector.
			// The lanes intersecting with lane x are [ mLanes[x - 1].getIntersectingLaneEndIndex() ... mLanes[x].getIntersectingLaneEndIndex() )
			std::deque<unsigned int> mIntersectingLaneIds;

			// Lanes that are so close to one another that an entity can change from one lane to the other are stored in this multimap.
			// The key is the id of a lane where the entity starts and the mapped values are the ids where the entity may end up.
			LaneChangeOptionContainer mLaneChangeOptions;
		};
	}
}

#include "qsf_ai/worldModel/trafficLanes/LaneCollection-inl.h"

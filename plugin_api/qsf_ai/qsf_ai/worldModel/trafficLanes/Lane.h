// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/base/UnsignedFloat.h"
#include "qsf_ai/base/Direction.h"
#include "qsf_ai/worldModel/trafficLanes/LaneNode.h"

#include <qsf/base/manager/CharArrayBitSet.h>
#include <qsf/math/AxisAlignedBoundingBox.h>

#include <cstddef>


namespace qsf
{
	namespace ai
	{
		/**
		* @brief A single lane defines an atomic area where an entity might move along in the traffic lane representation.
		* A lane is basically a point list with some free space projected to the sides of each line segment inside the navigation plane.
		* This creates a series of connected and partially overlapping rectangular areas.
		* Because the free space is available in all directions you need to add a semicircle at each segments end point to get the full picture.
		* A lane may be connected at its endpoints to other lanes.
		*
		* Lanes are simple objects copyable via the default mechanisms.
		*/
		class Lane
		{
		public:
			friend class std::allocator<Lane>;
			friend struct serialization::serializer<Lane>;

			// The area of a lane segment is defined by four points that describe a trapezoid.
			static const unsigned int NUM_LANE_AREA_DEFINING_POINTS = 4;

			// Hard coded limit for the number of different entity configurations that are supported as tags.
			static const unsigned int NUM_SUPPORTED_ENTITY_CONFIGURATIONS = 32;

			// Start and end node are the unique indices of the respective start and end node inside the global node vector.
			// The position end index is one past the end of the last position list of all points inside the global positions vector.
			explicit Lane(unsigned int startNode, unsigned int endNode, const std::vector<LaneNode3D>& nodes, UnsignedFloat length, unsigned int typeId);

			unsigned int getStartNodeId() const;
			unsigned int getEndNodeId() const;
			unsigned int getTypeId() const;
			UnsignedFloat getLength() const;
			unsigned int getNumNodes() const;
			const std::vector<LaneNode3D>& getNodes() const;

			/** Access to the entity configuration flags that inform about if the specific entity configuration can use the lane.
			* By default all flags are set to true unless manually updated before.
			* There is currently a hard coded limit for the number of flags supported and any access with a value greater than 32 will lead to an index out of bounds exception.
			*/
			//@{
			bool canBeUsedByEntityConfiguration(unsigned int index) const;
			void setUsableByEntityConfiguration(unsigned int index, bool value);
			//@}

			// Returns the id of the node on the other end of the lane.
			// That means it returns the start node id of the end node id was passed and vice versa.
			// Asserts that the id passed is actually one of the two defining ids.
			unsigned int getOtherEndNodeId(unsigned int endNodeId) const;

			/** Returns whether this lane can be used to travel from nodeA to nodeB.
			* This is true if these are the start and end node of one lane respectively.
			* The direction allows to define further constraints -
			* Forward only allows traveling from start to end while backwards only allows traveling from end to start node.
			*/
			bool connects(unsigned int nodeA, unsigned int nodeB, direction::OneDimensional dir) const;

			// Access to the index of the intersecting lanes ids.
			//@{
			unsigned int getIntersectingLaneEndIndex() const;
			void tweakIntersectiongLaneEndIndex(int delta);
			void onIntersectionAdded();
			void onIntersectionRemoved();
			//@}

			// Access to the bounding box
			//@{
			inline const AxisAlignedBoundingBox& getAxisAlignedBoundingBox() const;
			inline void updateAxisAlignedBoundingBox();
			//@}

			// Since lanes can't easily be completely deleted we erase the core state here
			void eraseState();

			std::size_t calculateMemoryConsumption() const;

			// Made public for binary serialization
			Lane(); // needed for binary serialization

		private:
			// Ids of the start and end node of this lane
			//@{
			unsigned int mStartNodeId;
			unsigned int mEndNodeId;
			//@}

			// The actual positions that make up this lane
			std::vector<LaneNode3D> mPositions;

			// The length of this lane that is used to calculate costs during the search
			UnsignedFloat mLength;

			// Flags to define which types of entities can use this lane.
			// The types have user defined semantics.
			// Currently the default is that any type can use any lane.
			// TODO(vs) Currently hard coded number of entity configurations supported.
			CharArrayBitSet<NUM_SUPPORTED_ENTITY_CONFIGURATIONS> mUsableByEntityConfigurationFlags;

			// Unique ID of a lane type to use for this lane.
			// For technical reasons we can't store a pointer or reference here easily.
			// Instead the collection of lane types is stored in parallel and this id works as index into this collection.
			unsigned int mTypeId;

			// An index into the intersecting lanes vector for this lane pointing to the element directly after the last relevant for this item.
			// This saves memory and still allows accessing all elements in linear time.
			// The value is initialized to zero so that by default no lanes are intersecting.
			unsigned int mIntersectingLaneEndIndex;

			// An axis aligned bounding box around all lanes
			AxisAlignedBoundingBox mAxisAlignedBoundingBox;
			unsigned int mNodesInBoundingBox;
		};
	}
}

#include "qsf_ai/worldModel/trafficLanes/Lane-inl.h"

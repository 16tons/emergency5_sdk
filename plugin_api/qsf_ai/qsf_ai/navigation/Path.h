// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/Export.h"
#include "qsf_ai/navigation/Waypoint.h"
#include "qsf_ai/serialization/QsfBinarySerializationFwd.h"
#include "qsf_ai/navigation/EvadedCollisionInfo.h"

#include <qsf/reflection/CampClass.h>
#include <qsf/base/WeakPtr.h>
#include <qsf/base/error/ErrorHandling.h>
#include <qsf/map/Entity.h>
#include <qsf/serialization/binary/BasicTypeSerialization.h>
#include <qsf/serialization/binary/GlmTypeSerialization.h>
#include <qsf/serialization/binary/StlTypeSerialization.h>

#include <deque>


namespace qsf
{
	namespace ai
	{
		/** Defines a path that an entity can follow by using the AI navigation system.
		* The current simple version consists of a series of waypoints to arrive at.
		* There is a persistent marker to the current waypoint stored with the path.
		* Default copy c'tor and assignment operator are allowed by design.
		*/
		class QSF_AI_API_EXPORT Path
		{
		public:
			// necessary for private access for the binary serialization
			friend struct serialization::serializer<Path>;

			// state of a path - can't reuse logic::ProcessState easily because of adaptation state
			enum State
			{
				UNDER_CONSTRUCTION,
				RUNNING,
				NEEDS_ADAPTATION,	// main difference to TRY_ALTERNATIVE- the path is known to fail if we are not able to adapt the path. The search result overwrites the current data in any case
				TRY_ALTERNATIVE,	// main difference to NEEDS_ADAPTATION - we keep the current data if a new search doesn't reveal a better result.
				RUN_AND_ADAPT,		// we will adapt the path but still use the current path and run on it
				FINISHED,
			};

			Path(const std::deque<Waypoint>& nodes = std::deque<Waypoint>());

			void swap(Path& path);

			// state queries
			//@{
			bool isInClearedState() const; // cleared state means no nodes, state and node counter being ready for a new construction
			State getState() const;
			//@}

			// State change requests
			//@{
			void finishConstruction(); // needs to be called to move from states UNDER_CONSTRUCTION or NEEDS_ADAPTATION to RUNNING and start / continue executing the path
			void requestAdaptation(); // Tell the pathfinding to investigate the path beginning again
			void requestRunAndAdapt(); // Tell the pathfinding to investigate to change the path
			void forceAdaptationRequest(); // Tell the pathfinding to investigate the path beginning again
			void requestTryToFindAnAlternative(); // Tell the pathfinding to search again with current settings and state and see whether a new path is found.
			void onNoAlternativeFound(); // Called from the pathfinding in case an attempt to find an alternative path has failed
			void clear(); // completely obliterate state, nodes are emptied, state is set to UNDER_CONSTRUCTION
			//@}

			// Waypoint data access
			//@{
			const Waypoint& getCurrentNode() const;
			unsigned int getCurrentNodeIndex() const;
			const std::deque<Waypoint>& getNodes() const;
			Waypoint& getWaypoint(unsigned int index); // Throws an exception if index is out of range
			const Waypoint& getWaypoint(unsigned int index) const; // Throws an exception if index is out of range
			//@}

			// Path creation requests
			//@{
			// This contain some checks to avoid adding successive nodes with duplicate positions.
			// It is guaranteed that after this call there is a node with the exact position at the end though it may be that it was there before and no new node added.
			void addNode(const Waypoint& newWP);
			void addNodes(const std::deque<Waypoint>& newWPs);
			void popBack();
			void popFront();
			void prependNodes(const Path& otherPath);
			void prependNode(const Waypoint& newWP);
			void insertNode(const Waypoint& newWP, unsigned int position);
			void insertNodes(const std::deque<Waypoint>& newWPs, unsigned int position);
			void eraseNode(unsigned int position);
			void eraseNodesFromToPathEnd(unsigned int position); //erases this node and all following
			void eraseNodesBetween(unsigned int startErase, unsigned int stopErase); // erases nodes in the interval (startErase, stopErase]
			//@}

			// Path execution: Tries to select the next node in the path and returns whether there is yet another node to reach in the path
			bool selectNextNode();

			// Helper typedef for a pair of indices into the range of nodes inside the path
			typedef std::pair<unsigned int, unsigned int> IdRange;

			// Access to the range of node ids that were reserved for this path
			//@{
			void setReservationWindow(const IdRange& reservedIds);
			const IdRange& getReservationWindow() const;
			//@}

			// Returns whether there is at least one waypoint marked as evasion point in the path part that is not yet executed.
			bool containsPlannedEvasionPoint() const;
			// Removes all planned waypoints marked as evasion waypoints. These are temporary nodes added to evaded entities
			// Writes the removed indices to the reference parameter passed because we need this data in the prototypical use case.
			// The list is sorted in ascending order and starts with current, older evasion points are not counted
			// This list may contain duplicates as for successive evasion points, the same index is removed twice
			void removePlannedEvasionPoints(std::vector<unsigned int>& removedIndices);

			// Access to a list of entities that are currently avoided
			//@{
			void addCurrentlyEvadedEntity(const EvadedCollisionInfo& evaded);
			bool isAmongEvadedEntities(uint64 entityId) const;
			bool isSuccessfullyEvaded(uint64 entityId) const;
			bool isGoalBlockedBy(uint64 entityId) const;
			void removeFromEvadedEntities(uint64 entityId);
			typedef std::vector<EvadedCollisionInfo> EvadedEntitiesList;
			EvadedEntitiesList& getCurrentlyEvadedEntities();
			//@}

			bool mayUseRouterPathAdaptation() const;
			void setMayUseRouterPathAdaptation(bool enable);

			// Call this to inform about the path having a partial result integrated.
			// The only effect is that it allows to restart the search once after the path was finished and didn't achieve a result
			void onPartialLocalRouterResultIntegrated();
			bool mayRestartSearch() const;

		private:
			// internal helper to remove forward-backward jumps introduced by local path modification
			void cleanUp();

			State mState;

			EvadedEntitiesList mCurrentlyEvadedEntities;
			std::deque<Waypoint> mNodes;
			unsigned int mCurrentNode;
			IdRange mReservationRange; // range of nodes inside the path that were reserved successfully, including first excluding second

			// The flag basically tracks whether it makes sense to try to evade via the local route finder.
			// It is true from the beginning and is only set to false once a local route finder result fails to come closer to the goal.
			bool mMayUseRouterAdaptation;

			// The flag tracks whether there was a partial result integrated.
			// These partial results stem from the local route finder and mean that we need to get closer a bit first before we are able to find a perfect, complete solution.
			// Thus it is known that in these cases we are not at the goal in case the path is finished.
			bool mMayRestartSearch;

			QSF_CAMP_RTTI();
		};
	}

	namespace serialization
	{
		template <>
		struct serializer<ai::Path>
		{
			static void serialize(BinarySerializer& serializer, ai::Path& path)
			{
				serializer.serializeAs<short>(path.mState);
				serializer & path.mCurrentNode;
				serializer & path.mReservationRange;
				serializer & path.mNodes;

				unsigned int numEvadedEntities = static_cast<unsigned int>(path.mCurrentlyEvadedEntities.size());
				serializer & numEvadedEntities;
				for (ai::EvadedCollisionInfo& evaded : path.mCurrentlyEvadedEntities)
				{
					serializer & evaded;
				}
			}
		};
	}
}

QSF_CAMP_TYPE(qsf::ai::Path);

#include "qsf_ai/navigation/Path-inl.h"

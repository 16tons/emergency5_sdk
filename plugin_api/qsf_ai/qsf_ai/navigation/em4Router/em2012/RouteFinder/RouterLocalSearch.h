// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/navigation/em4Router/em2012/Bezier.h"
#include "qsf_ai/navigation/em4Router/em2012/RouteFinder/RouterTree.h"
#include "qsf_ai/navigation/em4Router/em2012/RouteFinder/RouterHelper.h"
#include "qsf_ai/navigation/em4Router/em2012/RouteFinder/CollisionBox.h"
#include "qsf_ai/navigation/em4Router/em2012/RouteFinder/RouterSearch.h"
#include "qsf_ai/navigation/em4Router/em2012/RouteFinder/RouterObjectContainer.h"

#include <qsf/base/manager/PagedAllocator.h>
#include <qsf/base/WeakPtr.h>

#include <glm/glm.hpp>

#include <boost/container/flat_map.hpp>
#include <boost/container/flat_set.hpp>

#include <queue>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class BridgeComponent;

	namespace ai
	{
		class ERouterTarget;
		class EGenericObject;
		class ECollisionObject;
		class ELocalMovementState;
	}
}


#define RF_ALLOWCOLLISIONS_ONPATH

// TODO(fw): The defines below could be removed as defines (of course, with the code they enable being permanently enabled!)
#define RF_SHORTEST_PATH

// use memory pool allocator instead of normal new and delete
#define RF_USE_ALLOCATOR


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace ai
	{


		/*!
		 * \brief Helper class to store nodes from start to target.
		 */
		class ERouterLocalSearchNode : public ERouterTreeNode<ERouterLocalSearchNode>
		{
		public:
			// to be used with SerializeFrom
			ERouterLocalSearchNode() :
				mCW(false), mIndex(getUninitialized<size_t>()), mCount(0), mDirection(0.0f), mHasDirection(false), mBridge(nullptr)
			{
			}

			// tree-node for start/end point
			ERouterLocalSearchNode(const glm::vec3& position_) :
				mCW(false), mIndex(-1), mCount(0), mPosition(position_), mDirection(0.0f), mHasDirection(false), mBridge(nullptr)
			{
			}

			// tree-node for end position with direction
			ERouterLocalSearchNode(const glm::vec3& position_, float direction_) :
				mCW(false), mIndex(-1), mCount(0), mPosition(position_), mDirection(direction_), mHasDirection(true), mBridge(nullptr)
			{
			}

			// tree-node for bridge entry/exit/avoidance point
			ERouterLocalSearchNode(const glm::vec3& position_, size_t index_, EGenericObject* bridge_) :
				mCW(false), mIndex(index_), mCount(0), mPosition(position_), mDirection(0.0f), mHasDirection(false), mBridge(bridge_)
			{
			}

			// tree-node for collision-avoidance-point
			ERouterLocalSearchNode(ECollisionObject* box_, bool cw_, size_t index_, const glm::vec3& position_) :
				mBox(box_), mCW(cw_), mIndex(index_), mCount(0), mPosition(position_), mDirection(0.0f), mHasDirection(false), mBridge(nullptr)
			{
			}

			// tree-node for following collision-avoidance-point at same object
			ERouterLocalSearchNode(ECollisionObject* box_, bool cw_, size_t index_, size_t count_, const glm::vec3& position_) :
				mBox(box_), mCW(cw_), mIndex(index_), mCount(count_), mPosition(position_), mDirection(0.0f), mHasDirection(false), mBridge(nullptr)
			{
			}

			// TODO(co) No (save games) serialization required
			// bool SerializeTo(ESerializer *stream_) const;
			// bool SerializeFrom(ESerializer *stream_);

			inline ECollisionObject* GetBox() const { return mBox.get(); }
			inline bool GetCW() const { return mCW; }
			inline size_t GetIndex() const { return mIndex; }
			inline size_t GetCount() const { return mCount; }
			inline const glm::vec3& GetPosition() const { return mPosition; }
			inline bool HasDirection() const { return mHasDirection; }
			inline float GetDirection() const { return mDirection; }
			inline bool IsBridgeTarget() const { return mBridge != nullptr; }
			inline EGenericObject* GetBridge() const { return mBridge; }
			inline void SetDirection(float dir_) { mDirection = dir_; mHasDirection = true; }
			inline void SetPosition(const glm::vec3& position_) { mPosition = position_; }

		private:
			WeakPtr<ECollisionObject> mBox;				// the collision box that created this node
			bool					  mCW;				// move cw around the object
			size_t					  mIndex;			// the index corner of the box to move around (only valid if mBox!=nullptr)
			size_t					  mCount;			// the number of corners used from this box (only valid if mBox!=nullptr)
			glm::vec3				  mPosition;		// the avoidance position
			float					  mDirection;		// the direction at the avoidance position (optional)
			bool					  mHasDirection;	// true if the direction is set
			EGenericObject* 		  mBridge;

		};

		/*!
		 * Class to store object states during the search.
		 */
		class ELocalMovementState
		{
		public:
			ELocalMovementState() :
				mDirection(0.0f),
				mMovement(0)
			{}

			ELocalMovementState(const glm::vec3& pos_, float dir_, char movement_) :
				mPosition(pos_),
				mDirection(dir_),
				mMovement(movement_)
			{ }

			inline const glm::vec3&  GetPosition() const { return mPosition; }
			inline float GetDirection() const { return mDirection; }
			inline char GetMovement() const { return mMovement; }

			// TODO(co) No (save games) serialization required
			/*
			bool SerializeTo(ESerializer *stream_) const
			{
			stream_->Write(mPosition);
			stream_->Write(mDirection);
			stream_->Write(mMovement);
			return true;
			}

			bool SerializeFrom(ESerializer *stream_)
			{
			stream_->Read(mPosition);
			stream_->Read(mDirection);
			stream_->Read(mMovement);
			return true;
			}
			*/

			friend bool operator < (const ELocalMovementState &m0, const ELocalMovementState &m1)
			{
				if (m0.mPosition.x < m1.mPosition.x)
					return true;
				if (m0.mPosition.x == m1.mPosition.x)
				{
					if (m0.mPosition.z < m1.mPosition.z)
						return true;
					if (m0.mPosition.z == m1.mPosition.z)
					{
						if (m0.mPosition.y < m1.mPosition.y)
							return true;
						if (m0.mPosition.y == m1.mPosition.y)
							return m0.mDirection < m1.mDirection;
					}
				}
				return false;
			}

			friend bool operator == (const ELocalMovementState &m0, const ELocalMovementState &m1)
			{
				return m0.mPosition == m1.mPosition && m0.mDirection == m1.mDirection;
			}

			friend bool operator <= (const ELocalMovementState &m0, const ELocalMovementState &m1)
			{
				return m0 < m1 || m0 == m1;
			}

			friend bool operator >(const ELocalMovementState &m0, const ELocalMovementState &m1)
			{
				return m1 < m0;
			}

			//private:
			glm::vec3 mPosition;
			float	  mDirection;
			char	  mMovement;
		};

		/*!
		 * Class to store how the move from one to the next state is done.
		 */
		class ELocalMove
		{
		public:
			ELocalMove() :
				mMovement(0)
			{}

			ELocalMove(char movement_, const ELocalMovementState& prevState_, const ELocalMovementState& nextState_) :
				mMovement(movement_), mPrevState(prevState_), mNextState(nextState_)
			{
				mCurve = Bezier(prevState_.GetPosition(), (prevState_.GetPosition() + nextState_.GetPosition())*0.5f,
					nextState_.GetPosition());
			}

			ELocalMove(char movement_, const Bezier &curve_, const ELocalMovementState& prevState_, const ELocalMovementState& nextState_) :
				mMovement(movement_), mCurve(curve_), mPrevState(prevState_), mNextState(nextState_)
			{
				mCurve.AdjustTargetY(nextState_.GetPosition().y);
			}

			// TODO(co) No (save games) serialization required
			/*
			bool SerializeTo(ESerializer *stream_) const
			{
			stream_->Write(mMovement);
			if (!mCurve.SerializeTo(stream_))
			return false;
			if (!mPrevState.SerializeTo(stream_))
			return false;
			if (!mNextState.SerializeTo(stream_))
			return false;
			return true;
			}

			bool SerializeFrom(ESerializer *stream_)
			{
			stream_->Read(mMovement);
			if (!mCurve.SerializeFrom(stream_))
			return false;
			if (!mPrevState.SerializeFrom(stream_))
			return false;
			if (!mNextState.SerializeFrom(stream_))
			return false;
			return true;
			}
			*/

			char GetMovement() const { return mMovement; }
			const Bezier & GetCurve() const { return mCurve; }
			const ELocalMovementState & GetPrevState() const { return mPrevState; }
			const ELocalMovementState & GetNextState() const { return mNextState; }

		private:
			char mMovement;
			Bezier mCurve;
			ELocalMovementState mPrevState;
			ELocalMovementState mNextState;
		};

		class CollisionInfo
		{
		public:
			CollisionInfo() :
				mDirection(0.0f),
				mIndex(0),
				mIsVehicle(false)
			{
			}

			CollisionInfo(const glm::vec3& pos_, float dir_, ECollisionObject* box_, size_t index_, bool isVehicle_) :
				mPosition(pos_),
				mDirection(dir_),
				mBox(box_),
				mIndex(index_),
				mIsVehicle(isVehicle_)
			{
			}

			// TODO(co) No (save games) serialization required
			// bool SerializeTo(ESerializer *stream_) const;
			// bool SerializeFrom(ESerializer *stream_);

			inline const glm::vec3& GetPosition() const { return mPosition; }
			inline float GetDirection() const { return mDirection; }
			inline const ECollisionObject* GetBox() const { return mBox.get(); }
			inline size_t GetIndex() const { return mIndex; }

			friend bool operator < (const CollisionInfo &ci0, const CollisionInfo &ci1)
			{
				if (ci0.mIsVehicle)  // ci0 reicht, da eh immer beide Boxen hierfür den selben Wert haben müssen
				{
					if (ci0.mBox.get() < ci1.mBox.get())
						return true;
					if (ci0.mBox.get() == ci1.mBox.get())
					{
						if (ci0.mIndex < ci1.mIndex)
							return true;
						if (ci0.mIndex == ci1.mIndex)
						{
							// Directions are expressed in degrees
							int d0 = (int)floorf(ci0.mDirection * 7.5f/*5.0f*/); // rad 0.2f Toleranz
							int d1 = (int)floorf(ci1.mDirection * 7.5f/*5.0f*/);
							if (d0 < d1)
								return true;
							if (d0 == d1)
							{
								int x0 = (int)floorf(ci0.mPosition.x / 2.5f/*1.6f*/); // 1.6 Meter Toleranz
								int x1 = (int)floorf(ci1.mPosition.x / 2.5f/*1.6f*/);
								if (x0 < x1)
									return true;
								if (x0 == x1)
								{
									int z0 = (int)floorf(ci0.mPosition.z / 2.5f/*1.6f*/);
									int z1 = (int)floorf(ci1.mPosition.z / 2.5f/*1.6f*/);
									return (z0 < z1);
								}
							}
						}
					}
				}
				// Personen
				else
				{
					if (ci0.mBox.get() < ci1.mBox.get())
						return true;
					if (ci0.mBox.get() == ci1.mBox.get())
					{
						if (ci0.mIndex < ci1.mIndex)
							return true;
						if (ci0.mIndex == ci1.mIndex)
						{
							int x0 = (int)floorf(ci0.mPosition.x / 0.08f); // 8cm Toleranz
							int x1 = (int)floorf(ci1.mPosition.x / 0.08f); // 8cm Toleranz
							if (x0 < x1)
								return true;
							if (x0 == x1)
							{
								int z0 = (int)floorf(ci0.mPosition.z / 0.08f); // 8cm Toleranz
								int z1 = (int)floorf(ci1.mPosition.z / 0.08f); // 8cm Toleranz
								return (z0 < z1);
							}
						}
					}
				}
				return false;
			}

			friend bool operator == (const CollisionInfo &ci0, const CollisionInfo &ci1)
			{
				// Directions are expressed in degrees
				if (ci0.mIsVehicle)  // ci0 reicht, da eh immer beide Boxen hierfür den selben Wert haben müssen
				{
					return (ci0.mBox.get() == ci1.mBox.get() && ci0.mIndex == ci1.mIndex &&
						fabs(ci0.mDirection - ci1.mDirection) <= 0.2f &&
						fabs(ci0.mPosition.x - ci1.mPosition.x) <= 1.6f &&
						fabs(ci0.mPosition.z - ci1.mPosition.z) <= 1.6f);
				}
				else
				{
					return (ci0.mBox.get() == ci1.mBox.get() && ci0.mIndex == ci1.mIndex &&
						fabs(ci0.mPosition.x - ci1.mPosition.x) <= 0.08f &&
						fabs(ci0.mPosition.z - ci1.mPosition.z) <= 0.08f);
				}
			}

		private:
			glm::vec3				  mPosition;
			float					  mDirection;
			WeakPtr<ECollisionObject> mBox;			// target box with which the collision occurred
			size_t					  mIndex;		// for corner of collision polygon
			bool					  mIsVehicle;	// is source object a vehicle?
		};

		enum SearchItemType
		{
			SIT_STRAIGHT,
			SIT_CURVE,
			SIT_SHUNTING,
			SIT_STRAIGHTSHUNTING
		};

		class ERouterLocalSearch;

		/*!
		 * Class for an element of the search queue.
		 * It stores a possible move from a start to a target node with straight, curve or shunting movement.
		 */
		class SearchItem
		{
		public:
			SearchItem();
			SearchItem(int prevMove_, const ELocalMovementState &state_, ERouterLocalSearchNode* start_, ERouterLocalSearchNode* target_,
				int step_, float startDist_, char movement_, const ERouterTarget *globalTarget_);
			SearchItem(int prevMove_, const ELocalMovementState &state_, ERouterLocalSearchNode* start_, ERouterLocalSearchNode* target_,
				int step_, float startDist_, char movement_, const Bezier &curve_, const ERouterTarget *globalTarget_);
			SearchItem(int prevMove_, const ELocalMovementState &state_, ERouterLocalSearchNode* start_, ERouterLocalSearchNode* target_,
				int step_, float startDist_, char movement_, const Bezier &curve0_, const Bezier &curve1_, const ERouterTarget *globalTarget_);

			// TODO(co) No (save games) serialization required
			// bool SerializeTo(ESerializer *stream_) const;
			// bool SerializeFrom(ESerializer *stream_, ERouterLocalSearch *search_);

			SearchItemType GetType() const { return mType; }
			const ELocalMovementState & GetState() const { return mState; }
			int GetPrevMove() const { return mPrevMove; }
			ERouterLocalSearchNode*  GetStart() const { return mStart; }
			ERouterLocalSearchNode*  GetTarget() const { return mTarget; }
			int GetStep() const { return mStep; }
			float GetStartDistance() const { return mStartDistance; }
			float GetTargetDistance() const { return mTargetDistance; }
			float GetEstimate() const { return mEstimate; }
			char GetMovement() const { return mMovement; }
			const Bezier & GetCurve0() const { return mCurve0; }
			const Bezier & GetCurve1() const { return mCurve1; }

			friend bool operator < (const SearchItem &si0, const SearchItem &si1)
			{
				return si0.mEstimate > si1.mEstimate;
			}

		private:
			SearchItemType mType;

			ELocalMovementState mState;			// movement state at the start node
			int mPrevMove;
			ERouterLocalSearchNode* mStart;
			ERouterLocalSearchNode* mTarget;
			int mStep;
			float mStartDistance;				// move distance of the start node from the beginning
			float mTargetDistance;				// move distance of the target node from the beginning
			float mEstimate;					// estimated total move distance from the beginning to the final target
			char mMovement;
			Bezier mCurve0;
			Bezier mCurve1;
		};

		/*!
		 * Class to represent a point that is used during the search when a collision is encounteres to try to
		 * avoid the collision.
		 */
		class AvoidancePoint
		{
		public:
			AvoidancePoint() :
				mBox(nullptr), mIndex(0), mBridge(nullptr), mCw(true)
			{
			}
			AvoidancePoint(ECollisionObject* b, size_t i, EGenericObject *bridge, bool cw) :
				mBox(b), mIndex(i), mBridge(bridge), mCw(cw)
			{
			}

			// TODO(co) No (save games) serialization required
			// bool SerializeTo(ESerializer *stream_) const;
			// bool SerializeFrom(ESerializer *stream_);

			friend bool operator < (const AvoidancePoint &ap0, const AvoidancePoint ap1)
			{
				if (ap0.mBox.get() < ap1.mBox.get())
					return true;
				if (ap0.mBox.get() == ap1.mBox.get())
				{
					if (ap0.mIndex < ap1.mIndex)
						return true;
					if (ap0.mIndex == ap1.mIndex)
					{
						if (ap0.mCw < ap1.mCw)
							return true;
						if (ap0.mCw == ap1.mCw)
							return (ap0.mBridge < ap1.mBridge);
					}
				}
				return false;
			}

			friend bool operator == (const AvoidancePoint &ap0, const AvoidancePoint &ap1)
			{
				return (ap0.mBox.get() == ap1.mBox.get() && ap0.mIndex == ap1.mIndex && ap0.mCw == ap1.mCw && ap0.mBridge == ap1.mBridge);
			}

			//private:
			WeakPtr<ECollisionObject> mBox;
			size_t mIndex;
			EGenericObject* mBridge;
			bool mCw;
		};

		/*!
		 * \brief Class that implements search for a path without using streets.
		 */
		class ERouterLocalSearch : public ERouterSearch
		{
		public:
			ERouterLocalSearch();
			ERouterLocalSearch(EGenericObject* obj_, const ERouterObjectState &state_, const ERouterTarget *target_, int flags_);
			virtual ~ERouterLocalSearch();

			// TODO(co) No (save games) serialization required
			// virtual bool SerializeTo(ESerializer *stream_) const;
			// virtual bool SerializeFrom(ESerializer *stream_);

			virtual void Compute(double timeslice_);		// update the search-computation
			virtual ERouterSearchResult GetResult(ERouterPath &path_);

			virtual void DebugDraw(FastDebugDraw& fastDebugDraw) const;

		#ifdef ROUTER_DEBUG
			void PrintSearchQueue() const;
		#endif

			ERouterLocalSearchNode*  GetTreeNode(int id_) const;

		protected:
			enum ESKIP_CHECK
			{
				SKIP_IGNORE,
				SKIP_DELETE,
				SKIP_KEEP,
			};
			ESKIP_CHECK CheckSkipObstacle(ECollisionObject* obj_);

		private:
		#ifdef RF_SHORTEST_PATH
			typedef std::priority_queue<SearchItem> SearchQueue;
		#else
			typedef std::deque<SearchItem> SearchQueue;
		#endif

		#ifdef RF_USE_ALLOCATOR
			typedef PagedAllocator<ERouterLocalSearchNode> NodeAllocator;
			static NodeAllocator nodeAllocator;
		#endif

			void ClearSearchTree();

			int TryMovement(const SearchItem &item_, ELocalMovementState &targetState_);

			int TryStraightMove(const SearchItem &item_, ELocalMovementState &targetState_, bool dontHandleCollision_ = false);
			int TryCurveMove(const SearchItem &item_, ELocalMovementState &targetState_);
			int TryShuntingMove(const SearchItem &item_, ELocalMovementState &targetState_);

			void HandleCollision(const SearchItem &item_, ECollisionObject* box_);

			bool IsObjectInWater(const SearchItem &item_, ECollisionBox &box_, const CollisionList &collList_);

			bool CheckBridge(ERouterLocalSearchNode* start_, ERouterLocalSearchNode* target_,
				const ELocalMovementState &startState_, int lastMove_, int lastStepCount_, float startDistance_);

			bool searchClosestPosition(const glm::vec3& startPosition, const glm::vec3& endPosition, uint32 bridgeId, uint64 bridgeEntityId, uint32 targetWalkableLevel, glm::vec3& outClosestPosition);

			void BuildBezierPath(const std::vector<int> &revMoves_);

		#ifdef RF_ALLOWCOLLISIONS_ONPATH
			bool AddCollisionNode(const SearchItem &item_, ERouterLocalSearchNode* node_, ERouterLocalSearchNode* start_, ERouterLocalSearchNode* target_, bool tooNear_ = false);
		#else
			bool AddCollisionNode(const SearchItem &item_, ERouterLocalSearchNode* node_, ERouterLocalSearchNode* start_, ERouterLocalSearchNode* target_);
		#endif
			void AddSearchQueueItem(const SearchItem &item_);
			void AddTargetChilds(ERouterLocalSearchNode* node_, bool addQueueItems_ = false);
			int AddMoveToHistory(int prevMove_, const ELocalMove &move_);

		#ifdef RF_ALLOWCOLLISIONS_ONPATH
			void AddQueueItems(bool tooNear_, ERouterLocalSearchNode* start_, ERouterLocalSearchNode* target_, const ELocalMovementState &startState_,
				int lastMove_, int lastStepCount_, float startDistance_, bool noShunting = false);
			void AddQueueItemsFront(bool tooNear_, ERouterLocalSearchNode* start_, ERouterLocalSearchNode* target_, const ELocalMovementState &startState_,
				int lastMove_, int lastStepCount_, float startDistance_, bool noShunting);
			void AddQueueItemsBack(bool tooNear_, ERouterLocalSearchNode* start_, ERouterLocalSearchNode* target_, const ELocalMovementState &startState_,
				int lastMove_, int lastStepCount_, float startDistance_, bool noShunting);
			void AddQueueItemsSide(bool tooNear_, ERouterLocalSearchNode* start_, ERouterLocalSearchNode* target_, const ELocalMovementState &startState_,
				int lastMove_, int lastStepCount_, float startDistance_, bool noShunting, float turnAngle_);
		#else
			void AddQueueItems(ERouterLocalSearchNode* start_, ERouterLocalSearchNode* target_, const ELocalMovementState &startState_,
				int lastMove_, int lastStepCount_, float startDistance_, bool noShunting = false);
			void AddQueueItemsFront(ERouterLocalSearchNode* start_, ERouterLocalSearchNode* target_, const ELocalMovementState &startState_,
				int lastMove_, int lastStepCount_, float startDistance_, bool noShunting);
			void AddQueueItemsBack(ERouterLocalSearchNode* start_, ERouterLocalSearchNode* target_, const ELocalMovementState &startState_,
				int lastMove_, int lastStepCount_, float startDistance_, bool noShunting);
			void AddQueueItemsSide(ERouterLocalSearchNode* start_, ERouterLocalSearchNode* target_, const ELocalMovementState &startState_,
				int lastMove_, int lastStepCount_, float startDistance_, bool noShunting, float turnAngle_);
		#endif

			ERouterLocalSearchNode* CreateNode(const glm::vec3& pos_);
			ERouterLocalSearchNode* CreateNode(const glm::vec3& pos_, float dir_);
			ERouterLocalSearchNode* CreateNode(const glm::vec3& pos_, size_t index_, EGenericObject *bridge_);
			ERouterLocalSearchNode* CreateNode(ECollisionObject* box_, bool cw_, size_t index_, const glm::vec3& pos_);
			ERouterLocalSearchNode* CreateNode(ECollisionObject* box_, bool cw_, size_t index_, size_t count_, const glm::vec3& pos_);

			float mFindBoxStepSize;


			typedef boost::container::flat_map<int, ERouterLocalSearchNode*> TreeNodeMap;
			TreeNodeMap mTreeNodes;						// storage for treenodes of mSearchTree

			ERouterLocalSearchNode* mSearchTree;		// the search tree to the target
			SearchQueue mSearchQueue;					// the queue of potential movements to the target

			typedef boost::container::flat_map<AvoidancePoint, ERouterLocalSearchNode*> CollisionNodeMap;
			CollisionNodeMap mCollisionNodeMap;			// map to store created search tree nodes (just to find out if the same avoidance point already exists)

			ELocalMovementState mMovementState;			// first (initial) movement state
			typedef boost::container::flat_map<int, ELocalMove> LocalMoveMap;		// FIXME: why use map here?
			typedef boost::container::flat_map<int, int> MovementHistory;
			LocalMoveMap mMoveMap;						// store all (collisionfree) moves
			MovementHistory mHistory;					// store the previous move for each move

			typedef boost::container::flat_set<AvoidancePoint> AvoidancePointSet;
			AvoidancePointSet mReachedPoints;		// there's a guaranteed way to those points (the points are polygon corners)

			typedef boost::container::flat_set<CollisionInfo> CollisionHistory;
			CollisionHistory mCollisionHistory;	// store all collisions which happen during the search

			boost::container::flat_set<int> mIgnoreInitialCollisionObjectIDs;

			float mAvoidanceDistance;		// distance to extend avoidance points of polygons
		#ifdef RF_ALLOWCOLLISIONS_ONPATH
			float mAvoidanceDistanceForToNearOnPurpose;		// distance to extend avoidance points of polygons for vehicles for toNearOnPurpose Move
			bool  mToNearOnPurposeAllow;
		#endif
			float mMinCollisionDistance;

			int mFinalMove;
			float mMinDistTarget;				// mimimum distance to target so far
			int mTotalSteps;					// number of steps to reach the target state

			bool mAllowBackward;
			bool mAllowHardTurns;
			bool mAllowCurves;
			bool mAllowShunting;

			float mBezierFactor;

			float mPrecheckDistance;

			ECollisionBox mCollisionBox;

			ECollisionVObject* mWaterObject;

			float mUnitsPerTick;			// average stepsize for one move

			// variables to monitor queue and tree sizes
			int mStepCount;
			int mMaxQueueSize;
			int mMaxTreeSize;

			// The rest here is debugging stuff
			//  -> Actually only for internal builds (additional check to find erroneous behavior), but we need to make sure end-user SDK is the same with or without ENDUSER define
			RouterTimer mTimers[23];	// Timers for performance measurement
			std::vector<ECollisionBox> mLastStepBoxes;
			std::vector<Bezier> mTries;
			int mDebugLastCollisionObjectID;
		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // ai
} // qsf

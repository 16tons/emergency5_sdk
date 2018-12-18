// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/serialization/binary/BinarySerializer.h>

#include <glm/detail/type_mat.hpp>

#include <boost/container/flat_map.hpp>

// extern BLVar e4_rf_debug;	// TODO(co) Migrate BLVar to settings group or debug settings group

#include "qsf_ai/Export.h"
#include "qsf_ai/navigation/em4Router/em2012/RouteFinder/RouterHelper.h"
#include "qsf_ai/navigation/em4Router/em2012/RouteFinder/RouterPolygon.h"
#include "qsf_ai/navigation/em4Router/em2012/RouteFinder/CollisionBox.h"
#include "qsf_ai/navigation/em4Router/em2012/RouteFinder/RouterTarget.h"
#include "qsf_ai/navigation/em4Router/em2012/RouteFinder/RouterPath.h"
#include "qsf_ai/navigation/em4Router/em2012/RouteFinder/RouterObjectContainer.h"
#include "qsf_ai/navigation/em4Router/em2012/RouteFinder/RouterEnvironment.h"
#include "qsf_ai/navigation/em4Router/em2012/RouteFinder/RouterTree.h"
#include "qsf_ai/navigation/em4Router/em2012/RouteFinder/RouterSearch.h"
#include "qsf_ai/navigation/em4Router/em2012/RouteFinder/RouterLocalSearch.h"
#include "qsf_ai/navigation/em4Router/em2012/RouteFinder/RouterObjectState.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace ai
	{


		/*!
		 * \brief Singleton class to compute routes.
		 * The class is also an interface (facade) to add, remove or change objects and streets to the router-environment.
		 */
		class QSF_AI_API_EXPORT ERouteFinder : public BLSingleton<ERouteFinder>
		{
			DECLARESINGLETON(ERouteFinder);

		public:
			// TODO(co) We don't need the street handling in here
			/*
			void AddStreet(EStreet *street_, bool updateStreetGraph_=true);
			void UpdateStreet(EStreet *street_, bool updateStreetGraph_=true);
			void RemoveStreet(EStreet *street_, bool updateStreetGraph_=true);
			void ComputeGameStreetGraph();
			*/

			void AddObject(EActor* obj_);
			bool UpdateObject(EGenericObject* obj_);
			void UpdateObjectMovingStatus(EGenericObject* obj_);
			void RemoveObject(EActor* obj_);
			void UpdateCollisionBoxes(EGenericObject* obj_);
			void ComputeAvoidancePolygon(EGenericObject* obj_);
			void ChangeObjectTerrain(EGenericObject* obj_, ETerrain* terrain_ = nullptr);
			// owner: -1: world, 0-x: player
			void SetOwnerPlayer(EActor* obj_, int owner_);

			void UpdateVirtualObject(EVirtualObject* vobj_);
			void UpdateVirtualObjectTerrain(EVirtualObject* vobj_);	// Use when only terrain has changed, it's much faster than UpdateVirtualObject

			void GetPolygonOfCollisionBox(EGenericObject* obj_, ERouterPolygon& polygon_);

			void Clear();

			bool IsInWater(EGenericObject* obj_, const ERouterObjectState& state_, float safety_ = 0.0f, bool checkForAllRotations_ = false);
			bool IsInWater(const glm::vec3& position_);
			void GetExtendedCollisionBoxesXY(std::vector<ECollisionBox>& resultBoxes_, EGenericObject* obj_, float extend_) const;
			bool HasCollision(EGenericObject* obj_, const ERouterObjectState& state_, int flags_, float safety_, bool useSpecialTerrain_, EObjectTerrainClass specialTerrain_);
			bool HasCollision(EGenericObject* obj_, const ERouterObjectState& state_, int flags_ = ERF_NONE, float safety_ = 0.0f);
			bool HasCollision(const glm::vec3& boxExtents_, int owner_, const ERouterObjectState& state_, EObjectTerrainClass terrain_, int flags_ = ERF_NONE);
			bool HasCollision(EActor* obj_, ECollisionBox& box_, int flags_, bool useSpecialTerrain_ = false, EObjectTerrainClass specialTerrain_ = EOTC_INVALID);
			bool FindCollisions(EGenericObject* obj_, const ERouterObjectState& state_, CollisionList& collisions_, int flags_ = ERF_NONE, float safety_ = 0.0f, long timeStampForUnmoved_ = 0);
			bool FindCollisions(EGenericObject* obj_, ECollisionBox& box_, CollisionList& collisions_, int flags_ = ERF_NONE, long timeStampForUnmoved_ = 0);
			bool FindCollisions(const glm::vec3& boxExtents_, int owner_, const ERouterObjectState& state_, CollisionList& collisions_, EObjectTerrainClass terrain_, int flags_ = ERF_NONE, long timeStampForUnmoved_ = 0);

			const CollisionObjectArray* GetCollisionBoxes(EGenericObject* obj_) const;
			ECollisionBox * GetCollisionBox(EGenericObject* obj_) const;
			const CollisionObjectArray* GetCollisionObjects(EActor* actor_) const;
			ECollisionObject*  GetCollisionObject(EActor* actor_) const;	// only first collision object
			int GetNrCollisionBoxes(EGenericObject* obj_) const;
			ECollisionBox * GetCollisionBox(EGenericObject* obj_, int index_) const;
			bool GetDistanceVectorToObject(EGenericObject* obj_, const glm::vec3& pos_, glm::vec3&  dist_, float* fdist_ = nullptr);
			bool GetDistanceVectorToObject(EGenericObject* obj_, const ERouterObjectState& state_, const glm::vec3& pos_, glm::vec3&  dist_, float* fdist_ = nullptr);

			int checkBridgeId(const glm::vec3& position) const;	// -1 for no bridge, >0 gives back bridgeId
			BridgeComponent* findBridge(const glm::vec3& position, bool ignoreWalkable = false) const;

			float GetSpeedFactor(EGenericObject* obj_, const glm::vec3& pos_) const;
			float GetSpeedFactor(EObjectTerrainClass class_, const glm::vec3& pos_) const;
			bool FindAvailablePosition(EGenericObject* obj_, glm::vec3& pos_, float radius_ = 1.0f, int flags_ = ERF_NONE);
			bool FindAvailablePosition(EGenericObject* obj_, glm::vec3& searchPos_, const glm::vec3& checkPos_, float radius_ = 1.0f, int flags_ = ERF_NONE);
			bool FindFreePosition(EGenericObject* obj_, glm::vec3& pos_, float radius_ = 2.0f, float safety_ = 0.01f, int flags_ = ERF_NONE) const;
			bool FindFreePosition(EGenericObject* obj_, glm::vec3& pos_, const glm::mat3x3& rot_,float radius_ = 2.0f, float safety_ = 0.01f, int flags_ = ERF_NONE) const;
			bool Traceline(const glm::vec3& start_, const glm::vec3& end_,
						   EObjectTerrainClass terrainClass_,
						   ERouterTraceLineFlagsSet flags_=RTLF_NONE,
						   int testedActorTypes_=0,	// TODO(co) Was "int testedActorTypes_=EAT_VIRTUAL|EAT_OBJECT|EAT_HOUSE|EAT_VEHICLE|EAT_PERSON,"
						   EActor** hitObject_ = nullptr,
						   glm::vec3* hitCoordinate_ = nullptr,
						   const EGenericObject* ignoreObject_ = nullptr,
						   const EGenericObject* ignoreObject2_ = nullptr);

			void OptimizeCollisionBoxes();
			void OptimizeAvoidancePolygons();

			// TODO(co) No (save games) serialization required
			// bool SerializeTo(ESerializer *stream_);
			// bool SerializeFrom(ESerializer *stream_);

			// search path from start to target without streets
			void RequestPath(EGenericObject* obj_, const ERouterObjectState& startState_, const ERouterTarget *target_, int flags_ = 0);
			void RequestPathBackwards(EGenericObject* obj_, const ERouterObjectState& startState_, const ERouterTarget *target_, int flags_ = 0);
			// search path from start- to target-position with only using streets - TODO(co) We don't need the street handling in here
			// void RequestStreetPath(EGenericObject* obj_, const ERouterObjectState& startState_, const ERouterTarget *target_, int flags_, EStreetType type_, EStreetRouterSearchMode searchMode_=SRSM_SHORTEST_PATH);
			// search path for helicopter - TODO(co) We don't need the helicopter handling in here
			// void RequestHeliPath(EGenericObject* obj_, const ERouterObjectState& startState_, const ERouterTarget *target_, int flags_=0, float landingDirection_=-1.0f);

			void CancelRequest(EGenericObject* obj_);
			bool HasRequest(EGenericObject* obj_);

			ERouterSearchResult GetResult(EGenericObject* obj_, ERouterPath &path_);
			ERouterSearchResult GetResultBackwards(EGenericObject* obj_, ERouterPath &path_);

			void Update();

			void DebugDraw(FastDebugDraw& fastDebugDraw) const;

			// debugging functions
			void Step();

			// TODO(co) I assume we can remove "ERouterEnvironment::ExportObjects()"
			// void ExportObjects() const;

			// TODO(co) We don't need the street handling in here
			// void UpdateStreets();

			void RemoveAllBlockedStaticObjects();

			// TODO(co) We don't need the street handling in here
			// bool CheckStreetConnections(EStreetType type_);
			// bool IsStreetNodeArrayConnected(EStreetType type_);

			ERouterEnvironment & GetEnvironment() { return mEnvironment; }

			// TODO(co) We don't need the route finder test at the moment
			/*
			EPathTest* AddPathTest();
			EPathTest* FindPathTest(const BLString &name_);
			void DeletePathTest(EPathTest* test_);
			void DeleteAllPathTests();
			inline const BLList<EPathTest*> &GetPathTests() const { return mPathTests; }
			void RunPathTests();

			// scene load&save
			bool Save(BLFile *file_);
			bool Load(BLFile *file_);
			*/

		private:
			ERouteFinder();
			~ERouteFinder();

			void AddToScheduleByPriority(ERouterSearch* search_);

			ERouterEnvironment mEnvironment;

			typedef boost::container::flat_map<EGenericObject*, ERouterSearch*> SearchMap;
			SearchMap mSearches;

			typedef std::list<ERouterSearch*> Schedule;
			Schedule mSchedule;

			ERouterPath mlastPathForDebugDraw;	///< Actually only for internal builds (additional check to find erroneous behavior), but we need to make sure end-user SDK is the same with or without ENDUSER define

			// BLList<EPathTest*> mPathTests;	// TODO(co) We don't need the route finder test at the moment
		};

//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // ai
} // qsf

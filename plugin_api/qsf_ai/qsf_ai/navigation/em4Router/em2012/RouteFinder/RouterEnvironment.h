// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/Export.h"
#include "qsf_ai/navigation/em4Router/em2012/RouteFinder/CollisionBox.h"
#include "qsf_ai/navigation/em4Router/em2012/RouteFinder/RouterHelper.h"
#include "qsf_ai/navigation/em4Router/em2012/RouteFinder/routefindertypes.h"
#include "qsf_ai/navigation/em4Router/em2012/RouteFinder/RouterObjectContainer.h"
#include "qsf_ai/navigation/em4Router/em2012/terrain.h"

#include <map>
#include <vector>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class FastDebugDraw;

	namespace ai
	{
		class EActor;
		class EPerson;
		class ETerrain;
		class ERouterPolygon;
		class EGenericObject;
		class EVirtualObject;
		class ERouterObjectState;
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace ai
	{


		//[-------------------------------------------------------]
		//[ Classes                                               ]
		//[-------------------------------------------------------]
		/*!
		 * \brief Class to store the environment that is used to compute routes.
		 */
		class QSF_AI_API_EXPORT ERouterEnvironment
		{
		public:
			ERouterEnvironment();
			~ERouterEnvironment();

			void AddObject(EActor* obj_);
			bool UpdateObject(EGenericObject* obj_);
			void UpdateObjectMovingStatus(EGenericObject* obj_);
			void RemoveObject(EActor* obj_);
			void UpdateCollisionBoxes(EGenericObject* obj_);
			void ComputeAvoidancePolygon(EGenericObject* obj_);
			void ChangeObjectTerrain(EGenericObject* obj_, ETerrain* terrain_);
			void SetOwnerPlayer(EActor* obj_, int owner_);

			void UpdateVirtualObject(EVirtualObject *vobj_);
			void UpdateVirtualObjectTerrain(EVirtualObject *vobj_);	// Use when only terrain has changed, it's much faster than UpdateVirtualObject

			void GetPolygonOfCollisionBox(EGenericObject* obj_, ERouterPolygon &polygon_);

			// TODO(co) No (save games) serialization required
			// bool SerializeTo(ESerializer *stream_);
			// bool SerializeFrom(ESerializer *stream_);

			bool IsInWater(EGenericObject* obj_, const ERouterObjectState &state_, float safety_, bool checkForAllRotations_);
			bool IsInWater(const glm::vec3& position_);
			bool HasCollision(const glm::vec3& boxExtents_, int owner_, const ERouterObjectState &state_, EObjectTerrainClass terrain_, int flags_);
			bool HasCollision(EGenericObject* obj_, const ERouterObjectState &state_, int flags_, float safety_, bool useSpecialTerrain_ = false, EObjectTerrainClass specialTerrain_ = EOTC_ANYTHING);
			bool HasCollision(EActor* obj_, ECollisionBox &box_, int flags_, bool useSpecialTerrain_ = false, EObjectTerrainClass specialTerrain_ = EOTC_ANYTHING);
			bool FindCollisions(EGenericObject* obj_, const ERouterObjectState &state_, CollisionList &collisions_, int flags_, float safety_, long timeStampForUnmoved_);
			bool FindCollisions(EGenericObject* obj_, ECollisionBox &box_, CollisionList &collisions_, int flags_, long timeStampForUnmoved_);
			bool FindCollisions(const glm::vec3& boxExtents_, int owner_, const ERouterObjectState &state_, CollisionList &collisions_, EObjectTerrainClass terrain_, int flags_, long timeStampForUnmoved_);

			void GetExtendedCollisionBoxes(std::vector<ECollisionBox> &resultBoxes_, EGenericObject* obj_, float extend_) const;
			void GetExtendedCollisionBoxesXY(std::vector<ECollisionBox> &resultBoxes_, EGenericObject* obj_, float extend_) const;

			ECollisionObject*  GetCollisionObject(int id_) const;
			void FindBoxes(const glm::vec3& min_, const glm::vec3& max_, int owner_, CollisionList &boxes_, int flags_) const;
			//void FindBoxes(const glm::vec3& min_, const glm::vec3& max_, CollisionList &boxes_, CollisionList &erasedboxes_, int flags_);
			void FindBoxesOnLine(const glm::vec3& start_, const glm::vec3& end_, int owner_, CollisionList &boxes_, int flags_) const;

			const CollisionObjectArray* GetCollisionBoxes(EGenericObject* obj_) const;
			ECollisionBox * GetCollisionBox(EGenericObject* obj_) const;	// only first collision box
			const CollisionObjectArray* GetCollisionObjects(EActor *actor_) const;
			ECollisionObject*  GetCollisionObject(EActor *actor_) const;	// only first collision object
			int GetNrCollisionBoxes(EGenericObject* obj_) const;
			ECollisionBox * GetCollisionBox(EGenericObject* obj_, size_t index_) const;
			bool GetDistanceVectorToObject(EGenericObject* obj_, const glm::vec3& pos_, glm::vec3&  dist_, float *fdist_);
			bool GetDistanceVectorToObject(EGenericObject* obj_, const ERouterObjectState &state_, const glm::vec3& pos_, glm::vec3&  dist_, float *fdist_);

			float GetSpeedFactor(EGenericObject* obj_, const glm::vec3& pos_) const;
			float GetSpeedFactor(EObjectTerrainClass class_, const glm::vec3& pos_) const;
			bool FindAvailablePosition(EGenericObject* obj_, glm::vec3& searchPos_, const glm::vec3& checkPos_, float radius_, int flags_ = ERF_NONE);
			bool FindFreePosition(EGenericObject* obj_, glm::vec3& pos_, float radius_, float safety_, int flags_ = ERF_NONE, int bridgeId = -1) const;
			bool FindFreePosition(EGenericObject* obj_, glm::vec3& pos_, const glm::mat3x3& rot_, float radius_, float safety_, int flags_ = ERF_NONE, int bridgeId = -1) const;

			bool Traceline(const glm::vec3& start_, const glm::vec3& end_,
				EObjectTerrainClass terrainClass_,
				ERouterTraceLineFlagsSet flags_ = RTLF_NONE,
				int testedActorTypes_ = 0,	// TODO(co) Was "int testedActorTypes_=EAT_VIRTUAL|EATF_GENERICOBJECT"
				EActor** hitObject_ = nullptr,
				glm::vec3* hitCoordinate_ = nullptr,
				const EGenericObject* ignoreObject_ = nullptr,
				const EGenericObject* ignoreObject2_ = nullptr);

			void Clear();

			void DebugDraw(FastDebugDraw& fastDebugDraw) const;

			void OptimizeCollisionBoxes();
			void OptimizeAvoidancePolygons();

			// TODO(co) I assume we can remove "ERouterEnvironment::ExportObjects()"
			// void ExportObjects() const;

			//ECollisionBox* CreateEntityBox(EGenericObject* obj_, BaseGameEntity_cl* entity_) const;
			ECollisionBox* CreateCollisionBoxForPersons(EPerson* person_) const;

			void ComputeCollisionBoxes(EActor* obj_, CollisionObjectArray& boxes_) const;
			//void ComputeCollisionBoxesFromPhysicsDesc(EGenericObject* obj_, EPrototypeChild *child_, EPrototypePhysicsDesc& physicsDesc, CollisionObjectArray& boxes_) const;
			//void ComputeSingleCollisionBoxFromPhysicsDesc(EGenericObject* obj_, EPrototypeChild *child_, EPrototypePhysicsDesc& physicsDesc, CollisionObjectArray& boxes_) const;

			void RemoveAllBlockedStaticObjects();

			// TODO(co) We don't need the street handling in here
			// ERouterStreetEnvironment& GetStreets(EStreetType type_)	{ return mStreetEnvironment[type_]; }

		protected:

			enum CHECK_POS_FLAGS
			{
				CPS_DEBUG_FIND_AVAILABLE,
				CPS_FIND_AVAILABLE,
				CPS_DEBUG_FIND_FREE,
				CPS_FIND_FREE,
			};

			bool CheckAvailablePositions(EGenericObject* obj_, glm::vec3& searchPos_, const glm::vec3& checkPos_, float radius_, const std::vector<ECollisionBox> &objBoxes_, CollisionList & boxes_, int flags_, CHECK_POS_FLAGS cpflags_);
			bool CheckFreePositions(EGenericObject* obj_, glm::vec3& pos_, const glm::mat3x3& rot_, float radius_, std::vector<ECollisionBox> &objBoxes_, CollisionList boxes_, int bridgeId, int flags_, CHECK_POS_FLAGS cpflags_) const;

		private:
			bool ComputeCollisionBox(EGenericObject* obj_, ECollisionBox &box_) const;

			// TODO
			// - data-structure to store obstacles with fast insert/remove and very fast finding

			// map objects to collision-objects
			// FIXME: should i rather store pointers to arrays here to avoid copy-ops?
			CollisionObjectMap mObjectMap;

			// store boxes by ids for fast lookup
			CollisionObjectIdMap mColObjIdMap;

			// TODO(co) We don't need the street handling in here
			// map streets to street-segments
			// ERouterStreetEnvironment	mStreetEnvironment[ST_MAX];

			ERouterObjectContainer mObjects;
			ERouterObjectContainer mVirtualObjects;

			// For debugging
			//  -> Actually only for internal builds (additional check to find erroneous behavior), but we need to make sure end-user SDK is the same with or without ENDUSER define
			typedef std::map<glm::vec3, int, CompareVec> PointMap;
			mutable PointMap mFreePoints;
			mutable PointMap mAvailPoints;
			ECollisionBox mDebugCollisionBox;

		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // ai
} // qsf

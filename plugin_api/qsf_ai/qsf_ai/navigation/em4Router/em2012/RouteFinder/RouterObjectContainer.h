// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/navigation/em4Router/em2012/terrain.h"

#include <glm/glm.hpp>

#include <unordered_map>
#include <unordered_set>
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
		class ECollisionBox;
		class ECollisionObject;
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace ai
	{


		// FIXME: would a std::list be better here?
		typedef std::vector<ECollisionObject*> CollisionObjectArray;
		typedef std::unordered_map<EActor*, CollisionObjectArray> CollisionObjectMap;
		typedef std::unordered_map<int, ECollisionObject*> CollisionObjectIdMap;
		typedef std::unordered_set<ECollisionObject*> CollisionList;	// TODO: is set still needed?

		/*!
		 * \brief Class to store obstacle objects for fast lookup for the router.
		 */
		class ERouterObjectContainer
		{
		public:
			ERouterObjectContainer();
			~ERouterObjectContainer();

			// TODO(co) No (save games) serialization required
			// bool SerializeTo(ESerializer *stream_);
			// bool SerializeFrom(ESerializer *stream_);

			void Add(ECollisionObject* box_);
			void AddVirtual(ECollisionObject* box_);
			void Remove(ECollisionObject* box_);
			void Update(ECollisionObject* box_);

			void Clear();

			bool HasCollision(EActor* obj_, ECollisionBox& box_, int flags_, bool useSpecialTerrain_ = false, EObjectTerrainClass specialTerrain_ = EOTC_INVALID);
			bool FindCollisions(EActor* obj_, ECollisionBox& box_, CollisionList& collisions_, int flags_, long timeStampForUnmoved_ = 0, bool useSpecialTerrain_ = false, EObjectTerrainClass specialTerrain_ = EOTC_INVALID);

			bool IsInWater(const glm::vec3& position_);

			float GetSpeedFactor(EGenericObject* obj_, const glm::vec3& pos_) const;
			float GetSpeedFactor(EObjectTerrainClass class_, const glm::vec3& pos_) const;

			void FindBoxes(const glm::vec3& min_, const glm::vec3& max_, int owner_, CollisionList& boxes_, int flags_) const;
			void FindBoxesOnLine(const glm::vec3& start_, const glm::vec3& end_, int owner_, CollisionList& boxes_, int flags_) const;

			void OptimizeCollisionBoxes(CollisionObjectMap& boxMap, CollisionObjectIdMap &colObjIdMap);

			void DebugDraw(FastDebugDraw& fastDebugDraw) const;

		private:
			void ComputeGridLimits(const glm::vec3& min_, const glm::vec3& max_, int& x0_, int& y0_, int& x1_, int& y1_) const;
			int GridIndex(float x_, float y_) const;
			bool TestForMerge(ECollisionBox* box_, size_t& index1_, size_t& index2_);

			int mWidth;
			int mHeight;
			int mHalfMaxSize;
			int mShiftFactor;
			std::vector<CollisionObjectArray> mCells;

			mutable int mLastChecked;
		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // ai
} // qsf

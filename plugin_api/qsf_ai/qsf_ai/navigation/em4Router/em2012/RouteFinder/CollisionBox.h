// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/Export.h"
#include "qsf_ai/navigation/em4Router/em2012/RouteFinder/RouterPolygon.h"
#include "qsf_ai/navigation/em4Router/em2012/OrientedBox.h"
#include "qsf_ai/navigation/em4Router/em2012/terrain.h"

#include <qsf/base/WeakPtr.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class FastDebugDraw;
	class PolygonComponent;

	namespace ai
	{
		class EActor;
		class EVirtualObject;
		class ERouterObjectState;
		class ECollisionBox;
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
		//[ Global definitions                                    ]
		//[-------------------------------------------------------]
		enum ECollisionBoxType
		{
			ECB_STATIC,					// never changes during the game
			ECB_DYNAMIC,				// may be moved around during the game
			ECB_DYNAMIC_MOVING,			// dynamic object that is currently moving
			ECB_VIRTUAL
		};


		//[-------------------------------------------------------]
		//[ Classes                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Base class for router collision objects
		*/
		class ECollisionObject : public WeakPtrTarget
		{
			// MergeBoxes(...) accesses the protected static member sIdGen
			friend ECollisionBox* MergeBoxes(const ECollisionBox* box1_, const ECollisionBox* box2_);

		public:
			ECollisionObject();
			ECollisionObject(EActor* obj_, bool forceCollision_ = false, bool isVirtual = false);
			virtual ~ECollisionObject();

			// TODO(co) No (save games) serialization required
			// virtual bool SerializeTo(ESerializer *stream_) const;
			// virtual bool SerializeFrom(ESerializer *stream_);

			virtual void Update();
			virtual void CalcType();
			void UpdateTerrain(EActor* obj_, const ETerrain* terrain_ = nullptr);	// to be called when terrain settings for EActor were changed

			int GetID() const { return mId; }
			ECollisionBoxType GetType() const { return mType; }
			inline bool isBox() const { return mType != ECB_VIRTUAL; }
			inline bool isVObject() const { return mType == ECB_VIRTUAL; }

			EActor* GetObject() const { return mObject; }
			inline void SetObject(EActor* obj_) { mObject = obj_; }

			virtual ERouterPolygon& GetPolygon() { return mPolygon; };
			virtual bool IsPolygonConcav() = 0;

			inline float GetSpeed(EObjectTerrainClass class_) const { return mSpeedFactors[class_]; }
			float GetSpeed(EActor& actor_) const;
			bool CompareSpeedFactors(ECollisionObject* box_);
			bool IsWater() const { return mIsWater; }

			// owner: -1: world, 0-x: player
			int GetOwnerPlayer() const { return mOwnerPlayer; }
			void SetOwnerPlayer(int owner_) { mOwnerPlayer = owner_; }

			void TestStatic();
			bool IsStatic() const { return mIsStatic; }
			bool HasNoStaticFlag() const { return mHasNoStaticFlag; }

			inline int GetLastChecked() { return mLastChecked; };
			inline void SetLastChecked(int val) { mLastChecked = val; };

			virtual bool IntersectsLine(const glm::vec3& start_, const glm::vec3& end_, glm::vec3* intersectPoint_ = nullptr) const { return false; }
			virtual bool IsPointInside(const glm::vec3& point_) const { return false; }

			virtual const glm::vec3& GetMin() const = 0;
			virtual const glm::vec3& GetMax() const = 0;

			virtual void SetLastChangeTime();
			virtual long GetLastChangeTime() const { return mLastChangeTime; }

			inline void SetGridCoords(int x0_, int y0_, int x1_, int y1_)
			{
				mGridX0 = x0_;
				mGridY0 = y0_;
				mGridX1 = x1_;
				mGridY1 = y1_;
			}
			inline void GetGridCoords(int& x0_, int& y0_, int& x1_, int& y1_) const
			{
				x0_ = mGridX0;
				y0_ = mGridY0;
				x1_ = mGridX1;
				y1_ = mGridY1;
			}
			virtual void DebugDraw(FastDebugDraw&) const {}

		private:
			long mLastChangeTime;

		protected:
			int mId;
			ECollisionBoxType mType;
			bool mForceCollision;		// box will collide with everything (terrain independent) when true
			EActor* mObject;			// corresponding actor object
			ERouterPolygon mPolygon;	// avoidance polygon

			int mOwnerPlayer;
			float mSpeedFactors[_EOTC_NUMCLASSES];
			bool mIsWater;
			bool mIsStatic;
			bool mHasNoStaticFlag;

			static int sIdGen;
			int mLastChecked;

			int mGridX0, mGridY0, mGridX1, mGridY1;

			// debug
			std::string mTerrainName;
		};


		class QSF_AI_API_EXPORT ECollisionVObject : public ECollisionObject
		{
		public:
			//ECollisionVObject();
			//explicit ECollisionVObject(EVirtualObject* obj_);
			explicit ECollisionVObject(const PolygonComponent& polygonComponent, EActor* obj_);

			// TODO(co) No (save games) serialization required
			// virtual bool SerializeTo(ESerializer *stream_) const;
			// virtual bool SerializeFrom(ESerializer *stream_);

			virtual void CalcType() override;

			virtual bool IntersectsLine(const glm::vec3& start_, const glm::vec3& end_, glm::vec3* intersectPoint_ = nullptr) const override;
			virtual bool IsPointInside(const glm::vec3& point_) const override;
			bool IsPointInsideWithExtendedHeight(const glm::vec3& point_, float heightextent_ = 2.0f) const;
			bool IsBoxCompleteInside(ECollisionBox* box_) const;
			float GetDistance(const glm::vec3& point_, glm::vec3* nearestPoint_ = nullptr) const;

			virtual const glm::vec3& GetMin() const override { return mMin; }
			virtual const glm::vec3& GetMax() const override { return mMax; }

			const PolygonComponent& getPolygonComponent() const { return *mPolygonComponent; }
			virtual bool IsPolygonConcav() override { return mIsPolygonConcav; };

			virtual void DebugDraw(FastDebugDraw& fastDebugDraw) const override;

		private:
			void buildFromPolygonComponent(const PolygonComponent& polygonComponent);

		private:
			glm::vec3 mMin;
			glm::vec3 mMax;
			const PolygonComponent* mPolygonComponent;
			bool mIsPolygonConcav;
		};


		/**
		*  @brief
		*    Class to represent an oriented bounding box that is used for collision tests.
		*    The box is centered around its position.
		*/
		class QSF_AI_API_EXPORT ECollisionBox : public ECollisionObject
		{
			friend ECollisionBox* MergeBoxes(const ECollisionBox* box1_, const ECollisionBox* box2_);
			friend float ComputeMergedBoxVolume(const ECollisionBox* box1_, const ECollisionBox* box2_);

		public:
			ECollisionBox();
			ECollisionBox(const glm::vec3& extents_, const glm::vec3& position_, const glm::mat3x3& rotation_, const glm::vec3& relPos_, const glm::mat3x3& relRot_, EActor* obj_, bool forceCollision_ = false);
			void initialize(const glm::vec3& extents_, const glm::vec3& position_, const glm::mat3x3& rotation_, const glm::vec3& relPos_, const glm::mat3x3& relRot_, EActor* obj_, bool forceCollision_ = false);

			// TODO(co) No (save games) serialization required
			// virtual bool SerializeTo(ESerializer *stream_) const;
			// virtual bool SerializeFrom(ESerializer *stream_);

			inline const glm::vec3& GetPosition() const { return mBox.GetPosition(); }
			inline const glm::vec3& GetExtents() const { return mBox.GetExtents(); }
			inline const glm::mat3x3&  GetRotation() const { return mBox.GetRotation(); }
			inline float GetBoundingRadius() const { return mBox.GetBoundingRadius(); }
			inline float GetBoundingRadiusXZ() const { return mBox.GetBoundingRadiusXZ(); }
			inline glm::vec3 ComputeDistanceVector(const glm::vec3& position_) const { return mBox.ComputeDistanceVector(position_); }

			inline void SetObject(EActor *obj_) { mObject = obj_; }
			void SetExtents(const glm::vec3& extents_, bool computePolygon_);

			void SetObjectState(const ERouterObjectState &state_, bool computePolygon_);
			void SetObjectState(const glm::vec3& pos_, const glm::mat3x3& rot_, bool computePolygon_);
			void SetObjectState(const glm::vec3& pos_, bool computePolygon_);
			void SetObjectStateWhileRouting(const glm::vec3& pos_, const glm::mat3x3& rot_);

			const glm::vec3& GetRelPos() const { return mRelPos; }
			void SetRelPos(const glm::vec3& relPos_, bool computePolygon_);

			const glm::mat3x3& GetRelRot() const { return mRelRot; }
			void SetRelRot(const glm::mat3x3& relRot_, bool computePolygon_);

			inline float ComputeVolume() const { return mBox.GetExtents().x * mBox.GetExtents().y * mBox.GetExtents().z * 8.0f; }

			float DistanceSegment2D(const glm::vec3& p0_, const glm::vec3& p1_);

			virtual bool IntersectsLine(const glm::vec3& start_, const glm::vec3& end_, glm::vec3* intersectPoint_ = nullptr) const override;
			bool Intersects(const ECollisionObject* colObj_) const;

			virtual const glm::vec3& GetMin() const override { return mBox.GetMin(); }
			virtual const glm::vec3& GetMax() const override { return mBox.GetMax(); }

			int GetBottomSurface(float minAbsUp_ = 0.0f)	{ return mBox.GetBottomSurface(minAbsUp_); }
			float ComputeBottomArea(float minAbsUp_ = 0.0f)	{ return mBox.ComputeBottomArea(minAbsUp_); }

			virtual bool IsPointInside(const glm::vec3& point_) const override { return mBox.IsInside(point_); }

			virtual void DebugDraw(FastDebugDraw& fastDebugDraw) const override;

			virtual ERouterPolygon &GetPolygon() override;
			void inline SetPolygonNeedUpdate() { mPolygonNeedUpdate = true; };
			void ComputePolygon();
			virtual bool IsPolygonConcav() override { return false; }

			bool IsMergingAllowed();

			// How to handle intersections with virtual objects
			enum EIntersectVO
			{
				IVO_BOX_CORNERS,	// check only if boxcorners are within VO (default)
				IVO_ALL_CORNERS		// check if boxcorners in VO or VOcorners in box
			};
			void SetIntersectVO(EIntersectVO ivo_) { mIntersectVO = ivo_; }
			EIntersectVO GetIntersectVO()		   { return mIntersectVO; }

		protected:
			void SetPosition(const glm::vec3& position_);
			void SetRotation(const glm::mat3x3& rotation_);

		private:
			EOrientedBox mBox;
			glm::vec3	 mRelPos;
			glm::mat3x3  mRelRot;
			bool		 mPolygonNeedUpdate;
			EIntersectVO mIntersectVO;
			bool		 mIsRotationallySymmetrical;
		};


		ECollisionBox* MergeBoxes(const ECollisionBox* box1_, const ECollisionBox* box2_);
		float ComputeMergedBoxVolume(const ECollisionBox* box1_, const ECollisionBox* box2_, glm::vec3& extents, float &floorArea_);


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // ai
} // qsf

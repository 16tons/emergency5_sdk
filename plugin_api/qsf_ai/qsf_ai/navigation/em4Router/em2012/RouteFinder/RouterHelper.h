// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/navigation/em4Router/em2012/Bezier.h"

#include <glm/gtc/constants.hpp>
#include <glm/gtx/euler_angles.hpp>


//#define ROUTER_DEBUG
#define ROUTER_TIMER_LEVEL 3


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace ai
	{
		// class ERouterStreetSegment;	// TODO(co) We don't need the street handling in here
		class EGenericObject;
		class ERouterObjectState;
		class ELocalMovementState;
		class ERouterTarget;
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace ai
	{


		// TODO(fw): This is just some stuff to make it compile anyhow...
		#if 1
			#define BLVF_NONE 0

			template<typename T>
			class BLVar
			{
			public:
				BLVar(const char* name, T value, int ignored) : mValue(value) {}
				T Float() const { return mValue; }
				T Bool() const  { return mValue; }
				T Int() const   { return mValue; }

			private:
				T mValue;
			};
		#endif


		int ConvexHull(const glm::vec3* pts_, int n_, glm::vec3* hull_);
		float DistSegment2D(const glm::vec3& a0_, const glm::vec3& a1_, const glm::vec3& b0_, const glm::vec3& b1_);
		//bool InSegment(const glm::vec3& p_, const ERouterStreetSegment &s_);	// TODO(co) We don't need the street handling in here
		bool InSegment(const glm::vec3& p_, const glm::vec3& s0_, const glm::vec3& s1_);
		//int IntersectSegments(const ERouterStreetSegment &s0_, const ERouterStreetSegment &s1_, glm::vec3& i0_, glm::vec3& i1_);	// TODO(co) We don't need the street handling in here
		bool IntersectSegments2D(const glm::vec3& s00_, const glm::vec3& s01_, const glm::vec3& s10_, const glm::vec3& s11_);
		// bool IsEndpoint(const ERouterStreetSegment &s_, const glm::vec3& p_);	// TODO(co) We don't need the street handling in here

		// From EM4 "Utility.h"
		float ComputeDistanceToLineXYWithoutEndpoints(const glm::vec3& point_, const glm::vec3& p0_, const glm::vec3& p1_);

		void LocalToObjectState(EGenericObject* obj_, const ELocalMovementState &localState_, ERouterObjectState &objState_);
		void MovePositionToFloor(glm::vec3& position_, float fallheight = 3.0f);	// TODO(co) Was "float fallheight=MOVETOFLOOR_LIFTHEIGHT"

		bool IsTargetBehind(const glm::vec3& p0_, const glm::vec3& p1_, const ERouterTarget *t_);
		bool IsTargetBehind(const glm::vec3& p0_, const glm::vec3& p1_, const glm::vec3& target_);

		// Generic interpolation
		template<class T> inline T Interpolate(float alpha, const T& x0, const T& x1)
		{
			QSF_ASSERT(alpha >= 0.f, "QSF AI: Interpolate() failed", QSF_REACT_NONE);
			QSF_ASSERT(alpha <= 1.f, "QSF AI: Interpolate() failed", QSF_REACT_NONE);
			return x0 + ((x1 - x0) * alpha);
		}

		// compute the difference between two angles (in radians)
		inline float AngleDifference(float a0_, float a1_)
		{
			QSF_ASSERT(0 <= a0_ && a0_ <= 2 * glm::pi<float>(), "QSF AI: AngleDifference() failed", QSF_REACT_NONE);
			QSF_ASSERT(0 <= a1_ && a1_ <= 2 * glm::pi<float>(), "QSF AI: AngleDifference() failed", QSF_REACT_NONE);
			float result = fabs(a0_ - a1_);
			if (result > glm::pi<float>())
				result = 2 * glm::pi<float>() - result;
			return result;
		}

		// Replacement in QSF for "Vision::Math.AngleBetweenVectors"
		// Returns the angle in degrees between two vectors
		inline float AngleBetweenVectors(const glm::vec3& vector1, const glm::vec3& vector2)
		{
			const float dotProduct = glm::dot(glm::normalize(vector1), glm::normalize(vector2));
			return glm::degrees(std::fabs(acos(dotProduct)));
		}

		inline float GetSqrLen(const glm::vec3& u_)
		{
			return u_.x*u_.x + u_.y*u_.y + u_.z*u_.z;
		}

		// dot product (2d), returns cos of angle between u_ and v_
		inline float dot(const glm::vec3& u_, const glm::vec3& v_)
		{
			return u_.x*v_.x + u_.z*v_.z;
		}

		// dot product (3d), returns cos of angle between u_ and v_
		inline float dot3(const glm::vec3& u_, const glm::vec3& v_)
		{
			return u_.x*v_.x + u_.y*v_.y + u_.z*v_.z;
		}

		// perp dot product (2d), returns sin of angle between u_ and v_
		inline float perp(const glm::vec3& u_, const glm::vec3& v_)
		{
			return u_.x*v_.z - u_.z*v_.x;
		}

		// perp operator (2d), returns a vector 90 degree ccw to p_ and with the same length as p_
		inline glm::vec3 perp(const glm::vec3& p_)
		{
			return glm::vec3(-p_.z, p_.y, p_.x);
		}

		/*!
		 * returns > 0 if p0_, p1_, p2_ are oriented ccw, < 0 if they are oriented cw, = 0 if they are collinear.
		 */
		inline float IsLeft(const glm::vec3& p0_, const glm::vec3& p1_, const glm::vec3& p2_)
		{
			return (p1_.x - p0_.x)*(p2_.z - p0_.z) - (p2_.x - p0_.x)*(p1_.z - p0_.z);
		}

		// simple class to compare vectors by x, y and z coord
		class CompareVec
		{
		public:
			bool operator () (const glm::vec3& v0_, const glm::vec3& v1_) const
			{
				if (v0_.x < v1_.x)
					return true;
				if (v0_.x == v1_.x)
				{
					if (v0_.z < v1_.z)
						return true;
					if (v0_.z == v1_.z)
						return v0_.y < v1_.y;
				}
				return false;
			}
		};

		inline bool ChangedMoveDir(const char &move0_, const char &move1_)
		{
			return (move0_ != move1_);
		}

		// computes the distance of p_ from the segment s0_,s1_
		inline float DistancePointSegment(const glm::vec3& p_, const glm::vec3& s0_, const glm::vec3& s1_)
		{
			glm::vec3 v = s1_ - s0_;
			glm::vec3 w = p_ - s0_;

			float c1 = dot(w, v);
			if (c1 <= 0.0f)
				return glm::distance(s0_, p_);

			float c2 = dot(v, v);
			if (c2 <= c1)
				return glm::distance(s1_, p_);

			float b = c1 / c2;
			glm::vec3 pb = s0_ + v*b;
			return glm::distance(pb, p_);
		}

		inline glm::vec3 NearestPointOnSegment(const glm::vec3& orig_, const glm::vec3& segStart_, const glm::vec3& segEnd_)
		{
			glm::vec3 v(segEnd_ - segStart_);
			glm::vec3 w(orig_ - segStart_);

			float c1 = glm::dot(w, v);
			if (c1 <= 0)
			{
				return segStart_;
			}

			float c2 = glm::dot(v, v);
			if (c2 <= c1)
			{
				return segEnd_;
			}
			v *= (c1 / c2);
			v += segStart_;

			return v;
		}

		// checks if the distance of pos_ fom curve_ is smaller than maxDist_
		inline bool CheckDistance(const Bezier &curve_, const glm::vec3& pos_, float maxDist_)
		{
			int n = (int)(curve_.GetLength()*0.05f) + 1;
			float d = 1.0f / n;
			glm::vec3 p0 = curve_.Eval0();
			for (int i = 1; i <= n; ++i)
			{
				glm::vec3 p1 = curve_.Eval(d*i);
				if (DistancePointSegment(pos_, p0, p1) < maxDist_)
					return true;
				p0 = p1;
			}
			return false;
		}

		// checks if the distance of pos_ fom curve_ is smaller than maxDist_
		inline bool CheckDistance2D(const Bezier &curve_, const glm::vec3& pos_, float maxDist_)
		{
			glm::vec3 pos = pos_;
			pos.y = 0.0f;

			int n = (int)(curve_.GetLength()*0.05f) + 1;
			float d = 1.0f / n;
			glm::vec3 p0 = curve_.Eval0();
			p0.y = 0.0f;

			for (int i = 1; i <= n; ++i)
			{
				glm::vec3 p1 = curve_.Eval(d*i);
				p1.y = 0.0f;
				if (DistancePointSegment(pos, p0, p1) < maxDist_)
					return true;
				p0 = p1;
			}
			return false;
		}

		// compute the axis aligned bounding box (in xy-plane), enlarged by size_
		inline void ComputeBoundaries(glm::vec3& minV_, glm::vec3& maxV_, float size_)
		{
			if (minV_.x > maxV_.x)
				std::swap(minV_.x, maxV_.x);
			if (minV_.z > maxV_.z)
				std::swap(minV_.z, maxV_.z);
			minV_.x -= size_;
			minV_.z -= size_;
			maxV_.x += size_;
			maxV_.z += size_;
		}
		inline float InterpolateDirection(float alpha, float x0, float x1)
		{
			QSF_ASSERT(alpha >= 0.f, "QSF AI: InterpolateDirection() failed", QSF_REACT_NONE);
			QSF_ASSERT(alpha <= 1.f, "QSF AI: InterpolateDirection() failed", QSF_REACT_NONE);
			QSF_ASSERT(x0 >= 0 && x0 <= 2.f*glm::pi<float>() + 1e4f, "QSF AI: InterpolateDirection() failed", QSF_REACT_NONE);
			QSF_ASSERT(x1 >= 0 && x1 <= 2.f*glm::pi<float>() + 1e4f, "QSF AI: InterpolateDirection() failed", QSF_REACT_NONE);

			if (x1 - x0 > glm::pi<float>())
				x0 += 2.f*glm::pi<float>();
			if (x0 - x1 > glm::pi<float>())
				x1 += 2.f*glm::pi<float>();

			float result = x0 + ((x1 - x0) * alpha);
			if (result >= 2.f*glm::pi<float>())
				result -= 2.f*glm::pi<float>();
			return result;
		}

		void InflateTriangle(glm::vec3& p0_, glm::vec3& p1_, glm::vec3& p2_, float inflateInUnits_);

		/*!
		 * Computes the direction (angle in the xy-plane to the x-axis from the given direction vector.
		 */
		inline float VectorToDirection(const glm::vec3& dir_)
		{
			float dir = atan2(-dir_.x, -dir_.z);
			if (dir < 0)
				dir += 2 * glm::pi<float>();
			return dir;
		}

		inline glm::vec3 DirectionToVector(float dir_)
		{
			return glm::vec3(-sin(dir_), 0.0f, -cos(dir_));
		}

		inline void EulerYawRadToMatrix(float yaw_, glm::mat3x3&  matrix_)
		{
			// "matrix_ = glm::eulerAngleZ(yaw_);" does not work? :/
			float cosZ = glm::cos(yaw_);
			float sinZ = glm::sin(yaw_);
			matrix_ = glm::mat3x3(
					cosZ,	0,	-sinZ,
					0,		1,	0,
					sinZ,	0,	cosZ);

		}

		inline void EulerYawToMatrix(float yaw_, glm::mat3x3&  matrix_)
		{
			EulerYawRadToMatrix(yaw_ * glm::pi<float>() / 180.0f, matrix_);
		}

		float RotationMatrixToDirection(const glm::mat3x3& rotMatrix_);

		// Access global time (as replacement inside QSF for "Globals::Timer->GetGlobalTime()")
		double GetGlobalTime();

		/*!
		 * Simple timer class for internal profiling.
		 */
		class RouterTimer
		{
		public:
			RouterTimer() : mLevel(0), mCount(0), mStartTime(0), mTime(0) {}

			void SetLevel(int level_) { mLevel = level_; }

			void Start();
			void Stop();

			int GetLevel() const { return mLevel; }
			double GetTime() const { return mTime; }
			int GetCount() const { return mCount; }

		private:
			int mLevel;
			int mCount;
			double mStartTime;
			double mTime;
		};

		// Check map boundaries (as replacement inside QSF for "EM3::Scene->GetMapOuterBoundaries()")
		// TODO(fw): This could be solved better, e.g. cache the MapPropertiesComponent
		class OuterMapBoundaries
		{
		public:
			OuterMapBoundaries();
			bool valid() const;
			bool isPointInsideXZ(const glm::vec3& point) const;

		private:
			bool	  mValid;
			glm::vec3 mBoxMinimum;
			glm::vec3 mBoxMaximum;
		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // ai
} // qsf

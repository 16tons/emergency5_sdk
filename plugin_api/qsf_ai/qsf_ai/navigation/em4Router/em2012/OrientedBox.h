// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/Export.h"

#include <qsf/math/Color3.h>

#include <glm/glm.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class BinarySerializer;
	class FastDebugDraw;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace ai
	{


		/*!
		 * \brief Class that implements an oriented box.
		 * The box is defined by extents, position and translation and is centered around its position.
		 */
		class QSF_AI_API_EXPORT EOrientedBox
		{
		public:
			EOrientedBox();
			EOrientedBox(const glm::vec3& extents_, const glm::vec3& position_, const glm::mat3x3& rotation_);
			void initialize(const glm::vec3& extents_, const glm::vec3& position_, const glm::mat3x3& rotation_);

			virtual ~EOrientedBox();

			void serialize(BinarySerializer& serializer);

			void SetPosition(const glm::vec3& position_);
			void SetRotation(const glm::mat3x3& rotation_);
			inline void SetPositionNoAdjust(const glm::vec3& position_)   { mPosition = position_; };
			inline void SetRotationNoAdjust(const glm::mat3x3& rotation_) { mRotation = rotation_; };
			void SetPositionAndRotation(const glm::vec3& position_, const glm::mat3x3& rotation_);
			void SetExtents(const glm::vec3& extents_);

			inline const glm::vec3&  GetPosition() const { return mPosition; }
			inline const glm::mat3x3&  GetRotation() const { return mRotation; }
			inline const glm::vec3&  GetExtents() const { return mExtents; }

			// return the transformed min and max coords
			const glm::vec3&  GetMin() const { if (!mMinMaxValid) ComputeMinMax(); return mMin; }
			const glm::vec3&  GetMax() const { if (!mMinMaxValid) ComputeMinMax(); return mMax; }
			void ValidateBox() const { if (!mMinMaxValid) ComputeMinMax(); }

			float GetBoundingRadius() const { return mBoundingRadius; }
			float GetBoundingRadiusXZ() const { return mBoundingRadiusXZ; }

			bool IntersectsLine(const glm::vec3& start_, const glm::vec3& end_, glm::vec3* intersectPoint_ = nullptr) const;
			bool inline ILHGetIntersection(float fDst1_, float fDst2_, glm::vec3 p1_, glm::vec3 p2_, glm::vec3& hit_) const;
			bool inline ILHInBox(glm::vec3& hit_, glm::vec3 extents_, int axis_) const;

			bool Intersects(const EOrientedBox &box_) const;

			bool IsInside(const glm::vec3& position_) const;
			bool IsInsideXY(const glm::vec3& position_) const;
			bool IsOnSurface(const glm::vec3& position_) const;

			float ComputeDistance(const glm::vec3& position_) const;
			glm::vec3 ComputeDistanceVector(const glm::vec3& position_) const;

			bool Draw(FastDebugDraw& fastDebugDraw, const Color3& color = Color3::WHITE) const;

			// Which surface is the bottom after transformation
			// 0 = bottom, 1 = front, 2=right, 3=back, 4=left, 5=top
			// If the vector pointing up is smaller minAbsUp return -1 (to avoid ambigious floors)
			int GetBottomSurface(float minAbsUp_ = 0.f);

			// returns a value < 0 for ambigious floors
			float ComputeBottomArea(float minAbsUp_ = 0.f);

		protected:
			void ComputeMinMax() const;

			glm::vec3 mExtents;
			glm::vec3 mPosition;
			glm::mat3x3 mRotation;

			// transformed minimum and maximum coords (AABBox)
			mutable glm::vec3 mMin;
			mutable glm::vec3 mMax;
			mutable bool mMinMaxValid;

			// bounding radius
			float mBoundingRadius;
			float mBoundingRadiusXZ;
		};


		// Tool functions
		extern int ComputeBottomSurface(const glm::mat3x3& rotation_, float minAbsUp_ = 0.0f);


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // ai
} // qsf

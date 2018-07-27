// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <glm/glm.hpp>

#include <vector>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace ai
	{


		/*!
		 * \brief Class to encapsulate and simplify computation of polygons to avoid obstacles.
		 */
		class ERouterPolygon
		{
		public:
			typedef std::vector<glm::vec3> VectorArray;
			typedef std::vector<float> FloatArray;

			ERouterPolygon();
			ERouterPolygon(const VectorArray& points_);

			// TODO(co) No (save games) serialization required
			// bool SerializeTo(ESerializer *stream_) const;
			// bool SerializeFrom(ESerializer *stream_);

			void Add(const glm::vec3& point_);

			void SetPoints(const VectorArray& points_);
			void SetPoints(const glm::vec3* points_, int n_);
			void swapPoints(std::vector<glm::vec3>& points_);

			void Clear();

			// There must be at least one point inside the list
			inline size_t Next(size_t i) const { return (i < mPoints.size() - 1) ? i + 1 : 0; }
			inline size_t Prev(size_t i) const { return (i > 0) ? i - 1 : mPoints.size() - 1; }

			inline int GetSize() const { return static_cast<int>(mPoints.size()); }
			const glm::vec3& GetPoint(size_t i) const { return mPoints[i]; }
			const glm::vec3& GetEdge(size_t i) const { return mEdges[i]; }

			void SetMaxExtensionDistance(int i, float maxDist_);

			float GetArea() const { return mArea; }

			void Inflate(float f_, bool limitSize_ = true);

			glm::vec3 ComputeExtendedPoint(size_t i, float dist_, bool limitSize_ = true) const;

			bool IsInside(const glm::vec3& point_) const;
			float ComputeDistance(const glm::vec3& point_, glm::vec3* nearestPoint_ = nullptr) const;
			float ComputeDistance2D(const glm::vec3& point_, glm::vec3* nearestPoint_ = nullptr) const;

			size_t GetMaxIndex(const glm::vec3& dir_) const;
			size_t GetNearestPointIndex(const glm::vec3& pos_) const;
			size_t GetNearestEdge(const glm::vec3& point_) const;

			bool IntersectWithLine(float x1_, float y1_, float x2_, float y2_, float* returnx_ = nullptr, float* returny_ = nullptr) const;
			bool IsConcavAt(size_t point_) const;
			bool IsPolygonConcav() const;
			bool IsPolygonIntersectingWithRect(float x1_, float y1_, float x2_, float y2_) const;

		private:
			void ComputeEdges(bool realloc_ = true);
			void ComputeArea();

			VectorArray mPoints;
			VectorArray mEdges;
			FloatArray mMaxExtDist;

			float mArea;
		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // ai
} // qsf

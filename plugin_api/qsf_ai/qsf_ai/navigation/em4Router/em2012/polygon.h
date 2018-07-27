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
#include <list>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace ai
	{


		struct EPolygonPoint
		{
			float x, y, z;
			bool selected;
			int id;
		};

		enum EPolygonOrientation
		{
			EPO_CW,		//!< clockwise
			EPO_CCW,	//!< counter clockwise
		};

		class EPolygon
		{
		public:
			typedef std::list<EPolygonPoint*> PointList;

		private:
			PointList mPoints;
			bool mClosed;

			void DrawHandle(const EPolygonPoint* Pnt) const;

			float Distance(const EPolygonPoint* P1, const EPolygonPoint* P2);
			void ComputeMinMax();
			mutable unsigned char ColorR, ColorG, ColorB, ColorAlpha;
			glm::vec3 mMin, mMax;

			int mVersion;	// eine Art Versionsnummer, die anderen zu erkennen gibt, ob sich
			// das Polygon geändert hat

			int mNextId;

		public:
			EPolygon();
			virtual ~EPolygon();

			inline int GetNumPoints() const { return (int)mPoints.size(); }
			inline const PointList& GetPoints() const { return mPoints; }

			void Clear();

		#if 0
			virtual void Draw(bool drawHandle_ = true) const;
			virtual EPolygonPoint* Add(const glm::vec3& Pnt, bool atBeginning = false);
			virtual EPolygonPoint* Add(float x, float y, float z, bool atBeginning = false);
			virtual EPolygonPoint* Insert(const EPolygonPoint* Prev, const glm::vec3& Pnt);
			virtual EPolygonPoint* Insert(const EPolygonPoint* Prev, float x, float y, float z);
			virtual EPolygonPoint* Insert(const glm::vec3& Pnt);
			virtual EPolygonPoint* Insert(float x, float y, float z);
			EPolygonPoint* NearestPoint(const glm::vec3& Pnt, const EPolygonPoint* Ignore = nullptr) const;
			EPolygonPoint* NearestPoint2(const glm::vec3& Pnt, const EPolygonPoint* Ignore = nullptr, const EPolygonPoint* Ignore2 = nullptr) const;
			EPolygonPoint* NearestPointToLine(const glm::vec3& s, const glm::vec3& t, float Radius = 1.0f) const;
			EPolygonPoint* PickPointFromLine(const glm::vec3& s, const glm::vec3& t) const;
			bool NearestSegment(const glm::vec3& pnt_, EPolygonPoint& start_, EPolygonPoint& end_, float nearEnough_ = 10.f) const;
			bool NearestSegment(const glm::vec3& pnt_, int& startIndex_, int& endIndex_, float nearEnough_ = 10.f) const;
			bool NearestSegment(const glm::vec3& pnt_, float& distance_, EPolygonPoint& start_, EPolygonPoint& end_) const;

			float DistanceToClosestLine(const glm::vec3& point_, bool allowInnerDistance_ = false, glm::vec3* nearestPointOnPoly_ = nullptr) const;
			float DistanceToClosestLine(const glm::vec3& point_, glm::vec3& nearestSegmentStart_, bool& isLeft_, bool allowInnerDistance_, bool closedPolygon_, int* index_ = 0) const;

			// berechnet den Schnittpunkt der gegebenen Gerade mit dem Polygon. Gibt zurück,
			// ob er existiert, und schreibt den Schnittpunkt als Anteil der Strecke in t_.
			// index_ enthält danach das Polygonsegment, auf dem der Schnittpunkt liegt
			bool IntersectWithLine(float x1, float y1, float x2, float y2, float& t_, int& index_) const;
			// liefert alle Schnittpunkte zurück
			bool IntersectWithLine2(float x1, float y1, float x2, float y2, std::vector<float>& t_, std::vector<int>& index_) const;

			void Remove(const EPolygonPoint* Point);
			void Remove(const glm::vec3& Pnt);
			void Remove(float x, float y, float z);

			float GetLength()const;

			bool IsClosed() const { return mClosed; }
			void SetClosed(bool closed_) { mClosed = closed_; };
		#endif

			void Move(float x, float y, float z);
			void Rotate(float angle_);
		#if 0
			void MovePoint(EPolygonPoint* Point, float x, float y, float z);
			EPolygonPoint* PickPoint(float x, float y, float z);
			void SubDivide(bool smooth_);
			void Optimize();
		#endif
			bool PointInsideXZ(float x, float y) const;
			glm::vec3 GetMidPoint() const;

		#if 0
			void SetColor(unsigned char R, unsigned char G, unsigned char B, unsigned char Alpha = 255) const;
		#endif
			void CopyFrom(const EPolygon* Poly);
		#if 0
			void ProjectOntoFloor(float ZOffset = 1.0f);
			void ProjectOntoGround(float ZOffset = 1.0f);
			void Flatten();
			void Shrink(float dist_, EPolygon *destPoly_) const;
			void Inflate(float dist_, EPolygon *destPoly_) const;

			void ClearSelection();
			void SetSelection(EPolygonPoint* point_, bool selected_);

			EPolygonOrientation GetOrientation() const;
			EPolygonOrientation GetTriOrientation(const EPolygonPoint* p1_, const EPolygonPoint* p2_, const EPolygonPoint* p3_) const;
		#endif

			inline const glm::vec3& GetMin() const { return mMin; }
			inline const glm::vec3& GetMax() const { return mMax; }

		#if 0
			// TODO(fw): Not necessary
			//virtual bool Save(BLFile *File_);
			//virtual bool Load(BLFile *File_);
			//virtual bool SerializeTo(ESerializer *Stream_);
			//virtual bool SerializeFrom(ESerializer *Stream_);

			void GetMaxSpanXY(float& x, float& y) const;
		#endif
		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // ai
} // qsf

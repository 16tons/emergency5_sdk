// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/debug/DebugDrawRequest.h"
#include "qsf/debug/request/SegmentDebugDrawRequest.h"
#include "qsf/debug/request/CircleDebugDrawRequest.h"
#include "qsf/debug/request/TriangleDebugDrawRequest.h"
#include "qsf/debug/request/PolygonDebugDrawRequest.h"
#include "qsf/debug/request/RectangleDebugDrawRequest.h"
#include "qsf/debug/request/LaneDebugDrawRequest.h"
#include "qsf/debug/request/TextDebugDrawRequest.h"

#include <deque>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	/**
	*	@brief A compound debug draw request is a collection of atomic debug draw requests that share the same life time.
	*	This allows the renderer to create a graphical representation for them all and optimize the creation process of complex structures (like a debugging view for a navigation mesh for example).
	*	A compound debug draw request only receives one unique ID for the complete structure.
	*	TODO(vs) Remember to include new debug draw types into this structure.
	*/
	class QSF_API_EXPORT CompoundDebugDrawRequest : public DebugDrawRequest
	{
	public:
		CompoundDebugDrawRequest();

		virtual void applyToObject(DebugDrawObject& debugDrawObject) const override;

		std::deque<SegmentDebugDrawRequest>	mSegments;
		std::deque<CircleDebugDrawRequest>		mCircles;
		std::deque<TriangleDebugDrawRequest>	mTriangles;
		std::deque<PolygonDebugDrawRequest>	mPolygons;
		std::deque<RectangleDebugDrawRequest>	mRectangles;
		std::deque<LaneDebugDrawRequest>		mLanes;
		std::deque<TextDebugDrawRequest>		mTexts;
	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf

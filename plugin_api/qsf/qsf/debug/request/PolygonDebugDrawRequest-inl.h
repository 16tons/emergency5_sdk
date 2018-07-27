// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/base/error/ErrorHandling.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline PolygonDebugDrawRequest::PolygonDebugDrawRequest(const std::vector<glm::vec3>& vertices, bool closed, const Color4& color, float filledOpacity) :
		mVertices(vertices),
		mClosed(closed),
		mColor(color),
		mFilledOpacity(filledOpacity)
	{
		QSF_WARN_IF(vertices.size() < 2, "Drawing a point list from " << vertices.size() << " points won't actually show anything",
			QSF_REACT_NONE);
		// TODO(vs) Determine how !closed but filled works and verify that it is clean, documented behavior
		// I am not sure whether we could introduce a warning here
	}

	inline PolygonDebugDrawRequest::~PolygonDebugDrawRequest()
	{
		// Nothing to do in here
	}


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	inline PolygonDebugDrawRequest::PolygonDebugDrawRequest(bool closed, const Color4& color, float filledOpacity) :
		mClosed(closed),
		mColor(color),
		mFilledOpacity(filledOpacity)
	{
		// Nothing to do in here
	}

	inline PolygonDebugDrawRequest::PolygonDebugDrawRequest() :
		mClosed(false),
		mFilledOpacity(1.0f)
	{
		// Nothing to do in here
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf

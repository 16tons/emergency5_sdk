// Copyright (C) 2012-2018 Promotion Software GmbH


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
	inline PolygonVolumeDebugDrawRequest::PolygonVolumeDebugDrawRequest(const std::vector<glm::vec3>& vertices, float height, const Color4& color, float filledOpacity) :
		mVertices(vertices),
		mHeight(height),
		mColor(color),
		mFilledOpacity(filledOpacity)
	{
		QSF_WARN_IF(vertices.size() < 2, "drawing a point list from " << vertices.size() << " points won't actually show anything",
			QSF_REACT_NONE);
		// TODO(vs) Determine how !closed but filled works and verify that it is clean, documented behavior
		// I am not sure whether we could introduce a warning here
	}

	inline PolygonVolumeDebugDrawRequest::~PolygonVolumeDebugDrawRequest()
	{
		// Nothing to do in here
	}


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	inline PolygonVolumeDebugDrawRequest::PolygonVolumeDebugDrawRequest() :
		mHeight(0.0f),
		mFilledOpacity(1.0f)
	{
		// Nothing to do in here
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf

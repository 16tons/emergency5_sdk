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
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	inline void LaneDebugDrawRequest::setLaneDebugDrawMaterial(uint32 laneType, const Color4& color, const std::string& texture)
	{
		setLaneDebugDrawColor(laneType, color);
		setLaneDebugDrawTexture(laneType, texture);
	}

	inline void LaneDebugDrawRequest::setLaneDebugDrawColor(uint32 laneType, const Color4& color)
	{
		sLaneDebugDrawMaterial.color[laneType] = color;
	}

	inline void LaneDebugDrawRequest::setLaneDebugDrawTexture(uint32 laneType, const std::string& texture)
	{
		sLaneDebugDrawMaterial.texture[laneType] = texture;
	}


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline LaneDebugDrawRequest::LaneDebugDrawRequest(const std::vector<glm::vec3>& vertices, float widthStart, float widthEnd, float arrowLength, uint32 laneType, bool isHighlighted) :
		mVertices(vertices),
		mWidthStart(widthStart),
		mWidthEnd(widthEnd),
		mArrowLength(arrowLength),
		mLaneType(laneType),
		mIsHighlighted(isHighlighted)
	{
		QSF_WARN_IF(vertices.size() < 2, "drawing a point list from " << vertices.size() << " points won't actually show anything",
			QSF_REACT_NONE);
	}

	inline LaneDebugDrawRequest::~LaneDebugDrawRequest()
	{
		// Nothing to do in here
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf

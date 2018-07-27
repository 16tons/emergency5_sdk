// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline RectangleDebugDrawRequest::RectangleDebugDrawRequest(const glm::vec2& corner1, const glm::vec2& corner2, const Color4& color, float filledOpacity) :
		PolygonDebugDrawRequest(true, color, filledOpacity)
	{
		mVertices.emplace_back(corner1.x, corner1.y, 1.0f);
		mVertices.emplace_back(corner1.x, corner2.y, 1.0f);
		mVertices.emplace_back(corner2.x, corner2.y, 1.0f);
		mVertices.emplace_back(corner2.x, corner1.y, 1.0f);
	}

	inline RectangleDebugDrawRequest::RectangleDebugDrawRequest(const glm::vec3& corner1, const glm::vec3& corner2, const Color4& color, float filledOpacity) :
		PolygonDebugDrawRequest(true, color, filledOpacity)
	{
		mVertices.push_back(corner1);
		mVertices.emplace_back(corner1.x, corner1.y, corner2.z);
		mVertices.push_back(corner2);
		mVertices.emplace_back(corner2.x, corner2.y, corner1.z);
	}

	inline RectangleDebugDrawRequest::~RectangleDebugDrawRequest()
	{
		// Nothing to do in here
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	inline RectangleDebugDrawRequest::RectangleDebugDrawRequest()
	{
		// Nothing to do in here
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf

// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline TriangleDebugDrawRequest::TriangleDebugDrawRequest(const glm::vec3& vertex1, const glm::vec3& vertex2, const glm::vec3& vertex3, const Color4& color, float filledOpacity) :
		PolygonDebugDrawRequest(true, color, filledOpacity)
	{
		mVertices.push_back(vertex1);
		mVertices.push_back(vertex2);
		mVertices.push_back(vertex3);
	}

	inline TriangleDebugDrawRequest::~TriangleDebugDrawRequest()
	{
		// Nothing to do in here
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	inline TriangleDebugDrawRequest::TriangleDebugDrawRequest() :
		PolygonDebugDrawRequest()
	{
		// Nothing to do in here
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf

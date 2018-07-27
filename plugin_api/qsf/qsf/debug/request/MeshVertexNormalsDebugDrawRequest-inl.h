// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline MeshVertexNormalsDebugDrawRequest::MeshVertexNormalsDebugDrawRequest(Ogre::v1::Entity& ogreEntity, float lineLength, const Color4& color) :
		mOgreEntity(&ogreEntity),
		mLineLength(lineLength),
		mColor(color)
	{
		// Nothing to do in here
	}

	inline MeshVertexNormalsDebugDrawRequest::~MeshVertexNormalsDebugDrawRequest()
	{
		// Nothing to do in here
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	inline MeshVertexNormalsDebugDrawRequest::MeshVertexNormalsDebugDrawRequest() :
		mOgreEntity(nullptr),
		mLineLength(0.0f)
	{
		// Nothing to do in here
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf

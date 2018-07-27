// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline BoxVolumeDebugDrawRequest::BoxVolumeDebugDrawRequest(const glm::vec3& center, const glm::quat& rotation, const glm::vec3& extents, float borderWidth, const Color4& color, float filledOpacity) :
		mTransform(center, rotation, extents),
		mBorderWidth(borderWidth),
		mColor(color),
		mFilledOpacity(filledOpacity)
	{
		// Nothing to do in here
	}

	inline BoxVolumeDebugDrawRequest::~BoxVolumeDebugDrawRequest()
	{
		// Nothing to do in here
	}


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	inline BoxVolumeDebugDrawRequest::BoxVolumeDebugDrawRequest() :
		mBorderWidth(0.0f),
		mFilledOpacity(1.0f)
	{
		// Nothing to do in here
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf

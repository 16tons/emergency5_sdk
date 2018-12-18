// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline IconDebugDrawRequest::IconDebugDrawRequest(const glm::vec3& topLeftPosition, const std::string& iconName, const Color4& tintColor, float width, float height) :
		mPosition(topLeftPosition),
		mIconName(iconName),
		mTintColor(tintColor),
		mWidth(width),
		mHeight(height)
	{
		// Nothing to do in here
	}

	inline IconDebugDrawRequest::~IconDebugDrawRequest()
	{
		// Nothing to do in here
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf

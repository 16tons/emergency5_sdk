// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline TextDebugDrawRequest::TextDebugDrawRequest(const std::string& text, const glm::vec2& windowPosition, const Color4& textColor) :
		mText(text),
		mUsingWorldPosition(false),
		mPosition(windowPosition.x, windowPosition.y, 0.0f)
	{
		mTextParameters.setTextColor(textColor);
	}

	inline TextDebugDrawRequest::TextDebugDrawRequest(const std::string& text, const glm::vec3& worldPosition, const Color4& textColor) :
		mText(text),
		mUsingWorldPosition(true),
		mPosition(worldPosition)
	{
		mTextParameters.setTextColor(textColor);
	}

	inline TextDebugDrawRequest::TextDebugDrawRequest(const std::string& text, const glm::vec2& windowPosition, const DebugTextParameters& debugTextParameters) :
		mText(text),
		mUsingWorldPosition(false),
		mPosition(windowPosition.x, windowPosition.y, 0.0f),
		mTextParameters(debugTextParameters)
	{
		// Nothing to do here
	}

	inline TextDebugDrawRequest::TextDebugDrawRequest(const std::string& text, const glm::vec3& worldPosition, const DebugTextParameters& debugTextParameters) :
		mText(text),
		mUsingWorldPosition(true),
		mPosition(worldPosition),
		mTextParameters(debugTextParameters)
	{
		// Nothing to do here
	}

	inline TextDebugDrawRequest::~TextDebugDrawRequest()
	{
		// Nothing to do in here
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	inline TextDebugDrawRequest::TextDebugDrawRequest()
	{
		// Nothing to do in here
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf

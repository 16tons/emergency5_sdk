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
	inline CircleDebugDrawRequest::CircleDebugDrawRequest(const glm::vec3& position, const glm::vec3& axisDirection, float radius, const Color4& color, bool filled, bool depthInteraction) :
		mPosition(position),
		mAxis(axisDirection),
		mRadius(radius),
		mColor(color),
		mFilled(filled),
		mDepthInteraction(depthInteraction)
	{
		QSF_ASSERT(axisDirection != glm::vec3(0,0,0), "Parameter axisDrection must not be a zero vector", QSF_REACT_NONE);
		QSF_ASSERT(radius > 0.0f, "Parameter radius must be > 0.0!", mRadius = 1.0f);
	}

	inline CircleDebugDrawRequest::~CircleDebugDrawRequest()
	{
		// Nothing to do in here
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	inline CircleDebugDrawRequest::CircleDebugDrawRequest()
	{
		// Nothing to do in here
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf

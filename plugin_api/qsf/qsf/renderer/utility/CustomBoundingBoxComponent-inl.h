// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/base/GetUninitialized.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline CustomBoundingBoxComponent::CustomBoundingBoxComponent(Prototype* prototype) :
		Component(prototype),
		mLocalMinimumCorner(0.0f, 0.0f, 0.0f),
		mLocalMaximumCorner(1.0f, 1.0f, 1.0f)
	{
		// Nothing here
	}

	inline CustomBoundingBoxComponent::~CustomBoundingBoxComponent()
	{
		// Nothing here
	}

	inline const glm::vec3& CustomBoundingBoxComponent::getLocalMinimumCorner() const
	{
		return mLocalMinimumCorner;
	}

	inline const glm::vec3& CustomBoundingBoxComponent::getLocalMaximumCorner() const
	{
		return mLocalMaximumCorner;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf

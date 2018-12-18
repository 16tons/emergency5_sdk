// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/debug/request/PolygonDebugDrawRequest.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Street preview debug draw request implementation
	*/
	class QSF_API_EXPORT StreetPreviewDebugDrawRequest : public PolygonDebugDrawRequest
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		// Necessary for allowing std containers to use the illegal default constructor
		friend std::allocator<StreetPreviewDebugDrawRequest>;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		StreetPreviewDebugDrawRequest(const glm::vec3& startPosition, const glm::vec3& startDirection, float startWidth, const glm::vec3& endPosition, float endWidth, const Color4& color = Color4::WHITE, float filledOpacity = 0.5f);

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~StreetPreviewDebugDrawRequest();


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		/** Default constructor creating an illegal state that is only needed for using these requests inside a std::vector */
		inline StreetPreviewDebugDrawRequest();


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/debug/request/StreetPreviewDebugDrawRequest-inl.h"

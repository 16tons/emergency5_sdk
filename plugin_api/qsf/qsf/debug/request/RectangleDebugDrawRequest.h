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
	*    Rectangle debug draw request implementation
	*/
	class RectangleDebugDrawRequest : public PolygonDebugDrawRequest
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] corner1
		*    Clip space position of first rectangle corner
		*  @param[in] corner2
		*    Clip space position of second rectangle corner
		*  @param[in] color
		*    Draw color, filled area is drawn in the same color made transparent
		*  @param[in] filledOpacity
		*    Opacity between 0.0f and 1.0f of the inner area; if 0.0f (or below), no inner area is drawn
		*/
		inline RectangleDebugDrawRequest(const glm::vec2& corner1, const glm::vec2& corner2, const Color4& color = Color4::WHITE, float filledOpacity = 0.0f);

		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] corner1
		*    World space position of first rectangle corner
		*  @param[in] corner2
		*    World space position of second rectangle corner
		*  @param[in] color
		*    Draw color, filled area is drawn in the same color made transparent
		*  @param[in] filledOpacity
		*    Opacity between 0.0f and 1.0f of the inner area; if 0.0f (or below), no inner area is drawn
		*/
		inline RectangleDebugDrawRequest(const glm::vec3& corner1, const glm::vec3& corner2, const Color4& color = Color4::WHITE, float filledOpacity = 0.0f);

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~RectangleDebugDrawRequest();


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		/** Default constructor creating an illegal state that is only needed for using these requests inside a std::vector */
		inline RectangleDebugDrawRequest();


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/debug/request/RectangleDebugDrawRequest-inl.h"

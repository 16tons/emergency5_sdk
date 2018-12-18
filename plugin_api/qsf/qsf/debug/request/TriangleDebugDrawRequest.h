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
	*    Triangle debug draw request implementation
	*/
	class QSF_API_EXPORT TriangleDebugDrawRequest : public PolygonDebugDrawRequest
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		// Necessary for allowing std containers to use the illegal default constructor
		friend std::allocator<TriangleDebugDrawRequest>;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] vertex1
		*    Position of first triangle vertex
		*  @param[in] vertex2
		*    Position of second triangle vertex
		*  @param[in] vertex3
		*    Position of third triangle vertex
		*  @param[in] color
		*    Draw color, filled area is drawn in the same color made transparent
		*  @param[in] filledOpacity
		*    Opacity between 0.0f and 1.0f of the inner area; if 0.0f (or below), the inner area will not be drawn at all
		*/
		inline TriangleDebugDrawRequest(const glm::vec3& vertex1, const glm::vec3& vertex2, const glm::vec3& vertex3, const Color4& color = Color4::WHITE, float filledOpacity = 0.0f);

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~TriangleDebugDrawRequest();


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		/** Default constructor creating an illegal state that is only needed for using these requests inside a std::vector */
		inline TriangleDebugDrawRequest();


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/debug/request/TriangleDebugDrawRequest-inl.h"

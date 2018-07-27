// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/debug/DebugDrawRequest.h"
#include "qsf/math/Color4.h"

#include <glm/glm.hpp>

#include <vector>


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
	*    Polygon volume debug draw request implementation
	*/
	class QSF_API_EXPORT PolygonVolumeDebugDrawRequest : public DebugDrawRequest
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		// Necessary for allowing std containers to use the illegal default constructor
		friend std::allocator<PolygonVolumeDebugDrawRequest>;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] vertices
		*    Positions of polygon vertices
		*  @param[in] height
		*    Height of the volume
		*  @param[in] color
		*    Draw color, filled area is drawn in the same color made transparent
		*  @param[in] filledOpacity
		*    Opacity between 0.0f and 1.0f of the inner area; if 0.0f (or below), the inner area will not be drawn at all
		*/
		inline PolygonVolumeDebugDrawRequest(const std::vector<glm::vec3>& vertices, float height, const Color4& color = Color4::WHITE, float filledOpacity = 0.0f);

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~PolygonVolumeDebugDrawRequest();


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		/** Default constructor creating an illegal state that is only needed for using these requests inside a std::vector */
		inline PolygonVolumeDebugDrawRequest();


	//[-------------------------------------------------------]
	//[ Public virtual qsf::DebugDrawRequest methods          ]
	//[-------------------------------------------------------]
	public:
		virtual void applyToObject(DebugDrawObject& debugDrawObject) const override;


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		std::vector<glm::vec3> mVertices;		///< List of vertices
		float				   mHeight;			///< If Height of the volume
		Color4				   mColor;			///< Draw color
		float				   mFilledOpacity;	///< Opacity of filled area


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/debug/request/PolygonVolumeDebugDrawRequest-inl.h"

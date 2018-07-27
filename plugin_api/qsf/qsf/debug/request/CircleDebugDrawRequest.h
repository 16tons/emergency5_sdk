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
	*    Circle debug draw request implementation
	*/
	class QSF_API_EXPORT CircleDebugDrawRequest : public DebugDrawRequest
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		// Necessary for allowing std containers to use the illegal default constructor
		friend std::allocator<CircleDebugDrawRequest>;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] position
		*    Segment start position
		*  @param[in] axisDirection
		*    Vector defining an axis to draw the circle around
		*  @param[in] radius
		*    Circle radius in world units (meters)
		*  @param[in] color
		*    Draw color, filled area is drawn in the same color made transparent
		*  @param[in] filled
		*    If true, circle if drawn as a filled circle
		*  @param[in] depthInteraction
		*    If false, circle is visible through other geometry
		*/
		inline CircleDebugDrawRequest(const glm::vec3& position, const glm::vec3& axisDirection, float radius, const Color4& color = Color4::WHITE, bool filled = false, bool depthInteraction = false);

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~CircleDebugDrawRequest();


	//[-------------------------------------------------------]
	//[ Public virtual qsf::DebugDrawRequest methods          ]
	//[-------------------------------------------------------]
	public:
		virtual void applyToObject(DebugDrawObject& debugDrawObject) const override;


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		glm::vec3 mPosition;			///< Position
		glm::vec3 mAxis;				///< Axis to draw the circle around
		float	  mRadius;				///< Circle radius in world units (meters)
		Color4	  mColor;				///< Draw color
		bool	  mFilled;				///< If true, draw filled
		bool	  mDepthInteraction;	///< If false, the circle is visible through other geometry


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		/** Default constructor creating an illegal state that is only needed for using these requests inside a std::vector */
		inline CircleDebugDrawRequest();


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/debug/request/CircleDebugDrawRequest-inl.h"

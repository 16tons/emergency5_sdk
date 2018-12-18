// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/math/Ray.h"


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
	*    Class representing a 3D segment, defined by two points (internally an origin point and a direction vector).
	*    Like Ray (and its base class Line) it inherits from a segment with zero direction (i.e. both points are identical) is illegal.
	*    If you try to create such a segment it will throw an exception.
	*/
	class QSF_API_EXPORT Segment : public Ray
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/** Named constructors to avoid confusion because creating a line from two vec3 is ambiguous.
		*   Throws exception when the direction is zero.
		*/
		//@{
		static Segment fromTwoPoints(const glm::vec3& first, const glm::vec3& second);
		static Segment fromOriginAndDirection(const glm::vec3& origin, const glm::vec3& direction);
		static Segment fromUpDirection(); // Returns a segment from (0.0f, 0.0f, 0.0f) with the direction of the unit Up vector
		//@}

		/** Return first point; this simply wraps Line::getOrigin() */
		const glm::vec3& getFirstPoint() const;

		/** Return second point */
		glm::vec3 getSecondPoint() const;

		/** Return extension; this simply wraps Line::getDirection() */
		const glm::vec3& getExtension() const;

		/** Set first point without modifying the second point */
		void setFirstPoint(const glm::vec3& point);

		/** Set second point without modifying the first point */
		void setSecondPoint(const glm::vec3& point);


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		/** Constructor is protected to avoid ambiguity, use named constructors instead. */
		Segment(const glm::vec3& origin, const glm::vec3& extension);


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf

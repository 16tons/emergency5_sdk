// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/Export.h"

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
	*    Class representing a 3D line, defined by an origin point and a (usually normalized) direction.
	*	 A line with zero direction is considered degenerated.
	*	 Trying to create such a line will lead to a thrown exception.
	*/
	class QSF_API_EXPORT Line
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/** Named constructors to avoid confusion because creating a line from two Vec3 is ambiguous.
		*   Throws exception when the direction is zero.
		*/
		//@{
		static Line fromTwoPoints(const glm::vec3& from, const glm::vec3& to);
		static Line fromOriginAndDirection(const glm::vec3& origin, const glm::vec3& direction);
		static Line fromUpDirection(); // Returns a line along the up axis that goes through (0.0f, 0.0f, 0.0f)
		//@}

		/** Set the origin of the line */
		void setOrigin(const glm::vec3& origin);

		/** Get the origin of the line */
		const glm::vec3& getOrigin() const;

		/** Set the direction of the line */
		void setDirection(const glm::vec3& dir, bool normalize = false);

		/** Get the direction of the line */
		const glm::vec3& getDirection() const;

		/** Get the position of a point t units along the line */
		glm::vec3 getPoint(float t) const;

		/** Get the nearest point on the line */
		glm::vec3 getNearestPoint(const glm::vec3& point) const;


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		/** Constructor is protected to avoid ambiguity, use named constructors instead. */
		Line(const glm::vec3& origin, const glm::vec3& direction);


	//[-------------------------------------------------------]
	//[ Protected members                                     ]
	//[-------------------------------------------------------]
	protected:
		glm::vec3 mOrigin;		///< Origin point of the line
		glm::vec3 mDirection;	///< Direction vector of the line; must not be zero vector, but is not necessarily normalized


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf

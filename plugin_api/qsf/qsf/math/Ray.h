// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/math/Line.h"


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
	*    Class representing a 3D ray, defined by an origin point and a (usually normalized) direction.
	*    Like the Line it inherits from a ray with zero direction is illegal.
	*    If you try to create such a ray it will throw an exception.
	*/
	class QSF_API_EXPORT Ray : public Line
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/** Named constructors to avoid confusion because creating a line from two vec3 is ambiguous.
		*   Throws exception when the direction is zero.
		*/
		//@{
		static Ray fromTwoPoints(const glm::vec3& from, const glm::vec3& to);
		static Ray fromOriginAndDirection(const glm::vec3& origin, const glm::vec3& direction);
		static Ray fromUpDirection(); // Returns a ray starting at (0.0f, 0.0f, 0.0f) and pointing along the up axis
		//@}


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		/** Constructor is protected to avoid ambiguity, use named constructors instead. */
		Ray(const glm::vec3& origin, const glm::vec3& direction);


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf

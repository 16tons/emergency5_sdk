// Copyright (C) 2012-2017 Promotion Software GmbH


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
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Line;
	class Transform;
}


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
	*    Class representing a 3D plane, defined by its normal vector and its distance to the coordinate system's origin
	*/
	class QSF_API_EXPORT Plane
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		/** The "positive side" of the plane is the half space to which the
		*	plane normal points. The "negative side" is the other half
		*	space. The flag "no side" indicates the plane itself.
		*/
		enum Side
		{
			NO_SIDE,
			POSITIVE_SIDE,
			NEGATIVE_SIDE,
			BOTH_SIDE
		};


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	public:
		static Plane fromOrthogonalLine(const Line& line);


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/** Default constructor */
		Plane();

		/** Construct a plane using the 4 constants directly */
		Plane(float a, float b, float c, float d);

		/** Construct a plane using a normal vector and a point to be on the plane */
		Plane(const glm::vec3& point, const glm::vec3& normal);

		/** Construct a plane using 3 points on the plane */
		Plane(const glm::vec3& point0, const glm::vec3& point1, const glm::vec3& point2);

		/** Get the plane's normal */
		const glm::vec3& getNormal() const;

		/** Get the plane's distance value */
		float getDistance() const;

		/** Redefine this plane based on 3 points */
		void redefine(const glm::vec3& point0, const glm::vec3& point1, const glm::vec3& point2);

		/** Redefine this plane based on a normal and a point */
		void redefine(const glm::vec3& point, const glm::vec3& normal);

		/** Get (signed) distance of the given point to the plane */
		float getDistance(const glm::vec3& point) const;

		/** Get side of the given point relative to the plane */
		Side getSide(const glm::vec3& point) const;

		/** Normalize the plane */
		float normalize();

		/**
		*  @brief
		*    Return a point on the plane
		*
		*  @return
		*    A point on the plane (-D*N)
		*/
		glm::vec3 getPointOnPlane() const;

		//[-------------------------------------------------------]
		//[ Comparison                                            ]
		//[-------------------------------------------------------]
		/** Comparison operator */
		bool operator==(const Plane& rhs) const;

		/** Comparison operator */
		bool operator!=(const Plane& rhs) const;

		//[-------------------------------------------------------]
		//[ Transformation                                        ]
		//[-------------------------------------------------------]
		Plane& operator *=(const Transform& worldTransform);


	//[-------------------------------------------------------]
	//[ Protected members                                     ]
	//[-------------------------------------------------------]
	protected:
		glm::vec3 mNormal;		///< Normal vector of the plane; must not be zero vector, but is not necessarily normalized
		float	  mDistance;	///< Origin point of the line


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf

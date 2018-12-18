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
	*    Axis align bounding box class
	*
	*  @remarks
	*    @code
	*        3+------+2  y
	*        /|     /|   |
	*       / |    / |   |
	*      / 0+---/--+1  *---x
	*    7+------+6 /   /
	*     | /    | /   z
	*     |/     |/
	*    4+------+5
	*    @endcode
	*
	*  @note
	*    - It's highly recommended (but not enforced by this class) that the minimum is really the minimum and the maximum is really the maximum
	*/
	class QSF_API_EXPORT AxisAlignedBoundingBox
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Axis
		*/
		enum Axis
		{
			X = 0,	///< X axis
			Y = 1,	///< Y axis
			Z = 2	///< Z axis
		};

		/**
		*  @brief
		*    Type returned from call to "qsf::AxisAlignedBoundingBox::intersectWithAabb()"
		*/
		enum IntersectionType
		{
			INSIDE,
			INTERSECT,
			OUTSIDE
		};


	//[-------------------------------------------------------]
	//[ Public data                                           ]
	//[-------------------------------------------------------]
	public:
		glm::vec3 minimum;	///< Minimum position
		glm::vec3 maximum;	///< Maximum position


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor setting all minimum and maximum components to 0
		*/
		inline AxisAlignedBoundingBox();

		/**
		*  @brief
		*    Copy constructor
		*
		*  @param[in] source
		*    Source to copy from
		*/
		inline AxisAlignedBoundingBox(const AxisAlignedBoundingBox& source);

		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] minimumMaximum
		*    Minimum and maximum position
		*/
		inline AxisAlignedBoundingBox(const glm::vec3& minimumMaximum);

		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] minimum
		*    Minimum position
		*  @param[in] maximum
		*    Maximum position
		*/
		inline AxisAlignedBoundingBox(const glm::vec3& minimum, const glm::vec3& maximum);

		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] minimumX
		*    X component of the minimum position
		*  @param[in] minimumY
		*    Y component of the minimum position
		*  @param[in] minimumZ
		*    Z component of the minimum position
		*  @param[in] maximumX
		*    X component of the maximum position
		*  @param[in] maximumY
		*    Y component of the maximum position
		*  @param[in] maximumZ
		*    Z component of the maximum position
		*/
		inline AxisAlignedBoundingBox(float minimumX, float minimumY, float minimumZ, float maximumX, float maximumY, float maximumZ);

		/**
		*  @brief
		*    Destructor
		*/
		inline ~AxisAlignedBoundingBox();

		/**
		*  @brief
		*    Copy operator
		*
		*  @param[in] source
		*    Source to copy from
		*
		*  @return
		*    Reference to this instance
		*/
		AxisAlignedBoundingBox& operator =(const AxisAlignedBoundingBox& source);

		/**
		*  @brief
		*    Set the minimum and maximum to - FLOAT_MAX and + FLOAT_MAX to create the biggest possible box
		*/
		inline void setToNumericLimit();

		/**
		*  @brief
		*    Return the center of the box
		*
		*  @return
		*    Center of the box ((maximum + minimum) / 2)
		*/
		inline glm::vec3 getCenter() const;

		/**
		*  @brief
		*    Return the width
		*
		*  @return
		*    Width (maximum.x - minimum.x)
		*/
		inline float getSizeX() const;

		/**
		*  @brief
		*    Return the height
		*
		*  @return
		*    Height (maximum.y - minimum.y)
		*/
		inline float getSizeY() const;

		/**
		*  @brief
		*    Return the depth
		*
		*  @return
		*    Depth (maximum.z - minimum.z)
		*/
		inline float getSizeZ() const;

		/**
		*  @brief
		*    Return the longest axis
		*
		*  @return
		*    The longest axis
		*/
		inline Axis getLongestAxis() const;

		/**
		*  @brief
		*    Return the length of the longest axis
		*
		*  @return
		*    The length of the longest axis
		*/
		inline float getLongestAxisLength() const;

		/**
		*  @brief
		*    Return the radius of a sphere placed at the origin (0, 0, 0) enclosing this axis align bounding box
		*
		*  @return
		*    The radius of a sphere placed at the origin (0, 0, 0) enclosing this axis align bounding box
		*/
		inline float getEnclosingRadius() const;

		/**
		*  @brief
		*    Return the radius of a sphere placed at the origin (0, 0, 0) inside this axis align bounding box
		*
		*  @return
		*    The radius of a sphere placed at the origin (0, 0, 0) inside this axis align bounding box
		*
		*  @note
		*    - Smaller as getEnclosingRadius()
		*    - Same as getLongestAxisLength() / 2
		*/
		inline float getInsideRadius() const;

		/**
		*  @brief
		*    Calculate the surface of the box
		*
		*  @return
		*    Surface of the box
		*/
		inline float calculateSurface() const;

		/**
		*  @brief
		*    Calculate the volume of the box
		*
		*  @return
		*    Volume of the box
		*/
		inline float calculateVolume() const;

		/**
		*  @brief
		*    Clip this box with another one
		*
		*  @param[in] enclosedAxisAlignedBoundingBox
		*    Axis align box to clip this box with
		*/
		inline void clipByAABox(const AxisAlignedBoundingBox& enclosedAxisAlignedBoundingBox);

		/**
		*  @brief
		*    Append a vertex to the cubic hull
		*
		*  @param[in] vertex
		*    Vertex to append
		*/
		inline void appendToCubicHull(const glm::vec3& vertex);

		/**
		*  @brief
		*    Combine two axis align boxes
		*
		*  @param[in] axisAlignedBoundingBox
		*    Axis align box to combine with this box
		*/
		inline void combineAABoxes(const AxisAlignedBoundingBox& axisAlignedBoundingBox);

		/**
		*  @brief
		*    Give back the minimal possible distance to point
		*
		*  @param[in] point
		*    A point in space
		*
		*  @return
		*    minimal distance between the box and point
		*/
		inline float getMinimumDistance(const glm::vec3& point) const;

		/**
		*  @brief
		*    Return the 8 corner vertices
		*
		*  @param[out] vertices
		*    This array will receive the 8 corner vertices
		*
		*  @see
		*    - Remarks of this class for the positions of the 8 corner vertices
		*/
		void getVertices(glm::vec3 vertices[8]) const;

		/**
		*  @brief
		*    Return one of the 8 corner vertices
		*
		*  @param[in] index
		*    Index of the corner vertex to return
		*
		*  @return
		*    The requested corner vertex
		*
		*  @see
		*    - getVertices()
		*/
		glm::vec3 getVertex(uint32 index) const;

		/**
		*  @brief
		*    Calculate the index of the nearest vertex in the AAB according to the normal vector of a clip plane
		*
		*  @param[in] clipPlaneNormal
		*    Clip plane normal
		*
		*  @return
		*    Index of the nearest vertex in the AAB according to the normal vector of a clip plane
		*/
		uint32 getNearestVertexIndex(const glm::vec3& clipPlaneNormal) const;

		/**
		*  @brief
		*    Calculate the index of the furthest vertex in the AAB according to the normal vector of a clip plane
		*
		*  @param[in] clipPlaneNormal
		*    Clip plane normal
		*
		*  @return
		*    Index of the furthest vertex in the AAB according to the normal vector of a clip plane
		*/
		uint32 getFurthestVertexIndex(const glm::vec3& clipPlaneNormal) const;

		/**
		*  @brief
		*    Check intersection with another axis aligned bounding box
		*
		*  @param[in] axisAlignedBoundingBox
		*    Axis aligned bounding box to check for intersection against
		*
		*  @return
		*    Determined intersection type
		*/
		IntersectionType intersectWithAabb(const AxisAlignedBoundingBox& axisAlignedBoundingBox) const;

		/**
		*  @brief
		*    Ensure that the minimum is really the minimum and the maximum is really the maximum
		*/
		void validateMinMax();

		/**
		*  @brief
		*    Per component addition
		*
		*  @param[in] vertex
		*    Vertex to add
		*
		*  @return
		*    The resulting axis aligned bounding box
		*/
		inline AxisAlignedBoundingBox operator+ (const glm::vec3& vertex) const;

		/**
		*  @brief
		*    Per component addition
		*
		*  @param[in] scalar
		*    Scalar to add
		*
		*  @return
		*    The resulting axis aligned bounding box
		*/
		inline AxisAlignedBoundingBox operator+ (float scalar) const;

		/**
		*  @brief
		*    Per component addition
		*
		*  @param[in] vertex
		*    Vertex to add
		*
		*  @return
		*    Reference to this instance
		*/
		inline AxisAlignedBoundingBox& operator+= (const glm::vec3& vertex);

		/**
		*  @brief
		*    Per component addition
		*
		*  @param[in] scalar
		*    Scalar to add
		*
		*  @return
		*    Reference to this instance
		*/
		inline AxisAlignedBoundingBox& operator+= (float scalar);

		/**
		*  @brief
		*    Per component subtraction
		*
		*  @param[in] vertex
		*    Vertex to subtract
		*
		*  @return
		*    The resulting axis aligned bounding box
		*/
		inline AxisAlignedBoundingBox operator- (const glm::vec3& vertex) const;

		/**
		*  @brief
		*    Per component subtraction
		*
		*  @param[in] scalar
		*    Scalar to subtract
		*
		*  @return
		*    The resulting axis aligned bounding box
		*/
		inline AxisAlignedBoundingBox operator- (float scalar) const;

		/**
		*  @brief
		*    Per component subtraction
		*
		*  @param[in] vertex
		*    Vertex to subtract
		*
		*  @return
		*    Reference to this instance
		*/
		inline AxisAlignedBoundingBox& operator-= (const glm::vec3& vertex);

		/**
		*  @brief
		*    Per component subtraction
		*
		*  @param[in] scalar
		*    Scalar to subtract
		*
		*  @return
		*    Reference to this instance
		*/
		inline AxisAlignedBoundingBox& operator-= (float scalar);

		/**
		*  @brief
		*    Per component multiplication
		*
		*  @param[in] vertex
		*    Vertex to multiplicate with
		*
		*  @return
		*    The resulting axis aligned bounding box
		*/
		inline AxisAlignedBoundingBox operator* (const glm::vec3& vertex) const;

		/**
		*  @brief
		*    Per component multiplication
		*
		*  @param[in] scalar
		*    Scalar to multiplicate with
		*
		*  @return
		*    The resulting axis aligned bounding box
		*/
		inline AxisAlignedBoundingBox operator* (float scalar) const;

		/**
		*  @brief
		*    Per component multiplication
		*
		*  @param[in] vertex
		*    Vertex to multiplicate with
		*
		*  @return
		*    Reference to this instance
		*/
		inline AxisAlignedBoundingBox& operator*= (const glm::vec3& vertex);

		/**
		*  @brief
		*    Per component multiplication
		*
		*  @param[in] scalar
		*    Scalar to multiplicate with
		*
		*  @return
		*    Reference to this instance
		*/
		inline AxisAlignedBoundingBox& operator*= (float scalar);

		/**
		*  @brief
		*    Per component division
		*
		*  @param[in] vertex
		*    Vertex to divide through
		*
		*  @return
		*    The resulting axis aligned bounding box
		*/
		inline AxisAlignedBoundingBox operator/ (const glm::vec3& vertex) const;

		/**
		*  @brief
		*    Per component division
		*
		*  @param[in] scalar
		*    Scalar to divide through
		*
		*  @return
		*    The resulting axis aligned bounding box
		*/
		inline AxisAlignedBoundingBox operator/ (float scalar) const;

		/**
		*  @brief
		*    Per component division
		*
		*  @param[in] vertex
		*    Vertex to divide through
		*
		*  @return
		*    Reference to this instance
		*/
		inline AxisAlignedBoundingBox& operator/= (const glm::vec3& vertex);

		/**
		*  @brief
		*    Per component division
		*
		*  @param[in] scalar
		*    Scalar to divide through
		*
		*  @return
		*    Reference to this instance
		*/
		inline AxisAlignedBoundingBox& operator/= (float scalar);


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/math/AxisAlignedBoundingBox-inl.h"

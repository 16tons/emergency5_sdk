// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/Export.h"

#include <string>
#include <assert.h>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/vector_angle.hpp>

#include <boost/noncopyable.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Segment;
	class Ray;
	class Line;
	class Plane;
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
	*    Static math class
	*
	*  @remarks
	*    In general we're using the following math libraries:
	*    - OpenGL mathematics library (GLM)
	*    - Bullet linear math
	*    - Boost math
	*
	*    Our QSF math class only provides math features which are not provided by the listed math libraries.
	*/
	class QSF_API_EXPORT Math : public boost::noncopyable
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const glm::vec2 GLM_VEC2_ZERO;		///< Zero vector
		static const glm::vec2 GLM_VEC2_UNIT_X;		///< Unit vector x-axis
		static const glm::vec2 GLM_VEC2_UNIT_Y;		///< Unit vector y-axis
		static const glm::vec2 GLM_VEC2_UNIT_XY;	///< Sum of unit vectors

		static const glm::vec3 GLM_VEC3_ZERO;		///< Zero vector
		static const glm::vec3 GLM_VEC3_UNIT_X;		///< Unit vector x-axis
		static const glm::vec3 GLM_VEC3_UNIT_Y;		///< Unit vector y-axis
		static const glm::vec3 GLM_VEC3_UNIT_Z;		///< Unit vector z-axis
		static const glm::vec3 GLM_VEC3_UNIT_XYZ;	///< Sum of unit vectors

		static const glm::vec4 GLM_VEC4_ZERO;		///< Zero vector
		static const glm::vec4 GLM_VEC4_UNIT_X;		///< Unit vector x-axis
		static const glm::vec4 GLM_VEC4_UNIT_Y;		///< Unit vector y-axis
		static const glm::vec4 GLM_VEC4_UNIT_Z;		///< Unit vector z-axis
		static const glm::vec4 GLM_VEC4_UNIT_W;		///< Unit vector w-axis
		static const glm::vec4 GLM_VEC4_UNIT_XYZW;	///< Sum of unit vectors

		static const glm::quat   GLM_QUAT_IDENTITY;		///< Identity quaternion
		static const glm::mat3x3 GLM_MAT3X3_IDENTITY;	///< Identity 3x3 matrix

		static const uint32 FNV1a_32_INITIAL_HASH = 0xcbf29ce4;
		static const uint64 FNV1a_64_INITIAL_HASH = 0xcbf29ce484222325;


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	public:
		//[-------------------------------------------------------]
		//[ Hash                                                  ]
		//[-------------------------------------------------------]
		static uint32 calculateFNV1a_32(const char* content, std::streamsize numberOfBytes, uint32 hash = FNV1a_32_INITIAL_HASH);
		static uint64 calculateFNV1a_64(const char* content, std::streamsize numberOfBytes, uint64 hash = FNV1a_64_INITIAL_HASH);

		//[-------------------------------------------------------]
		//[ Encryption / decryption                               ]
		//[-------------------------------------------------------]
		static std::string encryptString(const std::string& sourceString);
		static std::string decryptString(const std::string& encryptedString);

		//[-------------------------------------------------------]
		//[ Value                                                 ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Clamp the given value into the interval [minimum, maximum]
		*
		*  @param[in] value
		*    Value to check
		*  @param[in] minimum
		*    Minimum of the interval, must be <= maximum!
		*  @param[in] maximum
		*    Maximum of the interval, must be >= minimum!
		*
		*  @return
		*    The value within the interval [minimum, maximum]
		*/
		template <typename Real>
		static Real clamp(Real value, Real minimum, Real maximum);

		/**
		*  @brief
		*    Clamp the given value into the interval [0.0f, 1.0f]
		*
		*  @param[in] value
		*    Value to check
		*
		*  @return
		*    The value within the interval [0.0f, 1.0f]
		*/
		static inline float saturate(float value);

		/**
		*  @brief
		*    Ensure that the given value is within the given interval [minimum, maximum] by wrapping the value
		*
		*  @param[in] value
		*    Value to check
		*  @param[in] minimum
		*    Minimum of the interval, must be < maximum!
		*  @param[in] maximum
		*    Maximum of the interval, must be > minimum!
		*
		*  @return
		*    The value within the interval [minimum, maximum]
		*
		*  @note
		*    - In case of violating the interface specification by swapping minimum/maximum the result will not be different to the one of the correct order
		*/
		static float wrapToInterval(float value, float minimum, float maximum);

		/**
		*  @brief
		*    Makes a given value to a multiple of another given value
		*
		*  @param[in] value
		*    Value to make a multiple of the given second value (e.g. 0.9)
		*  @param[in] primaryValue
		*    Value the first one should be a multiple of (e.g. 1.5)
		*
		*  @return
		*    The updated value (e.g. 1.5)
		*/
		static float makeMultipleOf(float value, float primaryValue);

		//[-------------------------------------------------------]
		//[ Power of two                                          ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Check whether the given the value is a power of two
		*
		*  @param[in] value
		*    Value to check
		*
		*  @return
		*    "true" if the number is a power of two, else "false"
		*/
		static inline bool isPowerOfTwo(int value);

		/**
		*  @brief
		*    Return the next smallest power of two
		*
		*  @param[in] value
		*    Value to check
		*
		*  @return
		*    The next smallest power of two of the given value
		*/
		static int getNextSmallestPowerOfTwo(int value);

		//[-------------------------------------------------------]
		//[ Axis                                                  ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Calculate the major axis of a given vector, i.e. the axis (x,y,z) with highest absolute value
		*
		*  @param[in] vec
		*    The vector to get major axis of
		*  @return
		*    The resulting axis: 0 for x-axis, 1 for y-axis, 2 for z-axis
		*/
		static int getMajorAxisOfVector(const glm::vec3& vec);

		/**
		*  @brief
		*    Calculate the right/up vectors of the given vector
		*
		*  @param[in]  sourceVector
		*    Vector to calculate the right/up vectors for
		*  @param[out] rightVector
		*    The resulting right vector which is perpendicular to the source vector
		*  @param[out] upVector
		*    The resulting up vector which is perpendicular to the source vector
		*/
		static void getRightUpVector(const glm::vec3& sourceVector, glm::vec3& rightVector, glm::vec3& upVector);

		//[-------------------------------------------------------]
		//[ Rotation                                              ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Calculate a quaternion describing a rotation around an axis of a given angle.
		*
		*  @param[in] axis
		*    Axis vector to rotate around.
		*  @param[in] radians
		*    The rotation angle in radians.
		*  @return
		*    The resulting quaternion.
		*/
		static glm::quat getRotationAroundAxis(const glm::vec3& axis, float radians);

		/**
		*  @brief
		*    Calculate a quaternion from a direction vector.
		*
		*  @param[in] origin
		*    Position of the object that should be rotated.
		*  @param[in] target
		*    Target position where to rotate to.
		*  @param[in] upVector
		*    Hint for the "up" direction (local positive-y) of the resulting rotation.
		*  @return
		*    The resulting quaternion.
		*/
		static glm::quat getLookAtQuaternion(const glm::vec3& origin, const glm::vec3& target, const glm::vec3& upVector);

		/**
		*  @brief
		*    Calculate a quaternion from a direction vector.
		*
		*  @param[in] direction
		*    Direction that should be the "in" vector (local negative-z) of the resulting rotation.
		*  @param[in] upVector
		*    Hint for the "up" direction (local positive-y) of the resulting rotation.
		*  @return
		*    The resulting quaternion.
		*/
		static glm::quat getLookAtQuaternion(const glm::vec3& direction, const glm::vec3& upVector);

		/**
		*  @brief
		*    Calculate a quaternion from an up vector and a given rotation that delivers a yaw angle to preserve
		*
		*  @param[in] upVector
		*    Direction that should be the "up" vector (positive-y) of the resulting rotation
		*  @param[in] oldRotation
		*    Old rotation quaternion to take the yaw Euler angle from
		*  @return
		*    The resulting quaternion
		*/
		static glm::quat getRotationByUpVector(const glm::vec3& upVector, const glm::quat& oldRotation);

		/**
		*  @brief
		*    Return normalized direction in x,z world coordinates
		*
		*  @param[in] yaw
		*    Yaw Euler angle in degrees [0,360], clockwise orientation and starting from north for 0.0f
		*
		*  @return
		*    Normalized direction in x,z world coordinates
		*/
		static glm::vec2 eulerAnglesToXZDirectionVector(float yaw);

		/**
		*  @brief
		*    Convert direction vector to yaw Euler angle
		*
		*  @param[in] yaw
		*    Direction in x,z world coordinates
		*
		*  @return
		*    Yaw Euler angle in degrees [0,360], clockwise orientation and starting from north for 0.0f
		*/
		static float xzDirectionVectorToYawEulerAngle(const glm::vec2& direction);

		//[-------------------------------------------------------]
		//[ Intersect                                             ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Calculate intersection of a ray with a plane in 3D space
		*
		*  @param[in] ray
		*    Ray to be intersected with plane; intersections will be returned only in ray's positive direction
		*  @param[in] plane
		*    Plane to be intersected with ray
		*  @param[out] outIntersectionPoint
		*    Pointer to a vector that should hold the resulting intersection point, or nullptr; not written if there's no intersection
		*  @param[out] outIntersectionT
		*    Pointer to a float value that should hold the intersection point's relative position along the ray direction, or nullptr; not written if there's no intersection
		*
		*  @return
		*    True if there was an intersection found, false otherwise
		*/
		static bool intersectRayWithPlane(const Ray& ray, const Plane& plane, glm::vec3* outIntersectionPoint = nullptr, float* outIntersectionT = nullptr);

		/**
		*  @brief
		*    Calculate intersection of a line with a plane in 3D space
		*
		*  @param[in] line
		*    Line to be intersected with plane
		*  @param[in] plane
		*    Plane to be intersected with line
		*  @param[out] outIntersectionPoint
		*    Pointer to a vector that should hold the resulting intersection point, or nullptr; not written if there's no intersection
		*  @param[out] outIntersectionT
		*    Pointer to a float value that should hold the intersection point's position along the line direction, or nullptr; not written if there's no intersection
		*
		*  @return
		*    True if there was an intersection found, false otherwise
		*/
		static bool intersectLineWithPlane(const Line& line, const Plane& plane, glm::vec3* outIntersectionPoint = nullptr, float* outIntersectionT = nullptr);

		/**
		*  @brief
		*    Calculate intersection of a line segment with a plane in 3D space
		*
		*  @param[in] segment
		*    Line segment to be intersected with plane; intersections will be returned only if they are between the two points defining the segment.
		*  @param[in] plane
		*    Plane to be intersected with line segment
		*  @param[out] outIntersectionPoint
		*    Pointer to a vector that should hold the resulting intersection point, or nullptr; not written if there's no intersection
		*  @param[out] outIntersectionT
		*    Pointer to a float value that should hold the intersection point's position along the segment direction, or nullptr; not written if there's no intersection
		*
		*  @return
		*    True if there was an intersection found, false otherwise
		*/
		static bool intersectSegmentWithPlane(const Segment& segment, const Plane& plane, glm::vec3* outIntersectionPoint = nullptr, float* outIntersectionT = nullptr);

		/**
		*  @brief
		*    Calculate nearest point between two lines
		*
		*  @param[in] line1
		*    First line; note that the resulting point will be located on this line
		*  @param[in] line2
		*    Second line
		*  @param[out] outPoint
		*    Pointer to a vector that should hold the resulting point, or nullptr; not written if function returns false
		*  @param[out] outT
		*    Pointer to a float value that should hold the result point's position along line1, or nullptr; not written if function returns false
		*
		*  @return
		*    False only if lines are parallel, otherwise true
		*/
		static bool nearestPointBetweenLines(const Line& line1, const Line& line2, glm::vec3* outPoint = nullptr, float* outT = nullptr);

		/**
		*  @brief
		*    Calculate nearest point on a line to given point
		*
		*  @param[in] point
		*    The point.
		*  @param[in] line
		*    The line.
		*
		*  @return
		*    Resulting point on the line.
		*/
		static glm::vec3 getClosestPointOnLine(const glm::vec3& point, const Line& line);

		/**
		*  @brief
		*    Calculate nearest point on a line to given point as a factor applied to the direction of the line
		*
		*  @param[in] point
		*    The point.
		*  @param[in] line
		*    The line.
		*
		*  @return
		*    Factor that defines to the closest point when applied to the direction of the line.
		*/
		static float getClosestPointOnLineAsDirectionFactor(const glm::vec3& point, const Line& line);

		/**
		*  @brief
		*    Calculate nearest point on a ray to given point
		*
		*  @param[in] point
		*    The point.
		*  @param[in] ray
		*    The ray.
		*
		*  @return
		*    Resulting point on the ray.
		*/
		static glm::vec3 getClosestPointOnRay(const glm::vec3& point, const Ray& ray);

		/**
		*  @brief
		*    Calculate nearest point on a line segment to given point
		*
		*  @param[in] point
		*    The point.
		*  @param[in] segment
		*    The line segment.
		*
		*  @return
		*    Resulting point on the line segment.
		*/
		static glm::vec3 getClosestPointOnLineSegment(const glm::vec3& point, const Segment& segment);


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/math/Math-inl.h"

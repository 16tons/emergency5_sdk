// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/math/Line.h"

#include <glm/gtc/quaternion.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class BinarySerializer;
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
	*    3D transform class
	*
	*  @remarks
	*    The design goal of this class was to be as memory compact as possible and to provide a class
	*    which is able to convert between different representations of a 3D transform in an efficient way.
	*/
	class QSF_API_EXPORT Transform
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor (set's an identity transform matrix)
		*/
		inline Transform();

		/**
		*  @brief
		*    Copy constructor
		*
		*  @param[in] source
		*    Source to copy from
		*/
		inline Transform(const Transform& source);

		/**
		*  @brief
		*    Constructor from position, rotation and scale
		*
		*  @param[in] position
		*    Position to set
		*  @param[in] rotation
		*    Optional rotation to set
		*  @param[in] scale
		*    Optional scale to set
		*/
		inline explicit Transform(const glm::vec3& position, const glm::quat& rotation = glm::quat(), const glm::vec3& scale = glm::vec3(1.0f, 1.0f, 1.0f));

		/**
		*  @brief
		*    Constructor from transformation matrix
		*
		*  @param[in] transformMatrix
		*    Matrix to incorporate as transformation
		*/
		inline explicit Transform(const glm::mat4x4& transformMatrix);

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
		inline Transform& operator=(const Transform& source);

		/**
		*  @brief
		*    Equal compare operator
		*
		*  @param[in] other
		*    Other transform instance to compare with
		*
		*  @return
		*    "true" if both transform instances are equal (no epsilon used), else "false"
		*/
		inline bool operator==(const Transform& other) const;

		/**
		*  @brief
		*    Not equal compare operator
		*
		*  @param[in] other
		*    Other transform instance to compare with
		*
		*  @return
		*    "true" if both transform instances are not equal (no epsilon used), else "false"
		*/
		inline bool operator!=(const Transform& other) const;

		/**
		*  @brief
		*    Add operator
		*
		*  @param[in] other
		*    Other transform instance to add to this transform
		*
		*  @return
		*    Reference to this instance
		*/
		Transform& operator+=(const Transform& other);

		/**
		*  @brief
		*    Subtract operator
		*
		*  @param[in] other
		*    Other transform instance to subtract from this transform
		*
		*  @return
		*    Reference to this instance
		*/
		Transform& operator-=(const Transform& other);

		/**
		*  @brief
		*    Return the position
		*
		*  @return
		*    Position
		*/
		inline const glm::vec3& getPosition() const;

		/**
		*  @brief
		*    Set the position
		*
		*  @param[in] position
		*    New position
		*/
		inline void setPosition(const glm::vec3& position);

		/**
		*  @brief
		*    Return the rotation quaternion
		*
		*  @return
		*    Rotation quaternion
		*/
		inline const glm::quat& getRotation() const;

		/**
		*  @brief
		*    Set the rotation quaternion
		*
		*  @param[in] rotation
		*    New rotation quaternion
		*/
		inline void setRotation(const glm::quat& rotation);

		/**
		*  @brief
		*    Return the scale
		*
		*  @return
		*    Scale
		*/
		inline const glm::vec3& getScale() const;

		/**
		*  @brief
		*    Set the scale
		*
		*  @param[in] scale
		*    New scale
		*/
		inline void setScale(const glm::vec3& scale);

		/**
		*  @brief
		*    Set the position and rotation
		*
		*  @param[in] position
		*    New position
		*  @param[in] rotation
		*    New rotation quaternion
		*/
		inline void setPositionAndRotation(const glm::vec3& position, const glm::quat& rotation);

		//[-------------------------------------------------------]
		//[ Matrix                                                ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Set the current final transform matrix
		*
		*  @param[in] transform
		*    The current final transform matrix
		*
		*  @note
		*    - The scale can't be extracted correctly from the given transform matrix if one or
		*      two scale components are negative while another is/are not (we can't figure out which axis are negative!)
		*/
		void setMatrix(const glm::mat4x4& transform);

		/**
		*  @brief
		*    Calculate the current final transform matrix
		*
		*  @param[out] matrix
		*    Receives the calculated final transform matrix
		*
		*  @note
		*    - No internal caching due to memory constrains
		*/
		void calculateMatrix(glm::mat4x4& matrix) const;

		/**
		*  @brief
		*    Calculate the current final inverse transform matrix
		*
		*  @param[out] inverseMatrix
		*    Receives the calculated final inverse transform matrix
		*
		*  @note
		*    - No internal caching due to memory constrains
		*/
		void calculateInverseMatrix(glm::mat4x4& inverseMatrix) const;

		//[-------------------------------------------------------]
		//[ Transform                                             ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Create a transform that describes a local transformation
		*
		*  @param[in] worldTransform
		*    World space transformation to convert to local space
		*  @param[in] referenceTransform
		*    Reference transformation that defines local space coordinates and is expressed in world space coordinates
		*/
		void createLocalTransform(const Transform& worldTransform, const Transform& referenceTransform);

		/**
		*  @brief
		*    Create a transform object from a given local transformation
		*
		*  @param[in] localTransform
		*    Local space transformation to convert to world space
		*  @param[in] referenceTransform
		*    Reference transformation that defines local space coordinates and is expressed in world space coordinates
		*/
		inline void fromLocalTransform(const Transform& localTransform, const Transform& referenceTransform);

		/**
		*  @brief
		*    Transform a given world space position into a local space position relative to a given world space transform
		*
		*  @param[in] position
		*    World space position to convert to local space
		*
		*  @return
		*    The resulting local space position
		*
		*  @note
		*    - This transform instance is used as reference transformation that defines local space coordinates and is expressed in world space coordinates
		*/
		glm::vec3 vec3PositionWorldToLocal(const glm::vec3& position) const;

		/**
		*  @brief
		*    Transform a given local space position relative to a given world space transform into a world space position
		*
		*  @param[in] position
		*    Local space position to convert to world space
		*
		*  @return
		*    The resulting world space position
		*
		*  @note
		*    - This transform instance is used as reference transformation that defines local space coordinates and is expressed in world space coordinates
		*/
		glm::vec3 vec3PositionLocalToWorld(const glm::vec3& position) const;

		/**
		*  @brief
		*    Transform a given world space direction vector into a local space vector relative to a given world space transform
		*
		*  @param[in] direction
		*    World space direction vector to convert to local space; does not need to be normalized
		*
		*  @return
		*    The resulting local space vector
		*
		*  @note
		*    - This transform instance is used as reference transformation that defines local space coordinates and is expressed in world space coordinates
		*/
		glm::vec3 vec3DirectionWorldToLocal(const glm::vec3& direction) const;

		/**
		*  @brief
		*    Transform a given local space direction vector relative to a given world space transform into a world space vector
		*
		*  @param[in] direction
		*    Local space direction to convert to world space; does not need to be normalized
		*
		*  @return
		*    The resulting world space vector
		*
		*  @note
		*    - This transform instance is used as reference transformation that defines local space coordinates and is expressed in world space coordinates
		*/
		glm::vec3 vec3DirectionLocalToWorld(const glm::vec3& direction) const;

		/**
		*  @brief
		*    Transform a given world space rotation into a local space rotation relative to a given world space transform
		*
		*  @param[in] rotation
		*    World space rotation to convert to local space
		*
		*  @return
		*    The resulting local space position
		*
		*  @note
		*    - This transform instance is used as reference transformation that defines local space coordinates and is expressed in world space coordinates
		*    - The scaling is not used, in case of negative scaling we change the coordinate system, this is not supported, we get wrong results
		*/
		glm::quat quatRotationWorldToLocal(const glm::quat& rotation) const;

		/**
		*  @brief
		*    Transform a given local space rotation relative to a given world space transform into a world space rotation
		*
		*  @param[in] rotation
		*    Local space rotation to convert to world space
		*
		*  @return
		*    The resulting world space rotation
		*
		*  @note
		*    - This transform instance is used as reference transformation that defines local space coordinates and is expressed in world space coordinates
		*    - The scaling is not used, in case of negative scaling we change the coordinate system, this is not supported, we get wrong results
		*/
		glm::quat quatRotationLocalToWorld(const glm::quat& rotation) const;

		/**
		*  @brief
		*    Transform a given world space line into a local space line relative to a given world space transform
		*
		*  @param[in] line
		*    World space line to convert to local space
		*
		*  @return
		*    The resulting local space line
		*
		*  @note
		*    - This transform instance is used as reference transformation that defines local space coordinates and is expressed in world space coordinates
		*/
		Line lineWorldToLocal(const Line& line) const;

		/**
		*  @brief
		*    Transform a given local space line relative to a given world space transform into a world space line
		*
		*  @param[in] line
		*    Local space line to convert to world space
		*
		*  @return
		*    The resulting world space line
		*
		*  @note
		*    - This transform instance is used as reference transformation that defines local space coordinates and is expressed in world space coordinates
		*/
		Line lineLocalToWorld(const Line& line) const;

		//[-------------------------------------------------------]
		//[ Serialization                                         ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Serialize this transform instance
		*
		*  @param[in] serializer
		*    The serializer, which can be either in read or write mode
		*/
		void serialize(BinarySerializer& serializer);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		glm::vec3 mPosition;	///< Position as vector
		glm::quat mRotation;	///< Rotation as quaternion
		glm::vec3 mScale;		///< Scale as vector


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/math/Transform-inl.h"

// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <LinearMath/btVector3.h>
#include <LinearMath/btQuaternion.h>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	// File contains helper functionality for converting between glm and bullet low level formats

	/**
	* @brief
	* Convert between glm and bullet representation of a vector of three components.
	*
	* @remarks
	* The position is sometimes treated different between glm and bullet.
	* The physical position is often at the mass center for regular shapes like a box while the logical glm position is at the middle of the 2D ground plane.
	* That is the height is integrated in only one of the representations.
	* Therefore it can be very dangerous to simply convert positions between these two representations.
	* Use the convertPosition inside the BulletCollisionComponents when approaching bullet shape position transformation.
	*/
	//@{
	inline glm::vec3 convertVector3(const btVector3& in);
	inline btVector3 convertVector3(const glm::vec3& in);
	//@}

	/**
	* @brief
	* Convert between glm and bullet representations of a quaternion.
	*/
	//@{
	inline btQuaternion convertQuaternion(const glm::quat& in);
	inline glm::quat convertQuaternion(const btQuaternion& in);
	//@}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/math/GlmBulletConversion-inl.h"

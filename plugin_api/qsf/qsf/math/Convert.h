// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/Export.h"

#include <OGRE/OgreRay.h>
#include <OGRE/OgrePlane.h>
#include <OGRE/OgreVector2.h>
#include <OGRE/OgreVector3.h>
#include <OGRE/OgreQuaternion.h>
#include <OGRE/OgreColourValue.h>

#include <glm/fwd.hpp>

#include <boost/noncopyable.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Ogre
{
	class Frustum;
	class PlaneBoundedVolume;
}
namespace qsf
{
	class Ray;
	class Plane;
	class Frustum;
	class PlaneBoundedVolume;
	class Color4;
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
	*    Static type conversion class, for conversions between glm, OGRE and Qt types
	*/
	class QSF_API_EXPORT Convert : public boost::noncopyable
	{


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	public:
		/** Convert Ogre::Vector2 to glm::vec2 */
		static glm::vec2 getGlmVec2(const Ogre::Vector2& vec);

		/** Convert glm::vec2 to Ogre::Vector2 */
		static Ogre::Vector2 getOgreVector2(const glm::vec2& vec);

		/** Convert Ogre::Vector3 to glm::vec3 */
		static glm::vec3 getGlmVec3(const Ogre::Vector3& vec);

		/** Convert glm::vec3 to Ogre::Vector3 */
		static Ogre::Vector3 getOgreVector3(const glm::vec3& vec);

		/** Convert Ogre::Quaternion to glm::quat */
		static glm::quat getGlmQuat(const Ogre::Quaternion& quat);

		/** Convert glm::quat to Ogre::Quaternion */
		static Ogre::Quaternion getOgreQuaternion(const glm::quat& quaternion);

		/** Convert Ogre::Matrix4 to glm::mat4x4 */
		static glm::mat4x4 getGlmMat4x4(const Ogre::Matrix4& matrix);

		/** Convert glm::mat4x4 to Ogre::Matrix4 */
		static Ogre::Matrix4 getOgreMatrix4(const glm::mat4x4& mat);

		/** Convert Ogre::Ray to qsf::Ray */
		static Ray getRay(const Ogre::Ray& ogreRay);

		/** Convert qsf::Ray to Ogre::Ray */
		static Ogre::Ray getOgreRay(const Ray& ray);

		/** Convert Ogre::Plane to qsf::Plane */
		static Plane getPlane(const Ogre::Plane& ogrePlane);

		/** Convert qsf::Plane to Ogre::Plane */
		static Ogre::Plane getOgrePlane(const Plane& plane);

		/** Convert Ogre::Frustum to qsf::Frustum */
		static void getFrustum(const Ogre::Frustum& ogreFrustum, Frustum& outFrustum);

		/** Convert qsf::PlaneBoundedVolume to Ogre::PlaneBoundedVolume */
		static void getOgrePlaneBoundedVolume(const PlaneBoundedVolume& volume, Ogre::PlaneBoundedVolume& outOgreVolume);

		/** Convert qsf::Color4 to Ogre::ColourValue */
		static Ogre::ColourValue getOgreColourValue(const Color4& color);


	};


} // qsf

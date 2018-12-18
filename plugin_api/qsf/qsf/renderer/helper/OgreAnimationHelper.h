// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/Export.h"

#include <glm/fwd.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Ogre
{
	namespace v1
	{
		class Entity;
		class OldBone;
	}
}
namespace qsf
{
	class Entity;
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
	*    Static helper class providing OGRE animation utility functions
	*/
	class QSF_API_EXPORT OgreAnimationHelper
	{


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Get a bone's world transformation
		*
		*  @param[in] entity
		*    The entity whose transformation and animation state is used
		*  @param[in] boneName
		*    The bone name
		*  @param[out] outWorldPosition
		*    Resulting world position of the bone
		*  @param[out] outWorldRotation
		*    Resulting world rotation of the bone
		*
		*  @return
		*    "true" if everything went fine, "false" if e.g. the bone was not found
		*/
		static bool getBoneWorldTransformation(const Entity& entity, const std::string& boneName, glm::vec3& outWorldPosition, glm::quat& outWorldRotation);

		/**
		*  @brief
		*    Get a bone's world transformation
		*
		*  @param[in] ogreEntity
		*    The OGRE entity whose animation state is used
		*  @param[in] ogreBone
		*    The OGRE bone
		*  @param[out] outWorldPosition
		*    Resulting world position of the bone
		*  @param[out] outWorldRotation
		*    Resulting world rotation of the bone
		*/
		static void getBoneWorldTransformation(const Ogre::v1::Entity& ogreEntity, const Ogre::v1::OldBone& ogreBone, glm::vec3& outWorldPosition, glm::quat& outWorldRotation);

		/**
		*  @brief
		*    Get a bone pointer
		*
		*  @param[in] entity
		*    QSF entity reference
		*  @param[in] boneName
		*    Name of the OGRE bone
		*
		*  @return
		*    Pointer to a OGRE old bone, null pointer if not found
		*/
		static Ogre::v1::OldBone* getOgreBone(Entity& entity, const std::string& boneName);

		/**
		*  @brief
		*    Get a bone pointer
		*
		*  @param[in] entity
		*    QSF entity reference
		*  @param[in] boneName
		*    Name of the OGRE bone
		*
		*  @return
		*    Pointer to a OGRE old bone, null pointer if not found
		*/
		static Ogre::v1::OldBone* getOgreBone(Entity& entity, uint16 boneHandle);


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf

// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/Export.h"

#include <OGRE/OgreManualObject2.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Ogre
{
	class Ray;
	namespace v1
	{
		class Mesh;
	}
}
namespace qsf
{
	class PlaneBoundedVolume;
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
	*    Helper class for retrieving mesh intersection information
	*
	*  @todo
	*    - TODO(co) Review and cleanup texture coordinate calculation. Was added quick'n'dirty for the SANTRAIN project and hacks were added by an external SANTRAIN programmer.
	*/
	class QSF_API_EXPORT OgreMeshIntersectionHelper
	{


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	public:
		/** Mesh and ray check (ray and result is in local space) */
		static bool getMeshRayIntersection(Ogre::v1::Entity& ogreEntity, const Ogre::Ray& ogreRay, float& closestDistance, Ogre::v1::SubMesh** hitOgreSubMesh = nullptr, glm::vec3* outNormal = nullptr, glm::vec2* outTextureCoordinate = nullptr, bool lowestLod = true);
		static bool getMeshRayIntersection(const Ogre::v1::Mesh& mesh, const Ogre::Ray& ogreRay, float& closestDistance, Ogre::v1::SubMesh** hitOgreSubMesh = nullptr, glm::vec3* outNormal = nullptr, glm::vec2* outTextureCoordinate = nullptr);
		static bool getMeshRayIntersection(const Ogre::ManualObject& manualObject, const Ogre::Ray& ogreRay, float& closestDistance, Ogre::ManualObject::ManualObjectSection** hitOgreManualObjectSection = nullptr);

		/** Mesh and plane bounded volume check (volume is in local space) */
		static bool isMeshContainedInPlaneBoundedVolume(Ogre::v1::Entity& ogreEntity, const PlaneBoundedVolume& planeBoundedVolume, bool lowestLod = true);
		static bool isMeshContainedInPlaneBoundedVolume(const Ogre::v1::Mesh& mesh, const PlaneBoundedVolume& planeBoundedVolume);
		static bool isMeshContainedInPlaneBoundedVolume(const Ogre::ManualObject& manualObject, const PlaneBoundedVolume& planeBoundedVolume);


	//[-------------------------------------------------------]
	//[ Private static methods                                ]
	//[-------------------------------------------------------]
	private:
		static bool internalGetMeshRayIntersection(const Ogre::v1::Mesh& mesh, Ogre::v1::Entity* ogreEntity, bool useSoftwareBlendingVertices, const Ogre::Ray& ogreRay, float& closestDistance, Ogre::v1::SubMesh** hitOgreSubMesh, glm::vec3* outNormal, glm::vec2* outTextureCoordinate);
		static bool internalIsMeshContainedInPlaneBoundedVolume(const Ogre::v1::Mesh& mesh, Ogre::v1::Entity* ogreEntity, bool useSoftwareBlendingVertices, const PlaneBoundedVolume& planeBoundedVolume);


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf

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
	namespace v1
	{
		class Mesh;
	}
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
	*    Helper class for retrieving mesh information
	*/
	class QSF_API_EXPORT OgreMeshInformationHelper
	{


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	public:
		/** Get an OGRE entity's triangles, output lists are not cleared before new entries are added */
		static void getMeshInformation(Ogre::v1::Entity& entity, std::vector<Ogre::Vector3>& vertices, std::vector<uint32>& indices, const Ogre::Vector3& position, const Ogre::Quaternion& rotation, const Ogre::Vector3& scale);
		static void getMeshInformation(Ogre::v1::Entity& entity, std::vector<Ogre::Vector3>& vertices, std::vector<Ogre::Vector3>& vertexNormals, std::vector<uint32>& indices);

		/** Get an OGRE mesh's triangles, output lists are not cleared before new entries are added */
		static void getMeshInformation(const Ogre::v1::Mesh& mesh, std::vector<Ogre::Vector3>& vertices, std::vector<uint32>& indices);
		static void getMeshInformation(const Ogre::v1::Mesh& mesh, std::vector<Ogre::Vector3>& vertices, std::vector<Ogre::Vector3>& vertexNormals, std::vector<uint32>& indices);

		/** Get an OGRE manual object's triangles, output lists are not cleared before new entries are added */
		static void getMeshInformation(const Ogre::ManualObject& manualObject, std::vector<Ogre::Vector3>& vertices, std::vector<uint32>& indices, const Ogre::Vector3& position, const Ogre::Quaternion& rotation, const Ogre::Vector3& scale);

		/** Get an OGRE sub-mesh by using a index of the index buffer returned by "qsf::OgreMeshInformationHelper::getMeshInformation()" */
		static Ogre::v1::SubMesh* getSubMeshByIndex(const Ogre::v1::Mesh& mesh, uint32 index);

		/** Get an OGRE manual object section by using a index of the index buffer returned by "qsf::OgreMeshInformationHelper::getMeshInformation()" */
		static Ogre::ManualObject::ManualObjectSection* getManualObjectSectionByIndex(const Ogre::ManualObject& manualObject, uint32 index);


	//[-------------------------------------------------------]
	//[ Private static methods                                ]
	//[-------------------------------------------------------]
	private:
		static void internalGetMeshInformation(const Ogre::v1::Mesh& mesh, Ogre::v1::Entity* entity, bool useSoftwareBlendingVertices, std::vector<Ogre::Vector3>& vertices, std::vector<uint32>& indices, const Ogre::Vector3& position, const Ogre::Quaternion& rotation, const Ogre::Vector3& scale, Ogre::VertexElementSemantic ogreVertexElementSemantic);


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf

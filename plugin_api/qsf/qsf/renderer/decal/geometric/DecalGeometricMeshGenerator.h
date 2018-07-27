// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/renderer/mesh/MeshGenerator.h"
#include "qsf/math/Plane.h"

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class DecalGeometricComponent;
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
	*    Geometric decal mesh generator class
	*
	*  @todo
	*    - TODO(np) In this class we use a mechanism for texture atlas. Someone (maybe mr. ofenberg or
	*      mr. wendel) can extract this to an extra class to use it for other cases. :)
	*/
	class QSF_API_EXPORT DecalGeometricMeshGenerator : public MeshGenerator
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class DecalGeometricComponent;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		DecalGeometricMeshGenerator();

		/**
		*  @brief
		*    Destructor
		*
		*  @note
		*    - Not virtual by intent
		*/
		~DecalGeometricMeshGenerator();

		/**
		*  @brief
		*    Create a procedural OGRE mesh for the given geometric decal component
		*
		*  @param[in] ogreMeshName
		*    UTF-8 name of the OGRE mesh
		*  @param[in] decalGeometricComponent
		*    Geometric decal component to procedurally generate the OGRE mesh for
		*
		*  @return
		*    "true" if the mesh creating successful, else "false"
		*/
		bool createMesh(const std::string& ogreMeshName, DecalGeometricComponent& decalGeometricComponent);


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Cuts an edge by a plane
		*/
		MeshGenerator::RenderVertex cutEdgeByPlane(int planeIndex, const RenderVertex& vertex1, const RenderVertex& vertex2) const;

		/**
		*  @brief
		*    Add a triangle to vertex data
		*
		*  @param[in] vertices
		*    Must be valid and contain at least three vertices
		*/
		void createTriangle(RenderVertex* vertices);

		/**
		*  @brief
		*    Add a triangle to vertexdata cut by planes
		*
		*  @param[in] vertices
		*    Must be valid and contain at least three vertices
		*/
		void createTriangleSub(int planeIndex, RenderVertex* vertices);

		/**
		*  @brief
		*    Calculate decal planes
		*
		*  @param[in] size
		*    Size of decal
		*
		*  @param[in] position
		*    Position of decal
		*
		*  @param[in] rotation
		*    Rotation of decal
		*/
		void calculateDecalPlanes(float size, const glm::vec3& position, const glm::quat& rotation);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		RenderSubMesh*	mActiveSubMesh;	// Little helper to share variable between methods
		Plane			mDecalPlane[6];


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf

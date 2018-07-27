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
	class TraceDecalComponent;
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
	class QSF_API_EXPORT TraceDecalMeshGenerator : public MeshGenerator
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class TraceDecalComponent;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		TraceDecalMeshGenerator();

		/**
		*  @brief
		*    Destructor
		*
		*  @note
		*    - Not virtual by intent
		*/
		~TraceDecalMeshGenerator();

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
		bool createMesh(const std::string& ogreMeshName, TraceDecalComponent& decalGeometricComponent);
	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf

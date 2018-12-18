// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/base/Context.h"
#include "qsf/asset/AssetProxy.h"
#include "qsf/component/street/section/StreetSectionMeshGenerator.h"

#include <boost/container/flat_map.hpp>


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
	*    Static street section mesh manager
	*/
	class QSF_API_EXPORT StreetSectionMeshManager : public Context
	{


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Add a context reference
		*/
		static void addContextReference();

		/**
		*  @brief
		*    Release a context reference
		*/
		static void releaseContextReference();

		/**
		*  @brief
		*    Create a procedural OGRE mesh for the given street section component
		*
		*  @param[in] baseMesh
		*    The base mesh asset to use (e.g. "sample/mesh/default/street_01c")
		*  @param[in] ogreMeshName
		*    UTF-8 name of the OGRE mesh
		*  @param[in] nodes
		*    Nodes
		*  @param[in] derivedNodes
		*    Derived nodes
		*  @param[in] xyAxisScale
		*    X/y axis scale
		*  @param[in] blendOutLengthAtStart
		*    Blend out length at the start of the path
		*  @param[in] blendOutLengthAtEnd
		*    Blend out length at the end of the path
		*
		*  @return
		*    "true" if all went fine, else "false"
		*/
		static bool createMesh(const AssetProxy& baseMesh, const std::string& ogreMeshName, const std::vector<Node>& nodes, const std::vector<glm::vec3>& derivedNodes, const glm::vec2& xyAxisScale, float blendOutLengthAtStart, float blendOutLengthAtEnd);


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		typedef boost::container::flat_map<GlobalAssetId, StreetSectionMeshGenerator*> StreetSectionMeshGeneratorMap;


	//[-------------------------------------------------------]
	//[ Private static data                                   ]
	//[-------------------------------------------------------]
	private:
		static uint32						 mContextCounter;
		static StreetSectionMeshGeneratorMap mStreetSectionMeshGeneratorMap;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf

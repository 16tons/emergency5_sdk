// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/component/nodes/Node.h"
#include "qsf/renderer/mesh/MeshGenerator.h"
#include "qsf/renderer/mesh/AnalysedMesh.h"
#include "qsf/asset/AssetProxy.h"

#include <glm/glm.hpp>


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
	*    Procedural street section mesh generator class
	*/
	class QSF_API_EXPORT StreetSectionMeshGenerator : public MeshGenerator
	{


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Calculate the position of node before and node after
		*
		*  @param[in] nodeBefore
		*    The node before the base
		*  @param[in] nodeBase
		*    The node base node (where the curcation is created)
		*  @param[in] nodeAfter
		*    The node after the base
		*  @param[out] positionBefore
		*    The position the curvation starts
		*  @param[out] positionAfter
		*    The position the curvation ends
		*  @param[out] directionBefore
		*    The direction from base node to start of curvation
		*  @param[out] directionAfter
		*    The direction from base node to end of curvation
		*
		*  @remarks
		*    Calculate the direction from base to node before and base to node after
		*    node before ---<--- direction before ----- node base ----- direction after --->--- node after
		*/
		static void calculateCurvationPositionDirection(const Node& nodeBefore, const Node& nodeBase, const Node& nodeAfter, glm::vec3& positionBefore, glm::vec3& positionAfter, glm::vec3& directionBeforeBase, glm::vec3& directionBaseAfter);

		/**
		*  @brief
		*    Calculate the position of a circle, that hits both positions
		*
		*  @param[in] positionBefore
		*    The position the curvation starts
		*  @param[in] positionAfter
		*    The position the curvation ends
		*  @param[in] directionBefore
		*    The direction from base node to start of curvation
		*  @param[in] directionAfter
		*    The direction from base node to end of curvation
		*  @param[out] circlePosition
		*    Position of circle
		*  @param[out] circleRadius
		*    Radius of circle
		*
		*  @remarks
		*    For this the directions are also needed
		*    Input parameters are output parameters of "calculateCurvationPositionDirection"
		*
		*  @return
		*    Boolean whether circle postion calculation success creation
		*    (e.g. cooplanar nodes will fail)
		*/
		static bool calculateCirclePositionRadius(const glm::vec3& positionBefore, const glm::vec3& positionAfter, const glm::vec3& directionBeforeBase, const glm::vec3& directionBaseAfter, glm::vec3& circlePosition, float& circleRadius);

		/**
		*  @brief
		*    Calculate the derived node positions
		*
		*  @param[in] nodes
		*    Nodes
		*  @param[out] derivedNodes
		*    Will receive the derived local space node positions, list is not cleared before new entries are added
		*  @param[in] segmentsMin
		*    The minimum segments when the street section is straight. This value should have a minimum of 4 to prevent artefacts in curve creation.
		*  @param[in] segmentsMax
		*    The maximum segments when the curve becomes spiky
		*/
		static void calculateDerivedNodePositions(const std::vector<Node>& nodes, std::vector<glm::vec3>& derivedNodes, int segmentsMin, int segmentsMax);


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		StreetSectionMeshGenerator();

		/**
		*  @brief
		*    Destructor
		*
		*  @note
		*    - Not virtual by intent
		*/
		~StreetSectionMeshGenerator();

		/**
		*  @brief
		*    Return the base mesh asset to use
		*
		*  @return
		*    The base mesh asset to use (e.g. "sample/mesh/default/street_01c"), do no destroy the returned instance
		*/
		const AssetProxy& getBaseMesh() const;

		/**
		*  @brief
		*    Set the base mesh asset to use
		*
		*  @param[in] baseMesh
		*    The base mesh asset to use (e.g. "sample/mesh/default/street_01c")
		*/
		void setBaseMesh(const AssetProxy& baseMesh);

		/**
		*  @brief
		*    Create a procedural OGRE mesh for the given street section component
		*
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
		bool createMesh(const std::string& ogreMeshName, const std::vector<Node>& nodes, const std::vector<glm::vec3>& derivedNodes, const glm::vec2& xyAxisScale, float blendOutLengthAtStart, float blendOutLengthAtEnd);


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Analyse the base mesh and fill the shape data
		*/
		void analyseBaseMesh(const Ogre::v1::Mesh& ogreMesh);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		bool						mNeedAnalyseMesh;
		AssetProxy					mBaseMesh;
		AnalysedMesh::BaseMeshData	mBaseMeshData;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf

// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"

#include <qsf/renderer/mesh/MeshGenerator.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace em5
{
	class SimpleBarrierTapeComponent;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Simple warning tape mesh generator class
	*/
	class EM5_API_EXPORT SimpleBarrierTapeMeshGenerator : public qsf::MeshGenerator
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		struct SectionSpline
		{
			glm::vec3 mCoefficients[4];
		};


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		SimpleBarrierTapeMeshGenerator();

		/**
		*  @brief
		*    Destructor
		*
		*  @note
		*    - Not virtual by intent
		*/
		~SimpleBarrierTapeMeshGenerator();

		/**
		*  @brief
		*    Create a procedural OGRE mesh for the given geometric decal component
		*
		*  @param[in] ogreMeshName
		*    UTF-8 name of the OGRE mesh
		*  @param[in] barrierTapeComponent
		*    The warning tape component containing the hose section to create a mesh for
		*  @param[in] sectionIndex
		*    Index of the section whose mesh should be created
		*
		*  @return
		*    "true" if the mesh creating successful, else "false"
		*/
		bool createMesh(const std::string& ogreMeshName, const SimpleBarrierTapeComponent& barriertapeComponent, uint32 sectionIndex);

		/**
		*  @brief
		*    Update a procedural OGRE mesh for the given geometric decal component
		*
		*  @param[in] ogreMeshName
		*    UTF-8 name of the OGRE mesh
		*  @param[in] barrierTapeComponent
		*    The warning tape component containing the hose section to create a mesh for
		*  @param[in] sectionIndex
		*    Index of the section whose mesh should be created
		*
		*  @return
		*    "true" if the mesh creating successful, else "false"
		*/
		bool updateMesh(const std::string& ogreMeshName, const SimpleBarrierTapeComponent& barriertapeComponent, uint32 sectionIndex);

		void buildSectionSpline(const SimpleBarrierTapeComponent& barriertapeComponent, uint32 sectionIndex, SectionSpline& outSpline);
		void evaluateSectionSpline(const SectionSpline& spline, float t, glm::vec3& outPosition, glm::vec3& outDirection);


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void fillMeshGenerator(const std::string& ogreMeshName, const SimpleBarrierTapeComponent& barriertapeComponent, uint32 sectionIndex);


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

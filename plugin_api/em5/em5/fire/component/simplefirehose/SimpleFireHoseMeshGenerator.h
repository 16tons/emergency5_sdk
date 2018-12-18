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
namespace Ogre
{
	namespace v1
	{
		class Entity;
	}
}
namespace em5
{
	class SimpleFireHoseComponent;
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
	*    Simple fire hose mesh generator class
	*/
	class EM5_API_EXPORT SimpleFireHoseMeshGenerator : public qsf::MeshGenerator
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
		SimpleFireHoseMeshGenerator();

		/**
		*  @brief
		*    Destructor
		*
		*  @note
		*    - Not virtual by intent
		*/
		~SimpleFireHoseMeshGenerator();

		/**
		*  @brief
		*    Create a procedural OGRE mesh for the given geometric decal component
		*
		*  @param[in] ogreMeshName
		*    UTF-8 name of the OGRE mesh
		*  @param[in] fireHoseComponent
		*    The fire hose component containing the hose section to create a mesh for
		*  @param[in] sectionIndex
		*    Index of the section whose mesh should be created
		*  @param[in] ghost
		*    Ghost visualization?
		*
		*  @return
		*    "true" if the mesh creating successful, else "false"
		*/
		bool createMesh(const std::string& ogreMeshName, const SimpleFireHoseComponent& fireHoseComponent, uint32 sectionIndex, bool ghost);

		/**
		*  @brief
		*    Update a procedural OGRE mesh for the given geometric decal component
		*
		*  @param[in] ogreEntity
		*    OGRE entity from which to update the OGRE mesh from
		*  @param[in] fireHoseComponent
		*    The fire hose component containing the hose section to create a mesh for
		*  @param[in] sectionIndex
		*    Index of the section whose mesh should be created
		*  @param[in] ghost
		*    Ghost visualization?
		*
		*  @return
		*    "true" if the mesh creating successful, else "false"
		*/
		bool updateMesh(Ogre::v1::Entity& ogreEntity, const SimpleFireHoseComponent& fireHoseComponent, uint32 sectionIndex, bool ghost);

		void buildSectionSpline(const SimpleFireHoseComponent& fireHoseComponent, uint32 sectionIndex, SectionSpline& outSpline);
		void evaluateSectionSpline(const SectionSpline& spline, float t, glm::vec3& outPosition, glm::vec3& outDirection);


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void fillMeshGenerator(const SimpleFireHoseComponent& fireHoseComponent, uint32 sectionIndex, bool ghost);


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

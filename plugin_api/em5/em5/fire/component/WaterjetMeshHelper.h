// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <glm/glm.hpp>

#include <boost/noncopyable.hpp>

#include <vector>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Entity;
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
	*    Waterjet mesh helper class
	*/
	class WaterjetMeshHelper : public boost::noncopyable
	{


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	public:
		static std::string getMeshName(const qsf::Entity& entity);
		static void computeSmoothNormals(const std::vector<glm::vec3>& vertices, const std::vector<uint32>& indices, std::vector<glm::vec3>& normals);
		static void generateMesh(float startRadius, float endRadius, float endRadiusYFactor, const std::vector<float>& segments, float currentDistance, float targetDistance, qsf::Entity& entity, float alpha = 1.0f);


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

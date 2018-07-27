// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/map/query/MapQuery.h"

#include <glm/glm.hpp>

#include <vector>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Ogre
{
	class SphereSceneQuery;
}
namespace qsf
{
	class Component;
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
	*    Sphere map query interface
	*/
	class QSF_API_EXPORT SphereMapQuery : public MapQuery
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] map
		*    Map instance to perform the map query on, the instance must stay valid as long as this map query instance exists
		*/
		inline explicit SphereMapQuery(const Map& map);

		/**
		*  @brief
		*    Destructor
		*
		*  @note
		*    - Not virtual by intent
		*/
		~SphereMapQuery();

		/**
		*  @brief
		*    Get a list of components that overlap with the given sphere
		*
		*  @param[in] spherePosition
		*    World space sphere position
		*  @param[in] sphereRadius
		*    World space sphere radius
		*  @param[out] outComponents
		*    The list of component pointers that will be filled by the method; is not automatically cleared
		*
		*  @return
		*    The number of hit components
		*/
		uint32 getHitComponentsBySphere(const glm::vec3& spherePosition, float sphereRadius, std::vector<Component*>& outComponents);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		Ogre::SphereSceneQuery* mOgreSphereSceneQuery;	///< OGRE sphere scene query, can be a null pointer, we have to destroy it in case we no longer need it


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/map/query/SphereMapQuery-inl.h"

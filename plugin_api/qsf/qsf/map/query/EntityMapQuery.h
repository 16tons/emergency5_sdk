// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/map/query/MapQuery.h"

#include <glm/fwd.hpp>

#include <vector>
#include <string>


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
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Entity map query interface
	*
	*  @remarks
	*    Usage example:
	*    @code
	*    qsf::Entity* qsfEntity = qsf::EntityMapQuery(QSF_MAINMAP).getFirstInstanceByName("Bob");
	*    @endcode
	*
	*  @note
	*    - TODO(co) Add support for regular expressions (maybe also wildcards) to be able to perform more complex name matches instead of just simple comparisons
	*/
	class QSF_API_EXPORT EntityMapQuery : public MapQuery
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
		inline explicit EntityMapQuery(const Map& map);

		/**
		*  @brief
		*    Destructor
		*
		*  @note
		*    - Not virtual by intent
		*/
		inline ~EntityMapQuery();

		//[-------------------------------------------------------]
		//[ First instance                                        ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Return the first entity instance by using the name property of the metadata component found in any entity
		*
		*  @param[in] name
		*    The name of the entity, name is not unique
		*
		*  @return
		*    The requested entity, null pointer if no instance exists in map, do not destroy the returned instance, do not keep a reference to it outside the C runtime stack (use the unique entity ID instead)
		*/
		Entity* getFirstInstanceByName(const std::string& name) const;

		//[-------------------------------------------------------]
		//[ All instances                                         ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Return all entity instances by using the name property of the metadata component found in any entity
		*
		*  @param[in]  name
		*    The name of the entity, name is not unique
		*  @param[out] foundEntities
		*    Receives the found entity instances, list is not cleared before adding new entries, do not destroy the returned instances, do not keep a reference to it outside the C runtime stack (use the unique entity ID instead)
		*
		*  @return
		*    The number of found entities added to the given list
		*/
		uint32 getAllInstancesByName(const std::string& name, std::vector<Entity*>& foundEntities) const;

		/**
		*  @brief
		*    Return all entity instances found inside the given circle on the xz-plane
		*
		*  @param[in] center
		*    World space center of the circle
		*  @param[in] radius
		*    World space radius of the circle
		*  @param[out] outEntities
		*    Receives the found entity instances, do not destroy the instances, list is not cleared before new entries are added
		*
		*  @return
		*    The list of entity instances, do not destroy the returned instances, do not keep a reference to it outside the C runtime stack (use the unique component ID instead)
		*/
		bool getInstancesInCircle(const glm::vec3& center, float radius, std::vector<Entity*>& outEntities) const;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/map/query/EntityMapQuery-inl.h"

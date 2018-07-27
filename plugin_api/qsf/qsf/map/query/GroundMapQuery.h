// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/map/query/RayMapQuery.h"	// Needed for RayMapQuery::QueryAccuracy definition
#include "qsf/selection/EntitySelectionManager.h"

#include <glm/fwd.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Entity;
	class Component;
	class GroundMap;
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
	*    Ground map query interface
	*
	*  @remarks
	*    Usage example:
	*    @code
	*    float height = 0.0f;
	*    bool result = qsf::GroundMapQuery(QSF_MAINMAP).getHeightAtXZPosition(42.0f, 21.0f, height);
	*    @endcode
	*/
	class QSF_API_EXPORT GroundMapQuery : public MapQuery
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		typedef bool (*FilterCallback)(Component&, RayMapQuery::QueryAccuracy&);	///< Filter callback; receives the component to evaluate as parameter, returns "true" if the component passes the filter and "false" if the component gets filtered out


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
		*  @param[in] filterMask
		*    Bitmask to use for ground map filtering; only those ground maps are sampled that share at least one bit with the given mask
		*  @param[in] filterCallback
		*    Filter callback to use, can be a null pointer in which a default filter callback will be used
		*/
		explicit GroundMapQuery(const Map& map, uint32 filterMask = 0xffffffff, FilterCallback filterCallback = nullptr);

		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] map
		*    Map instance to perform the map query on, the instance must stay valid as long as this map query instance exists
		*  @param[in] filterCallback
		*    Filter callback to use, can be a null pointer in which a default filter callback will be used
		*  @param[in] groundMapId
		*    ID of the ground map to use as an optimization, "getUninitialized<uint32>()" to use no ground map and fallback to an expensive raycast, 0 is usually a good ground map choice
		*  @param[in] baseLevelOnly
		*    If "true", only the base heightmap level of the ground map will be sampled, otherwise other heights (bridges etc.) are considered as well
		*/
		explicit GroundMapQuery(const Map& map, FilterCallback filterCallback, uint32 groundMapId = 0, bool baseLevelOnly = false);

		/**
		*  @brief
		*    Destructor
		*
		*  @note
		*    - Not virtual by intent
		*/
		inline ~GroundMapQuery();

		/**
		*  @brief
		*    Evaluate the height at the given world space position
		*
		*  @param[in] position
		*    World space position to evaluate the height at
		*  @param[out] height
		*    On success, receives the evaluated world space height (y-axis) at the given world space position, on failure the content is not touched
		*  @param[in] allowFallback
		*    In case no ground map height value could be evaluated, allow an expensive fallback?
		*
		*  @return
		*    "true" if a height at the given world space position was found, else "false"
		*/
		bool getHeightAt(const glm::vec3& position, float& height, bool allowFallback = true);

		/**
		*  @brief
		*    Evaluate the height at the given world space position
		*
		*  @param[in] xPosition
		*    X world space position to evaluate the height at
		*  @param[in] zPosition
		*    Z world space position to evaluate the height at
		*  @param[out] height
		*    On success, receives the evaluated world space height (y-axis) at the given world space position, on failure the content is not touched
		*  @param[in] ignoreEntityIds
		*    Optional, if not a null pointer, the given entities will be ignored for the height calculation
		*
		*  @return
		*    "true" if a height at the given world space position was found, else "false"
		*/
		inline bool getHeightAtXZPosition(float xPosition, float zPosition, float& height, const EntitySelectionManager::IdSet* ignoreEntityIds = nullptr);

		/**
		*  @brief
		*    Evaluate the height and normal at the given world space position
		*
		*  @param[in] xPosition
		*    X world space position to evaluate the height at
		*  @param[in] zPosition
		*    Z world space position to evaluate the height at
		*  @param[out] height
		*    On success, receives the evaluated world space height (y-axis) at the given world space position, on failure the content is not touched
		*  @param[out] normal
		*    On success, receives the evaluated world space normal at the given world space position, on failure the content is not touched
		*  @param[in] ignoreEntityIds
		*    Optional, if not a null pointer, the given entities will be ignored for the height calculation
		*
		*  @return
		*    "true" if a height at the given world space position was found, else "false"
		*/
		inline bool getHeightAndNormalAtXZPosition(float xPosition, float zPosition, float& height, glm::vec3& normal, const EntitySelectionManager::IdSet* ignoreEntityIds = nullptr);

		/**
		*  @brief
		*    Evaluate the terrain height at the given world space position
		*
		*  @param[in] xPosition
		*    X world space position to evaluate the height at
		*  @param[in] zPosition
		*    Z world space position to evaluate the height at
		*  @param[out] height
		*    On success, receives the evaluated world space height (y-axis) at the given world space position, on failure the content is not touched
		*
		*  @return
		*    "true" if a height at the given world space position was found, else "false"
		*/
		inline bool getTerrainHeightAtXZPosition(float xPosition, float zPosition, float& height);

		/**
		*  @brief
		*    Evaluate the terrain height at the given world space position
		*
		*  @param[in] xPosition
		*    X world space position to evaluate the height at
		*  @param[in] zPosition
		*    Z world space position to evaluate the height at
		*  @param[out] height
		*    On success, receives the evaluated world space height (y-axis) at the given world space position, on failure the content is not touched
		*  @param[out] normal
		*    On success, receives the evaluated world space normal at the given world space position, on failure the content is not touched
		*
		*  @return
		*    "true" if a height at the given world space position was found, else "false"
		*/
		inline bool getTerrainHeightAndNormalAtXZPosition(float xPosition, float zPosition, float& height, glm::vec3& normal);

		/**
		*  @brief
		*    Convenience method to drop a given position to the ground below
		*
		*  @param[in, out] position
		*    World space position to drop to the ground
		*
		*  @return
		*    "true" if a height at the given world space position was found, else "false"
		*/
		inline bool dropToGround(glm::vec3& position);


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		bool getGroundDataAtXZPosition(float xPosition, float zPosition, float* height, glm::vec3* normal, const EntitySelectionManager::IdSet* ignoreEntityIds);
		bool getTerrainDataAtXZPosition(float xPosition, float zPosition, float* height, glm::vec3* normal);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		uint32		   mFilterMask;		///< Ground map filter bitmask
		FilterCallback mFilterCallback;	///< Filter callback to use, can be a null pointer


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/map/query/GroundMapQuery-inl.h"

// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"

#include <qsf/base/WeakPtr.h>
#include <qsf/map/query/RayMapQuery.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Ogre
{
	struct RaySceneQueryResultEntry;
}
namespace qsf
{
	class Entity;
	class Prototype;
	class Transform;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	struct RayMapQueryResponse
	{
		// Definitions
		enum Flag
		{
			ASYNCHRONOUS_RESPONSE = 1<<1,	///< Asynchronous response; flag will only be used if there's a response callback provided - TODO(co) Reserved for future use, not implemented yet
			POSITION_RESPONSE	  = 1<<2,	///< World space position query response data required
			FOR_SELECTION		  = 1<<3,	///< Indicates if only selectable entities should be returned
			EXCLUDE_TERRAIN		  = 1<<4,	///< Exclude the terrain for the check
			EXCLUDE_OGRE_ENTITIES = 1<<5	///< Exclude the OGRE entities for the check
		};
		typedef qsf::BitFlagSet<uint8, Flag> Flags;
		typedef boost::function<void(const RayMapQueryResponse&)> Callback;	///< Query response callback; called in case of asynchronous processing from the main thread

		// Data
		Flags					  flags;	///< Hint which query response data is required (used for skipping expensive work, inexpensive not required data might still be written)
		bool					  success;	///< Set if query was successful, i.e. the ray hit anything at all
		qsf::WeakPtr<qsf::Entity> entity;	///< Receives on success the entity the rest of the response data comes from, on failure the content is not touched
		glm::vec3				  position;	///< Receives on success the found world space position, on failure the content is not touched
		Callback				  callback;	///< Query response callback; called in case of asynchronous processing from the main thread

		RayMapQueryResponse() :
			success(false)
		{ }

		explicit RayMapQueryResponse(uint8 _flags) :
			flags(_flags),
			success(false)
		{ }

	};

	/**
	*  @brief
	*    Ray map query interface
	*/
	class EM5_API_EXPORT RayMapQuery : public qsf::RayMapQuery
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
		explicit RayMapQuery(const qsf::Map& map);

		/**
		*  @brief
		*    Destructor
		*
		*  @note
		*    - Not virtual by intent
		*/
		virtual ~RayMapQuery();

		/**
		*  @brief
		*    Returns the entity under the mouse position if any
		*
		*  @param[in]  normalizedMousePosition
		*    Normalized mouse position inside the render window to use for the ray generation
		*  @param[out] rayMapQueryResponse
		*    Receives the query response; in case of asynchronous processing use the query response callback instead to use the response
		*  @param[in] ignoreEntityIds
		*    Optional list of entity IDs to ignore, can be a null pointer
		*/
		void getEntityUnderNormalizedMousePosition(const glm::vec2& normalizedMousePosition, RayMapQueryResponse& rayMapQueryResponse, const IgnoreEntityIds* ignoreEntityIds = nullptr);

		/**
		*  @brief
		*    Returns the entity under the world position if any
		*
		*  @param[in]  mapPosition
		*    World position on the map to use for the ray generation
		*  @param[out] rayMapQueryResponse
		*    Receives the query response; in case of asynchronous processing use the query response callback instead to use the response
		*  @param[in] ignoreEntityIds
		*    Optional list of entity IDs to ignore, can be a null pointer
		*/
		void getEntityOnMapPosition(const glm::vec3& mapPosition, RayMapQueryResponse& rayMapQueryResponse, const IgnoreEntityIds* ignoreEntityIds = nullptr);

		void getEntityUnderMousePosition(qsf::Ray& ray, RayMapQueryResponse& rayMapQueryResponse, const IgnoreEntityIds* ignoreEntityIds = nullptr);


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void getFirstHitEntityAlongRay(const qsf::Ray& ray, RayMapQueryResponse& rayMapQueryResponse, bool forSelection, const qsf::RayMapQuery::QueryParameters& parameters = qsf::RayMapQuery::QueryParameters());
		bool isOgreEntityHitByRay(const qsf::Ray& ray, Ogre::Ray& ogreRay, qsf::Prototype& prototype, const qsf::Transform& transform, Ogre::MovableObject& ogreMovableObject, const Ogre::RaySceneQueryResultEntry& iterator, QueryAccuracy queryAccuracy, float& closestDistance, glm::vec3* outPosition = nullptr);


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

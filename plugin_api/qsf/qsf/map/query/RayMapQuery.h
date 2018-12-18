// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/math/BitFlagSet.h"
#include "qsf/map/query/MapQuery.h"
#include "qsf/asset/AssetSystemTypes.h"
#include "qsf/selection/EntitySelectionManager.h"

#include <glm/fwd.hpp>

#include <boost/optional.hpp>
#include <boost/function.hpp>
#include <boost/container/flat_set.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Ogre
{
	class RaySceneQuery;
	class MovableObject;
	class Ray;
}
namespace qsf
{
	class Component;
	class RenderWindow;
	class CameraComponent;
	class Ray;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	struct RayMapQueryResponse
	{
		// Definitions
		enum Flag
		{
			ASYNCHRONOUS_RESPONSE			  = 1<<1,	///< Asynchronous response; flag will only be used if there's a response callback provided - TODO(co) Reserved for future use, not implemented yet
			POSITION_RESPONSE				  = 1<<2,	///< World space position query response data required
			NORMAL_RESPONSE					  = 1<<3,	///< World space normal query response data required
			GLOBAL_MATERIAL_ASSET_ID_RESPONSE = 1<<4,	///< Global material asset ID query response data required
			TEXTURE_COORDINATE_RESPONSE		  = 1<<5	///< Texture coordinate query response data required
		};
		typedef BitFlagSet<uint8, Flag> Flags;
		typedef boost::function<void(const RayMapQueryResponse&)> Callback;	///< Query response callback; called in case of asynchronous processing from the main thread

		// Data
		Flags		  flags;					///< Hint which query response data is required (used for skipping expensive work, inexpensive not required data might still be written)
		Component*	  component;				///< Receives on success the component the rest of the response data comes from, on failure the content is not touched
		glm::vec3	  position;					///< Receives on success the found world space position, on failure the content is not touched
		glm::vec3	  normal;					///< Receives on success the found world space normal (or a simple world space up vector if accuracy is set to bounding box), on failure the content is not touched
		GlobalAssetId globalMaterialAssetId;	///< Receives on success the found global material asset ID, on failure the content is not touched
		glm::vec2	  textureCoordinate;		///< Receives on success the found texture coordinate, on failure the content is not touched
		Callback	  callback;					///< Query response callback; called in case of asynchronous processing from the main thread

		RayMapQueryResponse() :
			component(nullptr),
			globalMaterialAssetId(getUninitialized<GlobalAssetId>())
		{ }

		explicit RayMapQueryResponse(uint8 _flags) :
			flags(_flags),
			component(nullptr),
			globalMaterialAssetId(getUninitialized<GlobalAssetId>())
		{ }

	};

	/**
	*  @brief
	*    Ray map query interface
	*
	*  @remarks
	*    Usage example:
	*    @code
	*    // qsf::CameraComponent* qsfCameraComponent = ...
	*    // float xPosition = ...
	*    // float yPosition = ...
	*    qsf::Component* qsfComponent = qsf::RayMapQuery(QSF_MAINMAP).getFirstHitByCamera(*qsfCameraComponent, xPosition, yPosition);
	*    @endcode
	*/
	class QSF_API_EXPORT RayMapQuery : public MapQuery
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		enum QueryAccuracy
		{
			ACCURACY_BBOX,	///< Use bounding box accuracy for ray intersection test (faster)
			ACCURACY_MESH	///< Use polygon-based ray intersection test (slower, but more precise)
		};

		typedef boost::function<bool(Component&, QueryAccuracy&)> FilterCallback;	///< Filter callback; receives the component to evaluate as parameter and the to be used QueryAccuracy, the query accuracy can be changed (which changes the accuracy only for that entity), returns "true" if the component passes the filter and "false" if the component gets filtered out.

		enum QueryIncludeFlags
		{
			INCLUDE_NONE = 0,			///< Include none (currently only terrain and OGRE scene nodes) into the check of "what intersects with the ray"
			INCLUDE_TERRAIN = 1,		///< Include the terrain into the check of "what intersects with the ray"
			INCLUDE_OGRE_ENTITIES = 2,	///< Include the OGRE entities into the check of "what intersects with the ray"
			INCLUDE_ALL = 3				///< Include all (currently only terrain and Ogre scene nodes) into the check of "what intersects with the ray"
		};

		typedef boost::container::flat_set<uint64> IgnoreEntityIds;

		struct QueryParameters
		{
			IgnoreEntityIds	mIgnoreEntityIds;
			QueryAccuracy	mAccuracy;
			FilterCallback	mFilterCallback;	///< Called from inside the main thread as well in case of asynchronous processing
			int				mQueryIncludeFlags;
			float			mInitialClosestDistance;

			QueryParameters() :
				mAccuracy(ACCURACY_MESH),
				mQueryIncludeFlags(INCLUDE_ALL),
				mInitialClosestDistance(-1.0f)
			{}
		};


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	public:
		static boost::optional<Ray> getRayByRenderWindowPosition(const RenderWindow& renderWindow, int xPosition, int yPosition);
		static boost::optional<Ray> getRayByRenderWindowNormalizedPosition(const RenderWindow& renderWindow, float xNormalizedPosition, float yNormalizedPosition);


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
		inline explicit RayMapQuery(const Map& map);

		/**
		*  @brief
		*    Destructor
		*
		*  @note
		*    - Not virtual by intent
		*/
		~RayMapQuery();

		/**
		*  @brief
		*    Find the first component hit by a ray generated by using a given render window and a mouse position
		*
		*  @param[in]  renderWindow
		*    Render window to use for the ray generation
		*  @param[in]  xPosition
		*    X mouse position inside the render window to use for the ray generation
		*  @param[in]  yPosition
		*    Y mouse position inside the render window to use for the ray generation
		*  @param[out] rayMapQueryResponse
		*    Receives the query response; in case of asynchronous processing use the query response callback instead to use the response
		*  @param[in] ignoreEntityIds
		*    Optional, if not a null pointer, the given entities will be ignored
		*/
		void getFirstHitByRenderWindow(const RenderWindow& renderWindow, int xPosition, int yPosition, RayMapQueryResponse& rayMapQueryResponse, const EntitySelectionManager::IdSet* ignoreEntityIds = nullptr);
		void getFirstHitByRenderWindowNormalizedPosition(const RenderWindow& renderWindow, float normalizedXPosition, float normalizedYPosition, RayMapQueryResponse& rayMapQueryResponse, const EntitySelectionManager::IdSet* ignoreEntityIds = nullptr);

		/**
		*  @brief
		*    Find the first component hit by a ray generated by using a given camera and a normalized mouse position
		*
		*  @param[in]  cameraComponent
		*    Camera component to use for the ray generation
		*  @param[in]  xPosition
		*    Normalized x mouse position to use for the ray generation
		*  @param[in]  yPosition
		*    Normalized y mouse position to use for the ray generation
		*  @param[out] rayMapQueryResponse
		*    Receives the query response; in case of asynchronous processing use the query response callback instead to use the response
		*  @param[in] ignoreEntityIds
		*    Optional, if not a null pointer, the given entities will be ignored
		*/
		void getFirstHitByCamera(const CameraComponent& cameraComponent, float xPosition, float yPosition, RayMapQueryResponse& rayMapQueryResponse, const EntitySelectionManager::IdSet* ignoreEntityIds);

		/**
		*  @brief
		*    Find the first component hit by the given ray
		*
		*  @param[in] ray
		*    The world space ray to find an intersecting component with
		*  @param[out] rayMapQueryResponse
		*    Receives the query response; in case of asynchronous processing use the query response callback instead to use the response
		*  @param[in] queryParameters
		*    Query parameters for ray intersection test
		*/
		void getFirstHitAlongRay(const Ray& ray, RayMapQueryResponse& rayMapQueryResponse, const QueryParameters& queryParameters = QueryParameters());


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Get or created the OGRE ray scene query instance
		*
		*  @param[in] ogreRay
		*    The OGRE ray to be used by the ray scene query instance
		*  @param[in] queryMask
		*    Query mask to use
		*
		*  @return
		*    The OGRE ray scene query instance, null pointer on error
		*/
		Ogre::RaySceneQuery* getOrCreateOgreRaySceneQuery(Ogre::Ray& ogreRay, uint32 queryMask = QUERY_ENTITY_DEFAULT_MASK);

		/**
		*  @brief
		*    Checks if the ray has hit the terrain
		*
		*  @param[in] ray
		*    The world space ray to find the distance
		*  @param[in] ogreRay
		*    The ogre ray to find the position where the ray hits the terrain
		*  @param[in] queryParameters
		*    Query parameters for ray intersection test
		*  @param[out] closestDistance
		*    The distance to the ray when the terrain is hit
		*  @param[out] closestComponent
		*    The component instance of the terrain when the terrain is hit
		*
		*  @return
		*    "True" when the terrain was hit otherwise false
		*/
		bool isTerrainHitByRay(const Ray& ray, Ogre::Ray& ogreRay, const QueryParameters& queryParameters, float& closestDistance, Component*& closestComponent);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		Ogre::RaySceneQuery* mOgreRaySceneQuery;	///< OGRE ray scene query, can be a null pointer, we have to destroy it in case we no longer need it


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/map/query/RayMapQuery-inl.h"

// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/map/query/MapQuery.h"

#include <vector>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Ogre
{
	class PlaneBoundedVolumeListSceneQuery;
}
namespace qsf
{
	class Component;
	class PlaneBoundedVolume;
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
	*    Volume map query interface
	*/
	class QSF_API_EXPORT VolumeMapQuery : public MapQuery
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

		struct QueryParameters
		{
			QueryAccuracy mAccuracy;

			QueryParameters() : mAccuracy(ACCURACY_MESH) {}
		};


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
		inline explicit VolumeMapQuery(const Map& map);

		/**
		*  @brief
		*    Destructor
		*
		*  @note
		*    - Not virtual by intent
		*/
		~VolumeMapQuery();

		/**
		*  @brief
		*    Get a list of components that overlap with the given volume
		*
		*  @param[in] planeBoundedVolume
		*    The volume to find an overlapping components with
		*  @param[out] outComponents
		*    The list of component pointers that will be filled by the method; is not automatically cleared
		*  @param[in] parameters
		*    Optional, query parameters for volume intersection test
		*
		*  @return
		*    The number of hit components
		*/
		uint32 getHitComponentsByVolume(const PlaneBoundedVolume& planeBoundedVolume, std::vector<Component*>& outComponents, const QueryParameters& parameters = QueryParameters());


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		Ogre::PlaneBoundedVolumeListSceneQuery* mOgreVolumeSceneQuery;	///< OGRE volume scene query, can be a null pointer, we have to destroy it in case we no longer need it


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/map/query/VolumeMapQuery-inl.h"

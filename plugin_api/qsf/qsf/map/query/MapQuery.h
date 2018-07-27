// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/Export.h"

#include <boost/noncopyable.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Map;
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
	*    Abstract map query class
	*/
	class QSF_API_EXPORT MapQuery : public boost::noncopyable
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 QUERY_ENTITY_DEFAULT_MASK;			///< Default query mask for entities
		static const uint32 QUERY_FX_DEFAULT_MASK;				///< Default query mask for effects like billboard sets / particle systems
		static const uint32 QUERY_STATIC_GEOMETRY_DEFAULT_MASK;	///< Default query mask for static geometry
		static const uint32 QUERY_LIGHT_DEFAULT_MASK;			///< Default query mask for lights
		static const uint32 QUERY_FRUSTUM_DEFAULT_MASK;			///< Default query mask for frusta and cameras
		static const uint32 QUERY_DEBUG_MASK;					///< Flag for debug mode hint


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Destructor
		*
		*  @note
		*    - Not virtual by intent
		*/
		inline ~MapQuery();

		/**
		*  @brief
		*    Return the map instance to perform the map query on
		*
		*  @return
		*    The map instance to perform the map query on, do no destroy the returned instance
		*/
		inline const Map& getMap() const;


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] map
		*    Map instance to perform the map query on, the instance must stay valid as long as this map query instance exists
		*/
		inline explicit MapQuery(const Map& map);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		const Map* mMap;	///< Map instance to perform the map query on, always valid, do not destroy the instance


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/map/query/MapQuery-inl.h"

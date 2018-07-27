// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/platform/PlatformTypes.h"
#include "qsf/base/GetUninitialized.h"

#include <boost/container/flat_map.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Link;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Definitions (qsf namespace)                           ]
	//[-------------------------------------------------------]
	struct LinkAnchorId
	{
		uint32 mContextId;		///< Unique ID of a context, e.g. the map ID for an entity's link anchor
		uint64 mLocalId;		///< Unique ID inside the context, e.g. the entity ID for an entity's link anchor

		inline LinkAnchorId(uint32 contextId, uint64 localId);
		inline bool operator<(const LinkAnchorId& other) const;
	};

	// Declare specialization of getUninitialized() for LinkAnchorId
	template<> LinkAnchorId getUninitialized<LinkAnchorId>();

	/** Link connection map from a link anchor ID to the link */
	typedef boost::container::flat_map<LinkAnchorId, Link*> LinkConnectionMap;


	//[-------------------------------------------------------]
	//[ Namespace                                             ]
	//[-------------------------------------------------------]
	namespace link
	{


		//[-------------------------------------------------------]
		//[ Definitions (qsf::link namespace)                     ]
		//[-------------------------------------------------------]
		/** Link directions */
		enum Direction
		{
			FORWARD = 0,
			BACKWARD = 1
		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // link
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/link/LinkDefinitions-inl.h"

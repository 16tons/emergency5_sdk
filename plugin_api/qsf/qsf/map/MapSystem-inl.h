// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/base/error/BoostAssertHandler.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline bool MapSystem::hasMainMap() const
	{
		return (nullptr != mMainMap);
	}

	inline Map& MapSystem::getMainMap() const
	{
		// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
		QSF_ASSERT(nullptr != mMainMap, "The main map instance is invalid", QSF_REACT_THROW);
		return *mMainMap;
	}

	inline Map* MapSystem::getMapById(uint32 id) const
	{
		const MapMap::const_iterator iterator = mMapMap.find(id);
		return (iterator != mMapMap.cend()) ? iterator->second : nullptr;
	}

	inline const MapSystem::MapList& MapSystem::getMaps() const
	{
		return mMapList;
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::System methods                    ]
	//[-------------------------------------------------------]
	inline const char* MapSystem::getName() const
	{
		return "Map system";
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf

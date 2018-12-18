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
	inline Map& LayerManager::getMap() const
	{
		// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
		QSF_ASSERT(nullptr != mMap, "The map instance is invalid", QSF_REACT_THROW);
		return *mMap;
	}

	inline Layer& LayerManager::getRootLayer() const
	{
		// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
		QSF_ASSERT(nullptr != mRootLayer, "The root layer instance is invalid", QSF_REACT_THROW);
		return *mRootLayer;
	}

	inline Layer& LayerManager::getBaseLayer() const
	{
		// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
		QSF_ASSERT(nullptr != mBaseLayer, "The base layer instance is invalid", QSF_REACT_THROW);
		return *mBaseLayer;
	}

	inline Layer* LayerManager::getLayerById(uint32 id) const
	{
		const LayerMap::const_iterator iterator = mLayerMap.find(id);
		return (iterator != mLayerMap.cend()) ? iterator->second : nullptr;
	}

	inline const LayerManager::LayerMap& LayerManager::getLayerMap() const
	{
		return mLayerMap;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf

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
	inline RightsManager::RightsManager()
	{
		// Register all known and supported CAMP pluginable classes
		registerPluginables();
	}

	inline RightsManager::~RightsManager()
	{
		// Unregister all known and supported CAMP pluginable classes
		unregisterPluginables();

		// Sanity checks
		QSF_ASSERT(mRightList.empty(), "The QSF right list of the rights manager should be empty at this point in time, but it isn't", QSF_REACT_NONE);
		QSF_ASSERT(mRightMap.empty(), "The QSF right map of the rights manager should be empty at this point in time, but it isn't", QSF_REACT_NONE);
	}

	inline const Right* RightsManager::getRightById(const StringHash& id) const
	{
		const RightMap::const_iterator iterator = mRightMap.find(id);
		return (iterator != mRightMap.cend()) ? iterator->second : nullptr;
	}

	inline const RightsManager::RightList& RightsManager::getRights() const
	{
		return mRightList;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf

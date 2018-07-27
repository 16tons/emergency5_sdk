// Copyright (C) 2012-2017 Promotion Software GmbH


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
	inline PrototypeManager& PrototypeSystem::getMainManager() const
	{
		// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
		QSF_ASSERT(nullptr != mMainManager, "The main prototype manager instance is invalid", QSF_REACT_THROW);
		return *mMainManager;
	}

	inline PrototypeManager* PrototypeSystem::getManagerById(uint32 id) const
	{
		const ManagerMap::const_iterator iterator = mManagerMap.find(id);
		return (iterator != mManagerMap.cend()) ? iterator->second : nullptr;
	}

	inline const PrototypeSystem::ManagerList& PrototypeSystem::getManagers() const
	{
		return mManagerList;
	}

	inline const PrototypeFinalizerManager& PrototypeSystem::getPrototypeFinalizerManager() const
	{
		// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
		QSF_ASSERT(nullptr != mPrototypeFinalizerManager, "The prototype finalizer manager instance is invalid", QSF_REACT_THROW);
		return *mPrototypeFinalizerManager;
	}

	inline PrefabCache& PrototypeSystem::getPrefabCache() const
	{
		// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
		QSF_ASSERT(nullptr != mPrefabCache, "The prefab cache instance is invalid", QSF_REACT_THROW);
		return *mPrefabCache;
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::System methods                    ]
	//[-------------------------------------------------------]
	inline const char* PrototypeSystem::getName() const
	{
		return "Prototype system";
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf

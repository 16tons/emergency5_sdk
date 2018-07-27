// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline uint32 BasePrototypeManager::getId() const
	{
		return mId;
	}

	inline ComponentManager& BasePrototypeManager::getComponentManager() const
	{
		// The component manager is always valid
		return mComponentManager;
	}

	inline Prototype* BasePrototypeManager::getPrototypeById(uint64 id) const
	{
		// Find the prototype by its unique ID
		return mPrototypes.find(id);
	}

	inline const PrototypeHashMap& BasePrototypeManager::getPrototypes() const
	{
		return mPrototypes;
	}


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	inline void BasePrototypeManager::reservePrototypes(size_t numberOfPrototypes)
	{
		mPrototypes.reserve(numberOfPrototypes);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf

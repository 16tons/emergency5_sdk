// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline SystemContainer::SystemContainer()
	{
		// Nothing here
	}

	inline const std::vector<System*>& SystemContainer::getSystems() const
	{
		return mSystems;
	}


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	template<typename T> T* SystemContainer::createAndInsertSystem()
	{
		T* systemInstance = new T();
		mSystems.push_back(systemInstance);
		return systemInstance;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf

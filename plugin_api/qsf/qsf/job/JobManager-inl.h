// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline uint32 JobManager::getId() const
	{
		return mName.getHash();
	}

	inline const std::string& JobManager::getName() const
	{
		return mName.getName();
	}

	inline bool JobManager::isAutoupdate() const
	{
		return mJobProxy.isValid();
	}

	inline uint32 JobManager::getProviderJobManagerId() const
	{
		return mProviderJobManagerId;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf

// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline FreeplayEventManager& FreeplaySystem::getEventManager() const
	{
		return mEventManager;
	}

	inline FreeplayEventFactoryManager& FreeplaySystem::getFactoryManager() const
	{
		return mFactoryManager;
	}

	inline FreeplayEventGenerator& FreeplaySystem::getEventGenerator() const
	{
		return mEventGenerator;
	}

	inline FreeplayEventPoolManager& FreeplaySystem::getEventPoolManager() const
	{
		return mEventPoolManager;
	}

	inline ObjectiveManager& FreeplaySystem::getObjectiveManager() const
	{
		return mObjectiveManager;
	}

	inline CleanupManager& FreeplaySystem::getCleanupManager() const
	{
		return mCleanupManager;
	}

	inline DensityCalculator& FreeplaySystem::getDensityCalculator() const
	{
		return mDensityCalculator;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

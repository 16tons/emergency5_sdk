// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/base/error/ErrorHandling.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	inline Plugin* Plugin::instance()
	{
		return mGlobalPluginInstance;
	}

	inline Plugin& Plugin::getInstanceSafe()
	{
		QSF_CHECK(mGlobalPluginInstance, "Accessing uninitialized EM5 plugin instance",
			QSF_REACT_THROW);

		return *mGlobalPluginInstance;
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Plugin methods                    ]
	//[-------------------------------------------------------]
	inline const char* Plugin::getName() const
	{
		return "EMERGENCY 5 (EM5)";
	}

	inline const char* Plugin::getVendor() const
	{
		return "Promotion Software GmbH";
	}

	inline const char* Plugin::getDescription() const
	{
		return "EMERGENCY 5 (EM5)";
	}

	inline ModSystem& Plugin::getModSystem() const
	{
		// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
		QSF_ASSERT(nullptr != mModSystem, "The mod system instance is invalid", QSF_REACT_THROW);
		return *mModSystem;
	}

	inline FreeplaySystem& Plugin::getFreeplaySystem() const
	{
		// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
		QSF_ASSERT(nullptr != mFreeplaySystem, "The freeplay system instance is invalid", QSF_REACT_THROW);
		return *mFreeplaySystem;
	}

	inline HealthSystem& Plugin::getHealthSystem() const
	{
		// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
		QSF_ASSERT(nullptr != mHealthSystem, "The health system instance is invalid", QSF_REACT_THROW);
		return *mHealthSystem;
	}

	inline FireSystem& Plugin::getFireSystem() const
	{
		// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
		QSF_ASSERT(nullptr != mFireSystem, "The fire system instance is invalid", QSF_REACT_THROW);
		return *mFireSystem;
	}

	inline ActivitySystem& Plugin::getActivitySystem() const
	{
		// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
		QSF_ASSERT(nullptr != mActivitySystem, "The activity system instance is invalid", QSF_REACT_THROW);
		return *mActivitySystem;
	}

	inline qsf::game::AchievementSystem& Plugin::getAchievementSystem() const
	{
		// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
		QSF_ASSERT(nullptr != mAchievementSystem, "The achievement system instance is invalid", QSF_REACT_THROW);
		return *mAchievementSystem;
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::SystemContainer methods           ]
	//[-------------------------------------------------------]
	const char* Plugin::getSystemContainerName() const
	{
		return getName();
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

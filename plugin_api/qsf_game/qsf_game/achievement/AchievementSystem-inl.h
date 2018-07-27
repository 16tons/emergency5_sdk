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
	namespace game
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		inline SteamApiRuntimeLinking& AchievementSystem::getSteamApiRuntimeLinking()
		{
			// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
			QSF_ASSERT(nullptr != mSteamApiRuntimeLinking, "The Steam API runtime linking instance is invalid", QSF_REACT_THROW);
			return *mSteamApiRuntimeLinking;
		}

		inline void AchievementSystem::setWriteLock(bool writeLock)
		{
			mWriteLock = writeLock;
		}

		inline void AchievementSystem::getAchievementList(std::vector<const Achievement*>& achievementList) const
		{
			achievementList.assign(mAchievementList.begin(), mAchievementList.end());
		}


		//[-------------------------------------------------------]
		//[ Public virtual qsf::System methods                    ]
		//[-------------------------------------------------------]
		inline const char* AchievementSystem::getName() const
		{
			return "Achievement system";
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // game
} // qsf

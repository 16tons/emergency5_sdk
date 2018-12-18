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
	namespace game
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		inline bool SteamApiRuntimeLinking::isSteamApiEnabled() const
		{
			return mSteamApiEnabled;
		}

		inline void SteamApiRuntimeLinking::setSteamApiEnabled(bool enabled)
		{
			mSteamApiEnabled = enabled;
		}

		inline void SteamApiRuntimeLinking::setSteamAchievementsEnabled(bool enabled)
		{
			mSteamAchievementsEnabled = enabled;
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // game
} // qsf

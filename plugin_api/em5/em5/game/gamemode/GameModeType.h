// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{
	namespace gamemode
	{


		/** Game mode type */
		// When changing this, adapt the "em5::PlayerProgress" class accordingly
		enum Type
		{
			CAMPAIGN,		///< Campaign incl. main events
			FREEPLAY,		///< Single-player freeplay
			CHALLENGE,		///< Single-player challenge freeplay
			MULTIPLAY_COOP,	///< Multiplayer freeplay (COOP)
			MULTIPLAY_PVP,	///< Multiplayer freeplay (PVP)
			BENCHMARK		///< Benchmark
		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // gamemode
} // em5

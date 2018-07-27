// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/base/error/BoostAssertHandler.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline GameSimulation& Game::getSimulation() const
	{
		// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
		QSF_ASSERT(nullptr != mGameSimulation, "The game simulation instance is invalid", QSF_REACT_THROW);
		return *mGameSimulation;
	}

	inline GameAnimation& Game::getAnimation() const
	{
		// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
		QSF_ASSERT(nullptr != mGameAnimation, "The game animation instance is invalid", QSF_REACT_THROW);
		return *mGameAnimation;
	}

	inline qsf::SoundTrackManager& Game::getSoundTrackManager() const
	{
		// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
		QSF_ASSERT(nullptr != mSoundTrackManager, "The game music level system instance is invalid", QSF_REACT_THROW);
		return *mSoundTrackManager;
	}

	inline GameSessionConfiguration& Game::getGameSessionConfiguration() const
	{
		return mGameSessionConfiguration;
	}

	inline PlayerManager& Game::getPlayerManager() const
	{
		// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
		QSF_ASSERT(nullptr != mPlayerManager, "The player manager instance is invalid", QSF_REACT_THROW);
		return *mPlayerManager;
	}

	const RightsManager& Game::getRightsManager() const
	{
		// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
		QSF_ASSERT(nullptr != mRightsManager, "The rights manager instance is invalid", QSF_REACT_THROW);
		return *mRightsManager;
	}

	inline SelectionManager& Game::getSelectionManager() const
	{
		// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
		QSF_ASSERT(nullptr != mSelectionManager, "The selection manager instance is invalid", QSF_REACT_THROW);
		return *mSelectionManager;
	}

	inline qsf::game::GameCounterManager& Game::getGameCounterManager() const
	{
		// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
		QSF_ASSERT(nullptr != mGameCounterManager, "The game counter manager instance is invalid", QSF_REACT_THROW);
		return *mGameCounterManager;
	}

	inline qsf::game::GameTimerManager& Game::getGameTimerManager() const
	{
		// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
		QSF_ASSERT(nullptr != mGameTimerManager, "The game timer manager instance is invalid", QSF_REACT_THROW);
		return *mGameTimerManager;
	}

	inline qsf::logic::TargetPointManager& Game::getTargetPointManager() const
	{
		// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
		QSF_ASSERT(nullptr != mTargetPointManager, "The target point manager instance is invalid", QSF_REACT_THROW);
		return *mTargetPointManager;
	}

	inline OrderInfoManager& Game::getOrderInfoManager() const
	{
		// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
		QSF_ASSERT(nullptr != mOrderInfoManager, "The order info manager instance is invalid", QSF_REACT_THROW);
		return *mOrderInfoManager;
	}

	inline GangsterTypeManager& Game::getGangsterTypeManager() const
	{
		// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
		QSF_ASSERT(nullptr != mGangsterTypeManager, "The gangster type manager instance is invalid", QSF_REACT_THROW);
		return *mGangsterTypeManager;
	}

	inline SpecsGroupManager& Game::getSpecsGroupManager() const
	{
		// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
		QSF_ASSERT(nullptr != mSpecsGroupManager, "The specs group manager instance is invalid", QSF_REACT_THROW);
		return *mSpecsGroupManager;
	}

	inline HintSystem& Game::getHintSystem() const
	{
		// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
		QSF_ASSERT(nullptr != mHintSystem, "The hint system instance is invalid", QSF_REACT_THROW);
		return *mHintSystem;
	}

	inline CutscenePlayer& Game::getCutscenePlayer() const
	{
		// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
		QSF_ASSERT(nullptr != mCutscenePlayer, "The cutscene player instance is invalid", QSF_REACT_THROW);
		return *mCutscenePlayer;
	}

	inline OverlayManager& Game::getOverlayManager() const
	{
		// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
		QSF_ASSERT(nullptr != mOverlayManager, "The overlay manager instance is invalid", QSF_REACT_THROW);
		return *mOverlayManager;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

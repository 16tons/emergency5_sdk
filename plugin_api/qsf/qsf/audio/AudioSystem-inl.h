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
	inline AudioSystem::AudioSystem() :
		mAudioManager(nullptr)
	{
		// Nothing to do in here
	}

	inline AudioSystem::~AudioSystem()
	{
		// Nothing to do in here
	}

	inline AudioManager* AudioSystem::getAudioManager() const
	{
		return mAudioManager;
	}

	inline AudioManager& AudioSystem::getAudioManagerSafe() const
	{
		// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
		QSF_ASSERT(nullptr != mAudioManager, "The audio manager instance is invalid", QSF_REACT_THROW);
		return *mAudioManager;
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::System methods                    ]
	//[-------------------------------------------------------]
	inline const char* AudioSystem::getName() const
	{
		return "Audio system";
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf

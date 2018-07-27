// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/action/base/PlayAudioAction.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const qsf::NamedIdentifier PlayAudioAction::ACTION_ID	= "em5::PlayAudioAction";


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	PlayAudioAction::PlayAudioAction() :
		Action(ACTION_ID),
		mWaitForFinish(false),
		mInitSucceeded(false)
	{
		// Nothing here
	}

	PlayAudioAction::~PlayAudioAction()
	{
		// Nothing here
	}

	void PlayAudioAction::init(const std::string& audioSource, qsf::Entity* entity, bool spatial, bool loopAudio, bool waitForFinish, const boost::optional<qsf::Time> maxPlayTime, bool startAudioNowInInit)
	{
		mWaitForFinish = waitForFinish;
		mMaxPlayTime = maxPlayTime;

		mAudioProxy.sourceFile(audioSource);

		if (nullptr != entity)
		{
			mAudioProxy.contextEntity(*entity);
		}
		else
		{
			mAudioProxy.contextFree();
		}

		if (spatial)
		{
			mAudioProxy.spatial();
		}
		else
		{
			mAudioProxy.absolute();
		}

		if (loopAudio)
		{
			mAudioProxy.loop();
		}
		else
		{
			mAudioProxy.oneShot();
		}

		if (startAudioNowInInit)
		{
			mAudioProxy.play();
			mAudioProxy.synchronizeWithMultiplay();
		}

		mInitSucceeded = true;
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Actions methods                   ]
	//[-------------------------------------------------------]
	void PlayAudioAction::serialize(qsf::BinarySerializer& serializer)
	{
		serializer.serialize(mWaitForFinish);
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	bool PlayAudioAction::onStartup()
	{
		if (!mInitSucceeded)
			return false;

		mAudioProxy.play();
		mAudioProxy.synchronizeWithMultiplay();

		return true;
	}

	qsf::action::Result PlayAudioAction::updateAction(const qsf::Clock& clock)
	{
		// Check if the channel still exists
		if (!mAudioProxy.isPlaying())
		{
			// Audio is finished
			return qsf::action::RESULT_DONE;
		}

		if (!mWaitForFinish)
		{
			// Don't wait, but abort immediately
			mAudioProxy.detach();
			mAudioProxy.synchronizeWithMultiplay();
			return qsf::action::RESULT_DONE;
		}

		if (mMaxPlayTime.is_initialized())
		{
			// Countdown
			*mMaxPlayTime -= clock.getTimePassed();

			if (*mMaxPlayTime <= qsf::Time::ZERO)
			{
				// Time over
				return qsf::action::RESULT_DONE;
			}
		}

		// Wait for ending of the audio
		return qsf::action::RESULT_CONTINUE;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

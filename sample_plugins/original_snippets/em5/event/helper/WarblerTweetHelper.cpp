// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/event/helper/WarblerTweetHelper.h"
#include "em5/event/EventHelper.h"
#include "em5/freeplay/event/FreeplayEvent.h"
#include "em5/gui/EmergencyGui.h"
#include "em5/gui/IngameHud.h"
#include "em5/gui/hud/WarblerFeedWindow.h"
#include "em5/plugin/Jobs.h"
#include "em5/EM5Helper.h"

#include <qsf_game/component/event/EventTagHelper.h>

#include <qsf/map/Entity.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	WarblerTweetHelper::WarblerTweetHelper() :
		mWasTweeted(false),
		mMadeTweetPerson(false),
		mFreeplayEvent(nullptr)
	{
		// Nothing here
	}

	WarblerTweetHelper::~WarblerTweetHelper()
	{
		// Nothing here
	}

	void WarblerTweetHelper::initialize(const std::string& message, FreeplayEvent& freeplayEvent, const std::string& cameraEntityTag, const std::string& tweeterEntityTag, bool makeTweetPersonNow)
	{
		mMessage = message;
		mFreeplayEvent = &freeplayEvent;
		mCameraEntityTag = cameraEntityTag;
		mTweeterEntityTag = tweeterEntityTag;

		if (makeTweetPersonNow)
		{
			makeTweetPerson();
		}
	}

	void WarblerTweetHelper::tweetDelayed(const qsf::Time& delayTime)
	{
		qsf::jobs::JobConfiguration jobConfiguration;
		jobConfiguration.mTimeBetweenCalls = delayTime;
		mDelayJobProxy.registerAt(Jobs::SIMULATION_LOGIC, boost::bind(&WarblerTweetHelper::tweetNow, this), jobConfiguration);
	}

	void WarblerTweetHelper::tweetNow()
	{
		try
		{
			qsf::game::EventTagHelper eventTagHelper(QSF_MAKE_REF(mFreeplayEvent).getEventName());
			qsf::Entity* entity = nullptr;
			eventTagHelper.acquireTaggedEntity(entity, mCameraEntityTag);

			EM5_GUI.getIngameHud().getOrCreateWarblerFeedWindow().addWarblerTweet(mMessage, entity->getId());
		}
		catch (const std::exception&)
		{
			// Nothing here
		}

		// In case we delay the tweet, unregister
		mDelayJobProxy.unregister();

		makeTweetPerson();
		mWasTweeted = true;
	}

	void WarblerTweetHelper::makeTweetPerson()
	{
		if (mMadeTweetPerson)
			return;

		if (mTweeterEntityTag.empty())
			return;

		try
		{
			qsf::game::EventTagHelper eventTagHelper(QSF_MAKE_REF(mFreeplayEvent).getEventName());
			qsf::Entity* entity = nullptr;
			eventTagHelper.acquireTaggedEntity(entity, mTweeterEntityTag);

			EventHelper(*mFreeplayEvent).makeTweetPerson(*entity, true);
		}
		catch (const std::exception&)
		{
			// Nothing here
		}

		mMadeTweetPerson = true;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

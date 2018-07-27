// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"

#include <qsf/job/JobProxy.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace em5
{
	class FreeplayEvent;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Helper class for warbler tweets
	*/
	class EM5_API_EXPORT WarblerTweetHelper
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		WarblerTweetHelper();

		/**
		*  @brief
		*    Destructor
		*/
		~WarblerTweetHelper();

		void initialize(const std::string& message, FreeplayEvent& freeplayEvent, const std::string& cameraEntityTag, const std::string& tweeterEntityTag, bool makeTweetPersonNow = false);

		inline bool wasTweeted() const  { return mWasTweeted; }

		void tweetDelayed(const qsf::Time& delayTime);
		void tweetNow();
		void makeTweetPerson();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		bool mWasTweeted;
		bool mMadeTweetPerson;
		std::string mMessage;
		FreeplayEvent* mFreeplayEvent;
		std::string mCameraEntityTag;
		std::string mTweeterEntityTag;

		qsf::JobProxy	mDelayJobProxy;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

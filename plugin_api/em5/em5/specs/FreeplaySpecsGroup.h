// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/specs/SpecsGroup.h"

#include <qsf/time/Time.h>
#include <qsf/base/UniqueInstance.h>


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
	*    Freeplay specs group implementation
	*/
	class EM5_API_EXPORT FreeplaySpecsGroup : public SpecsGroup, public qsf::UniqueInstance<FreeplaySpecsGroup>
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		// Pluginable
		static const uint32 PLUGINABLE_ID;				///< "em5::FreeplaySpecsGroup" unique pluginable specs group ID
		static const uint32 START_WAIT_DURATION_MIN;	///< "StartWaitDurationMin" unique class property ID inside the class
		static const uint32 START_WAIT_DURATION_MAX;	///< "StartWaitDurationMax" unique class property ID inside the class
		static const uint32 WAIT_DURATION_MIN;			///< "WaitDurationMin" unique class property ID inside the class
		static const uint32 WAIT_DURATION_MAX;			///< "WaitDurationMax" unique class property ID inside the class
		static const uint32 MAX_EVENT_LIMIT;			///< "MaxEventLimit" unique class property ID inside the class
		static const uint32 MIN_EVENT_LIMIT;				///< "MinEventLimit" unique class property ID inside the class
		static const uint32 WON_EVENTS_FOR_NOT_DECREASE;	///< "WonEventsForNotDecrease" unique class property ID inside the class
		static const uint32 WON_EVENTS_FOR_INREASE;			///< "WonEventsForIncrease" unique class property ID inside the class
		static const uint32 WON_EVENTS_FOR_BUY_OPTIONS;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] specsGroupManager
		*    Optional pointer to the specs group manager this specs group should be registered to, can be a null pointer
		*/
		inline explicit FreeplaySpecsGroup(SpecsGroupManager* specsGroupManager);

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~FreeplaySpecsGroup();

		inline float getStartWaitDuration_Min() const;
		void setStartWaitDuration_Min(float delay);

		inline float getStartWaitDuration_Max() const;
		void setStartWaitDuration_Max(float delay);

		inline float getWaitDuration_Min() const;
		void setWaitDuration_Min(float delay);

		inline float getWaitDuration_Max() const;
		void setWaitDuration_Max(float delay);

		inline uint32 getMinEventLimit() const;
		void setMinEventLimit(uint32 minEvents);

		inline uint32 getMaxEventLimit() const;
		void setMaxEventLimit(uint32 maxEvents);

		inline uint32 getWonEventsForNotDecrease() const;
		void setWonEventsForNotDecrease(uint32 WonEventsForNotDecrease);

		inline uint32 getWonEventsForIncrease() const;
		void setWonEventsForIncrease(uint32 WonEventsForIncrease);

		inline uint32 getWonEventsForBuyOptions() const;
		void setWonEventsForBuyOptions(uint32 neededEvents);

		inline const qsf::Time& getWorldEventWaitTime() const;
		void setWorldEventWaitTime(const qsf::Time& waitTime);

		inline float getWorldEventTriggerChance() const;
		void setWorldEventTriggerChance(float triggerChance);

		inline const qsf::Time& getWorldEventMinDurationTime() const;
		void setWorldEventMinDurationTime(const qsf::Time& durationTime);

		inline const qsf::Time& getWorldEventMaxDurationTime() const;
		void setWorldEventMaxDurationTime(const qsf::Time& durationTime);

		inline const qsf::Time& getWorldEventWaitAfterEndTime() const;
		void setWorldEventWaitAfterEndTime(const qsf::Time& waitTime);

		inline uint32 getGlobalEventTryCount() const;
		void setGlobalEventTryCount(uint32 tryCount);

		inline float getDensityLimitFreeplay() const;
		inline void setDensityLimitFreeplay(float densityLimit);

		inline float getDensityLimitChallenge() const;
		inline void setDensityLimitChallenge(float densityLimit);

		inline float getDensityLimitMultiplay() const;
		inline void setDensityLimitMultiplay(float densityLimit);

		inline float getDensityLimitCampaign() const;
		inline void setDensityLimitCampaign(float densityLimit);

		inline float getChallengeAbortRatingDifference() const;
		inline void setChallengeAbortRatingDifference(float difference);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		float		mStartWaitDuration_Min;				///< The delay after the first event is started
		float		mStartWaitDuration_Max;				///< The maximum delay after the first event is started
		float		mWaitDuration_Min;					///< The delay of the breaks between two events
		float		mWaitDuration_Max;					///< The maximum delay between two events
		uint32		mMinEventLimit;						///< The maximum amount of freeplay events that run parallel
		uint32		mMaxEventLimit;						///< The maximum amount of freeplay events that run parallel
		uint32		mWonEventsForNotDecrease;			///< Number of won events to not decrease when two events are lost
		uint32		mWonEventsForIncrease;				///< Number of won events in a row before increasing the max event limit
		uint32		mWonEventsForBuyOptions;			///< Number of freeplay events to be won before buy options are unlocked
		qsf::Time	mWorldEventWaitTime;				///< Wait time for the world event start/ends
		float		mWorldEventTriggerChance;			///< Trigger chance to activate the world event phase
		qsf::Time	mWorldEventMinDurationTime;			///< Min Duration of the world event phase
		qsf::Time	mWorldEventMaxDurationTime;			///< Max Duration of the world event phase
		qsf::Time	mWorldEventWaitAfterEndTime;		///< Waiting time after a world event phase until the next can be triggered
		uint32		mGlobalEventTryCount;				///< How often a event should be tried to be triggered
		float		mDensityLimitFreeplay;				///< Event density limit for the "freeplay" gamemode, when this value is reached no more events will be triggered
		float		mDensityLimitChallenge;				///< Event density limit for the "challenge" gamemode, when this value is reached no more events will be triggered
		float		mDensityLimitMultiplay;				///< Event density limit for the "multiplay" gamemode, when this value is reached no more events will be triggered
		float		mDensityLimitCampaign;				///< Event density limit for the "campaign" gamemode, when this value is reached no more events will be triggered
		float		mChallengeAbortRatingDifference;	///< Abort value for the challenge mode, when the difference between the density limit and the current density is greater then this value: challenge failed


	//[-------------------------------------------------------]
	//[ CAMP reflection system                                ]
	//[-------------------------------------------------------]
	QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "em5/specs/FreeplaySpecsGroup-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(em5::FreeplaySpecsGroup)

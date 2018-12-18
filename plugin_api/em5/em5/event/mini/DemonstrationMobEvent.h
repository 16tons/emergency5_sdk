// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/freeplay/event/FreeplayEvent.h"

#include <qsf/base/PropertyHelper.h>
#include <qsf/base/WeakPtr.h>
#include <qsf/component/base/TransformComponent.h>
#include <qsf/message/MessageProxy.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace em5
{
	class BuildingComponent;
	class BlockAreaWithUnits;
	class WarblerTweetHelper;
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
	*    Emergency 2017 mini-event "Hamburg demonstration"
	*/
	class EM5_API_EXPORT DemonstrationMobEvent : public FreeplayEvent
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class FreeplayEventMetaclasses2;		// CAMP export of editable values for Game Design


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 FREEPLAY_EVENT_ID;		///< "em5::DemonstrationMobEvent" unique main event ID


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		DemonstrationMobEvent();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~DemonstrationMobEvent();


	//[-------------------------------------------------------]
	//[ Public virtual em5::FreeplayEvent methods             ]
	//[-------------------------------------------------------]
	public:
		virtual bool onStartup() override;
		virtual void onShutdown() override;
		virtual void onRun() override;
		virtual bool onFailure(EventResult& eventResult) override;
		virtual void updateFreeplayEvent(const qsf::Time& timePassed) override;
		virtual bool addEntityToEvent(qsf::Entity& targetEntity, eventspreadreason::Reason eventSpreadReason, bool newReason) override;
		virtual void hintCallback(Observer& hintObserver) override;
		virtual const qsf::Entity* getFocusEntity() override;
		virtual bool checkEventConfiguration() override;
		virtual void serialize(qsf::BinarySerializer& serializer) override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void setupScene();
		void startObjectives();
		void startHintObservers();

		void onGameTimerMessage(const qsf::MessageParameters& parameters);
		void onPersonArrestedOrInjured(const qsf::MessageParameters& parameters);

		void startExtremists();
		void startNextExtremist();
		void triggerArsonists();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Configuration
		float  mTimeToSecureDemo;		// In seconds, we don't use "qsf::Time" for direct connection to an editable property in event JSON
		uint32 mBurningObjectsToFail;
		uint32 mInactiveExtremistsToTriggerArsonists;

		// Event entities
		std::vector<qsf::WeakPtr<qsf::Entity>> mWaitingExtremists;
		std::vector<qsf::WeakPtr<qsf::Entity>> mStartedExtremists;
		std::vector<qsf::WeakPtr<qsf::Entity>> mArsonists;
		qsf::WeakPtr<qsf::Entity> mHouseFire01;

		BlockAreaWithUnits& mBlockAreaWithUnits;

		// Warbler tweet
		WarblerTweetHelper& mTweet01;
		WarblerTweetHelper& mTweet02;
		bool mTweet02TimerStarted;

		// Messages
		qsf::MessageProxy mGameTimerMessageProxy;
		qsf::MessageProxy mPersonArrestedMessageProxy;
		qsf::MessageProxy mPersonInjuredMessageProxy;

		enum State
		{
			STATE_SECURE_DEMO,
			STATE_STOP_EXTREMISTS,
			STATE_STOP_ALL
		};
		State mState;


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
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(em5::DemonstrationMobEvent)

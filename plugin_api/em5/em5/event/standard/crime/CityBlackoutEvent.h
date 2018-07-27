// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/freeplay/event/FreeplayEvent.h"

#include <qsf/base/WeakPtr.h>
#include <qsf/message/MessageProxy.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace em5
{
	class SpawnPointComponent;
	class WarblerTweetHelper;
}
namespace qsf
{
	class Layer;
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
	*    EMERGENCY 5 freeplay event "City blackout"
	*/
	class EM5_API_EXPORT CityBlackoutEvent : public FreeplayEvent
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 FREEPLAY_EVENT_ID;		///< "em5::CityBlackoutEvent" unique freeplay event ID


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		CityBlackoutEvent();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~CityBlackoutEvent();

		void init(qsf::Layer& eventLayer);

		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(DeadPersonsToFail, uint32, mDeadPersonsToFail);
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(BurningObjectsToFail, uint32, mBurningObjectsToFail);
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(MinTriggerTimeOfDay, qsf::Time, mMinTriggerTimeOfDay);
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(MaxTriggerTimeOfDay, qsf::Time, mMaxTriggerTimeOfDay);
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(CorrectionTimeOfDay, qsf::Time, mCorrectionTimeOfDay);
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(TimeToDefuseBomb, qsf::Time, mTimeToDefuseBomb)
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(TimeToWaitForExplosion, qsf::Time, mTimeToWaitForExplosion);
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(WaitTimeForArea02, qsf::Time, mWaitTimeForArea02);
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(TimeToRepairEbox, qsf::Time, mTimeToRepairEbox);
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(TimeToDamageEbox, qsf::Time, mTimeToDamageEbox);


	//[-------------------------------------------------------]
	//[ Public virtual em5::FreeplayEvent methods             ]
	//[-------------------------------------------------------]
	public:
		virtual bool onStartup() override;
		virtual void onShutdown() override;
		virtual void onRun() override;
		virtual bool onFailure(EventResult& eventResult) override;
		virtual bool onSuccess(EventResult& eventResult) override;
		virtual void updateFreeplayEvent(const qsf::Time& timePassed) override;
		virtual bool addEntityToEvent(qsf::Entity& targetEntity, eventspreadreason::Reason eventSpreadReason, bool newReason) override;
		virtual const qsf::Entity* getFocusEntity() override;
		virtual bool checkEventConfiguration() override;
		virtual void serialize(qsf::BinarySerializer& serializer) override;
		virtual void hintCallback(Observer& hintObserver) override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void startObjectives();
		void setupScene();
		void setupSceneMiniEvent();
		void setupSceneFreeplay();

		void setBlackoutAreaActive(uint32 index, bool blackout);
		void makeEboxDefective(uint32 index);
		void spawnFreeplayGangster();
		void startTrafficAccident(const std::string& accidentLayerName);
		void injurePerson(const std::string& eventTag);

		void onGameTimerMessage(const qsf::MessageParameters& parameters);
		void onBombExplosion(const qsf::MessageParameters& parameters);
		void onEboxDamageByGangster(const qsf::MessageParameters& parameters);
		void onEboxRepaired(const qsf::MessageParameters& parameters);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Configuration
		uint32	  mDeadPersonsToFail;
		uint32	  mBurningObjectsToFail;
		qsf::Time mMinTriggerTimeOfDay;
		qsf::Time mMaxTriggerTimeOfDay;
		qsf::Time mCorrectionTimeOfDay;
		qsf::Time mTimeToDefuseBomb;
		qsf::Time mTimeToWaitForExplosion;
		qsf::Time mWaitTimeForArea02;
		qsf::Time mTimeToRepairEbox;
		qsf::Time mTimeToDamageEbox;

		// Layers
		qsf::Layer*	mEventLayer;	///< Base layer for this event (for both mini-event and freeplay event)
		qsf::Layer*	mAddLayer;		///< "add" sub-layer for phase 1 of mini-event
		qsf::Layer*	mEraseLayer;	///< "erase" sub-layer for phase 1 of mini-event
		qsf::Layer*	mAddLayerP2;	///< "add" sub-layer for phase 2 of mini-event
		qsf::Layer*	mEraseLayerP2;	///< "erase" sub-layer for phase 2 of mini-event

		std::vector<std::pair<qsf::Layer*, qsf::Layer*>> mLayersToCleanup;		// First is the layer to cleanup, second is an optional "erase" layer to reactivate

		// Objects
		enum EboxState
		{
			EBOX_INTACT,
			EBOX_DEFECTIVE,
			EBOX_REPAIRED
		};
		struct EboxInfo
		{
			qsf::WeakPtr<qsf::Entity> mEbox;
			std::vector<qsf::Entity*> mAreaBoxes;
			EboxState mState;
		};
		std::vector<EboxInfo> mEboxInfos;
		qsf::WeakPtr<SpawnPointComponent> mTerroristSpawnPoint;
		qsf::WeakPtr<qsf::Entity> mBuildingToDestroy;
		qsf::WeakPtr<qsf::Entity> mGangster;
		qsf::WeakPtr<qsf::Entity> mBomb;

		// Warbler tweet
		WarblerTweetHelper& mTweet01;

		// Messages
		qsf::MessageProxy	mGameTimerMessageProxy;
		qsf::MessageProxy	mBombExplosionMessageProxy;	///< Message gets emitted by gangster (mini-event only)
		qsf::MessageProxy	mDamageEboxMessageProxy;	///< Message gets emitted by gangster (freeplay event only)
		qsf::MessageProxy	mRepairEboxMessageProxy;	///< Freeplay event only


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::CityBlackoutEvent)

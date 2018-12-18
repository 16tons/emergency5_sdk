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
#include <qsf/message/MessageProxy.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
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
	*    EMERGENCY 5 freeplay event "train accident"
	*/
	class EM5_API_EXPORT TrainAccidentEvent : public FreeplayEvent
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 FREEPLAY_EVENT_ID;		///< "em5::TrainAccidentEvent" unique freeplay event ID


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		TrainAccidentEvent();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~TrainAccidentEvent();

		void init(qsf::Layer* accidentLayer);

		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(PersonHangingTime, qsf::Time, mPersonHangingTime)
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(DeadPersonsToFail, int, mDeadPersonsToFail)
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(FailLimitBurningObjects, int, mFailLimitBurningObjects)


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

		bool getSubLayerFromMainLayer(qsf::Layer& mainLayer);
		void setupScene(qsf::Layer& mainLayer);

		bool checkForDLKActionInProgress();
		void checkForSelectedTechHeli();
		void checkForTrackPersonTweet();
		void onHangingPersonSaved();
		void onHangingPersonSavedPlacedFromDLK();
		void onEboxRepaired();
		void onBuriedFound(const qsf::MessageParameters& parameters);
		void onDropedPersonsFromHeli(const qsf::MessageParameters& parameters);

		void showSparkleEffect(bool active);
		void startFallingHangingPerson();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		uint32 mAccidentLayerId;

		qsf::Layer*	mEventLayer;
		qsf::Layer*	mEraseLayer;

		qsf::WeakPtr<qsf::Entity>	mHangingPerson;
		qsf::WeakPtr<qsf::Entity>	mEBoxEntity;
		qsf::WeakPtr<qsf::Entity>	mEBoxEffect;
		std::vector<qsf::WeakPtr<qsf::Entity>>	mEBoxPersons;
		qsf::WeakPtr<qsf::Entity>	mTweetCameraEntity;
		qsf::WeakPtr<qsf::Entity>	mTweetPersonEntity;

		// Settings
		qsf::Time	mPersonHangingTime;
		int			mDeadPersonsToFail;
		int			mFailLimitBurningObjects;

		// States
		bool		mHangingPersonRescued;
		qsf::MessageProxy	mHangingPersonSaveProxy;
		bool		mEboxRepaired;
		qsf::MessageProxy	mEboxRepairedProxy;
		qsf::MessageProxy	mBuriedFoundMessageProxy;
		qsf::MessageProxy	mPersonDropedByHelicopterProxy;
		bool		mCanPlayPickupPersonTweet;


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::TrainAccidentEvent)

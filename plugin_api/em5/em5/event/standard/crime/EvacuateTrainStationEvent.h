// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/freeplay/event/FreeplayEvent.h"

#include "qsf/base/WeakPtr.h"
#include "qsf/math/Transform.h"
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
	*    EMERGENCY 5 freeplay event "Evacuate TrainStation"
	*/
	class EM5_API_EXPORT EvacuateTrainStationEvent : public FreeplayEvent
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 FREEPLAY_EVENT_ID;		///< "em5::EvacuateTrainStationEvent" unique freeplay event ID


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		EvacuateTrainStationEvent();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~EvacuateTrainStationEvent();

		void init(qsf::Layer& eventLayer);

		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(TimeTillBombExplode, qsf::Time, mTimeTillBombExplode);
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(TimeToBombDefuse, qsf::Time, mTimeToBombDefuse);
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(PersonsLeaveTillTweet, int, mPersonsLeaveTillTweet);


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
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		struct PersonsPosition
		{
			uint64	entityId;
			qsf::Transform	finalTransform;
		};


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void startObjectives();
		bool getSubLayerFromMainLayer();
		void setupScene();
		void setupFleeingPerson(qsf::Entity& fleeingPerson);

		void onEvacuatePerson(const qsf::MessageParameters& parameters);
		void onBombDefused(const qsf::MessageParameters& parameters);
		void onFoundBomb(const qsf::MessageParameters& parameters);
		void activateBombCases();
		void activateRealBomb();

		void onReleasedFromMegaphone(const qsf::MessageParameters& parameters);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Settings
		qsf::Time	mTimeTillBombExplode;
		qsf::Time	mTimeToBombDefuse;
		qsf::Time	mTimeDelayLeavingStation;
		int			mPersonsLeaveTillTweet;

		// Setup
		qsf::Layer*	mMainLayer;
		qsf::Layer*	mEventLayer;
		qsf::Layer*	mEraseLayer;

		// Internal
		qsf::WeakPtr<qsf::Entity>	mStationEntity;	// This is the main entity for the event
		qsf::WeakPtr<qsf::Entity>	mStationEntity2;	// This is the second train station exit for the evacuated persons
		qsf::WeakPtr<qsf::Entity>	mEvacuationEntity;
		std::vector<qsf::WeakPtr<qsf::Entity>>	mFleeingCivil;
		std::vector<qsf::WeakPtr<qsf::Entity>>	mGapingCivil;
		std::vector<qsf::WeakPtr<qsf::Entity>>	mPotentialPersonsInArea;
		std::vector<qsf::WeakPtr<qsf::Entity>>	mFakeBombEntities;
		qsf::WeakPtr<qsf::Entity>	mBombEntity;
		qsf::WeakPtr<qsf::Entity>	mEvacuateArea;
		qsf::WeakPtr<qsf::Entity>	mTweetCameraEntity;
		boost::container::flat_set<uint64> mCivilistsToChaseAway;

		bool	mTrainStationIsEvacuated;
		qsf::MessageProxy	mEvacuatePersonsMessageProxy;
		bool	mPersonsShocked;
		bool	mBombDefused;
		qsf::MessageProxy	mBombExplodeMessageProxy;
		qsf::MessageProxy	mSearchBombMessageProxy;
		bool	mCanTweet;
		std::vector<PersonsPosition>	mPersonsPositions;
		int		mNumberPersonsEvacuated;

		qsf::MessageProxy	mReleasedFromMegaphoneMessageProxy;



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
QSF_CAMP_TYPE_NONCOPYABLE(em5::EvacuateTrainStationEvent)

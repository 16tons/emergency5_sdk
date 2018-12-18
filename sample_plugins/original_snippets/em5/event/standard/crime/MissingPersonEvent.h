// Copyright (C) 2012-2018 Promotion Software GmbH


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
#include <qsf/job/JobProxy.h>


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
	*    EMERGENCY 5 freeplay event "missing person"
	*/
	class EM5_API_EXPORT MissingPersonEvent : public FreeplayEvent
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 FREEPLAY_EVENT_ID;		///< "em5::MissingPersonEvent" unique freeplay event ID


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		MissingPersonEvent();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~MissingPersonEvent();

		/**
		*  @brief
		*    Initialize the event with values
		*/
		void init(qsf::Entity& entity, uint64 DogSearchSpotId);

		/**
		*  @brief
		*    Get name of the injury the missing person receives
		*/
		const std::string& getInjuryName() const;

		/**
		*  @brief
		*    Set name of the injury the missing person receives
		*/
		void setInjuryName(const std::string& injuryName);

		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(IsVictimDead, bool, mIsVictimDead)
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(KillerSeekTime, qsf::Time, mKillerSeekTime)
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(MurderTag, const std::string&, mMurderTag)
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(EscapeTargetTag, const std::string&, mEscapeTargetTag)


	//[-------------------------------------------------------]
	//[ Public virtual em5::FreeplayEvent methods             ]
	//[-------------------------------------------------------]
	public:
		virtual bool onStartup() override;
		virtual void onShutdown() override;
		virtual void onRun() override;
		virtual bool onSuccess(EventResult& eventResult) override;
		virtual bool onFailure(EventResult& eventResult) override;
		virtual void updateFreeplayEvent(const qsf::Time& timePassed) override;
		virtual void hintCallback(Observer& hintObserver) override;
		virtual const qsf::Entity* getFocusEntity() override;
		virtual bool checkEventConfiguration() override;
		virtual void serialize(qsf::BinarySerializer& serializer) override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void startObjectives();
		void startHintObservers();
		void onGameTimerMessage(const qsf::MessageParameters& parameters);

		void foundMissingPerson();
		bool checkIfMissingPersonIsFound();
		void removeIconEntity();
		void startKillerSeeking();

		void onDiagnosisDead(const qsf::MessageParameters& parameters);
		void onKillerDetected(const qsf::MessageParameters& parameters);
		void onRescueDogFindPerson(const qsf::MessageParameters& parameters);
		void findEntityInDivingSpot(const qsf::MessageParameters& parameters);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Configuration
		bool		mIsVictimDead;				///< Is the person dead (true) or injured (false)
		bool		mIsPersonUnderwater;		///< "true" if event has person in water and it is not rescued by diver
		uint64		mDogSearchSpotId;			///< ID of the spot were the dog finish searching (used for water events)
		glm::vec3	mMissingPersonPosition;		///< Position of the missing person (used as backup because we swap the person to optional search dog spot)
		std::string	mInjuryName;				///< Name of the injury the person receives at start of the event
		std::string	mMurderTag;
		std::string	mEscapeTargetTag;
		qsf::Time	mKillerSeekTime;			///< Time to seek the killer with sek drone
		qsf::Time	mHint01WaitTime;			///< Waiting time for hint 01 (30 sec)
		qsf::Time	mKillerSeekHint01WaitTime;	///< Waiting time for killer seek hint 01 (20 sec)

		// States
		bool		mIsPersonFound;				///< Is the missing person found
		bool		mIsKillerDetectedByDrone;	///< "true" if the killer detected by the police drone

		// Internal
		uint64		mKillerEntityId;			///< ID of the killer/murder person
		qsf::WeakPtr<qsf::Entity> mMissingPerson;
		qsf::WeakPtr<qsf::Entity> mIconEntity;

		qsf::MessageProxy	mDeadPersonDignosisProxy;
		qsf::MessageProxy	mKillerFoundMessageProxy;
		qsf::MessageProxy	mRescueDogFindPersonProxy;
		qsf::MessageProxy	mFindEntityFromDivingSpotProxy;

		qsf::MessageProxy	mGameTimerMessageProxy;


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::MissingPersonEvent)

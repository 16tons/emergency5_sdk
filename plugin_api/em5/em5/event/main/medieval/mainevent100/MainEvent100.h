// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/freeplay/event/main/MainEvent.h"

#include <qsf/job/JobProxy.h>
#include <qsf/base/WeakPtr.h>
#include <qsf/math/Transform.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace em5
{
	class MedievalPreludeScreen;
	class OrderInfo;
	class Player;
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
	*    EMERGENCY 2016 main event 100 "Medieval Cologne"
	*/
	class MainEvent100 : public MainEvent
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class MainEventMetaclasses;		// CAMP export of editable values for Game Design


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 FREEPLAY_EVENT_ID;		///< "em5::MainEvent100" unique freeplay event ID


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	public:
		static void setMedievalGraphics(bool medievalMode, int gameSessionConfiguration = 1);


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		MainEvent100();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~MainEvent100();


	//[-------------------------------------------------------]
	//[ Public virtual em5::FreeplayEvent methods             ]
	//[-------------------------------------------------------]
	public:
		//[-------------------------------------------------------]
		//[ Life cycle                                            ]
		//[-------------------------------------------------------]
		virtual bool onStartup() override;
		virtual void onShutdown() override;
		virtual void onRun() override;
		virtual bool onSuccess(EventResult& eventResult) override;
		virtual bool onFailure(EventResult& eventResult) override;

		//[-------------------------------------------------------]
		//[ Points evaluation                                     ]
		//[-------------------------------------------------------]
		virtual uint32 calculatePointGain(EventResult& eventResult) override;

		//[-------------------------------------------------------]
		//[ Callbacks from observers                              ]
		//[-------------------------------------------------------]
		virtual bool addEntityToEvent(qsf::Entity& targetEntity, eventspreadreason::Reason eventSpreadReason, bool newReason) override;
		virtual void hintCallback(Observer& hintObserver) override;

		//[-------------------------------------------------------]
		//[ Miscellaneous                                         ]
		//[-------------------------------------------------------]
		virtual const qsf::Entity* getFocusEntity() override;

		//[-------------------------------------------------------]
		//[ Serialize                                             ]
		//[-------------------------------------------------------]
		virtual void serialize(qsf::BinarySerializer& serializer) override;


	//[-------------------------------------------------------]
	//[ Protected virtual em5::MainEvent methods              ]
	//[-------------------------------------------------------]
	protected:
		virtual void onStartIntroRequest() override;
		virtual void onIntroStart(const qsf::MessageParameters& parameters) override;
		virtual void onIntroFinished(const qsf::MessageParameters& parameters) override;
		virtual void onUpdateMainEvent(const qsf::Time& timePassed) override;
		virtual void onPrologueFinished(const qsf::MessageParameters& parameters) override;
		virtual void onUpdateMusic(const qsf::Time& timePassed) override;
		virtual uint32 getEventNumber() override;
		virtual std::string getCamStartEventTag() override;
		virtual void showBriefingSupervisorMessage() override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void blockInput(bool block);
		void onMedievalPreludeScreenPlaybackFinished();

		/**
		*  @brief
		*    Is called after the intro was finished
		*/
		void introFinished(const qsf::MessageParameters& parameters);

		void gatherEventEntities();

		void registerPlagueDoctorForPlayer();
		void registerSpawnMechanic();
		void deregisterSpawnMechanic();

		void addPlaguePersonToEvent(qsf::Entity& plaguePerson);
		void addPlagueBuildingToEvent(qsf::Entity& plagueBuilding);

		void despawnPlagueDoctor();
		void setupPlagueDoctor(qsf::Entity& entity, OrderInfo& plagueDoctorOrderInfo, Player& localPlayer);
		void updatePlagueDoctorEvasion();

		void startPhase1();
		void startBoatCutscene();
		void startPhase2();
		void winall();	// Debug

		void onUpdateInfoScreen(const qsf::JobArguments& jobArguments);
		void fadeoutComplete(const qsf::MessageParameters& parameters);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Hint values
		qsf::Time mHint08WaitTime;
		qsf::Time mHint09WaitTime;
		qsf::Time mHintTabWaitTime;
		bool mCanStartHint08;
		bool mCanStartHint09;
		bool mCanStartHintTab;

		qsf::Time	mTimeGroupStarts1;
		qsf::Time	mTimeGroupStarts2;
		// Internal data
		MedievalPreludeScreen* mMedievalPreludeScreen;	///< Valid while the event is running, can be a null pointer, destroy the instance if you no longer need it
		qsf::WeakPtr<qsf::Entity> mDespawnHarbour;
		qsf::WeakPtr<qsf::Entity> mDespawnTriggerHarbour;
		qsf::WeakPtr<qsf::Entity> mHarbourMark;
		qsf::Time mTimeWaitTillStart;
		qsf::Time mPhase1TimeOut;

		struct UnitPosition
		{
			qsf::WeakPtr<qsf::Entity> mUnit;
			qsf::Transform mStartTransform;
			bool	mKeepInPhase2;
		};
		std::vector<UnitPosition>	mUnitPositions;

		// Trigger the groupstart in phase2 only once
		qsf::Time	mPhase1Active;
		qsf::Time	mPhase2Active;
		bool mCanGroupStart1;
		bool mCanGroupStart2;

		enum State
		{
			State_Intro,
			State_SelectDoctor,
			State_OpenInfoScreen,
			State_CloseInfoScreen,
			State_WaitTillStart,
			State_Phase1,
			State_BoatCutscene,
			State_Phase2,
			State_Wait_for_fadeout
		};

		State mCurrentState;
		qsf::JobProxy	mRegisterOpenHelpWindowJobProxy;	// Only active in specific gamestate


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::MainEvent100)

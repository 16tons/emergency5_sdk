// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/freeplay/event/main/MainEvent.h"


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
	*    EMERGENCY 5 main event 10 "Plane Crash"
	*/
	class MainEvent10 : public MainEvent
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class MainEventMetaclasses;		// CAMP export of editable values for Game Design


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 FREEPLAY_EVENT_ID;		///< "em5::MainEvent10" unique freeplay event ID


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		MainEvent10();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~MainEvent10();


	//[-------------------------------------------------------]
	//[ Public virtual em5::FreeplayEvent methods             ]
	//[-------------------------------------------------------]
	public:
		//[-------------------------------------------------------]
		//[ Life cycle                                            ]
		//[-------------------------------------------------------]
		virtual bool onStartup() override;
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
	//[ Protected virtual em5::MainEvent methods              ]
	//[-------------------------------------------------------]
	protected:
		virtual void onIntroFinished(const qsf::MessageParameters& parameters) override;
		virtual void onUpdateMainEvent(const qsf::Time& timePassed) override;
		virtual void onUpdateMusic(const qsf::Time& timePassed) override;
		virtual uint32 getEventNumber() override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void gatherEventEntities();
		void hurtPeopleIntro();

		void startObjectives();
		void defineObjectives();
		void startHintObservers();

		void onDebrisLifted(const qsf::MessageParameters& parameters);

		void startFinalCutscene(const std::string* cutscene);
		void endFinalCutscene();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Connected to CAMP

		std::vector<qsf::Entity*>	mVictims;					// Openly lying around and injured
		std::vector<qsf::Entity*>	mBlocks;					// Blocks that may hide injured
		std::vector<qsf::Entity*>	mBuried;					// Injured hiding under debris
		std::vector<qsf::Entity*>	mDrowning;					// Drowning people
		std::vector<qsf::Entity*>	mBurningFromStart;			// Fires to start at the beginning
		std::vector<qsf::Entity*>	mDestroyLight;				// Entities to be destroyed lightly
		std::vector<qsf::Entity*>	mDestroyHeavy;				// Entities to be destroyed heavily

		std::vector<std::string>	mInjuriesOpenVictims;		// Injuries of mVictims
		std::vector<std::string>	mInjuriesBuriedVictims;		// Injuries of mBuried

		qsf::Entity*				mFocusEntity;

		qsf::MessageProxy			mHint02DebrisLiftedProxy;

		uint32			mAmountOfBuriedPeople;
		uint32			mMaxDeadCivilPersons;
		float			mHint01LifeEnergy;
		qsf::Time		mHint02WaitTime;
		float			mHint03LifeEnergy;
		float			mHint05LifeEnergy;
		qsf::Time		mHint07WaitTime;
		qsf::Time		mHint08WaitTime;
		qsf::Time		mAchievementTime;

		bool			mHint01Triggered;
		bool			mHint02Triggered;
		bool			mHint03Triggered;
		bool			mHint05Triggered;
		bool			mHint16Triggered;
		bool			mHint07Triggered;
		bool			mHint08Triggered;
		bool			mAchievementTimeOk;


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::MainEvent10)

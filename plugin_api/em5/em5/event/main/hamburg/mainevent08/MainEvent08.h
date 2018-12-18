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
	*    EMERGENCY 5 main event 08 "Flood"
	*/
	class MainEvent08 : public MainEvent
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class MainEventMetaclasses;		// CAMP export of editable values for Game Design


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 FREEPLAY_EVENT_ID;		///< "em5::MainEvent08" unique freeplay event ID


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		MainEvent08();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~MainEvent08();


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
	//[ Protected virtual em5::MainEvent methods              ]
	//[-------------------------------------------------------]
	protected:
		virtual void onIntroFinished(const qsf::MessageParameters& parameters) override;
		virtual void onPrologueFinished(const qsf::MessageParameters& parameters) override;
		virtual void onUpdateMainEvent(const qsf::Time& timePassed) override;
		virtual void onUpdateMusic(const qsf::Time& timePassed) override;
		virtual uint32 getEventNumber() override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void gatherEventEntities();
		void hurtPeopleIntro();

		void onPersonPlacedInAmbulance();	// Used for Hint06
		void onFirstTimeDive();				// Used for Hint11

		void onPickupPersonWithHelicopter(const qsf::MessageParameters& parameters);	// Used for Hint13
		void onPersonDeath(const qsf::MessageParameters& parameters);	// Used for Hint18 & Hint19
		void onSunkenCarFound(const qsf::MessageParameters& parameters);
		void onSunkenCarRetrieved(const qsf::MessageParameters& parameters);

		void startObjectives();
		void defineObjectives();
		void startHintObservers();

		void startFinalCutscene(const std::string* cutscene);
		void endFinalCutscene();
		bool checkAchievementConditions();

		void deactivateFireComponentsUnderWater();
		void activateFireComponentsUnderWater();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Connected to CAMP

		// Hint related

		float						mHint02LifeEnergy;
		bool						mHint02Triggered;

		qsf::MessageProxy			mDiveCommandListenerProxy;
		float						mHint03LifeEnergy;
		float						mHint03LifeEnergyNoDiver;
		bool						mHint03DiverSent;
		bool						mHint03Triggered;

		float						mHint04LifeEnergy;
		bool						mHint04Triggered;

		qsf::Time					mHint06WaitTime;
		int							mHint06NeededAmount;
		int							mHint06CurrentAmount;
		qsf::MessageProxy			mPersonPlacedInAmbulanceProxy; // Used for Hint06
		bool						mHint06Triggered;

		qsf::Time					mHint07WaitTime;
		bool						mHint07Triggered;

		qsf::Time					mHint08WaitTime;
		bool						mHint08Triggered;
		qsf::MessageProxy			mPersonPlacedInBoatProxy; // Used for Hint08

		qsf::Time					mHint09WaitTime;
		bool						mHint09Triggered;

		qsf::Time					mHint10WaitTime;
		bool						mHint10Triggered;

		qsf::Time					mHint12WaitTime;
		bool						mHint12Triggered;

		qsf::MessageProxy			mPersonFromRoofRescuedProxy;
		qsf::Time					mHint13WaitTime;
		bool						mHint13TriggeredorObsolete;

		bool						mHint16Triggered;

		bool						mHint17Triggered;

		qsf::MessageProxy			mPeopleDyingProxy;
		bool						mHint18Triggered;
		bool						mHint19Triggered;

		// References
		std::vector<qsf::Entity*>	mEnclosed;				// People trapped in sunken cars
		std::vector<qsf::Entity*>	mPeopleOnRoof;			// People trapped on the roofs
		std::vector<qsf::Entity*>	mBorderBuildings;		// People trapped on the roofs
		std::vector<qsf::Entity*>	mOilrigParts;			// People trapped on the roofs
		std::vector<qsf::Entity*>	mEmptyDivingSpots;		// Places where diver can go and search
		std::vector<qsf::Entity*>	mOccupiedDivingSpots;	// Places where diver can go and search
		qsf::Entity*				mCutter;				// Cutter. Needed as focus entity.
		//std::vector<qsf::Entity*>	mSunkenCars;			// Cars hidden in divingspots
		qsf::Entity*				mFocusEntity;
		std::vector<qsf::Entity*>	mBurnStart;				// Things that burn from the beginning

		std::vector<qsf::Entity*>	mRoof1People;			// People on roofs
		std::vector<qsf::Entity*>	mRoof2People;			// People on roofs
		std::vector<qsf::Entity*>	mRoof3People;			// People on roofs
		std::vector<qsf::Entity*>	mRoof2PeopleInjured;	// People on roofs
		std::vector<qsf::Entity*>	mRoof3PeopleInjured;	// People on roofs

		std::vector<uint64>			mFireEntitiesUnderWater;	// At the beginning, pick entities under water and in no burn layer and deactivate the fire component

		// Misc
		int							mMaximumDeaths;
		int							mSunkenCarsAmount;
		qsf::MessageProxy			mRetrieveSunkenCarsProxy;
		qsf::MessageProxy			mSunkenCarsProxy;
		qsf::MessageProxy			mSunkenCarRetrieveProxy;


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::MainEvent08)

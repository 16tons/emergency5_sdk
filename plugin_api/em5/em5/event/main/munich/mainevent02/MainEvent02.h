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
	*    EMERGENCY 5 main event 02 "Munich hang glider accident"
	*/
	class MainEvent02 : public MainEvent
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class MainEventMetaclasses;		// CAMP export of editable values for Game Design


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 FREEPLAY_EVENT_ID;		///< "em5::MainEvent02" unique freeplay event ID


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		MainEvent02();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~MainEvent02();


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
		virtual void onIntroStart(const qsf::MessageParameters& parameters) override;
		virtual void onIntroFinished(const qsf::MessageParameters& parameters) override;
		virtual void onPrologueFinished(const qsf::MessageParameters& parameters) override;

		//[-------------------------------------------------------]
		//[ Regular update                                        ]
		//[-------------------------------------------------------]
		virtual void onUpdateMainEvent(const qsf::Time& timePassed) override;
		virtual void onUpdateMusic(const qsf::Time& timePassed) override;

		virtual uint32 getEventNumber() override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void gatherEventEntities();

		void startObjectives();
		void startHintObservers();

		/**
		*  @brief
		*    Start the fire inside the cut scene
		*/
		void startFallCutscene();

		/**
		*  @brief
		*    Reset the player controls
		*/
		void onFallCutsceneFinished(const qsf::MessageParameters& parameters);

		/**
		*  @brief
		*    Squad Vehicle has started burning - mission fail.
		*/
		void onSquadRoadVehicleStartBurning();

		/**
		*  @brief
		*    Create the save the hang glider objective
		*/
		void createSaveHanggliderObjective(qsf::Entity& hangglider);

		/**
		*  @brief
		*    Called when hang glider has been saved
		*/
		void onHangGliderSaved();

		/**
		*  @brief
		*    Called when hang glider has been placed from the DLK
		*/
		void onHangGliderPlacedFromDlk();

		/**
		*  @brief
		*    Called whenever a building is extinguished (needed for Hint08)
		*/
		void onBuildingExtinguished(const qsf::MessageParameters& parameters);

		/**
		*  @brief
		*    Called when hang glider has been loaded into an ambulance
		*/
		void onHangGliderTransportedToHQ();

		void onGetsHeatEnergyPerson(const qsf::MessageParameters& parameters);

		/**
		*  @brief
		*    Count down timer for the hang glider fall
		*/
		void updateHangGliderFallingCountDown(const qsf::JobArguments& jobArguments);

		void startFire(const qsf::MessageParameters& parameters);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Connected to CAMP
		qsf::Time					mHangGliderFallTime; // 300 sec.
		float						mHangGliderLifeEnergy; // Life energy after being rescued.

		int							mMaxBurningBuildings; // 7

		qsf::Time					mHint01WaitTime;	// 20 sec.
		qsf::Time					mHint01WaitTime2;   // 30 sec.

		qsf::Time					mHint04WaitTime;	// 90 sec.
		qsf::Time					mHint04WaitTime2;	// 135 sec.

		int							mMaxBurningVegetations; // 12
		qsf::Time					mHint05WaitTime2;	// 45 sec.

		qsf::Time					mHint07WaitTime;	// 45 sec.
		qsf::Time					mHint07WaitTime2;	// 90 sec.

		qsf::Time					mHint02WaitTime;	// 90 sec.

		float						mHint10LifeEnergy;	// 15%

		qsf::Time					mHint11WaitTime;	// 120 sec.
		qsf::Time					mHint13WaitTime;	// 250 sec.
		float						mHint14LifeEnergy;	// 15%

		qsf::Time					mHint15WaitTime;	// 60 sec.

		qsf::Time					mHint18WaitTime;	// 220 sec.
		qsf::Time					mHint20WaitTime;	// 225 sec.

		qsf::Time					mHint27WaitTime;	// 20 sec.

		bool						mHint01Triggered;
		bool						mHint03Triggered;
		bool						mHint04Triggered;
		bool						mHint05Triggered;
		bool						mHint16Triggered;

		// Internal
		std::vector<qsf::Entity*>	mWorkers;			///< The entities of the workers
		qsf::Entity*				mHangGlider;		///< The entity of the hang-glider
		qsf::Entity*				mWindowPerson;		///< The entity of a person on balcony

		std::vector<qsf::Entity*>	mFires;				///< The entities of the fire places
		qsf::Entity*				mDLK;				///< DLK target

		qsf::MessageProxy			mBuildingExtinguishedProxy;

		qsf::JobProxy				mUpdateJobProxy;
		qsf::MessageProxy			mHangGliderSaveProxy;
		qsf::Time					mTimePassed;
		qsf::Time					mTimeToFall;

		qsf::MessageProxy			mHeatEnergyPersonProxy;

		bool						mHangGliderSaved;

		// Achievement
		bool						mHangGliderSavedPerfect;	///< This flag is true at the beginning and is set to false if the hint 11, 13 or 14 is triggered. Its used to unlock the achievement for perfect save of the glider.

		// Ending
		bool						mPlayingWinFadeOut;
		bool						mHasStartedFallCutscene;


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::MainEvent02)

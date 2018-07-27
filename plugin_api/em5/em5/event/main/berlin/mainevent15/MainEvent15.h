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
	*    EMERGENCY 5 main event 15 "Assassination"
	*/
	class MainEvent15 : public MainEvent
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class MainEventMetaclasses;	// CAMP export of editable values for Game Design


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 FREEPLAY_EVENT_ID;	///< "em5::MainEvent15" unique freeplay event ID


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		MainEvent15();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~MainEvent15();

		// Debug Function while the SEK Drone is not implemented
		void debugGangsterFound();


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
		virtual void hintCallbackWithMessage(Observer& hintObserver, const qsf::MessageParameters& parameters) override;

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

		void createAssassin();
		void findGangster();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Connected to CAMP
		int							mFail01Count;
		qsf::Time					mHint01WaitTime;
		float						mHint03LifeEnergy;
		float						mHint04LifeEnergy;
		float						mHint05LifeEnergy;
		float						mHint06LifeEnergy;
		qsf::Time					mHint08WaitTime;
		qsf::Time					mHint10WaitTime;
		qsf::Time					mAssassinEscapeTime;

		// Internal
		qsf::Entity*				mPresident;
		std::vector<qsf::Entity*>	mMIB;
		qsf::Entity*				mChancellor;
		qsf::Entity*				mChancellorBlock;
		qsf::Entity*				mCrashcar01;
		qsf::Entity*				mCrashcar02;
		qsf::Entity*				mCrashcar01Victim;
		qsf::Entity*				mCrashcar02Victim;
		std::vector<qsf::Entity*>   mStartFires;
		std::vector<qsf::Entity*>   mStartDestroyed;
		std::vector<qsf::Entity*>	mWindowPeople;
		std::vector<qsf::Entity*>	mGaper;
		std::vector<qsf::Entity*>	mHidingSpots;
		qsf::Entity*				mAssassinHidingSpot;
		qsf::WeakPtr<qsf::Entity>	mAssassin;
		std::vector<qsf::Entity*>	mBurnWarnings;
		std::vector<qsf::Entity*>	mBurnFails;

		bool						mFoundAssassin;
		bool						mCutCarOpen;

		qsf::JobProxy				mUpdateJobProxy;
		qsf::Time					mTimePassed;

		// Ending
		bool						mPlayingWinFadeOut;


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::MainEvent15)

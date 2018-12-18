// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/freeplay/event/main/MainEvent.h"

#include <qsf/base/WeakPtr.h>
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
	*    EMERGENCY 5 main event 07 "Gang War"
	*/
	class MainEvent07 : public MainEvent
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class MainEventMetaclasses;		// CAMP export of editable values for Game Design


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 FREEPLAY_EVENT_ID;		///< "em5::MainEvent07" unique freeplay event ID


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		MainEvent07();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~MainEvent07();


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
		virtual void hintCallbackWithMessage(Observer& hintObserver, const qsf::MessageParameters& parameters);

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
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		struct GangsterConfiguration
		{
			qsf::WeakPtr<qsf::Entity> mGangsterEntity;
			qsf::Time mTimeTillShot;

			enum Gang
			{
				GANG_1,
				GANG_2
			} mGang;

			enum State
			{
				SPAWNED,				// Spawned, not yet kneeling down
				MANUAL_SHOOTING_ANIMATIONS,	// Hiding behind car and shooting other faction (hard coded animations)
				FREE_GANGSTER,			// Gangster found target and follow it. No way back
				DISABLED				// Gangster has been injured or arrested
			} mState;

			GangsterConfiguration(qsf::Entity& gangsterEntity, Gang gang);
		};


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void gatherEventEntities();

		void startObjectives();
		void startHintObservers();

		void updateGangsters(const qsf::Time& timePassed);
		void updateGangsterAnimation(GangsterConfiguration& gangster, const qsf::Time& timePassed);
		qsf::Entity* findBestTarget(GangsterConfiguration& gangster) const;
		bool targetIsBeingAttacked(qsf::Entity& targetEntity) const;
		int getTargetPriority(qsf::Entity& targetEntity) const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Connected to CAMP
		float mPlayerAttackDistance;
		float mHint01LifeEnergy;
		int mHint03Count;
		int mHint04Count;
		qsf::Time mMinShootTime;
		qsf::Time mMaxShootTime;
		float mShotInjuryProbability;
		qsf::Time mHint13WaitTime;
		qsf::Time mHint13WaitTime2;
		qsf::Time mHint14WaitTime;
		qsf::Time mHint14WaitTime2;
		qsf::Time mHint12WaitTime;
		qsf::Time mHint16WaitTime;
		int mHint17Count;
		float mHint20LifeEnergy;
		int mHint21FireCount;
		qsf::Time mHint27WaitTime;
		int mFail02Count;
		int mFail04Count;
		int mFail05Count;
		int mFail06Count;

		// Achievement
		bool mAllGangstersArrested;

		// Internal
		std::vector<GangsterConfiguration>   mGangsters;
		boost::container::flat_map<qsf::Entity*, Observer*> mArrestObservers;

		std::vector<qsf::Entity*>   mInjuredGangsters1;
		std::vector<qsf::Entity*>   mInjuredGangsters2;
		std::vector<qsf::Entity*>   mFires;
		boost::container::flat_set<qsf::Entity*>	mUniqueBurningBuildings;
		qsf::Entity*				mGangsterVehicle;
		qsf::Entity*				mGang1Car;
		qsf::Entity*				mGang2Car;
		qsf::JobProxy				mUpdateJobProxy;
		qsf::Time					mTimePassed;
		bool						mHint01Played;

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
QSF_CAMP_TYPE_NONCOPYABLE(em5::MainEvent07)

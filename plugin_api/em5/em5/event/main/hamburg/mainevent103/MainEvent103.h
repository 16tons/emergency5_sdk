// Copyright (C) 2012-2018 Promotion Software GmbH


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
	*    EMERGENCY 5 main event 103 "Hamburg"
	*/
	class MainEvent103 : public MainEvent
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class MainEventMetaclasses;		// CAMP export of editable values for Game Design


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 FREEPLAY_EVENT_ID;		///< "em5::MainEvent103" unique freeplay event ID


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		MainEvent103();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~MainEvent103();


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
		virtual void onUpdateMainEvent(const qsf::Time& timePassed) override;
		virtual void onPrologueFinished(const qsf::MessageParameters& parameters) override;
		virtual void onUpdateMusic(const qsf::Time& timePassed) override;
		virtual uint32 getEventNumber() override;
		virtual std::string getCamStartEventTag() override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void spawnVehicleWave();
		void spawnVehicle(int spawnPointId);
		void enableSpawningFromeBrawlerWave4();

		void gatherEventEntities();
		void removeGangsterFromList(uint64 targetEntity);

		void startObjectives();
		void startHintObservers();

		void spawnBrawl(int id);
		void onStartArrestGangsterMessage(const qsf::MessageParameters& parameters);
		void onStartBurningBuildingMessage(const qsf::MessageParameters& parameters);

		void triggerHint01();

		void onStopBurningBuildingMessage(const qsf::MessageParameters& parameters);
		void onStartBurningObject(const qsf::MessageParameters& parameters);
		void onStopBurningObject(const qsf::MessageParameters& parameters);
		bool isBarrierSafe(const glm::vec3& center, float radius);
		void checkBarrierSafeness(qsf::Entity& barrier1, bool& safeFlag);
		void onObjectiveCompleted(const qsf::MessageParameters& messageParameters);


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		enum NPCVehicleState
		{
			DRIVING_TO_TARGET,
			DRIVING_TO_DESPAWNPOINT
		};

		struct NPCVehicle
		{
			int mSpawnPointId;
			uint64 mVehicleId;
			uint64 mTargetId;
			float lastDistance;
			qsf::Time mCount;
			NPCVehicleState mState;
		};


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		std::vector<uint64> mGangsterIds;
		std::vector<uint64> mGangsterIdsBrawl1;
		std::vector<uint64> mGangsterIdsBrawl2;
		std::vector<uint64> mGangsterIdsBrawl3;
		std::vector<uint64> mGangsterIdsBrawl4;

		int mBarrierCount;
		float mNPCWaitingtime;
		int mIndexNPCVehicleSpawnTimeBarrier1;
		int mIndexNPCVehicleSpawnTimeBarrier2;

		qsf::Time mEventTime;
		std::vector<NPCVehicle> mNPCVehicles;

		bool mTriggeredBrawl1;
		bool mTriggeredBrawl2;
		bool mTriggeredBrawl3;
		bool mTriggeredBrawl4;
		bool mCanTriggerBrawl4;

		int mFreeGangsterCount;
		qsf::MessageProxy mGangsterMessageProxy;
		qsf::MessageProxy mFireBuildingStartProxy;
		qsf::MessageProxy mFireBuildingStopProxy;
		qsf::MessageProxy mFireObjectStartProxy;
		qsf::MessageProxy mFireObjectStopProxy;
		qsf::MessageProxy mObjectiveCompletedProxy;
		qsf::Time mActualHint01CoolDown;
		int mBurningObjects;
		int mBurningBuildings;
		bool mTriggerdHint03Brawl1;
		bool mTriggerdHint03Brawl2;
		bool mTriggerdHint03Brawl3;
		bool mTriggerdHint03Brawl4;

		bool mTriggerdHint06Brawl1;
		bool mTriggerdHint06Brawl2;
		bool mTriggerdHint06Brawl3;
		bool mTriggerdHint06Brawl4;
		bool mTriggeredHint02;
		bool mTriggerdHint07;
		bool mTriggerdUnitHint01;
		bool mTriggerdSupervisorMessageBurningBuilding;

		std::vector<qsf::WeakPtr<qsf::Entity>> mBarriers;
		std::vector<bool> mIsBarrierSafe;

		qsf::Time mHint03CoolDown;

		// Connected to CAMP
		int mAiPoliceCars;

		std::vector<qsf::Time> mNPCVehcileSpawnTimeBarrier1;
		std::vector<qsf::Time> mNPCVehcileSpawnTimeBarrier2;

		qsf::Time mBrawl1StartTime;
		qsf::Time mBrawl2StartTime;
		qsf::Time mBrawl3StartTime;
		qsf::Time mBrawl4StartTime;

		qsf::Time mHint01CoolDownTime;
		int mHint01MaxBurningObjects;

		qsf::Time mHint03WaitTime;
		qsf::Time mHint06WaitTime;

		int mHint07OfficeCount;
		qsf::Time mHint07WaitTime;
		qsf::Time mHint09WaitTime;
		qsf::Time mHint10WaitTime;
		int mHint10GangsterCount;

		float mBrawlerUnarmedAggroRange;
		float mBrawlerUnarmedDeAggroRange;
		qsf::Time mBrawlerUnarmedCheerTime;
		qsf::Time mArsonistCheerTime;


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::MainEvent103)

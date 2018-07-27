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
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace em5
{
	class DoorComponent;
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
	*    EMERGENCY 5 main event 15 "Assassination"
	*/
	class MainEvent104 : public MainEvent
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class MainEventMetaclasses;	// CAMP export of editable values for Game Design


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		typedef qsf::WeakPtr<qsf::Entity> WeakEntityPtr;
		typedef std::vector<WeakEntityPtr> WeakPtrEntityList;

		struct TrapBomb
		{
			// Is the bomb active?
			bool mIsHot;

			// Was the bomb discovered?
			bool mIsDiscovered;

			// Bomb entity ref
			WeakEntityPtr mBomb;

			// Trap trigger entity ref
			WeakEntityPtr mTrigger;

			// Visual representation of the trigger zone entity ref
			WeakEntityPtr mMarker;

			TrapBomb() :
				mIsHot(false),
				mIsDiscovered(false)
			{}
		};

		struct ConvoyTruck
		{
			// Did the truck enter the quarantine zone?
			bool mReachedTrigger;

			// Did the truck parked on the target spot in the quarantine zone?
			bool mReachedTarget;

			// Truck entity ref
			WeakEntityPtr mTruck;

			// Truck parking spot entity ref
			WeakEntityPtr mParkingTarget;
		};

		typedef std::vector<ConvoyTruck> ConvoyTruckList;

		enum TerroristZoneState
		{
			TerroristZoneState_Blocked,
			TerroristZoneState_SemiBlocked,
			TerroristZoneState_Open
		};


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 FREEPLAY_EVENT_ID;	///< "em5::MainEvent104" unique freeplay event ID


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		MainEvent104();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~MainEvent104();


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

		virtual uint32 getEventNumber() override;
		virtual std::string getCamStartEventTag() override;


	//[-------------------------------------------------------]
	//[ Private static methods                                ]
	//[-------------------------------------------------------]
	private:
		static bool isInsideBox(const qsf::TransformComponent& boxTransform, const glm::vec3& position);


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void gatherEventEntities();

		void startObjectives();
		void startHintObservers();

		void onTerrorbaseFound(const qsf::MessageParameters& messageParameters);

		void handleTerrorbaseFound();

		void openGate(DoorComponent& door);
		void closeGate(DoorComponent& door);

		void onUnitMovementBlockedRequest(const qsf::MessageParameters& messageParameters);
		void onSEKDroneInvalidTargetRequest(const qsf::MessageParameters& messageParameters);

		bool isEntityTerroristBaseTerrorist(const uint64 potentialTargetEntityId);

		bool isEntityInList(const WeakPtrEntityList& entities, const uint64 entityId) const;

		void onBombDefused(const qsf::MessageParameters& messageParameters);

		void handleBombDefused(const uint64 bombEntityId);

		void onDoorEboxRepaired(const qsf::MessageParameters& messageParameters);
		void onAnythingRepaired(const qsf::MessageParameters& messageParameters);

		void onTruckRepaired(WeakEntityPtr &truck);

		void truckEntersCheeringZone();
		void truckLeavesCheeringZone();
		void checkCivilsCheeringCondition();

		void onObjectiveCompleted(const qsf::MessageParameters& messageParameters);

		void onObjectiveNeedTransportInjuredCompleted();

		void onAllInjuredPersonsAtBrandenburgerTorTransported();

		void onTerrorBaseRaidCompleted();

		void unlockYardGate();

		void setupTerrorist(const WeakEntityPtr& terrorist);

		void discoverSecondTrap();

		void setTrapVisibility(TrapBomb& trap, bool visible);
		bool isTrapTriggerActivated(TrapBomb& trap);

		bool isAnySquadUnitInsideBox(const qsf::TransformComponent &triggerBoxTransform, bool excludeDefuseRobot = false, bool excludeVehicles = false) const;

		void activateTrapBomb(TrapBomb& trap);

		void onTrapBombDiscovered(TrapBomb &trap);

		void initializeTrap(TrapBomb& trap);
		void defuseTrap(TrapBomb& trap);

		void setupGangstersHiddenInTerrorBaseBuilding();

		void debugDefuseTrap01();
		void debugDefuseTrap02();
		void debugRepairAllTrucks();

		void onGunShot(const qsf::MessageParameters& messageParameters);
		void startTerroristBackup();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Connected to CAMP
		qsf::Time mME103Hint02WaitTime;
		qsf::Time mGE01Hint02WaitTime;
		qsf::Time mME104Hint02WaitTime;
		qsf::Time mME103Hint02CooldownTime;
		uint32 mME104Hint02TerroristCount;
		float mTerroristAggroRange;
		float mTerroristDeAggroRange;
		float mTerroristChainPullRange;
		float mTerroristFirearmAttackRange;
		int mMaximumInjuredSquadMembers;
		int mMaximumDeadPersons;

		// Internal
		WeakPtrEntityList mInjured;
		WeakPtrEntityList mCars;
		WeakPtrEntityList mCarTrappedPersons;
		WeakPtrEntityList mTerrorists;
		WeakPtrEntityList mTerroristsOutside;
		WeakPtrEntityList mTerroristBackup;
		WeakPtrEntityList mCivilsCheering;
		WeakPtrEntityList mGateDoors;

		WeakEntityPtr mTerrorDronetarget;
		WeakEntityPtr mBlocking;
		WeakEntityPtr mTerroristZone;
		WeakEntityPtr mTerroristTrigger;
		WeakEntityPtr mEbox;
		WeakEntityPtr mTrap02TerrorbaseTrigger;
		WeakEntityPtr mQuarantineMarker;
		WeakEntityPtr mQuarantineTrigger;
		WeakEntityPtr mQuarantineBlocking;
		WeakEntityPtr mTerrorbase;

		TrapBomb mTrap01;
		TrapBomb mTrap02;

		ConvoyTruckList mTrucks;

		int mNumTrucksRepaired;
		bool mSecondTrapDiscovered;
		bool mTerroristBackupArrived;
		bool mTransportedAllInitiallyInjuredPersons;

		qsf::Time mME103Hint02Cooldown;

		// How many trucks are currently in the cheering zone, i.e. the civils should play the cheering animation while this is >0
		int mNumberOfConvoyTrucksInCheeringZone;
		TerroristZoneState mTerroristZoneState;

		qsf::MessageProxy mTerrorbaseFoundProxy;
		qsf::MessageProxy mUnitMovementBlockedRequestProxy;
		qsf::MessageProxy mSEKDroneInvalidTargetRequestProxy;
		qsf::MessageProxy mBombDefusedProxy;
		qsf::MessageProxy mDoorEboxRepairedProxy;
		qsf::MessageProxy mObjectiveCompletedProxy;
		qsf::MessageProxy mAnythingRepairedProxy;
		qsf::MessageProxy mGunShotProxy;

		bool mTriggeredGE01_Hint02;
		bool mTriggeredME103_Hint04;
		bool mTriggeredME104_Hint02;
		bool mTriggeredME104_Hint06;


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::MainEvent104)

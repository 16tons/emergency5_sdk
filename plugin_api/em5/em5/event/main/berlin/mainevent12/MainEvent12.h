// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/component/building/BuildingComponent.h"
#include "em5/freeplay/event/main/MainEvent.h"

#include <qsf/base/WeakPtr.h>
#include <qsf/job/JobProxy.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace em5
{
	class PathMovementComponent;
	class ObjectiveHelper;
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
	*    EMERGENCY 5 main event 12 "Wannsee-Reaktor"
	*/
	class MainEvent12 : public MainEvent
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class MainEventMetaclasses;		// CAMP export of editable values for Game Design


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 FREEPLAY_EVENT_ID;		///< "em5::MainEvent12" unique freeplay event ID


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		MainEvent12();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~MainEvent12();


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
		virtual void showSupervisorMessageForFinalEvaluation(uint32 pointGain) override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void gatherEventEntities();

		void onGasPipeRepaired(const qsf::MessageParameters& parameters);
		void onScientistsRescuedFromReactorBuilding(const qsf::MessageParameters& parameters);
		void onScientistsNotRescuedFromReactorBuilding(const qsf::MessageParameters& parameters);
		void onRescuedScientistStoppedMoving(const qsf::MessageParameters& parameters);
		void onCloudReachesEndOfPath(const qsf::MessageParameters& parameters);

		void onTruckDriverDiedInsideVehicle(const qsf::MessageParameters& parameters);
		void onTruckDriverRescuedFromVehicle(const qsf::MessageParameters& parameters);

		void onReactorStoppedBurning(const qsf::MessageParameters& parameters);
		void onReactorGetsExtinguished(const qsf::MessageParameters& parameters);
		void onDecontaminationVehicleOrdered(const qsf::MessageParameters& parameters);

		void checkObjectivesForSubervisorMessages(const qsf::MessageParameters& parameters);

		void startContaminationCloudMove();
		void startContaminationCloud();
		void stopContaminationCloud();
		void startObjectives();
		void startHintObservers();

		void checkForFailedConditionOfInjuredSquad(qsf::Entity& injuredSquadEntity, const ObjectiveHelper& objectiveHelper);

		bool checkIfAnyFireFighterAsExitVehicle();
		void handleScientists();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		qsf::Entity*							mGasPipeEboxEntity;
		qsf::Entity*							mGasFlameLogicEntity;
		qsf::Entity*							mGasFlameGfxEntity;
		qsf::Entity*							mTruckEntity;
		qsf::Entity*							mTruckDriverEntity;
		qsf::Entity*							mReactorEntity;
		qsf::Entity*							mCloudEntity;
		qsf::Entity*							mCloudPathEntity;
		qsf::Entity*							mGaperAreaEntity;
		std::vector<qsf::Entity*>				mContaminatedPersons;
		std::vector<qsf::Entity*>				mInitialBurningEntities;
		std::vector<qsf::Entity*>				mTrappedScientists;
		std::vector<qsf::WeakPtr<qsf::Entity>>	mGaperEntities;
		std::vector<qsf::Entity*>				mFailEntities;		///< List of entities which causes a fail when they start burn
		std::vector<qsf::Entity*>				mHint13Entities;	///< List of entities which triggers HINT13 when they start to burn
		qsf::WeakPtr<BuildingComponent>			mReactorBuildingComponent;
		qsf::WeakPtr<PathMovementComponent>		mCloudPathMovementComponent;
		qsf::WeakPtr<qsf::TransformComponent>	mScientistTargetPosition;
		glm::vec3								mScientistTargetPositionOffset;
		uint32									mLetCloudMoveTimerId;
		uint32									mDisableCloudTimerId;

		qsf::MessageProxy						mGasPipeRepairedMessageProxy;
		qsf::MessageProxy						mRescuedFromBuildingMessageProxy;
		qsf::MessageProxy						mTrappedPersonsNotRescuedFromBuildingMessageProxy;
		qsf::MessageProxy						mScientistStoppedMovingMessageProxy;
		qsf::MessageProxy						mCloudHasReachedPathEndMessageProxy;
		qsf::MessageProxy						mTruckDriverDiedInsideVehicleMessageProxy;
		qsf::MessageProxy						mTruckDriverRescuedFromVehicleMessageProxy;
		qsf::MessageProxy						mCloudMoveDelayMessageProxy;
		qsf::MessageProxy						mReactorFireExtinguishedMessageProxy;
		qsf::MessageProxy						mReactorStartedFireExtinguishingMessageProxy;
		qsf::MessageProxy						mObjectiveChangedMessageProxy;					///< Message proxy to handle changes in objectives by this event(Needed for the supervisor objective comments)
		qsf::MessageProxy						mDecontaminationVehicleOrderedMessageProxy;

		// Connected to CAMP
		float									mGasPipeRepairTime;
		float									mInitialBurningRadiusForTruck;
		float									mInitialBurningStartEnergyPercentage;		///< Start energy percentage value for the entities which gets inflamed by the truck
		qsf::Time								mTrappedScientistsRescueTime;
		float									mTrappedScientistsRemainingLifeEnergyAfterRescue;
		qsf::Time								mContaminationCloudStartDelay;
		qsf::Time								mContaminationCloudMoveTime;
		uint32									mMaxDeadCivilians;
		uint32									mMaxDeadUnitPerson;
		uint32									mMaxContaminatedPersons;
		uint32									mMaxContaminatedPersonsForAchievement;
		float									mHint01LifeEnergy;
		float									mHint02LifeEnergy;
		qsf::Time								mHint03WaitTime;
		qsf::Time								mHint04WaitTime;
		qsf::Time								mHint05WaitTime;
		float									mHint07DamageEnergy;
		float									mHint08LifeEnergy;
		float									mHint12DamageEnergy;
		qsf::Time								mHint14WaitTime;
		float									mHint14BLifeEnergy;
		qsf::Time								mHint19WaitTime;

		// Internal
		bool									mHint01Triggered;
		bool									mHint02Triggered;
		bool									mHint04Triggered;
		bool									mHint07Triggered;
		bool									mHint08Triggered;
		bool									mHint09Triggered;
		bool									mHint12Triggered;
		bool									mHint13Triggered;
		bool									mHint14Triggered;
		bool									mHint15Triggered;
		bool									mHint16Triggered;
		bool									mHint17Triggered;

		bool									mDecontaminationVehicleOrdered;
		bool									mContaminationCloudVanished;

		bool									mObjective02Succeeded;
		bool									mObjective05Succeeded;
		uint8									mUntrappedScientistsCount;
		bool									mCanScientistsAddedToReactor;

		boost::container::flat_set<uint32>		mAlreadyPlayedObjectiveSupervisorMessages;	///< Contains a list of objective supervisor messages, which were already played


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::MainEvent12)

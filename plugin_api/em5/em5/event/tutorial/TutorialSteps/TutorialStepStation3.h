// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/event/tutorial/TutorialStep.h"

#include <qsf/job/JobProxy.h>
#include <qsf/math/Transform.h>
#include <qsf/message/MessageProxy.h>
#include <qsf/reflection/type/CampQsfTime.h>


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
	*    EMERGENCY 5 tutorial step station 3
	*/
	class TutorialStepStation3 : public TutorialStep
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		TutorialStepStation3(Tutorial& tutorial);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~TutorialStepStation3();


	//[-------------------------------------------------------]
	//[ Public virtual em5::TutorialStep methods              ]
	//[-------------------------------------------------------]
	public:
		virtual void startup() override;
		virtual bool update(const qsf::Time& timePassed) override;
		virtual void shutdown() override;
		virtual const std::string getLayerId() const override;
		virtual const std::string getEraseLayerId() const override;
		virtual const qsf::Entity* getFocusEntity() override;


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		enum class STATE
		{
			STARTUP,
			WAIT_FOR_GANGSTER_REACHES_END_POSITION,
			SUPERVISOR33_START,
			WAIT_FOR_SUPERVISOR33_END,
			WAIT_FOR_GOTO_EVENT_BTN_PRESSED,
			SUPERVISOR34_START,
			WAIT_FOR_SUPERVISOR34_END,
			SUPERVISOR35_START,
			WAIT_FOR_SUPERVISOR35_END,
			ORDER_STW_WAIT_FOR_VEHICLE_LIST_WINDOW_OPENS,
			ORDER_STW_WAIT_FOR_VEHICLE_ORDER_WINDOW_OPENS,
			ORDER_STW_WAIT_FOR_SENDBUTTON_CLICKED,
			ORDER_STW_WAIT_FOR_GET_SEND,
			WAIT_STW_REACHES_TARGET_POSITION,
			WAIT_STW_SELECTED,
			WAIT_FOR_POLICEMEN_LEAVES_VEHICLE,
			GANGSTER_ATTACKS_POLICEMAN_INIT,
			GANGSTER_ATTACKS_POLICEMAN_START,
			WAIT_FOR_FIGHT_STARTED,
			SUPERVISOR36_START,
			WAIT_FOR_SUPERVISOR36_END,
			WAIT_FOR_POLICEMAN_EQUIPPED_WITH_PEPPERSPRAY,
			SUPERVISOR37_START,
			WAIT_FOR_SUPERVISOR37_END,
			WAIT_FOR_USE_PEPPERSPRAY_EXECUTED,
			WAIT_FOR_STUNNING_THE_GANGSTER,
			WAIT_FOR_HANDCUFFING_THE_GANGSTER,
			END,
		};


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void onStwGetsSpawned(const qsf::MessageParameters& parameters);
		void onLetPersonExitStwVehicle(const qsf::MessageParameters& parameters);
		void onFightStarted(const qsf::MessageParameters& parameters);
		void onGetPepperSprayCommandExecuted(const qsf::MessageParameters& parameters);
		void onUsePepperSprayCommandExecuted(const qsf::MessageParameters& parameters);
		void onGangsterGotStunned(const qsf::MessageParameters& parameters);
		void onGetHandcuffsCommandExecuted(const qsf::MessageParameters& parameters);
		void onArrestGangsterCommandExecuted(const qsf::MessageParameters& parameters);
		void onSelectionChanged(const qsf::MessageParameters& parameters);

		void gatherAllEventEntities();
		void setupGangsterForLeavingTheSupermarket();
		void setupPersonEntityExitBuildingAtDoor(qsf::Entity& personEntity, const qsf::Entity& buildingEntity);
		void setupPersonEntityToLeaveTheSupermarket(qsf::Entity& personEntity, const qsf::Transform& targetTransform, float waitTimeOffsetInSeconds);
		void equipGangsterWithCase();

		void ShowTutorialObjectives19();
		void ShowTutorialObjectives20();
		void ShowTutorialObjectives21();
		void ShowTutorialObjectives22();

		bool isPoliceVehicleListHudWindowOpen(const qsf::Time& timePassed) const;
		bool isStwVehicleOrderHudWindowOpen(const qsf::Time& timePassed) const;

		void showStwTargetEntityOnMapAndOnMinimap(bool show);

		void checkForHint22(const qsf::Time& timePassed);

		void letGangsterAttackNearestPolicemen();
		void attackTarget(qsf::Entity& gangsterEntity, qsf::Entity& victimEntity);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		STATE				mCurrentState;
		bool				mGotoEventBtnPressed;
		qsf::Entity*		mGangsterPerson;
		qsf::Entity*		mGangsterTargetPositionEntity;
		qsf::Entity*		mCaseEntity;
		qsf::Transform		mGansterFinalTransform;
		qsf::Entity*		mMaleWaverPerson;
		qsf::Transform		mMaleWaverPersonTargetTransform;			///< The target transform which the person should have on end
		qsf::Entity*		mFemaleWaverPerson;
		qsf::Transform		mFemaleWaverPersonTargetTransform;			///< The target transform which the person should have on end
		qsf::Entity*		mStwOrderAreaEntity;
		qsf::Time			mRemainingWaitTimeBeforeSupervisor34Start;
		bool				mCheckForHint22;							///< Inidicates if the check for hint 17 should be done
		qsf::Time			mRemainingWaitTimeForHint22;
		qsf::Entity*		mUnitEntity;
		qsf::Entity*		mFightingPoliceman;
		qsf::Time			mRemainingWaitTimeForHint23;
		bool				mHint23Shown;
		qsf::Time			mRemainingWaitTimeForHint25;
		bool				mHint25Shown;
		qsf::Time			mRemainingWaitTimeForHint27;
		bool				mHint27Shown;
		int					mHudActionButtonIdEnable;
		int					mHudActionButtonIdBlink;
		bool				mCurrentSelectedIsPolicePerson;
		qsf::Transform		mCameraTransformOnGotToEventClick;

		qsf::MessageProxy	mMessageProxy;
		qsf::MessageProxy	mSelectionChangeMessageProxy;

		bool				mSkippedOneFrame;	// Dummy variable to indicate if the have skipped one frame


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

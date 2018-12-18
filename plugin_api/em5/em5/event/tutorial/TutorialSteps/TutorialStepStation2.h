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
#include <qsf/message/MessageProxy.h>
#include <qsf/math/Transform.h>
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
	*    EMERGENCY 5 tutorial step station 2
	*
	*/
	class TutorialStepStation2 : public TutorialStep
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		TutorialStepStation2(Tutorial& tutorial);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~TutorialStepStation2();


	//[-------------------------------------------------------]
	//[ Public virtual em5::TutorialStep methods              ]
	//[-------------------------------------------------------]
	public:
		virtual void startup() override;
		virtual bool update(const qsf::Time& timePassed) override;
		virtual void shutdown() override;
		virtual const std::string getLayerId() const override;
		virtual const std::string getEraseLayerId() const override;


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		enum class STATE
		{
			STARTUP,
			WAIT_FOR_ALL_FIREFIGHTERS_ENTERED_THE_VEHICLE,
			WAIT_SOME_TIME_FOR_STARTING_FIRST_SUPERVISOR_MESSAGE, // State to wait ~2 seconds before showing first supervisor message in this station
			INTRODUCE_MINIMAP_WAIT_FOR_SUPERVISOR20_END,
			INTRODUCE_MINIMAP_WAIT_FOR_SUPERVISOR21_END,
			WAIT_FOR_USER_CLICKS_IN_MINIMAP_ONTO_HIGHLIGHTED_AREA,
			INTRODUCTION_SCROLL_STARTUP,
			INTRODUCTION_SCROLL_WAIT_FOR_SUPERVISOR22_END,
			WAIT_FOR_USER_MOVES_THE_CAMERA,
			MOVE_CAMERA_TO_RW_STARTUP,
			WAIT_FOR_CAMERA_REACHED_RW,
			SUPERVISOR23_START,
			WAIT_FOR_SUPERVISOR23_END,
			WAIT_FOR_OBJECTIVES10_ACCOMPLISHED,
			WAIT_RW_REACHES_TARGET_POSITION,
			SUPERVISOR24_START,
			WAIT_FOR_SUPERVISOR24_END,
			WAIT_FOR_RESCUECUTTER_EQUIPPED,
			SUPERVISOR25_START,
			WAIT_FOR_SUPERVISOR25_END,
			WAIT_FOR_RESCUE_COMMAND_EXECUTED,
			WAIT_FOR_INJURED_PERSON_LEFT_THE_VEHICLE,
			SUPERVISOR26_START,
			WAIT_FOR_SUPERVISOR26_END,
			ORDER_NEF_WAIT_FOR_VEHICLE_LIST_WINDOW_OPENS,
			ORDER_NEF_WAIT_FOR_VEHICLE_ORDER_WINDOW_OPENS,
			ORDER_NEF_WAIT_FOR_SENDBUTTON_CLICKED,
			ORDER_NEF_WAIT_FOR_GET_SEND,
			WAIT_NEF_REACHES_TARGET_POSITION,
			WAIT_FOR_NEF_SELECTED,
			WAIT_FOR_DOCTOR_LEAVES_VEHICLE,
			SUPERVISOR27_START,
			WAIT_FOR_SUPERVISOR27_END,
			WAIT_FOR_DOCTOR_SELECTED,
			WAIT_FOR_PERSON_GETS_TREATED,
			WAIT_FOR_PERSON_HAS_BEEN_TREATED,
			SUPERVISOR29_START,
			WAIT_FOR_SUPERVISOR29_END,
			ORDER_RTW_WAIT_FOR_VEHICLE_LIST_WINDOW_OPENS,
			ORDER_RTW_WAIT_FOR_VEHICLE_ORDER_WINDOW_OPENS,
			ORDER_RTW_WAIT_FOR_SENDBUTTON_CLICKED,
			ORDER_RTW_WAIT_FOR_GET_SEND,
			WAIT_RTW_REACHES_TARGET_POSITION,
			WAIT_FOR_RTW_SELECTED,
			WAIT_FOR_PARAMETICS_LEAVES_VEHICLE,
			SUPERVISOR30_START,
			WAIT_FOR_SUPERVISOR30_END,
			WAIT_FOR_PARAMEDICS_SELECTED,
			WAIT_FOR_PARAMEDICS_PICKSUP_PERSON,
			WAIT_FOR_RTW_STARTS_GOING_TO_HQ,
			SUPERVISOR31_START,
			WAIT_FOR_SUPERVISOR31_END,
			ORDER_ASF_WAIT_FOR_VEHICLE_LIST_WINDOW_OPENS,
			ORDER_ASF_WAIT_FOR_VEHICLE_ORDER_WINDOW_OPENS,
			ORDER_ASF_WAIT_FOR_SENDBUTTON_CLICKED,
			ORDER_ASF_WAIT_FOR_GET_SEND,
			WAIT_ASF_REACHES_TARGET_POSITION,
			WAIT_FOR_ASF_SELECTED,
			WAIT_FOR_ASF_STARTS_PICKUP_VEHICLE,
			WAIT_FOR_ASF_HAS_PICKED_UP_VEHICLE,
			SUPERVISOR32_START,
			WAIT_FOR_SUPERVISOR32_END,
			WAIT_FOR_OBJECTIVES18_ACCOMPLISHED,
			END,
		};


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void onMinimapClicked(const qsf::MessageParameters& parameters);
		void onSelectionChangedForObjective10a(const qsf::MessageParameters& parameters);
		void onCheckInRwTargetCommandSend(const qsf::MessageParameters& parameters);
		void onLetPersonExitRwVehicle(const qsf::MessageParameters& parameters);
		void onFirefighterSelectedCheck(const qsf::MessageParameters& parameters);
		void onGetRescuecutterCommandExecuted(const qsf::MessageParameters& parameters);
		void onCutRoadvehicleCommandExecuted(const qsf::MessageParameters& parameters);
		void onPersonRescuredFromVehicle(const qsf::MessageParameters& parameters);
		void onNefGetsSpawned(const qsf::MessageParameters& parameters);
		void onLetPersonExitNefVehicle(const qsf::MessageParameters& parameters);
		void onTreatPersonCommandExecuted(const qsf::MessageParameters& parameters);
		void onPersonIsTransportable(const qsf::MessageParameters& parameters);
		void onRtwGetsSpawned(const qsf::MessageParameters& parameters);
		void onLetPersonExitRtwVehicle(const qsf::MessageParameters& parameters);
		void onPersonGetsPickedUp(const qsf::MessageParameters& parameters);
		void onAsfGetsSpawned(const qsf::MessageParameters& parameters);
		void onCarGetsPickedUp(const qsf::MessageParameters& parameters);
		void onCarGotPickedUp(const qsf::MessageParameters& parameters);
		void onEnterVehicleCommandExecuted(const qsf::MessageParameters& parameters);
		void onReturnToBaseCommandExecuted(const qsf::MessageParameters& parameters);

		void gatherAllEventEntities();

		bool hasAllFirefightersEnteredTheVehicle(bool pushEnterWhenActionEmpty = false);
		bool hasDoctorEnteredTheVehicle();

		void setEventNameForObjectiv8a();
		void ShowTutorialObjective8a();
		void ShowTutorialObjectives9();
		void ShowTutorialObjectives10();
		void ShowTutorialObjectives11();
		void ShowTutorialObjectives12();
		void ShowTutorialObjectives13();
		void ShowTutorialObjectives14();
		void ShowTutorialObjectives15();
		void ShowTutorialObjectives16();
		void ShowTutorialObjectives17();
		void ShowTutorialObjectives18();

		void setupChecksForObjectives10();
		void setupChecksForObjectives11();
		void setupChecksForObjectives12();

		void showRwTargetEntityOnMapAndOnMinimap(bool show);
		void showNefTargetEntityOnMapAndOnMinimap(bool show);
		void showRtwTargetEntityOnMapAndOnMinimap(bool show);
		void showAsfTargetEntityOnMapAndOnMinimap(bool show);

		void setupMinimapClickRestriction();
		void showHighlightAreaOnMinimap(bool show);
		void activateEntity(qsf::Entity* entity, bool active);

		void enableCameraScrolling(bool enable);
		// We only need xz position the hight doesn't matter
		const glm::vec2 getCameraPosition();

		bool hasPlayerScrolled(const qsf::Time& timePassed);

		void flyCameraToOrderedRwEntity();

		bool isAmbulanceVehicleListHudWindowOpen(const qsf::Time& timePassed);
		bool isNefVehicleOrderHudWindowOpen(const qsf::Time& timePassed);
		bool isRtwVehicleOrderHudWindowOpen(const qsf::Time& timePassed);
		bool isTecVehicleListHudWindowOpen(const qsf::Time& timePassed);
		bool isAsfVehicleOrderHudWindowOpen(const qsf::Time& timePassed);

		void onWaitForObjectives18Completed(const qsf::Time& timePassed);

		void checkForHint16(const qsf::Time& timePassed);
		void checkForHint17(const qsf::Time& timePassed);
		void checkForHint18(const qsf::Time& timePassed);

		void setupCameraToShowAccident();
		void letUnitMoveToTargetPosition(qsf::Entity& unitEntity, const glm::vec3& targetPosition);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		STATE				mCurrentState;
		qsf::Entity*		mMinimapClickRestrictionEntity;			///< The entity with a polygon component which is used to restrict the click area in the minimap
		qsf::Entity*		mCrashedCarEntity;						///< The entity of the crashed car
		qsf::Entity*		mInjuredDriverEntity;					///< The entity of the driver
		qsf::Entity*		mRwOrderTargetEntity;					///< The entity which describes the area where the RW can only be ordered
		qsf::Entity*		mNefOrderTargetEntity;					///< The entity which describes the area where the NEF can only be ordered
		qsf::Entity*		mRtwOrderTargetEntity;					///< The entity which describes the area where the RTW can only be ordered
		qsf::Entity*		mAsfOrderTargetEntity;					///< The entity which describes the area where the ASF can only be ordered
		bool				mHint11Triggered;
		glm::vec2			mScrollStartPosition;					///< The camera position in 2d coordinates (we only need the xz changes for the scroll objectives)
		qsf::Time			mRemainingTimeScrolledSlow;
		qsf::Time			mRemainingTimeScrolledFast;
		bool				mCheckForHint12;						///< Indicates if the check for hint 12 should be done
		qsf::Time			mRemainingWaitTimeForHint12;
		qsf::Time			mRemainingWaitTimeForHint14Check;
		qsf::Entity*		mUnitEntity;							///< The entity of the unit. Only valid while the unit moves to its target position (while this all command and selection input is blocked).
		qsf::Time			mRemainingWaitTimeForHint15;
		bool				mHint15Triggered;
		qsf::Entity*		mRtwUnitEntity;							///< The entity of the RTW unit. This is needed for the check if the RTW starts moving to HQ
		qsf::Entity*		mRwUnitEntity;							///< The entity of the RW unit. This is needed for the check for objectives 18 (let all units go back to HQ)
		qsf::Entity*		mNefUnitEntity;							///< The entity of the NEF unit. This is needed for the check for objectives 18 (let all units go back to HQ)
		uint64				mAsfUnitEntityId;						///< The entity ID of the ASF, needed for the is selected check
		qsf::Time			mRemainingWaitTimeForSupervisor31Start;
		bool				mCheckForHint16;						///< Indicates if the check for hint 16 should be done
		qsf::Time			mRemainingWaitTimeForHint16;
		bool				mCheckForHint17;						///< Indicates if the check for hint 17 should be done
		qsf::Time			mRemainingWaitTimeForHint17;
		bool				mCheckForHint18;						///< Indicates if the check for hint 18 should be done
		qsf::Time			mRemainingWaitTimeForHint18;
		bool				mCheckForHint20;						///< Indicates if the check for hint 20 should be done
		qsf::Time			mRemainingWaitTimeForHint20;
		bool				mCheckForHint21;						///< Indicates if the check for hint 21 should be done
		qsf::Time			mRemainingWaitTimeForHint21;
		bool				mReturnToBaseForNefInitiated;
		bool				mReturnToBaseForRwInitiated;
		qsf::Transform		mCameraTransformOnMinimapClick;
		bool				mLetMinimapMarkerBlink;
		bool				mBlinkState;
		glm::vec3			mFirefighterTargetAfterCutout;
		qsf::Entity*		mFirefigherWithRescueCutter;
		qsf::Entity*		mDoctor;

		qsf::MessageProxy	mMessageProxy;
		qsf::MessageProxy	mMessageProxyForHint21;

		bool	mSkippedOneFrame;	// Dummy variable to indicate if the have skipped one frame


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

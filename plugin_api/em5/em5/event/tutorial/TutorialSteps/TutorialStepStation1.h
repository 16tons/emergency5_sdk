// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/event/tutorial/TutorialStep.h"

#include <qsf/job/JobProxy.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class CameraControlComponent;
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
	*    EMERGENCY 5 tutorial step station 1
	*
	*/
	class TutorialStepStation1 : public TutorialStep
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		TutorialStepStation1(Tutorial& tutorial);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~TutorialStepStation1();


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
			OBJECTIVE_INTRODUCTION_STARTUP,
			OBJECTIVE_INTRODUCTION_INTRO,
			OBJECTIVE_INTRODUCTION_WAIT_FOR_SUPERVISOR_END,
			OBJECTIVE_INTRODUCTION_END,
			WAIT_FOR_CUTSCENE_END,
			ZOOM_ROTATION_INPUT_INTRO,
			ZOOM_ROTATION_INPUT_SETUP,
			ZOOM_ROTATION_INPUT_WAIT_FOR_USER_INPUT,
			ZOOM_ROTATION_INPUT_END,
			ORDER_UNIT_INTRO,
			ORDER_UNIT_WAIT_FOR_SUPERVISOR6_END,
			ORDER_UNIT_WAIT_FOR_VEHICLE_LIST_WINDOW_OPENS,
			ORDER_UNIT_WAIT_FOR_SUPERVISOR7_END,
			ORDER_UNIT_WAIT_FOR_VEHICLE_ORDER_WINDOW_OPENS,
			ORDER_UNIT_WAIT_FOR_SUPERVISOR8_END,
			ORDER_UNIT_WAIT_FOR_VEHICLE_HELP_WINDOW_OPENS,
			ORDER_UNIT_WAIT_FOR_VEHICLE_HELP_VIDEO_END,
			ORDER_UNIT_SUPERVISOR9_START,
			ORDER_UNIT_WAIT_FOR_SUPERVISOR9_END,
			ORDER_UNIT_WAIT_FOR_RW_GET_SEND,
			ORDER_UNIT_RW_GOT_ORDERED,
			ORDER_UNIT_WAIT_CAMERA_REACHES_HQ,
			ORDER_UNIT_WAIT_RW_REACHES_POSITION,
			CAMERA_FLIGHT_START,
			CAMERA_FLIGHT_END,
			LET_UNITS_EXIT_VEHICLE_STARTUP,
			LET_UNITS_EXIT_VEHICLE_WAIT_FOR_SUPERVISOR10_END,
			LET_UNITS_EXIT_VEHICLE_WAIT_FOR_RW_IS_SELECTED,
			LET_UNITS_EXIT_VEHICLE_WAIT_FOR_SUPERVISOR11_END,
			LET_UNITS_EXIT_VEHICLE_WAIT_FOR_EXIT_BUTTON_PRESSED,
			LET_UNITS_EXIT_VEHICLE_WAIT_FOR_EXITING,
			EQUIPPED_UNIT_WITH_FIREHOSE_WAIT_FOR_SUPERVISOR12_END,
			EQUIPPED_UNIT_WITH_FIREHOSE_WAIT_FOR_FIREFIGHTER_SELECTED,
			EQUIPPED_UNIT_WITH_FIREHOSE_SUPERVISOR13_STARTUP,
			EQUIPPED_UNIT_WITH_FIREHOSE_WAIT_FOR_SUPERVISOR13_END,
			EQUIPPED_UNIT_WITH_FIREHOSE_WAIT_FOR_COMMAND_START,
			INTRODUCE_DISPLAY_OF_INTERACTION_OBJECTS_SUPERVISOR14_START,
			INTRODUCE_DISPLAY_OF_INTERACTION_OBJECTS_WAIT_FOR_SUPERVISOR14_END,
			INTRODUCE_DISPLAY_OF_INTERACTION_OBJECTS_SUPERVISOR15_START,
			INTRODUCE_DISPLAY_OF_INTERACTION_OBJECTS_WAIT_FOR_SUPERVISOR15_END,
			INTRODUCE_DISPLAY_OF_INTERACTION_OBJECTS_WAIT_FOR_COMMAND_START,
			INTRODUCE_DISPLAY_OF_INTERACTION_OBJECTS_WAIT_FOR_EXTINGUISH_START,
			INTRODUCE_DISPLAY_OF_INTERACTION_OBJECTS_WAIT_BEFORE_COOLING_INTRODUCTION,
			COOLING_SUPERVISOR16_START,
			COOLING_WAIT_FOR_SUPERVISOR16_END,
			COOLING_EQUIPPED_UNIT_WITH_FIREHOSE_WAIT_FOR_FIREFIGHTER_SELECTED,
			COOLING_EQUIPPED_UNIT_WITH_FIREHOSE_WAIT_FOR_COMMAND_START,
			COOLING_SUPERVISOR17_START,
			COOLING_WAIT_FOR_SUPERVISOR17_END,
			COOLING_WAIT_FOR_COOL_COMMAND_EXECUTED_ON_TARGET,
			COOLING_WAIT_FOR_COOLING_STARTED,
			WAIT_FOR_STOPPING_FIRE_AFTER_COOLING_STARTED,
			MULTISELECT_STARTUP,
			MULTISELECT_SUPERVISOR18_START,
			MULTISELECT_WAIT_FOR_SUPERVISOR18_END,
			MULTISELECT_WAIT_FOR_ALL_FIREFIGHTER_PERSONS_SELECTED,
			MULTISELECT_SUPERVISOR19_START,
			MULTISELECT_WAIT_FOR_SUPERVISOR19_END,
			MULTISELECT_WAIT_FOR_ENTER_VEHICLE_COMMAND,
			END,
		};


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void introFinished(const qsf::MessageParameters& parameters);
		void onRwGetsOrdered(const qsf::MessageParameters& parameters);
		void onRwGetsSpawned(const qsf::MessageParameters& parameters);
		void onRwReachesTargetPosition(const qsf::MessageParameters& parameters);
		void onLetPersonExitRwVehicle(const qsf::MessageParameters& parameters);
		void onSelectionChangedForSingleSelect(const qsf::MessageParameters& parameters);
		void onGetHoseCommandExecuted(const qsf::MessageParameters& parameters);
		void onExtinguishWithHoseCommandExecuted(const qsf::MessageParameters& parameters);
		void onExtinguishingStarted(const qsf::MessageParameters& parameters);
		void onGetHoseCommandExecutedForCooling(const qsf::MessageParameters& parameters);
		void onCoolWithHoseCommandExecuted(const qsf::MessageParameters& parameters);
		void onCoolingStarted(const qsf::MessageParameters& parameters);
		void onSelectionChanged(const qsf::MessageParameters& parameters);
		void onEnterVehicleCommandExecuted(const qsf::MessageParameters& parameters);

		void waitForHelpVideoEnd(const qsf::JobArguments& jobArguments);
		void waitForCameraReachesHq(const qsf::JobArguments& jobArguments);

		void setupSnackBoothPart();

		qsf::CameraControlComponent* getCameraControlComponent();
		// The zoom state is the distance to target value
		int getCameraZoomState();
		// We need only one value and there only the numbers bfore the period
		int getCameraRotation();

		void ShowTutorialObjective1();

		void ShowTutorialObjective2();
		void enableInputForObjective2();
		bool checkPlayerInputForObjective2a();
		bool checkPlayerInputForObjective2b();

		void ShowTutorialObjective3a();
		void ShowTutorialObjective3b();
		void ShowTutorialObjective3c();
		void ShowTutorialObjective3d();
		void ShowTutorialObjective4a();
		void ShowTutorialObjective4b();
		void ShowTutorialObjective5a();
		void ShowTutorialObjective5b();
		void ShowTutorialObjectives6();
		void ShowTutorialObjective6c();
		void ShowTutorialObjective6d();
		void ShowTutorialObjective6e();
		void ShowTutorialObjectives7();
		void ShowTutorialObjective7c();

		bool isVehicleHelpWindowVisible(const qsf::Time& timePassed) const;
		bool hasHelpVideoEnded(const qsf::Time& timePassed) const;

		void setupWaitForRwGetsOrdered();

		void moveFirefightersToTheirTargetPositions();

		bool haveAllFireFightersBeDisconnectedFromHydrants();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		STATE						mCurrentState;
		qsf::Entity*				mSnackBoothEntity;								///< The entity for the first tutorial (burning snack booth)
		qsf::Entity*				mSnackBoothCoolingEntity;						///< The entity for the first tutorial (cooling snack booth)
		qsf::Entity*				mSnackBoothOrderAreaEntity;						///< The entity which defines the area in which the player should only be able to order the unit
		qsf::Entity*				mHqCameraPositionEntity;
		qsf::Entity*				mFirefighterRwEntity;
		std::vector<qsf::Entity*>	mFirefighterPersonTargetEntities;				///< The three firefighters move to one of these 3 target positions after the booth is extinguished
		int							mInitialTargetToDistanceValue;
		bool						mPlayerDidZoomIn;
		bool						mPlayerDidZoomOut;
		int							mInitialCameraRotation;
		bool						mPlayerDidRotateLeft;
		bool						mPlayerDidRotateRight;
		qsf::Time					mRemainingWaitTimeForExitVehicleFinished;
		qsf::Time					mRemainingWaitTimeForHint00;
		bool						mHint00Triggered;
		qsf::Time					mRemainingWaitTimeForHint01;
		bool						mHint01Triggered;
		qsf::Time					mRemainingWaitTimeForHint02;
		bool						mHint02Triggered;
		qsf::Time					mRemainingWaitTimeForHint03;
		bool						mHint03Triggered;
		qsf::Time					mRemainingWaitTimeForHint04;
		qsf::Time					mRemainingWaitTimeForHint05;
		bool						mHint05Triggered;
		qsf::Time					mRemainingWaitTimeForHint08;
		bool						mHint08Triggered;
		qsf::Time					mRemainingWaitTimeForPlayingSupervisor15;
		qsf::MessageProxy			mMessageProxy;									///< Listens to messages that affect the lifecycle
		qsf::JobProxy				mRealtimeUpdateJobProxy;						///< Needed to get updates when the help video is playing and the game is paused


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

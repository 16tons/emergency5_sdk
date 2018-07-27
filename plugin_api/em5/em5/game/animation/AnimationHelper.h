// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"

#include <qsf/asset/AssetProxy.h>
#include <qsf/time/Time.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Entity;
	class MeshAnimationChannel;
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
	*    EMERGENCY 5 animation helper class
	*/
	class EM5_API_EXPORT AnimationHelper
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		struct AnimationMaskData
		{
			qsf::AssetProxy	mPrimeAnimation;
			qsf::AssetProxy	mMaskAnimation;
			bool			mLoopAnimation;
			bool			mReverse1;
			bool			mReverse2;
		};


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		inline explicit AnimationHelper(qsf::Entity& entity);

		/**
		*  @brief
		*    Destructor
		*/
		inline ~AnimationHelper();

		bool isAnimationPlaying(const std::string& animationName);
		bool isAnimationPlaying() const;
		qsf::MeshAnimationChannel* playAnimation(const std::string& animationName, bool loop = false, bool reverse = false, qsf::Time blendTime = qsf::Time::ZERO);
		qsf::MeshAnimationChannel* playAnimation(const qsf::AssetProxy& animationAsset, bool loop = false, bool reverse = false, qsf::Time blendTime = qsf::Time::ZERO);

		// Reset the PersonAnimationController, so it can choose again what kind of animation should be played. Use this to interrupt animations.
		void clearAnimation();
		std::string getCurrentAnimation();
		std::string getCurrentAnimationDebugString();

		// Movement
		std::string getWalkAnimation() const;

		std::string getRunAnimationBegin() const;
		std::string getRunAnimation(AnimationMaskData& animationMaskData) const;
		std::string getRunAnimationEnd() const;

		std::string getSlowWalkAnimation() const;

		std::string getIdleAnimation(AnimationMaskData& animationMaskData) const;
		std::string getAnimationLookingUp() const;

		std::string getAnimationBoardAnimation() const;
		std::string getAnimationDeboardAnimation() const;
		std::string getAnimationPlacePerson() const;

		// Firefighting
		std::string getAnimationFirefighterInstallHydrant(bool useFloorHydrant = false) const;
		std::string getAnimationFirefighterUseFireHose() const;
		std::string getAnimationFirefighterUseExtinguisher() const;

		std::string getAnimationFirefighterUseRescueCutter1() const;
		std::string getAnimationFirefighterUseRescueCutter2() const;
		std::string getAnimationFirefighterUseRescueCutter3() const;

		std::string getAnimationFirefighterUseAxe1() const;
		std::string getAnimationFirefighterUseAxe2() const;
		std::string getAnimationFirefighterUseAxe3() const;

		std::string getAnimationFirefighterUseSaw1() const;
		std::string getAnimationFirefighterUseSaw2() const;
		std::string getAnimationFirefighterUseSaw3() const;

		std::string getAnimationFiremanPullStart() const;
		std::string getAnimationFiremanPullLoop() const;
		std::string getAnimationFiremanPullEnd() const;
		std::string getAnimationFiremanPullIdleStart() const;
		std::string getAnimationFiremanPullIdleLoop() const;

		std::string getAnimationFiremanPullObjectStart() const;
		std::string getAnimationFiremanPullObjectLoop() const;
		std::string getAnimationFiremanPullObjectEnd() const;

		std::string getAnimationFiremanPutOnAbcSuit01() const;
		std::string getAnimationFiremanPutOnAbcSuit02() const;
		std::string getAnimationFiremanPutOnMask() const;
		std::string getAnimationFiremanLeadContaminatedLoop() const;
		std::string getAnimationFiremanIdleContaminatedLoop() const;

		// Firefighting - diver
		std::string getAnimationDiverWalking() const;
		std::string getAnimationDiverStartSwimming() const;
		std::string getAnimationDiverSwimming() const;
		std::string getAnimationDiverIdleWater() const;
		std::string getAnimationDiverIdleLand() const;
		std::string getAnimationDiveDown() const;
		std::string getAnimationDiveUp() const;
		std::string getAnimationGrabDrowning() const;
		std::string getAnimationRescueDrowningWater() const;
		std::string getAnimationDiverDrowningLand() const;
		std::string getAnimationRescueDrowningLand() const;
		std::string getAnimationPutDrowningInBoat() const;
		std::string getAnimationPutDrowningOnGround() const;
		std::string getAnimationDiverEnterBoat() const;
		std::string getAnimationDiverLeavingBoat() const;

		// Ambulance
		std::string getAnimationParamedicRunFront() const;
		std::string getAnimationParamedicRunBack() const;
		std::string getAnimationParamedicIdleFront() const;
		std::string getAnimationParamedicIdleBack() const;
		std::string getAnimationParamedicLowerStretcherFront() const;
		std::string getAnimationParamedicLowerStretcherBack() const;
		std::string getAnimationParamedicRaiseStretcherFront() const;
		std::string getAnimationParamedicRaiseStretcherBack() const;
		std::string getAnimationDoctorGetEquipmentFromCar() const;

		std::string getAnimationEquipmentStretcherLower() const;
		std::string getAnimationEquipmentStretcherRaise() const;
		std::string getAnimationEquipmentOpenMedikit() const;
		std::string getAnimationEquipmentCloseMedikit() const;
		std::string getAnimationEquipmentOpenToolbox() const;
		std::string getAnimationEquipmentCloseToolbox() const;

		std::string getAnimationVictimOnStretcher() const;

		// Rescue leader / dog
		std::string getAnimationRescueDogFoundVictim() const;
		std::string getAnimationRescueDogSittingBark() const;
		std::string getAnimationRescueDogArrivedLeader() const;
		std::string getAnimationPoliceDogMenace() const;
		std::string getAnimationResueDogDie() const;
		std::string getAnimationRescueDogLeaderSendDog() const;
		std::string getAnimationRescueDogLeaderCancelDog() const;

		// Doctor Historic
		std::string getAnimationDoctorHistoricSendPersonsHome() const;
		std::string getAnimationDoctorHistoricCleanHome() const;

		// Police
		std::string getAnimationPoliceArrestGangster01() const;
		std::string getAnimationPoliceArrestGangster03() const;
		std::string getAnimationPoliceLeadArrestedLoop() const;
		std::string getAnimationPoliceLeadArrestedIdle() const;
		std::string getAnimationPoliceMegaphoneWalkingLoop() const;
		std::string getAnimationPoliceMegaphoneIdleLoop() const;
		std::string getAnimationPoliceBarrierTape01() const;
		std::string getAnimationPoliceBarrierTapeLoop() const;
		std::string getAnimationPoliceBarrierTape03() const;
		std::string getAnimationPoliceStopPerson() const;
		std::string getAnimationPolicePeppersprayStandingLoop() const;
		std::string getAnimationPoliceRegulateTrafficLoop() const;

		// SEK specialist
		std::string getAnimationSEKSpecialistIdle() const;
		std::string getAnimationSEKSpecialistGetPistol() const;
		std::string getAnimationSEKSpecialistPutAwayPistol() const;
		std::string getAnimationSEKSpecialistIdlePistol() const;
		std::string getAnimationSEKSpecialistPistol01() const;
		std::string getAnimationSEKSpecialistPistol02() const;
		std::string getAnimationSEKSpecialistPistol02End() const;
		std::string getAnimationSEKSpecialistPistolShoot() const;
		std::string getAnimationSEKSpecialistRunPistol01() const;
		std::string getAnimationSEKSpecialistRunPistol02Loop() const;
		std::string getAnimationSEKSpecialistRunPistol02End() const;
		std::string getAnimationSEKSpecialistRunShootPistol01() const;
		std::string getAnimationSEKSpecialistRunShootPistol02Loop() const;
		std::string getAnimationSEKSpecialistRunShootPistol02End() const;
		std::string getAnimationSEKSpecialistLeadArrestedLoop() const;
		std::string getAnimationSEKSpecialistLeadArrestedIdle() const;
		std::string getAnimationSEKSpecialistRopingLoop() const;
		std::string getAnimationSEKSpecialistRopingEnd() const;

		std::string getAnimationSEKSpecialistGetRifle() const;
		std::string getAnimationSEKSpecialistPutAwayRifle() const;
		std::string getAnimationSEKSpecialistIdleRifle() const;
		std::string getAnimationSEKSpecialistKneelRifleStart() const;
		std::string getAnimationSEKSpecialistKneelRifleLoop() const;
		std::string getAnimationSEKSpecialistKneelRifleEnd() const;
		std::string getAnimationSEKSpecialistShootRifle() const;
		std::string getAnimationSEKSpecialistRunRifle01() const;
		std::string getAnimationSEKSpecialistRunRifle02Loop() const;
		std::string getAnimationSEKSpecialistRunRifle02End() const;
		std::string getAnimationSEKSpecialistGetGrenade() const;
		std::string getAnimationSEKSpecialistIdleGrenade() const;
		std::string getAnimationSEKSpecialistThrowGrenade() const;

		std::string getAnimationGuardsmanEquipSword() const;

		// SEK robot
		std::string getAnimationSEKRobotDefuseBomb() const;

		// Tech
		std::string getAnimationRescueWithHeli01() const;
		std::string getAnimationRescueWithHeli02() const;
		std::string getAnimationEngineerGetEquipmentFromCar() const;
		std::string getAnimationEngineerOpenToolbox() const;
		std::string getAnimationEngineerGetDown() const;
		std::string getAnimationEngineerRepairSittingStart() const;
		std::string getAnimationEngineerRepairSittingLoop() const;
		std::string getAnimationEngineerRepairSittingEnd() const;
		std::string getAnimationEngineerRepairStandingStart() const;
		std::string getAnimationEngineerRepairStandingLoop() const;
		std::string getAnimationEngineerRepairStandingEnd() const;

		// Gangster
		std::string getAnimationGangsterGettingArrested01() const;
		std::string getAnimationGangsterGettingArrested03() const;
		std::string getAnimationGangsterWalkArrestedLoop() const;
		std::string getAnimationGangsterIdleArrested() const;
		std::string getAnimationGangsterCowerFearfullyStart() const;
		std::string getAnimationGangsterCowerFearfullyLoop() const;
		std::string getAnimationGangsterCowerFearfullyEnd() const;

		// Civilists
		std::string getAnimationCivilGape() const;
		std::string getAnimationCivilGape3() const;
		std::string getAnimationMobilephoneGetOutOfPocket() const;
		std::string getAnimationMobilephonePutInPocket() const;
		std::string getAnimationMobilephoneUseTalk() const;
		std::string getAnimationMobilephoneUsePhotographNear() const;
		std::string getAnimationMobilephoneUsePhotograph() const;
		std::string getAnimationUmbrellaOpen() const;
		std::string getAnimationUseObject() const;
		std::string getAnimationBootleWalk() const;
		std::string getAnimationAngry() const;

		std::string getAnimationWaveObject() const;
		std::string getAnimationStandIdleObjectRighthand() const;
		std::string getAnimationStandIdleBottle() const;
		std::string getAnimationFemaleStandIdleBottle() const;
		std::string getAnimationHooliganCheeringWithFlag() const;
		std::string getAnimationHooliganCheeringWithBottle() const;
		std::string getAnimationFemalePointingWithBottle() const;

		std::string getAnimationWaveHelpStanding() const;
		std::string getAnimationWaveHelpFromWindow() const;
		std::string getAnimationLookingUpWaving() const;
		std::string getAnimationCheer() const;

		std::string getAnimationDuckDownStart() const;
		std::string getAnimationDuckDownIdle() const;
		std::string getAnimationDuckDownEnd() const;

		std::string getAnimationWince() const;

		std::string getAnimationTalkExcited() const;
		std::string getAnimationTalkStanding() const;
		std::string getAnimationTalkStanding01() const;
		std::string getAnimationTalkStanding02() const;

		std::string getAnimationPlayGuitar() const;

		std::string getAnimationManipulateObjectStanding() const;
		std::string getAnimationManipulateObjectKneeling() const;

		std::string getAnimationBreatheDeeply() const;
		std::string getAnimationBreatheDeeplyLoop() const;
		std::string getAnimationJumpDown() const;

		// Historic witch
		std::string getAnimationWitchCaptured() const;
		std::string getAnimationWitchRescuing() const;

		// Injuries and medical cares
		std::string getCollapseAnimation(bool isKneeing = false) const;
		std::string getAnimationGenericLightLying() const;
		std::string getAnimationGenericNormalLying() const;
		std::string getAnimationGenericLightStanding() const;
		std::string getAnimationKneeingToLying() const;
		std::string getAnimationKneeingToStanding() const;
		std::string getAnimationInjuredDrowning() const;
		std::string getAnimationInjuredDrowningDanger() const;
		std::string getAnimationInjuredDrowningDead() const;
		std::string getAnimationInjuredDead() const;
		std::string getAnimationInjuredPulledStart() const;
		std::string getAnimationInjuredPulledLoop() const;
		std::string getAnimationInjuredPulledEnd() const;
		std::string getAnimationContaminatedWalkLoop() const;
		std::string getAnimationContaminatedIdleLoop() const;
		std::string getAnimationHangingHelpless() const;

		std::string getAnimationDoctorGetDown() const;
		std::string getAnimationDoctorOpenMedicCase() const;
		std::string getAnimationDoctorFromCaseToLyingPatient() const;
		std::string getAnimationDoctorFromLyingPatientToCase() const;
		std::string getAnimationDoctorStandUpWithInstruments() const;
		std::string getAnimationDoctorGetDefibrillator() const;
		std::string getAnimationDoctorTurnFromBaseToMonitor() const;
		std::string getAnimationDoctorDefibrillatorGetPads() const;
		std::string getAnimationDoctorDefibrillatorTurnToTarget() const;
		std::string getAnimationDoctorDefibrillatorUse() const;
		std::string getAnimationDefibrillateTarget() const;

		std::string getAnimationDoctorDiagnosis() const;
		std::string getAnimationDoctorMedicalCare() const;

		std::string getAnimationDoctorPlagueQuickTest() const;

		// Drown
		std::string getAnimationDrowningGetRescuedStart() const;
		std::string getAnimationDrowningGetRescuedMove() const;
		std::string getAnimationDrowningGetsPutInBoat() const;
		std::string getAnimationDrowningGround() const;
		std::string getAnimationDrowningGroundLoop() const;
		std::string getAnimationDrowningGroundPlace() const;

		// Vehicle
		std::string getAnimationCraneExtend() const;
		std::string getAnimationCraneLowToHigh() const;
		std::string getAnimationCraneLowToHigh_boat() const;
		std::string getAnimationHeavyCraneExtend() const;
		std::string getAnimationHeavyCraneLowToHigh() const;
		std::string getAnimationHeavyCraneCockpit() const;
		std::string getAnimationHeavyCraneHook() const;
		std::string getAnimationSupportLegExtend() const;
		std::string getAnimationHelicopterHatchOpen() const;
		std::string getAnimationHelicopterHatchClose() const;
		std::string getAnimationAirplaneHatchOpen() const;
		std::string getAnimationImtSidesOpen() const;

		// Fight animations
		// 01 stands for the attacking animation, 02 for defending with the weapon
		std::string getAnimationFistFight01Start() const;
		std::string getAnimationFistFight01Loop() const;
		std::string getAnimationFistFight01End() const;
		std::string getAnimationFistFight02() const;

		std::string getAnimationFightBaseballBat01Start() const;
		std::string getAnimationFightBaseballBat01Loop() const;
		std::string getAnimationFightBaseballBat01End() const;
		std::string getAnimationFightBaseballBat02() const;

		std::string getAnimationFightKnife01Start() const;
		std::string getAnimationFightKnife01Loop() const;
		std::string getAnimationFightKnife01End() const;
		std::string getAnimationFightKnife02() const;

		std::string getAnimationFightTorch01Start() const;
		std::string getAnimationFightTorch01Loop() const;
		std::string getAnimationFightTorch01End() const;
		std::string getAnimationFightTorch02() const;

		std::string getAnimationFightSword01Start() const;
		std::string getAnimationFightSword01Loop() const;
		std::string getAnimationFightSword01End() const;
		std::string getAnimationFightSword02() const;

		std::string getAnimationGettingHit() const;

		std::string getAnimationIdleThrowingWeapon() const;
		std::string getAnimationThrowObject() const;

		std::string getAnimationShootPistolStanding() const;

		std::string getAnimationShootPistolRunningStart() const;
		std::string getAnimationShootPistolRunningLoop() const;
		std::string getAnimationShootPistolRunningEnd() const;

		std::string getAnimationKneelingWithWeaponStart() const;
		std::string getAnimationKneelingWithWeaponLoop() const;
		std::string getAnimationKneelingWithWeaponEnd() const;

		// Stunned
		std::string getAnimationStunnedByPepperspray() const;
		std::string getAnimationStunnedByFlashGrenade() const;

		// Animated objects
		std::string getAnimationBuoy() const;
		std::string getAnimationWellPumping() const;

		// Supervisor
		std::string getAnimationSupervisorSpeaking() const;
		std::string getAnimationSupervisorIdle() const;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		const std::string& getPersonType() const;
		std::string getRealPersonTypeOrShardedAnimationName(const std::string& animationName) const;


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		qsf::Entity& mEntity;	///< The entity the class gets all its information from


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "em5/game/animation/AnimationHelper-inl.h"

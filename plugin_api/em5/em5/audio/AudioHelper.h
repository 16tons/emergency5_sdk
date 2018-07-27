// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"

#include <qsf/time/Time.h>

#include <boost/noncopyable.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace em5
{
	class AudioProxy;
}
namespace qsf
{
	class Entity;
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
	*    Static EMERGENCY 5 audio helper class
	*/
	class EM5_API_EXPORT AudioHelper : public boost::noncopyable
	{


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	public:
		static void playSound(const std::string& sfxFilename, AudioProxy& audioProxy);
		static void playSound(const std::string& sfxFilename, AudioProxy& audioProxy, qsf::Entity& entity);
		static void playDelayedSound(const std::string& sfxFilename, AudioProxy& audioProxy, qsf::Entity& entity, const qsf::Time& delay);
		static void playLoopSound(const std::string& sfxFilename, AudioProxy& audioProxy, qsf::Entity& entity);

		static std::string getGenderedCivilSound(const qsf::Entity& entity, const std::string& audioName);

		// Siren
		static void setSirenFirefighter(AudioProxy& audioProxy, qsf::Entity& entity);
		static void setSirenFirefighterFLB(AudioProxy& audioProxy, qsf::Entity& entity);
		static void setSirenPolice(AudioProxy& audioProxy, qsf::Entity& entity);
		static void setSirenRescue(AudioProxy& audioProxy, qsf::Entity& entity);
		static void setSirenTech(AudioProxy& audioProxy, qsf::Entity& entity);
		static void setSirenHistoric(AudioProxy& audioProxy, qsf::Entity& entity);

		// Vehicle sounds
		static void playOpenNormalDoorSound(AudioProxy& audioProxy, qsf::Entity& entity);
		static void playCloseNormalDoorSound(AudioProxy& audioProxy, qsf::Entity& entity);
		static void playOpenTrunkDoorSound(AudioProxy& audioProxy, qsf::Entity& entity);
		static void playCloseTrunkDoorSound(AudioProxy& audioProxy, qsf::Entity& entity);
		static void playOpenSlidingDoorSound(AudioProxy& audioProxy, qsf::Entity& entity);
		static void playCloseSlidingDoorSound(AudioProxy& audioProxy, qsf::Entity& entity);
		static void playRWRollUpDoorSound(AudioProxy& audioProxy, qsf::Entity& entity);
		static void playExtinguishFireWithTLFSound(AudioProxy& audioProxy, qsf::Entity& entity);
		static void playRotorAcceleratingSound(AudioProxy& audioProxy, qsf::Entity& entity);
		static void playRotorNormalSound(AudioProxy& audioProxy, qsf::Entity& entity);
		static void playRotorDecceleratingSound(AudioProxy& audioProxy, qsf::Entity& entity);
		static void playTowCraneMove(AudioProxy& audioProxy, qsf::Entity& entity);
		static void playTowCraneRope(AudioProxy& audioProxy, qsf::Entity& entity);
		static void playTowCraneClonk(AudioProxy& audioProxy, qsf::Entity& entity);
		static void playCraneExtendLegs(AudioProxy& audioProxy, qsf::Entity& entity);
		static void playCraneHideLegs(AudioProxy& audioProxy, qsf::Entity& entity);
		static void playCraneCockpitUp(AudioProxy& audioProxy, qsf::Entity& entity);
		static void playCraneCockpitDown(AudioProxy& audioProxy, qsf::Entity& entity);
		static void playCraneMove(AudioProxy& audioProxy, qsf::Entity& entity);
		static void playCraneHook(AudioProxy& audioProxy, qsf::Entity& entity);
		static void playCraneDeployObject(AudioProxy& audioProxy, qsf::Entity& entity);
		static void playHelicopterRopeInstall(AudioProxy& audioProxy, qsf::Entity& entity);
		static void playHelicopterRopeMove(AudioProxy& audioProxy, qsf::Entity& entity);
		static void playHelicopterRopeAttach(AudioProxy& audioProxy, qsf::Entity& entity);
		static void playHelicopterSearchLightOn(AudioProxy& audioProxy, qsf::Entity& entity);
		static void playSEKDroneStart(AudioProxy& audioProxy, qsf::Entity& entity);
		static void playSEKDroneMove(AudioProxy& audioProxy, qsf::Entity& entity);
		static void playSEKDroneClick(AudioProxy& audioProxy, qsf::Entity& entity);
		static void playFWDroneMove(AudioProxy& audioProxy, qsf::Entity& entity);
		static void playSEKRoboterRamp(AudioProxy& audioProxy, qsf::Entity& entity);
		static void playSEKRoboterDefuseBombe(AudioProxy& audioProxy, qsf::Entity& entity);
		static void playDecontaminatePerson(AudioProxy& audioProxy, qsf::Entity& entity);
		static void playDLKLadderMove(AudioProxy& audioProxy, qsf::Entity& entity);
		static void playDLKLadderKlonk(AudioProxy& audioProxy, qsf::Entity& entity);
		static void playDLKExtinguish(AudioProxy& audioProxy, qsf::Entity& entity);
		static void playFLBExtinguish(AudioProxy& audioProxy, qsf::Entity& entity);
		static void playBoatUnloadIntoWater(AudioProxy& audioProxy, qsf::Entity& entity);
		static void playBoatLoadFromWater(AudioProxy& audioProxy, qsf::Entity& entity);
		static void playBoatDropOnTransporter(AudioProxy& audioProxy, qsf::Entity& entity);
		static void playCommandoCarRadarSound(AudioProxy& audioProxy, qsf::Entity& entity);
		static void playCommandoCarUseFirePlane(AudioProxy& audioProxy);
		static void playFirePlaneMovingSound(AudioProxy& audioProxy, qsf::Entity& entity);
		static void playFirePlaneEffectStart(AudioProxy& audioProxy, qsf::Entity& entity);
		static void playFirePlaneEffectImpact(AudioProxy& audioProxy, qsf::Entity& entity);

		// Personnel sounds
		static void playDoctorOpenCase(AudioProxy& audioProxy, qsf::Entity& entity);
		static void playDoctorCloseCase(AudioProxy& audioProxy, qsf::Entity& entity);
		static void playDoctorDefibrilator(AudioProxy& audioProxy, qsf::Entity& entity, qsf::Time delay = qsf::Time::ZERO);
		static void playDoctorEkgDiagnose(AudioProxy& audioProxy, qsf::Entity& entity);
		static void playDoctorEkgDeath(AudioProxy& audioProxy, qsf::Entity& entity);
		static void playDoctorVentilation(AudioProxy& audioProxy, qsf::Entity& entity);
		static void playMedicStretcherMove(AudioProxy& audioProxy, qsf::Entity& entity);
		static void playMedicStretcherFold(AudioProxy& audioProxy, qsf::Entity& entity, qsf::Time delay = qsf::Time::ZERO);
		static void playRescueDogWhistle(AudioProxy& audioProxy, qsf::Entity& entity, qsf::Time delay = qsf::Time::ZERO);
		static void playRescueDogPant(AudioProxy& audioProxy, qsf::Entity& entity);
		static void playRescueDogBark(AudioProxy& audioProxy, qsf::Entity& entity);
		static void playEngineer(AudioProxy& audioProxy, qsf::Entity& entity);
		static void playEngineerReparingBuildingSound(AudioProxy& audioProxy, qsf::Entity& entity, qsf::Time delay = qsf::Time::ZERO);
		static void playPolicePepperSpray(AudioProxy& audioProxy, qsf::Entity& entity);
		static void playPoliceHandcuffs(AudioProxy& audioProxy, qsf::Entity& entity, qsf::Time delay = qsf::Time::ZERO);
		static void playPoliceMegaphone(AudioProxy& audioProxy, qsf::Entity& entity);
		static void playSEKRoping(AudioProxy& audioProxy, qsf::Entity& entity);
		static void playSEKPistoleShot(AudioProxy& audioProxy, qsf::Entity& entity);
		static void playSEKRifleShot(AudioProxy& audioProxy, qsf::Entity& entity);
		static void playSEKFlashGrenade(AudioProxy& audioProxy, qsf::Entity& entity);
		static void playFirefighterExtinguisherStartSound(AudioProxy& audioProxy, qsf::Entity& entity);
		static void playFirefighterExtinguisherExtinguishSound(AudioProxy& audioProxy, qsf::Entity& entity);
		static void playFirefighterHoseConnectSound(AudioProxy& audioProxy, qsf::Entity& entity);
		static void playFirefighterHoseExtinguishSound(AudioProxy& audioProxy, qsf::Entity& entity);
		static void playFirefighterSawStart(AudioProxy& audioProxy, qsf::Entity& entity);
		static void playFirefighterSawRunning(AudioProxy& audioProxy, qsf::Entity& entity);
		static void playFirefighterScissorsCompressorDrop(AudioProxy& audioProxy, qsf::Entity& entity);
		static void playFirefighterScissorsCompressorRunning(AudioProxy& audioProxy, qsf::Entity& entity);
		static void playFirefighterScissorsSound(AudioProxy& audioProxy, qsf::Entity& entity);
		static void playFirefighterAxe(AudioProxy& audioProxy, qsf::Entity& entity);
		static void playFirefighterHistoricExtinguisherStart(AudioProxy& audioProxy, qsf::Entity& entity);
		static void playFirefighterHistoricExtinguisherRunning(AudioProxy& audioProxy, qsf::Entity& entity);
		static void playFirefighterHistoricSawStart(AudioProxy& audioProxy, qsf::Entity& entity);
		static void playFirefighterHistoricSawRunning(AudioProxy& audioProxy, qsf::Entity& entity);
		static void playDiverDive(AudioProxy& audioProxy, qsf::Entity& entity);
		static void playDiverEmerge(AudioProxy& audioProxy, qsf::Entity& entity);
		static void playHistoricDoctorSendHome(AudioProxy& audioProxy, qsf::Entity& entity);
		static void playHistoricDoctorCleanHome(AudioProxy& audioProxy, qsf::Entity& entity);
		static void playHistoricDoctorCleanHome2(AudioProxy& audioProxy, qsf::Entity& entity);

		// Civil vehicles
		static void playCivilPkwHorn(AudioProxy& audioProxy, qsf::Entity& entity);		// TODO(tl): Not implemented yet
		static void playCivilBusHorn(AudioProxy& audioProxy, qsf::Entity& entity);		// TODO(tl): Not implemented yet
		static void playCivilLkwHorn(AudioProxy& audioProxy, qsf::Entity& entity);		// TODO(tl): Not implemented yet
		static void playCivilHonkingFest(AudioProxy& audioProxy, qsf::Entity& entity);	// TODO(tl): Not implemented yet

		static void playTramStart(AudioProxy& audioProxy, qsf::Entity& entity);		// TODO(tl): Not implemented yet
		static void playTramRunning(AudioProxy& audioProxy, qsf::Entity& entity);		// TODO(tl): Not implemented yet
		static void playTramStop(AudioProxy& audioProxy, qsf::Entity& entity);			// TODO(tl): Not implemented yet

		static void playICEStart(AudioProxy& audioProxy, qsf::Entity& entity);			// TODO(tl): Not implemented yet
		static void playICERunning(AudioProxy& audioProxy, qsf::Entity& entity);		// TODO(tl): Not implemented yet
		static void playICEStop(AudioProxy& audioProxy, qsf::Entity& entity);			// TODO(tl): Not implemented yet

		static void playTrainStart(AudioProxy& audioProxy, qsf::Entity& entity);		// TODO(tl): Not implemented yet
		static void playTrainRunning(AudioProxy& audioProxy, qsf::Entity& entity);		// TODO(tl): Not implemented yet
		static void playTrainStop(AudioProxy& audioProxy, qsf::Entity& entity);		// TODO(tl): Not implemented yet

		static void playCivilFreighter(AudioProxy& audioProxy, qsf::Entity& entity);	// TODO(tl): Not implemented yet
		static void playCivilCutter(AudioProxy& audioProxy, qsf::Entity& entity);		// TODO(tl): Not implemented yet

		// Gangster
		static void playGangsterFistVersion1(AudioProxy& audioProxy, qsf::Entity& entity, qsf::Time delay = qsf::Time::ZERO);
		static void playGangsterFistVersion2(AudioProxy& audioProxy, qsf::Entity& entity, qsf::Time delay = qsf::Time::ZERO);

		static void playGangsterKickVersion1(AudioProxy& audioProxy, qsf::Entity& entity, qsf::Time delay = qsf::Time::ZERO);
		static void playGangsterKickVersion2(AudioProxy& audioProxy, qsf::Entity& entity, qsf::Time delay = qsf::Time::ZERO);

		static void playGangsterPistoleShot(AudioProxy& audioProxy, qsf::Entity& entity);

		// Building
		static void playBuildingBreakdownSmallSound(AudioProxy& audioProxy, qsf::Entity& entity, qsf::Time delay = qsf::Time::ZERO);
		static void playBuildingBreakdownMediumSound(AudioProxy& audioProxy, qsf::Entity& entity, qsf::Time delay = qsf::Time::ZERO);
		static void playBuildingBreakdownBigSound(AudioProxy& audioProxy, qsf::Entity& entity, qsf::Time delay = qsf::Time::ZERO);
		static void playBuildingWindowDestroyedSound(AudioProxy& audioProxy, qsf::Entity& entity, qsf::Time delay = qsf::Time::ZERO);
		static void playBuildingAttackingSound1(AudioProxy& audioProxy, qsf::Entity& entity, qsf::Time delay = qsf::Time::ZERO);
		static void playBuildingAttackingSound2(AudioProxy& audioProxy, qsf::Entity& entity, qsf::Time delay = qsf::Time::ZERO);
		static void playBuildingEvacuateSound(AudioProxy& audioProxy, qsf::Entity& entity, qsf::Time delay = qsf::Time::ZERO);
		static void playBuildingEvacuateSound2(AudioProxy& audioProxy, qsf::Entity& entity, qsf::Time delay = qsf::Time::ZERO);

		// Destruction
		static void playExplosionSmallSound(AudioProxy& audioProxy, qsf::Entity& entity);
		static void playExplosionVehicleSound(AudioProxy& audioProxy, qsf::Entity& entity);
		static void playExplosionBigSound(AudioProxy& audioProxy, qsf::Entity& entity);
		static void playExplosionVeryBigSound(AudioProxy& audioProxy, qsf::Entity& entity);
		static void playTreeDownSound(AudioProxy& audioProxy, qsf::Entity& entity);

		// Events
		static void playBombTicking(AudioProxy& audioProxy, qsf::Entity& entity);
		static void playBombExplosion(AudioProxy& audioProxy, qsf::Entity& entity);
		static void playDiveSpot(AudioProxy& audioProxy, qsf::Entity& entity);

		static void playControlBoxSparks(AudioProxy& audioProxy, qsf::Entity& entity);
		static void playControlBoxFlash(AudioProxy& audioProxy, qsf::Entity& entity);
		static void playControlBoxStartBurning(AudioProxy& audioProxy, qsf::Entity& entity);

		// ME01
		static void playME01SparksShowerSound(AudioProxy& audioProxy, qsf::Entity& entity);
		static void playME01KerosineIgnitionSound(AudioProxy& audioProxy, qsf::Entity& entity);

		// GUI
		static void playLeftMouseClickSound(AudioProxy& audioProxy);
		static void playRightMouseClickSound(AudioProxy& audioProxy);


	//[-------------------------------------------------------]
	//[ Private static methods                                ]
	//[-------------------------------------------------------]
	private:
		static const std::string& getPersonType(const qsf::Entity& entity);


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Default constructor
		*/
		inline AudioHelper();

		/**
		*  @brief
		*    Destructor
		*/
		inline ~AudioHelper();


	//[-------------------------------------------------------]
	//[ Private static data                                   ]
	//[-------------------------------------------------------]
	private:
		static const float mDoorSoundsMaximumDistance;		///< Maximum distance for opening and closing door sounds. This is used to set the maximum distance for audio proxy. (Changes the maximum listener distance at which the source will no longer be attenuated)
		static const float mDoorSoundsReferenceDistance;	///< Reference distance for opening and closing door sounds


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "em5/audio/AudioHelper-inl.h"

// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/game/achievement/Achievement.h"		// Not actually needed, but whoever includes this header will most probably need the EM5 achievement definition as well

#include <qsf_game/achievement/AchievementSystem.h>	// Not actually needed, but whoever includes this header will most probably need the achievement system as well


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
	*    Static EMERGENCY 5 achievements collection class
	*/
	class EM5_API_EXPORT Achievements
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		// General achievements
		static const qsf::NamedIdentifier EM5_ACHIEVEMENT_01;	// "Erste Schritte"
		static const qsf::NamedIdentifier EM5_ACHIEVEMENT_02;	// "Brandmeister"
		static const qsf::NamedIdentifier EM5_ACHIEVEMENT_03;	// "Lebensretter"
		static const qsf::NamedIdentifier EM5_ACHIEVEMENT_04;	// "Panzerknacker-Knacker"

		// Mainevent specific achievements
		static const qsf::NamedIdentifier EM5_ACHIEVEMENT_05;	// ME01 achievement
		static const qsf::NamedIdentifier EM5_ACHIEVEMENT_06;	// ME02 achievement
		static const qsf::NamedIdentifier EM5_ACHIEVEMENT_07;	// ME04 achievement
		static const qsf::NamedIdentifier EM5_ACHIEVEMENT_08;	// ME05 achievement
		static const qsf::NamedIdentifier EM5_ACHIEVEMENT_09;	// ME06 achievement
		static const qsf::NamedIdentifier EM5_ACHIEVEMENT_10;	// ME07 achievement
		static const qsf::NamedIdentifier EM5_ACHIEVEMENT_11;	// ME08 achievement
		static const qsf::NamedIdentifier EM5_ACHIEVEMENT_12;	// ME10 achievement
		static const qsf::NamedIdentifier EM5_ACHIEVEMENT_14;	// ME12 achievement
		static const qsf::NamedIdentifier EM5_ACHIEVEMENT_15;	// ME14 achievement
		static const qsf::NamedIdentifier EM5_ACHIEVEMENT_16;	// ME15 achievement
		static const qsf::NamedIdentifier EM5_ACHIEVEMENT_17;	// All silver medals
		static const qsf::NamedIdentifier EM5_ACHIEVEMENT_18;	// All gold medals

		// Campaign
		static const qsf::NamedIdentifier EM5_ACHIEVEMENT_19;
		static const qsf::NamedIdentifier EM5_ACHIEVEMENT_20;
		static const qsf::NamedIdentifier EM5_ACHIEVEMENT_21;
		static const qsf::NamedIdentifier EM5_ACHIEVEMENT_22;
		static const qsf::NamedIdentifier EM5_ACHIEVEMENT_23;
		static const qsf::NamedIdentifier EM5_ACHIEVEMENT_24;

		// World achievements
		static const qsf::NamedIdentifier EM5_ACHIEVEMENT_28;	// Won all kinds of world events in Munich (4)
		static const qsf::NamedIdentifier EM5_ACHIEVEMENT_29;	// Won all kinds of world events in Hamburg (3)
		static const qsf::NamedIdentifier EM5_ACHIEVEMENT_30;	// Won all kinds of world events in Berlin (3)
		static const qsf::NamedIdentifier EM5_ACHIEVEMENT_31;	// Won all kinds of world events in all maps (4 + 3 + 3 = 10)

		// Freeplay achievements
		static const qsf::NamedIdentifier EM5_ACHIEVEMENT_32;
		static const qsf::NamedIdentifier EM5_ACHIEVEMENT_33;
		static const qsf::NamedIdentifier EM5_ACHIEVEMENT_34;
		static const qsf::NamedIdentifier EM5_ACHIEVEMENT_35;
		static const qsf::NamedIdentifier EM5_ACHIEVEMENT_36;
		static const qsf::NamedIdentifier EM5_ACHIEVEMENT_37;

		// Challenge achievements
		static const qsf::NamedIdentifier EM5_ACHIEVEMENT_38;
		static const qsf::NamedIdentifier EM5_ACHIEVEMENT_39;
		static const qsf::NamedIdentifier EM5_ACHIEVEMENT_40;
		static const qsf::NamedIdentifier EM5_ACHIEVEMENT_41;
		static const qsf::NamedIdentifier EM5_ACHIEVEMENT_42;
		static const qsf::NamedIdentifier EM5_ACHIEVEMENT_43;

		// Multiplayer together specific achievements
		static const qsf::NamedIdentifier EM5_ACHIEVEMENT_44;
		static const qsf::NamedIdentifier EM5_ACHIEVEMENT_45;
		static const qsf::NamedIdentifier EM5_ACHIEVEMENT_46;

		// Multiplayer against each other specific achievements
		static const qsf::NamedIdentifier EM5_ACHIEVEMENT_47;
		static const qsf::NamedIdentifier EM5_ACHIEVEMENT_48;
		static const qsf::NamedIdentifier EM5_ACHIEVEMENT_49;

		// EMERGENCY 2016 achievements
		static const qsf::NamedIdentifier EM5_ACHIEVEMENT_ME100;
		static const qsf::NamedIdentifier EM5_ACHIEVEMENT_ME101;
		static const qsf::NamedIdentifier EM5_ACHIEVEMENT_ME102;
		static const qsf::NamedIdentifier EM5_ACHIEVEMENT_ME103;
		static const qsf::NamedIdentifier EM5_ACHIEVEMENT_ME104;
		static const qsf::NamedIdentifier EM5X_ACHIEVEMENT_MINI01;
		static const qsf::NamedIdentifier EM5X_ACHIEVEMENT_MINI02;
		static const qsf::NamedIdentifier EM5X_ACHIEVEMENT_MINI03;
		static const qsf::NamedIdentifier EM5X_ACHIEVEMENT_MINI04;
		static const qsf::NamedIdentifier EM5X_ACHIEVEMENT_MINI05;
		static const qsf::NamedIdentifier EM5X_ACHIEVEMENT_MINI06;
		static const qsf::NamedIdentifier EM5X_ACHIEVEMENT_MINI07;
		static const qsf::NamedIdentifier EM5X_ACHIEVEMENT_MINI08;

		// EMERGENCY 2017 achievements
		static const qsf::NamedIdentifier EM2017_ACHIEVEMENT_ME200;
		static const qsf::NamedIdentifier EM2017_ACHIEVEMENT_ME201;
		static const qsf::NamedIdentifier EM2017_ACHIEVEMENT_ME202;
		static const qsf::NamedIdentifier EM2017_ACHIEVEMENT_ME203;
		static const qsf::NamedIdentifier EM2017_ACHIEVEMENT_ME204;
		static const qsf::NamedIdentifier EM2017_ACHIEVEMENT_MINI01;
		static const qsf::NamedIdentifier EM2017_ACHIEVEMENT_MINI02;
		static const qsf::NamedIdentifier EM2017_ACHIEVEMENT_MINI03;
		static const qsf::NamedIdentifier EM2017_ACHIEVEMENT_MINI04;
		static const qsf::NamedIdentifier EM2017_ACHIEVEMENT_MINI05;
		static const qsf::NamedIdentifier EM2017_ACHIEVEMENT_MINI06;
		static const qsf::NamedIdentifier EM2017_ACHIEVEMENT_MINI07;
		static const qsf::NamedIdentifier EM2017_ACHIEVEMENT_MINI08;

		// EMERGENCY 20 Years achievements
		static const qsf::NamedIdentifier EM20_ACHIEVEMENT_ME310;

		// General ranks
		static const qsf::NamedIdentifier R1_TITLE;		// EMERGENCY 5 Rookie
		static const qsf::NamedIdentifier R2_TITLE;		// EMERGENCY 5 DELUXE-Einsatzleiter
		static const qsf::NamedIdentifier R3_TITLE;
		static const qsf::NamedIdentifier R4_TITLE;
		static const qsf::NamedIdentifier R5_TITLE;
		static const qsf::NamedIdentifier R6_TITLE;
		static const qsf::NamedIdentifier R7_TITLE;
		static const qsf::NamedIdentifier R8_TITLE;
		static const qsf::NamedIdentifier R9_TITLE;
		static const qsf::NamedIdentifier R10_TITLE;
		static const qsf::NamedIdentifier R11_TITLE;
		static const qsf::NamedIdentifier R12_TITLE;
		static const qsf::NamedIdentifier R13_TITLE;
		static const qsf::NamedIdentifier R14_TITLE;
		static const qsf::NamedIdentifier R15_TITLE;
		static const qsf::NamedIdentifier R16_TITLE;
		static const qsf::NamedIdentifier R17_TITLE;
		static const qsf::NamedIdentifier R18_TITLE;
		static const qsf::NamedIdentifier R19_TITLE;
		static const qsf::NamedIdentifier R20_TITLE;
		static const qsf::NamedIdentifier R21_TITLE;
		static const qsf::NamedIdentifier R22_TITLE;
		static const qsf::NamedIdentifier R23_TITLE;
		static const qsf::NamedIdentifier R24_TITLE;
		static const qsf::NamedIdentifier R25_TITLE;
		static const qsf::NamedIdentifier R26_TITLE;
		static const qsf::NamedIdentifier R27_TITLE;
		static const qsf::NamedIdentifier R28_TITLE;
		static const qsf::NamedIdentifier R29_TITLE;
		static const qsf::NamedIdentifier R30_TITLE;
		static const qsf::NamedIdentifier R31_TITLE;
		static const qsf::NamedIdentifier R32_TITLE;

		// Medals
		static const qsf::NamedIdentifier EM5_MEDAL_GE01;	///< Munich "em5::MainEvent01"
		static const qsf::NamedIdentifier EM5_MEDAL_GE02;	///< Munich "em5::MainEvent02"
		static const qsf::NamedIdentifier EM5_MEDAL_GE04;	///< Munich "em5::MainEvent04"
		static const qsf::NamedIdentifier EM5_MEDAL_GE05;	///< Munich "em5::MainEvent05"
		static const qsf::NamedIdentifier EM5_MEDAL_GE06;	///< Hamburg "em5::MainEvent06"
		static const qsf::NamedIdentifier EM5_MEDAL_GE07;	///< Hamburg "em5::MainEvent07"
		static const qsf::NamedIdentifier EM5_MEDAL_GE08;	///< Hamburg "em5::MainEvent08"
		static const qsf::NamedIdentifier EM5_MEDAL_GE10;	///< Hamburg "em5::MainEvent10"
		static const qsf::NamedIdentifier EM5_MEDAL_GE12;	///< Berlin "em5::MainEvent12"
		static const qsf::NamedIdentifier EM5_MEDAL_GE14;	///< Berlin "em5::MainEvent14"
		static const qsf::NamedIdentifier EM5_MEDAL_GE15;	///< Berlin "em5::MainEvent15"

		// Achievements needed progress
		static const float NEEDED_TIME_ACHIEVEMENT_05;		///< Maximum time to rescue all original injured persons: 480 sec / 8 min
		static const float NEEDED_TIME_ACHIEVEMENT_08;		///< Maximum time to unhide the buried persons: 480 sec / 8 min
		static const float NEEDED_PROGRESS_ACHIEVEMENT_09;	///< Needed number of extinguished fires
		static const float NEEDED_MEDALS_ACHIEVEMENT_17;	///< Needed number of silver medals, (equals number all mainevents, no included mainevents from addons or dlcs)
		static const float NEEDED_MEDALS_ACHIEVEMENT_18;	///< Needed number of gold medals, (equals number all mainevents, no included mainevents from addons or dlcs)


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Register the achievements
		*/
		static void registerAchievements(qsf::game::AchievementSystem& achievementSystem);

		/**
		*  @brief
		*    Get the medal name by a given main event number
		*/
		static const qsf::NamedIdentifier& getMedalNameByMainEventNumber(uint16 mainEventNumber);

		/**
		*  @brief
		*    Is the medal part of the main game or an addon/dlc/patch
		*/
		static bool isMainGameMedal(uint16 medalNumber);


	//[-------------------------------------------------------]
	//[ Private static data                                   ]
	//[-------------------------------------------------------]
	private:
		static std::vector<qsf::NamedIdentifier>						mTitleList;			///< This list will holds all rank identifiers in the correct order
		static boost::container::flat_map<uint16, qsf::NamedIdentifier>	mMainGameMedals;	///< Medal names by main event numbers


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

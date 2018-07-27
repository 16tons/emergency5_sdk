// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"
#include "em5/game/gamemode/GameModeType.h"

#include <qsf/base/NamedIdentifier.h>
#include <qsf/reflection/type/CampQsfTime.h>

#include <boost/noncopyable.hpp>
#include <boost/container/flat_map.hpp>
#include <boost/container/flat_set.hpp>
#include <boost/dynamic_bitset.hpp>

#include <memory> // For std::unique_ptr


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class BinarySerializer;
}
namespace em5
{
	class AccountManager;
	class HighscoreManager;
	class MapIdentifier;
	class MainEventIdentifier;
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
	*    EMERGENCY 5 player progress class
	*/
	class PlayerProgress : public boost::noncopyable
	{


	//[-------------------------------------------------------]
	//[ Friend classes                                        ]
	//[-------------------------------------------------------]
		friend AccountManager;


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const char LOCAL_PLAYER[];

		struct WorldEventProgress
		{
			bool		mWorldEventsActive;	///< Indicates if the world events are currently active
			qsf::Time	mWaitTime;			///< Remaining wait time until the world event state changes

			inline WorldEventProgress() :
				mWorldEventsActive(false),
				mWaitTime(qsf::Time::ZERO)
			{}
		};

		enum MedalType
		{
			MEDAL_NONE,
			MEDAL_BRONZE,
			MEDAL_SILVER,
			MEDAL_GOLD
		};

		typedef boost::container::flat_set<qsf::NamedIdentifier>	NameSet;
		typedef std::map<qsf::NamedIdentifier, WorldEventProgress>	WorldEventProgressMap;
		typedef std::pair<const qsf::NamedIdentifier, MedalType>	Medal;
		typedef std::map<qsf::NamedIdentifier, MedalType>			MedalMap;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		PlayerProgress();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~PlayerProgress();

		/**
		*  @brief
		*    Reset the current player progress to initial state
		*/
		void resetProgress(bool clearAchievementsAndMedals);

		//[-------------------------------------------------------]
		//[ Total score                                           ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Return the current total score
		*/
		uint32 getTotalScore() const;

		/**
		*  @brief
		*    Sets total score
		*/
		void setTotalScore(uint32 totalScore);

		/**
		*  @brief
		*    Adds points to the total score
		*/
		void addPointsToTotalScore(uint32 points);

		//[-------------------------------------------------------]
		//[ Unlock progress                                       ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Return whether or not the tutorial is considered to have been played
		*/
		bool isTutorialPlayed() const;

		/**
		*  @brief
		*    Set whether or not the tutorial is considered to have been played (freeplay and multiplay is unlocked in this case)
		*/
		void setTutorialPlayed(bool unlock);

		/**
		*  @brief
		*    Check if a map is unlocked
		*/
		bool isMapUnlocked(const MapIdentifier& mapIdentifier) const;

		/**
		*  @brief
		*    Check if a main event is unlocked
		*/
		bool isMainEventUnlocked(const MainEventIdentifier& mainEventIdentifier) const;

		/**
		*  @brief
		*    Check if a main event is won in campaign
		*/
		bool isMainEventWonInCampaign(const MainEventIdentifier& mainEventIdentifier) const;

		/**
		*  @brief
		*    Unlock the given map
		*/
		void setMapUnlocked(const MapIdentifier& mapIdentifier);

		/**
		*  @brief
		*    Unlock the given main event
		*/
		void setMainEventUnlocked(const MainEventIdentifier& mainEventIdentifier);

		/**
		*  @brief
		*    Unlock the given main event
		*/
		void setMainEventWonInCampaign(const MainEventIdentifier& mainEventIdentifier);

		/**
		*  @brief
		*     Check if world event is won
		*/
		bool isWorldEventWonMunich(const qsf::NamedIdentifier& eventIdentifier) const;
		bool isWorldEventWonHamburg(const qsf::NamedIdentifier& eventIdentifier) const;
		bool isWorldEventWonBerlin(const qsf::NamedIdentifier& eventIdentifier) const;

		/**
		*  @brief
		*    Set world event won
		*/
		void setWorldEventWonMunich(const qsf::NamedIdentifier& eventIdentifier);
		void setWorldEventWonHamburg(const qsf::NamedIdentifier& eventIdentifier);
		void setWorldEventWonBerlin(const qsf::NamedIdentifier& eventIdentifier);

		/**
		*  @brief
		*    Get number of world events won
		*/
		uint16 getWorldEventsWonMunich() const;
		uint16 getWorldEventsWonHamburg() const;
		uint16 getWorldEventsWonBerlin() const;

		/**
		*  @brief
		*    Get the world event progress for a given map
		*/
		WorldEventProgress& getWorldEventProgress(const MapIdentifier& mapIdentifier);

		/**
		*  @brief
		*    Check if a certain campaign was started at least once
		*/
		bool wasCampaignStarted(uint32 campaignIndex) const;

		/**
		*  @brief
		*    Check if all campaigns were started at least once
		*/
		bool wereAllCampaignsStarted() const;

		/**
		*  @brief
		*    Set campaign started flag
		*/
		void setCampaignStarted(uint32 campaignIndex, bool started = true);

		/**
		*  @brief
		*    Get the highscore manager
		*/
		HighscoreManager& getHighscoreManager();

		//[-------------------------------------------------------]
		//[ Medals                                                ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Unlock the specified medal
		*
		*  @note
		*    - Does nothing if the player already unlocked a better medalType for the given medalIdentifier
		*    - Fires a "MedalUnlocked" message into the message system if a better medal was unlocked
		*/
		void unlockMedal(const qsf::NamedIdentifier& medalIdentifier, MedalType medalType);

		/**
		*  @brief
		*    Triggers an achievement if all needed medals are unlocked
		*
		*  @note
		*    - Unlocks the "silver medal" achievement if the player has unlocked for every event a silver medal or obove
		*    - Unlocks the "gold medal" achievement if the player has unlocked for every event a gold medal
		*/
		void unlockAchievementsOnMedal();

		const Medal* getMedal(const qsf::StringHash& medalIdentifier) const;

		const MedalMap& getMedalMap() const;

		bool isCampaignComplete(uint32 campaignIndex = 0) const;

		//[-------------------------------------------------------]
		//[ Fixed event completion                                ]
		//[-------------------------------------------------------]
		bool isFixedEventPoolCompleted(const MapIdentifier& mapIdentifier) const;
		void setFixedEventPoolCompleted(const MapIdentifier& mapIdentifier, bool completed = true);

		//[-------------------------------------------------------]
		//[ Serialization                                         ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Serialization of the player progress
		*
		*  @exception
		*    Throws "std::exception" in case on an error
		*/
		void serialize(qsf::BinarySerializer& serializer);


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Load player progress from disk ("" = local player)
		*/
		bool loadProgress(const std::string& screenName);

		/**
		*  @brief
		*    Save player progress to disk ("" = local player)
		*/
		bool saveProgress(const std::string& screenName);


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		std::string getAbsoluteProgressFilename(const std::string& screenName) const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Achievements are not part of this class, but managed in the "qsf::game::AchievementSystem"

		// Player progress
		uint32								mTotalScore;				///< The player's current total score
		bool								mTutorialPlayed;			///< If "true", the player has played the tutorial and may access freeplay and multiplay
		NameSet								mMapsUnlocked;				///< Set of maps available for the player
		NameSet								mWonMainEventInCampaign;	///< Hold all main events won in the campaign
		NameSet								mMainEventsUnlocked;		///< Set of main events available for the player
		NameSet								mWonWorldEventsMunich;		///< A map of all won world events in Munich (all game modes)
		NameSet								mWonWorldEventsHamburg;		///< A map of all won world events in Hamburg (all game modes)
		NameSet								mWonWorldEventsBerlin;		///< A map of all won world events in Berlin (all game modes)
		WorldEventProgressMap				mWorldEventProgressList;	///< A map of world event progress per map for the player (for the freeplay game mode)
		MedalMap							mMedalMap;					///< A map of the medals, the player received
		std::unique_ptr<HighscoreManager>	mHighscoreManager;
		boost::container::flat_set<uint32>	mCampaignsStarted;			///< Indices of campaigns started at least once
		bool								mCompletedFixedEventPools[4]; ///< Index is map index (0 for Munich ... 3 for Cologne)


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5


namespace qsf
{
	// serialization specialization
	namespace serialization
	{
		template <>
		struct serializer<em5::PlayerProgress::WorldEventProgress>
		{
			inline static void serialize(BinarySerializer& serializer, em5::PlayerProgress::WorldEventProgress& data)
			{
				serializer & data.mWorldEventsActive;
				serializer & data.mWaitTime;
			}
		};

		template<>
		struct serializer<em5::PlayerProgress::MedalType>
		{
			inline static void serialize(qsf::BinarySerializer& serializer, em5::PlayerProgress::MedalType& value)
			{
				if (serializer.isReading())
				{
					// We only read/write a single byte, but the datatype is four bytes long. So ensure it's initialized correctly.
					value = em5::PlayerProgress::MEDAL_NONE;
					serializer.serializeRawBlock(&value, 1);
				}
				else
				{
					serializer.serializeRawBlock(&value, 1);
				}
			}
		};
	}
}

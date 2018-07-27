// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once

//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/network/matchmaking/MatchmakingTypes.h"

#include <qsf/base/manager/Manager.h>
#include <qsf/base/GetUninitialized.h>
#include <qsf/time/DateTime.h>

#include <boost/container/flat_set.hpp>

#include <map>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Global definitions                                    ]
	//[-------------------------------------------------------]
	struct HighscoreEntry
	{
		uint32 score;
		qsf::DateTime date;
		std::string name;

		inline HighscoreEntry() :
			score(qsf::getUninitialized<uint32>())
		{
			// Nothing to do in here
		}

		inline HighscoreEntry(uint32 score, const qsf::DateTime& date, const std::string& name) :
			score(score),
			date(date),
			name(name)
		{
			// Nothing to do in here
		}

		inline bool operator<(const HighscoreEntry& rhs) const
		{
			return (score < rhs.score || (score == rhs.score && (date < rhs.date || (date == rhs.date && name < rhs.name))));
		}
	};

	struct HighscoreOnlineEntry : HighscoreEntry
	{
		uint32 id;
		uint32 rank;
		std::string token;
	};

	struct HighscoreOnlinePageEntry : HighscoreEntry
	{
		std::string token;
	};

	struct HighscoreOnlinePage
	{
		uint32 total;
		uint32 perPage;
		uint32 currentPage;
		uint32 lastPage;
		uint32 from;
		uint32 to;
		std::vector<HighscoreOnlinePageEntry> entries;
	};


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    EMERGENCY 5 highscore manager class
	*/
	class EM5_API_EXPORT HighscoreManager : public qsf::Manager
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		// The ordering is the same as in MapIdentifier
		enum Map
		{
			MAP_UNKNOWN = -1,
			MAP_MUNICH = 0,
			MAP_HAMBURG = 1,
			MAP_BERLIN = 2,
			MAP_COLOGNE = 3,
		};


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	public:
		static std::string getMainEventRankingId(Map map, uint32 mainEventNumber);
		static std::string getFreeplayEndlessRankingId(Map map);
		static std::string getFreeplayChallengeRankingId(Map map);
		static std::string getMultiplayerRankingId(Map map, em5::multiplayer::GameMode gameMode, uint32 numberOfPlayers, uint32 duration);


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		HighscoreManager();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~HighscoreManager();

		//[-------------------------------------------------------]
		//[ Local highscores                                      ]
		//[-------------------------------------------------------]
		uint32 submitLocalHighscore(const std::string& rankingId, const std::string& name, uint32 score);
		const boost::container::flat_set<HighscoreEntry>& getLocalHighscores(const std::string& rankingId);

		//[-------------------------------------------------------]
		//[ Online highscores                                     ]
		//[-------------------------------------------------------]
		/*
		*  @return
		*    Rank
		*/
		uint32 submitOnlineHighscore(const std::string& rankingId, const std::string& name, uint32 score, const std::string& group = "");

		bool getOnlineHighscorePage(const std::string& rankingId, uint32 pageIndex, HighscoreOnlinePage& output);

		bool getOnlineHighscoreSingleUserEntry(const std::string& rankingId, const std::string& name, HighscoreOnlineEntry& output);


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		boost::container::flat_set<HighscoreEntry>& getLocalHighscoreSet(const std::string& rankingId);
		bool saveLocalHighscore(const std::string& rankingId, const boost::container::flat_set<HighscoreEntry>& entries);
		bool loadLocalHighscore(const std::string& rankingId, boost::container::flat_set<HighscoreEntry>& entries);
		void serialize(qsf::BinarySerializer& serializer, boost::container::flat_set<HighscoreEntry>& entries);

		// Older version support
		void loadLocalHighscoreVersion1(const std::string& rankingId, boost::container::flat_set<HighscoreEntry>& entries);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		std::map<uint32, boost::container::flat_set<HighscoreEntry>> mLocalHighscores;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

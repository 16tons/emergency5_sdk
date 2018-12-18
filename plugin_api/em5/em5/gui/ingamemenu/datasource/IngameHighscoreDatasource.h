// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <Rocket/Controls/DataSource.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace em5
{
	struct HighscoreOnlinePage;
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
	*    Ingame highscore datasource
	*/
	class IngameHighscoreDatasource : public Rocket::Controls::DataSource
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		IngameHighscoreDatasource();
		virtual ~IngameHighscoreDatasource();

		/**
		*  @brief
		*    Setup datasource for which highscore type data should be fetched
		*/
		void setupFreeplayEndlessHighscores();
		void setupFreeplayChallengeHighscores();
		void setupMainEventHighscores(uint32 eventId);
		void setupMultiplayerCOOPHighscores();
		void setupMultiplayerPVPHighscores();

		/**
		*  @brief
		*    Let the datasource fetch the highscore data from server
		*
		*  @param[in] currentRank:
		*    The rank for which the data should be fetched
		*
		*  @return
		*    True on success, False on failure
		*/
		bool fetch(const uint32 currentRank);

		int getRowIndexForRank(uint32 rank) const;
		uint32 getMaxRank() const;


	//[-------------------------------------------------------]
	//[ Public virtual Rocket::Controls::DataSource methods   ]
	//[-------------------------------------------------------]
	public:
		virtual void GetRow(Rocket::Core::StringList& row, const Rocket::Core::String& table, int rowIndex, const Rocket::Core::StringList& columns) override;
		virtual int GetNumRows(const Rocket::Core::String& table) override;


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		// This struct represents one row in the highscore table
		struct TableData
		{
			uint32 rankAsNumber;
			Rocket::Core::String rank;
			Rocket::Core::String name;
			Rocket::Core::String points;
		};


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		bool buildTableData(uint32 entryId, const std::string& highscoreId);

		void addDataToTable(const HighscoreOnlinePage& hiscorePage, uint32 firstRank, uint32 lastRank);
		void addSpecialRow();
		void addTopTenItems(const HighscoreOnlinePage& highscorePage);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		std::vector<TableData*> mTableData;
		std::string				mHighscoreId;
		uint32					mMaxRank;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

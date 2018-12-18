// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/game/gamemode/GameModeType.h"

#include <Rocket/Controls/DataSource.h>


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
	*    Load campaign data source
	*/
	class LoadCampaignDataSource : public Rocket::Controls::DataSource
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		LoadCampaignDataSource();
		virtual ~LoadCampaignDataSource();
		void addNewSaveGame();
		void holdNewSaveGame(const Rocket::Core::String& filename);
		void removeNewSaveGame();
		void removeSaveGameByIndex(uint32 index);
		const Rocket::Core::String& getCompleteFileNameByIndex(uint32 index) const;
		const Rocket::Core::String& getFileNameByIndex(uint32 index) const;
		void rebuidFirstRows();
		void rebuidDatagrid();


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
		struct TableData
		{
			gamemode::Type		 gameMode;
			uint32				 campaignIndex;
			Rocket::Core::String score;
			Rocket::Core::String date;
			Rocket::Core::String time;
			Rocket::Core::String file; // This is our filename
			Rocket::Core::String newFile;
			std::string completeTimeString;

			struct sortDate
			{
				bool operator() (TableData* a, TableData* b)
				{
					return (a->completeTimeString > b->completeTimeString);
				}
			};
		};


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		std::vector<TableData*> mTableData;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

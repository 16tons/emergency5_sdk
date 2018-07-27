// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/network/matchmaking/MatchmakingTypes.h"

#include <qsf/asset/AssetProxy.h>

#include <Rocket/Controls/DataSource.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace em5
{
	class MatchmakingClient;
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
	*    Multiplayer server data source
	*/
	class MultiplayerServerDataSource : public Rocket::Controls::DataSource
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		enum SortColumn
		{
			SORT_LOCKED = 0,
			SORT_NAME,
			SORT_COUNT,
			SORT_MAP,
			SORT_MODE,
			SORT_DURATION,
			SORT_LATENCY,
			_NUM_ITEMS
		};


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		explicit MultiplayerServerDataSource();
		~MultiplayerServerDataSource();

		/**
		*  @brief
		*    Sort the table vector
		*/
		void sortTable(SortColumn sortColumn);

		/**
		*  @brief
		*    Connect/Disconnect to the server
		*/
		bool connectToMatchmakingServer();
		void disconnectFromMatchmakingServer(bool destroy = false);

		/**
		*  @brief
		*    Update the list
		*/
		void updateHostList();

		const multiplayer::HostEntry& getHostEntryFromRowIndex(int rowIndex) const;


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
		/**
		*  @brief
		*    TableData structure to store the server information for the gui
		*/
		struct TableData
		{
			Rocket::Core::String	locked;
			Rocket::Core::String	name;
			Rocket::Core::String	server;
			Rocket::Core::String	count;
			Rocket::Core::String	maxcount;
			Rocket::Core::String	mapName;
			Rocket::Core::String	mode;
			Rocket::Core::String	duration;
			Rocket::Core::String	latency;
			size_t					hostEntryIndex;

			// Static sorting functions
			static bool compareBools(Rocket::Core::String& a, Rocket::Core::String& b)
			{
				return (b == "true");
			}

			static bool compareStrings(Rocket::Core::String& a, Rocket::Core::String& b)
			{
				return a.ToLower() < b.ToLower();
			}

			static bool compareNumbers(Rocket::Core::String& a, Rocket::Core::String& b)
			{
				try
				{
					return std::stoi(a.CString()) < std::stoi(b.CString());
				}
				catch (boost::bad_lexical_cast)
				{
					return true;
				}
			}

			// Functors for sorting (less)
			struct sortLocked
			{
				bool operator() (TableData* a, TableData* b)
				{
					return compareBools(a->locked, b->locked);
				}
			};

			struct sortName
			{
				bool operator() (TableData* a, TableData* b)
				{
					return compareStrings(a->name, b->name);
				}
			};

			struct sortCount
			{
				bool operator() (TableData* a, TableData* b)
				{
					return compareNumbers(a->count, b->count);
				}
			};

			struct sortMap
			{
				bool operator() (TableData* a, TableData* b)
				{
					return compareStrings(a->mapName, b->mapName);
				}
			};

			struct sortMode
			{
				bool operator() (TableData* a, TableData* b)
				{
					return compareNumbers(a->mode, b->mode);
				}
			};

			struct sortDuration
			{
				bool operator() (TableData* a, TableData* b)
				{
					return compareNumbers(a->duration, b->duration);
				}
			};

			struct sortLatency
			{
				bool operator() (TableData* a, TableData* b)
				{
					return compareNumbers(a->latency, b->latency);
				}
			};
		};


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		// MatchmakingClient signals
		void setConnected();
		void setConnectFailed();
		void setConnectionLost();
		void setHostList(const std::vector<multiplayer::HostEntry>& hostEntries);

		/**
		*  @brief
		*    Clear the vector and the table entries
		*/
		void deleteHostListEntries();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		std::vector<TableData*>				mTableData;
		std::vector<multiplayer::HostEntry>	mHostEntryList;
		SortColumn							mSortColumn;
		std::shared_ptr<MatchmakingClient>	mMatchmakingClient;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

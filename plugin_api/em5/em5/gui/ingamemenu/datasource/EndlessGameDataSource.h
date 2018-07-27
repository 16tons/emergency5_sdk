// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
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
	*    Endless game data source
	*/
	class EndlessGameDataSource : public Rocket::Controls::DataSource
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		EndlessGameDataSource();
		virtual ~EndlessGameDataSource();


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
			Rocket::Core::String rank;
			Rocket::Core::String name;
			Rocket::Core::String points;
		};


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		std::vector<TableData*>	mTableData;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <Rocket/Core/Element.h>
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
	*    Account status title data source
	*/
	class AccountStatusTitleDataSource : public Rocket::Controls::DataSource
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		explicit AccountStatusTitleDataSource();
		~AccountStatusTitleDataSource();

		void setup();


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
			Rocket::Core::String icon;
			Rocket::Core::String name;
			Rocket::Core::String locked;
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

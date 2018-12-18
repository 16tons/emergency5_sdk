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
	class Mod;
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
	*    ModificationWindowDataSource
	*/
	class ModificationWindowDataSource : public Rocket::Controls::DataSource
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		struct TableData
		{
			Rocket::Core::String modName;
			Rocket::Core::String activated;
			Rocket::Core::String modification;
			Rocket::Core::String description;
			Rocket::Core::String image;
			uint32 modOrderIndex;
		};


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		explicit ModificationWindowDataSource();
		~ModificationWindowDataSource();

		void insertNewRow(const Rocket::Core::String& table, TableData* data);
		void restoreTable(const Rocket::Core::String& table);
		void changeIsActivated(uint32 modID, const Rocket::Core::String& table);

		void pushModUp(const Rocket::Core::String& modName);
		void pushModDown(const Rocket::Core::String& modName);

		void recollectTableData();
		void saveOrder();

		void updateAfterModsAdded();
		void updateAfterModRemoved(const std::string& modName);

		TableData* getFirstTableData() { return mTableDataModifications[0]; };


	//[-------------------------------------------------------]
	//[ Public virtual Rocket::Controls::DataSource methods   ]
	//[-------------------------------------------------------]
	public:
		virtual void GetRow(Rocket::Core::StringList& row, const Rocket::Core::String& table, int rowIndex, const Rocket::Core::StringList& columns) override;
		virtual int GetNumRows(const Rocket::Core::String& table) override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		TableData* createTableDataForMod(const Mod& mod);
		void deleteModificationTableData();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		std::vector<TableData*> mTableDataModifications;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

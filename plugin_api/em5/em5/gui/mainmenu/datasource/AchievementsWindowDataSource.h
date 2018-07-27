// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/game/achievement/Achievement.h"

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
	*    Achievements window data source
	*/
	class AchievementsWindowDataSource : public Rocket::Controls::DataSource
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		struct TableData
		{
			Rocket::Core::String icon;
			Rocket::Core::String name;
			float				 progress;
			float				 neededprogress;
			Rocket::Core::String description;
			uint32				 points;
			Rocket::Core::String unlocked;
			uint32				 campaignIndex;
		};


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		explicit AchievementsWindowDataSource();
		~AchievementsWindowDataSource();

		uint32 getUnlocked();
		void setupAbstract();
		void setupGeneral();
		void setupMainEvent();
		void setupFreeplay();
		void setupChallenge();
		void setupMultiplayCOOP();
		void setupMultiplayAgainst();


	//[-------------------------------------------------------]
	//[ Public virtual Rocket::Controls::DataSource methods   ]
	//[-------------------------------------------------------]
	public:
		virtual void GetRow(Rocket::Core::StringList& row, const Rocket::Core::String& table, int rowIndex, const Rocket::Core::StringList& columns) override;
		virtual int GetNumRows(const Rocket::Core::String& table) override;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		void setupInternal(bool allAchievementPlaces, Achievement::AchievementPlace achievementPlace);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		std::vector<TableData*> mTableData;
		uint32					mUnlocked;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

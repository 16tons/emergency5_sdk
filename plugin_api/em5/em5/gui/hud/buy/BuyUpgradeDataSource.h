// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/game/player/Player.h"

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
	*    Data grid used to display the current buyable upgrades
	*/
	class BuyUpgradeDataSource : public Rocket::Controls::DataSource
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		BuyUpgradeDataSource();

		/**
		*  @brief
		*    Add a new buyable upgrade to table
		*/
		void addBuyableUpgrades(const Player::BuyOption& buyOptions, uint32 upgradeID);

		/**
		*  @brief
		*    Clears the datagrid; this seems to be necessary in order to shut libRocket down properly
		*/
		void clear();


	//[-------------------------------------------------------]
	//[ Public virtual Rocket::Controls::DataSource methods   ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*	 Includes the "columns" StringList "title" you going to get the title.
		*	 Includes the "columns" StringList "icons" you going to get the icon.
		*	 Includes the "columns" StringList "info" you going to get the info text.
		*	 Includes the "columns" StringList "cost" you going to get the cost.
		*	 Includes the "columns" StringList "upgradeId" you going to get the upgrade ID.
		*/
		virtual void GetRow(Rocket::Core::StringList& row, const Rocket::Core::String& table, int rowIndex, const Rocket::Core::StringList& columns) override;
		virtual int GetNumRows(const Rocket::Core::String& table) override;


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		struct BuyableUpgradeTableData
		{
			Rocket::Core::String mTitle;
			Rocket::Core::String mIcon;
			Rocket::Core::String mInfoText;
			uint32		mCost;
			uint32		mUpgradeId;

			BuyableUpgradeTableData(const Rocket::Core::String& title, const Rocket::Core::String& icon, const Rocket::Core::String& infoText, uint32 cost, uint32 upgradeId) :
				mTitle(title),
				mIcon(icon),
				mInfoText(infoText),
				mCost(cost),
				mUpgradeId(upgradeId)
			{
				// Nothing
			}
		};
		typedef std::vector<BuyableUpgradeTableData> BuyableUpgradeTableDataArray;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		BuyableUpgradeTableDataArray mTableData;	///< The Information our datagrid will use to fill itself


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

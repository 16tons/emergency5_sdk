// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline uint32 CampaignProgress::getCampaignIndex() const
	{
		return mCampaignIndex;
	}

	inline uint32 CampaignProgress::getCurrentCredits() const
	{
		return mCurrentCredits;
	}

	inline void CampaignProgress::setCurrentCredits(uint32 creditsAmount)
	{
		mCurrentCredits = creditsAmount;
	}

	inline uint32 CampaignProgress::getCurrentMapIndex() const
	{
		return mCurrentMapIndex;
	}

	inline void CampaignProgress::setCurrentMapIndex(uint32 mapIndex)
	{
		mCurrentMapIndex = mapIndex;
	}

	inline uint32 CampaignProgress::getCurrentPhaseIndex() const
	{
		return mCurrentPhaseIndex;
	}

	inline void CampaignProgress::setCurrentPhaseIndex(uint32 phaseIndex)
	{
		mCurrentPhaseIndex = phaseIndex;
	}

	inline const qsf::NamedIdentifier& CampaignProgress::getLastMainEvent() const
	{
		return mLastMainEvent;
	}

	inline void CampaignProgress::setLastMainEvent(const qsf::NamedIdentifier& mainevent)
	{
		mLastMainEvent = mainevent;
	}

	inline int32 CampaignProgress::getLastStandardEventIndex() const
	{
		return mLastStandardEventIndex;
	}

	inline void CampaignProgress::setLastStandardEventIndex(int32 eventIndex)
	{
		mLastStandardEventIndex = eventIndex;
	}

	inline void CampaignProgress::clearBoughtUnits()
	{
		mBoughtUnits.clear();
	}

	inline void CampaignProgress::addBoughtUnit(const std::string& unitName)
	{
		mBoughtUnits.push_back(unitName);
	}

	inline const std::vector<std::string>& CampaignProgress::getBoughtUnitList()
	{
		return mBoughtUnits;
	}

	inline void CampaignProgress::clearBoughtUpgrades()
	{
		mBoughtUpgrades.clear();
	}

	inline void CampaignProgress::addBoughtUpgrade(const std::string& upgradeName)
	{
		mBoughtUpgrades.push_back(upgradeName);
	}

	inline const std::vector<std::string>& CampaignProgress::getBoughtUpgradeList()
	{
		return mBoughtUpgrades;
	}

	inline uint32 CampaignProgress::getFinishedFreeplayEventsCount() const
	{
		return mFinishedFreeplayEvents;
	}

	inline void CampaignProgress::setFinishedFreeplayEventsCount(uint32 number)
	{
		mFinishedFreeplayEvents = number;
	}

	inline const CampaignDefinition& CampaignProgress::getCampaignDefinition() const
	{
		return *mCampaignDefinition;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

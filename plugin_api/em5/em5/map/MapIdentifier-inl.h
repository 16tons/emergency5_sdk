// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline bool MapIdentifier::isOriginalMap() const
	{
		return (mIndex >= 0);
	}

	inline int32 MapIdentifier::getIndex() const
	{
		return mIndex;
	}

	inline const std::string& MapIdentifier::getIdentifier() const
	{
		return mIdentifier;
	}

	inline const std::string& MapIdentifier::getLocalAssetName() const
	{
		return mLocalAssetName.getName();
	}

	inline const MainEventIdentifier& MapIdentifier::getSecondLastMainEventIdentifier() const
	{
		return mSecondLastMainEventIdentifier;
	}

	inline const std::string& MapIdentifier::getWorldEventEventPools() const
	{
		return mWorldEventEventPools;
	}

	inline const std::vector<std::string>& MapIdentifier::getSeasonalEvents() const
	{
		return mSeasonalEvents;
	}

	inline bool MapIdentifier::operator==(const MapIdentifier& other) const
	{
		return (mLocalAssetName == other.mLocalAssetName);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

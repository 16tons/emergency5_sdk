// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	inline Player* PlayerManager::getLocalPlayer()
	{
		return mLocalPlayer;
	}

	inline Player& PlayerManager::getLocalPlayerSafe()
	{
		QSF_CHECK(nullptr != mLocalPlayer, "Local player instance is invalid", QSF_REACT_THROW);
		return *mLocalPlayer;
	}


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline PlayerManager::PlayerManager() :
		mWasDeserialized(false)
	{
		// Nothing here
	}

	inline PlayerManager::~PlayerManager()
	{
		// Nothing here
	}

	inline Player* PlayerManager::getPlayerByIndex(uint32 playerIndex) const
	{
		// Is the player in the list?
		PlayerMap::const_iterator iterator = mPlayers.find(playerIndex);
		if (iterator != mPlayers.end())
		{
			// Found him
			return iterator->second;
		}
		else
		{
			// None found
			return nullptr;
		}
	}

	inline const PlayerManager::PlayerList& PlayerManager::getAllPlayers() const
	{
		return mPlayerList;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

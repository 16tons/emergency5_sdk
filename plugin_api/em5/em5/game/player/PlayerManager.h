// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/game/player/Player.h"			// Not directly needed, but whoever needs the manager will probably also need the player class included

#include <qsf/base/manager/Manager.h>

#include <boost/container/flat_map.hpp>

#include <vector>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	class BinarySerializer;
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
	*    EMERGENCY 5 player manager
	*/
	class EM5_API_EXPORT PlayerManager : public qsf::Manager
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		typedef boost::container::flat_map<uint32, Player*> PlayerMap;	///< Maps player indices to player instances
		typedef std::vector<Player*> PlayerList;						///< Holds a list of players (no special ordering guaranteed)


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Return the instance of the local player
		*/
		inline static Player* getLocalPlayer();

		/**
		*  @brief
		*    Return the instance of the local player
		*/
		inline static Player& getLocalPlayerSafe();


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		inline PlayerManager();

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~PlayerManager();

		/**
		*  @brief
		*    Start up the player manager
		*/
		void startup();

		/**
		*  @brief
		*    Shutdown the player manager
		*/
		void shutdown();

		/**
		*  @brief
		*    Return a player by his index, or null pointer of there is none with this index
		*
		*  @return
		*    Pointer to the respective player or null pointer if there is no player with that index
		*/
		inline Player* getPlayerByIndex(uint32 playerIndex) const;

		/**
		*  @brief
		*    Return a list of all players
		*
		*  @return
		*    The list of all players
		*/
		inline const PlayerList& getAllPlayers() const;

		/**
		*  @brief
		*    Reset the blocking of the unit pool
		*/
		void unblockAllPlayersUnitPool();

		/**
		*  @brief
		*    Serialization of the timer
		*/
		void serialize(qsf::BinarySerializer& serializer);


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void destroyAllPlayers();


	//[-------------------------------------------------------]
	//[ Private static data                                   ]
	//[-------------------------------------------------------]
	private:
		static Player* mLocalPlayer;	///< Local player; can be a null pointer if there is no local player at all


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		PlayerMap	mPlayers;		///< Map of players in this session
		PlayerList	mPlayerList;	///< List of all players, this list is only used to speedup iteration over all players

		bool		mWasDeserialized;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "em5/game/player/PlayerManager-inl.h"

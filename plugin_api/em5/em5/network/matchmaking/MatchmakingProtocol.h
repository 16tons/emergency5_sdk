// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/network/matchmaking/MatchmakingProtocolBase.h"
#include "em5/network/matchmaking/MatchmakingTypes.h"


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
	*    Matchmaking protocol class
	*/
	class EM5_API_EXPORT MatchmakingProtocol : public MatchmakingProtocolBase
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		MatchmakingProtocol(QsfProtocol* parent, MatchmakingClient* client);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~MatchmakingProtocol();

		void requestRegisterHost(const multiplayer::HostEntry& hostEntry, const std::string& gameModificationString) const;

		void requestTestOpenPort() const;

		void updateHost(const multiplayer::HostEntry& hostEntry) const;

		void unregisterHost() const;

		void requestHostList(const std::string& gameModificationString) const;

		void requestProxyServerProperties() const;


	//[-------------------------------------------------------]
	//[ Public virtual em5::MatchmakingProtocol methods       ]
	//[-------------------------------------------------------]
	public:
		virtual void handlePacket(const qsf::packet::BinaryPacketBase& packet) override;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		MatchmakingClient* mClient;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

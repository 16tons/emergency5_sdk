// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"

#include <qsf/asset/AssetSystemTypes.h>
#include <qsf/base/GetUninitialized.h>
#include <qsf/reflection/object/Object.h>

#include <boost/algorithm/string.hpp>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{
	namespace multiplayer
	{


		//[-------------------------------------------------------]
		//[ Global definitions                                    ]
		//[-------------------------------------------------------]
		enum RegisterHostStatus ///< Connected to CAMP
		{
			REGISTER_SUCCESS,
			REGISTER_PORT_NOT_OPEN,
			REGISTER_UNKNOWN_ERROR = 255
		};

		enum GameMode ///< Connected to CAMP
		{
			GAMEMODE_PVP,
			GAMEMODE_COOP
		};

		struct HostEntry : public qsf::Object
		{
			// If changing this struct, make sure to update the CAMP metaclass registration as well
			bool				hasPassword;		///< "true" is the game session is password protected, "false" otherwise
			std::string			sessionName;		///< The name of the game session
			uint32				maximumPlayerCount;	///< Maximum number of players in this game session
			uint32				currentPlayerCount;	///< Current number of connected players in this game session
			qsf::GlobalAssetId	globalMapAssetId;	///< Global asset ID of the map the game session will be played on
			GameMode			gameMode;			///< Game mode that will be played in this game session
			uint32				duration;			///< Duration of the game session in minutes
			bool				playerSeparation;	///< If set, players can't influence each other (referring to AI)
			uint64				proxySessionId;		///< Holds the proxy session id when the host is connected to a multiplayer proxy and has created a session
			std::string			proxyHostname;		///< The hostname of the multiplayer proxy to which the host is connected to
			uint16				proxyPort;			///< The port of the multiplayer proxy to which the host is connected to

			// Internal
			std::string	hostname;
			uint16		port;
			std::string	ipv6_address;

			// Pure internal do not serialize it
			std::vector<std::string> ipv6_adressList;

			inline HostEntry() :
				hasPassword(false),
				maximumPlayerCount(qsf::getUninitialized<uint32>()),
				currentPlayerCount(qsf::getUninitialized<uint32>()),
				globalMapAssetId(qsf::getUninitialized<qsf::GlobalAssetId>()),
				gameMode(qsf::getUninitialized<GameMode>()),
				duration(qsf::getUninitialized<uint32>()),
				playerSeparation(true),
				proxySessionId(qsf::getUninitialized<uint64>()),
				proxyPort(qsf::getUninitialized<uint16>()),
				port(qsf::getUninitialized<uint16>())
			{
				// Nothing to do in here
			}

			inline HostEntry(const HostEntry& x)
			{
				operator=(x);
			}

			inline HostEntry& operator=(const HostEntry& x)
			{
				hasPassword		   = x.hasPassword;
				sessionName		   = x.sessionName;
				maximumPlayerCount = x.maximumPlayerCount;
				currentPlayerCount = x.currentPlayerCount;
				globalMapAssetId   = x.globalMapAssetId;
				gameMode		   = x.gameMode;
				duration		   = x.duration;
				playerSeparation   = x.playerSeparation;
				proxySessionId	   = x.proxySessionId;
				proxyHostname      = x.proxyHostname;
				proxyPort          = x.proxyPort;
				hostname		   = x.hostname;
				port			   = x.port;
				ipv6_address       = x.ipv6_address;

				splitIPv6Addresses();
				return *this;
			}

			inline void splitIPv6Addresses()
			{
				ipv6_adressList.clear();
				if (!ipv6_address.empty())
				{
					boost::split(ipv6_adressList, ipv6_address, boost::is_any_of(","));
				}
			}


			//[-------------------------------------------------------]
			//[ CAMP reflection system                                ]
			//[-------------------------------------------------------]
			QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // multiplayer
} // em5


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE(em5::multiplayer::RegisterHostStatus)
QSF_CAMP_TYPE(em5::multiplayer::GameMode)
QSF_CAMP_TYPE(em5::multiplayer::HostEntry)

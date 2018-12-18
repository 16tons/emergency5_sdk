// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/network/Server.h"
#include "qsf/network/layered/QsfProtocol.h"

#include <boost/container/flat_map.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class QsfServerConnection;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Abstract network server class
	*/
	class QSF_API_EXPORT QsfServer : public Server
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		inline QsfServer();

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~QsfServer();

		void disconnect(uint32 clientId);


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Server methods                    ]
	//[-------------------------------------------------------]
	public:
		virtual void onClientConnected(uint32 clientId) override;
		virtual void onClientDisconnected(uint32 clientId) override;
		virtual void onPacketReceived(uint32 clientId, const std::vector<char>& packet) override;


	//[-------------------------------------------------------]
	//[ Public virtual qsf::QsfServer methods                 ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Returns a list of protocols that are implemented in this server
		*/
		virtual const QsfProtocolList getProtocols(uint32 clientId) const = 0;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		boost::container::flat_map<uint32, QsfServerConnection*> mConnections;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/network/layered/server/QsfServer-inl.h"

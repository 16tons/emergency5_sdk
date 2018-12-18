// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/Export.h"

#include <boost/function.hpp>
#include <boost/noncopyable.hpp>

#include <vector>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class QsfProtocol;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Type definitions                                      ]
	//[-------------------------------------------------------]
	typedef uint32 QsfProtocolId;
	typedef std::vector<QsfProtocolId> QsfProtocolIdList;

	typedef QsfProtocol* QsfProtocolGenerator(QsfProtocol*);
	typedef std::vector<std::pair<QsfProtocolId,boost::function<QsfProtocolGenerator> > > QsfProtocolList;


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Abstract network protocol class
	*/
	class QSF_API_EXPORT QsfProtocol : public boost::noncopyable
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		inline QsfProtocol(QsfProtocol* parent);

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~QsfProtocol();


	//[-------------------------------------------------------]
	//[ Public virtual qsf::QsfProtocol methods               ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Commands the protocol to close its connection.
		*
		*  @note
		*    - Call the parent implementation if the disconnect should actually be performed on the network layer.
		*/
		virtual void disconnect();

		/**
		*  @brief
		*    Commands the protocol to wrap up the given packet data and send it to the connected peer.
		*
		*  @param[in] packet
		*    Packet data to send to the peer
		*
		*  @return
		*    "true" on success, "false" otherwise
		*
		*  @note
		*    - Always call the parent implementations to pass the data on
		*/
		virtual bool sendPacket(std::vector<char>& packet) const;

		/**
		*  @brief
		*    Called when the network connection with the peer has been successfully established.
		*
		*  @note
		*    - Always call the parent implementation
		*/
		virtual void onConnected();

		/**
		*  @brief
		*    Called when the network connection with the peer was closed.
		*
		*  @note
		*    - Always call the parent implementation
		*/
		virtual void onDisconnected();

		/**
		*  @brief
		*    Called whenever a packet has been received from the peer.
		*
		*  @param[in] packet
		*    Packet data that was received from the peer
		*/
		virtual void onReceivePacket(const std::vector<char>& packet) = 0;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		QsfProtocol* mParent;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/network/layered/QsfProtocol-inl.h"

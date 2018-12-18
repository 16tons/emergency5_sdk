// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/network/layered/QsfConnection.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class QsfServer;
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
	*    Network server connection class
	*/
	class QSF_API_EXPORT QsfServerConnection : public QsfConnection
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		inline QsfServerConnection(QsfServer* server, uint32 clientId);

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~QsfServerConnection();


	//[-------------------------------------------------------]
	//[ Public virtual qsf::QsfProtocol methods               ]
	//[-------------------------------------------------------]
	public:
		virtual void disconnect() override;
		virtual bool sendPacket(std::vector<char>& packet) const override;
		virtual void onConnected() override;
		virtual void onDisconnected() override;
		virtual void onReceivePacket(const std::vector<char>& packet) override;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::QsfConnection methods          ]
	//[-------------------------------------------------------]
	protected:
		virtual const QsfProtocolList getProtocols() const override;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		QsfServer* mServer;
		uint32 mClientId;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/network/layered/server/QsfServerConnection-inl.h"

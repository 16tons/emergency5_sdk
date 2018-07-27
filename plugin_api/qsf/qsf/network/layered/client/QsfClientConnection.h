// Copyright (C) 2012-2017 Promotion Software GmbH


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
	class QsfClient;
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
	*    Network client connection class
	*/
	class QSF_API_EXPORT QsfClientConnection : public QsfConnection
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		inline explicit QsfClientConnection(QsfClient* client);

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~QsfClientConnection();


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
		QsfClient* mClient;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/network/layered/client/QsfClientConnection-inl.h"

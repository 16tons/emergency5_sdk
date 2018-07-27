// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/network/Client.h"
#include "qsf/network/layered/client/QsfClientConnection.h"


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
	*    Abstract network client class
	*/
	class QSF_API_EXPORT QsfClient : public Client
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		inline QsfClient();

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~QsfClient();


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Client methods                    ]
	//[-------------------------------------------------------]
	public:
		virtual void onConnected() override;
		virtual void onDisconnected() override;
		virtual void onPacketReceived(const std::vector<char>& packet) override;


	//[-------------------------------------------------------]
	//[ Public virtual qsf::QsfClient methods                 ]
	//[-------------------------------------------------------]
	public:
		virtual const QsfProtocolList& getProtocols() const = 0;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		QsfClientConnection mConnection;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/network/layered/client/QsfClient-inl.h"

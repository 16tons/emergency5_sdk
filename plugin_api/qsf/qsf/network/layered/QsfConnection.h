// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/network/layered/QsfProtocol.h"


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
	*    Abstract network connection class
	*/
	class QSF_API_EXPORT QsfConnection : public QsfProtocol
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		inline QsfConnection();

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~QsfConnection();


	//[-------------------------------------------------------]
	//[ Public virtual qsf::QsfProtocol methods               ]
	//[-------------------------------------------------------]
	public:
		virtual void disconnect() override = 0;
		virtual bool sendPacket(std::vector<char>& packet) const override = 0;
		virtual void onConnected() override = 0;
		virtual void onDisconnected() override = 0;
		virtual void onReceivePacket(const std::vector<char>& packet) override = 0;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::QsfConnection methods          ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Returns a list of protocols that are supported for this connection
		*/
		virtual const QsfProtocolList getProtocols() const = 0;


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		QsfProtocol* mProtocol;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/network/layered/QsfConnection-inl.h"

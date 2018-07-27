// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/network/layered/packet/QsfPacket.h"
#include "qsf/network/layered/QsfProtocol.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace packet
	{


		//[-------------------------------------------------------]
		//[ Classes                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Network client handshake class
		*/
		class QSF_API_EXPORT ClientHandshake : public QsfPacket
		{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Default constructor
			*/
			inline ClientHandshake();

			/**
			*  @brief
			*    Destructor
			*/
			inline virtual ~ClientHandshake();

			inline void setSelectedProtocolId(QsfProtocolId protocolId);

			inline QsfProtocolId getSelectedProtocolId() const;


		//[-------------------------------------------------------]
		//[ Public virtual methods                                ]
		//[-------------------------------------------------------]
		public:
			virtual bool receive(const void* data, size_t size) override;
			virtual bool send(QsfProtocol* transport) const override;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			QsfProtocolId mSelectedProtocolId;


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // packet
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/network/layered/packet/ClientHandshake-inl.h"

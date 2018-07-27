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
		*    Network handshake class
		*/
		class QSF_API_EXPORT ServerHandshake : public QsfPacket
		{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Default constructor
			*/
			inline ServerHandshake();

			/**
			*  @brief
			*    Destructor
			*/
			inline virtual ~ServerHandshake();

			inline const QsfProtocolIdList& getProtocolIds() const;

			inline void setProtocolIds(const QsfProtocolList& protocolIds);
			inline void setProtocolIds(const QsfProtocolIdList& protocols);


		//[-------------------------------------------------------]
		//[ Public virtual qsf::QsfPacket methods                 ]
		//[-------------------------------------------------------]
		public:
			virtual bool receive(const void* data, size_t size) override;
			virtual bool send(QsfProtocol* transport) const override;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			QsfProtocolIdList mProtocolIds;


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // packet
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/network/layered/packet/ServerHandshake-inl.h"

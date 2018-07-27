// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"
#include "em5/network/matchmaking/MatchmakingTypes.h"

#include <qsf/network/layered/packet/BinaryPacket.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{
	namespace packet
	{


		//[-------------------------------------------------------]
		//[ Classes                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    TODO(co) Comment me
		*/
		class EM5_API_EXPORT ProxyServerPropertiesRequest : public qsf::packet::BinaryPacket<ProxyServerPropertiesRequest>
		{


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			static const uint32 PACKET_ID;


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Default constructor
			*/
			ProxyServerPropertiesRequest();


		//[--------------------------------------------------------]
		//[ Public virtual em5::packet::SerializedPacket methods   ]
		//[--------------------------------------------------------]
		public:
			virtual void serialize(qsf::BinarySerializer& serializer) override;


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // packet
} // em5

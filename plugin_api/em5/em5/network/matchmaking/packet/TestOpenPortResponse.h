// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"

#include <qsf/network/layered/packet/BinaryPacket.h>
#include <qsf/serialization/binary/StlTypeSerialization.h>


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
		*    This class represents a network package sent by the server as a response to the TestOpenPortRequest, telling the client (=game host) if it is reachable from the internet or not
		*/
		class EM5_API_EXPORT TestOpenPortResponse : public qsf::packet::BinaryPacket<TestOpenPortResponse>
		{


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			static const uint32 PACKET_ID;	///< StringHash of the string "em5::packet::TestOpenPortResponse"

			struct ReachableAddress
			{
				std::string address;
				bool isIpv4;
			};

			struct ResultSet
			{
				bool testedIPv4;
				bool testedIPv6;
				std::vector<ReachableAddress> reachableAddresses;
			};


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Default constructor
			*/
			TestOpenPortResponse();

			const ResultSet& getResultSet() const;
			void setResultSet(const ResultSet& resultSet);

			bool getOverallResult() const;


		//[--------------------------------------------------------]
		//[ Public virtual em5::packet::SerializedPacket methods   ]
		//[--------------------------------------------------------]
		public:
			virtual void serialize(qsf::BinarySerializer& serializer) override;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			bool mOverallResult; ///< Holds information if any connection test was successfull.
			ResultSet mResult;


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // packet
} // em5

namespace qsf
{
	namespace serialization
	{
		// Serialization specialization for TestOpenPortResponse::ReachableAddress for use with the QSF binary serialization
		template<>
		struct serializer<em5::packet::TestOpenPortResponse::ReachableAddress>
		{
			inline static void serialize(BinarySerializer& serializer, em5::packet::TestOpenPortResponse::ReachableAddress& value)
			{
				serializer & value.address;
				serializer & value.isIpv4;
			}
		};

		// Serialization specialization for TestOpenPortResponse::ResultSet for use with the QSF binary serialization
		template<>
		struct serializer<em5::packet::TestOpenPortResponse::ResultSet>
		{
			inline static void serialize(BinarySerializer& serializer, em5::packet::TestOpenPortResponse::ResultSet& value)
			{
				serializer & value.testedIPv4;
				serializer & value.testedIPv6;
				serializer & value.reachableAddresses;
			}
		};
	}
}

// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace packet
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		inline BinaryPacketBase::BinaryPacketBase()
		{
			// Nothing to do in here
		}

		inline BinaryPacketBase::~BinaryPacketBase()
		{
			// Nothing to do in here
		}

		inline void BinaryPacketBase::deserialize(const BinarySerializer& serializer)
		{
			// This method is just a wrapper with correct constness (the following call will not modify the provided serializer)
			serialize(const_cast<BinarySerializer&>(serializer));
		}

		inline void BinaryPacketBase::serialize(BinarySerializer& serializer) const
		{
			// This method is just a wrapper with correct constness (the following call will not modify the this packet)
			const_cast<BinaryPacketBase&>(*this).serialize(serializer);
		}


		//[--------------------------------------------------------]
		//[ Public virtual qsf::packet::BinaryPacketBase methods   ]
		//[--------------------------------------------------------]
		template<class PacketClass>
		inline uint32 BinaryPacket<PacketClass>::getPacketId() const
		{
			return PacketClass::PACKET_ID;
		}

		inline void BinaryPacketBase::serialize(qsf::BinarySerializer& serializer)
		{
			// Nothing to do in here
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // packet
} // qsf

// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline QsfBinaryProtocol::QsfBinaryProtocol(QsfProtocol* parent) :
		QsfProtocol(parent)
	{
		// Nothing to do in here
	}

	inline QsfBinaryProtocol::~QsfBinaryProtocol()
	{
		// Nothing to do in here
	}


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	template<class PacketType>
	inline void QsfBinaryProtocol::registerPacket()
	{
		mInPackets[PacketType::PACKET_ID] = boost::factory<PacketType*>();
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf

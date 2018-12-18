// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/network/layered/QsfProtocol.h"
#include "qsf/network/layered/packet/BinaryPacket.h"

#include <boost/container/flat_map.hpp>
#include <boost/functional/factory.hpp>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Type definitions                                      ]
	//[-------------------------------------------------------]
	typedef packet::BinaryPacketBase* PacketGenerator();
	typedef boost::container::flat_map<uint32, boost::function<PacketGenerator> > PacketList;


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Abstract editor asset protocol class
	*/
	class QSF_API_EXPORT QsfBinaryProtocol : public QsfProtocol
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		inline QsfBinaryProtocol(QsfProtocol* parent);

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~QsfBinaryProtocol();

		bool sendPacket(const packet::BinaryPacketBase& packet) const;


	//[-------------------------------------------------------]
	//[ Public virtual em5::MatchmakingProtocol methods       ]
	//[-------------------------------------------------------]
	public:
		virtual void handlePacket(const packet::BinaryPacketBase& packet) = 0;


	//[-------------------------------------------------------]
	//[ Public virtual qsf::QsfProtocol methods               ]
	//[-------------------------------------------------------]
	public:
		virtual void onReceivePacket(const std::vector<char>& packet) override;


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		template<class PacketType>
		inline void registerPacket();


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		PacketList mInPackets;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/network/layered/QsfBinaryProtocol-inl.h"

// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/serialization/binary/BinarySerializer.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class BinarySerializer;
}


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
		class QSF_API_EXPORT BinaryPacketBase : public boost::noncopyable
		{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Default constructor
			*/
			inline BinaryPacketBase();

			/**
			*  @brief
			*    Destructor
			*/
			inline virtual ~BinaryPacketBase();

			/**
			*  @brief
			*    Deserializes the packet from the given qsf::BinarySerializer object.
			*
			*  @param[in] serializer
			*    qsf::BinarySerializer object to deserialize from
			*/
			inline void deserialize(const BinarySerializer& serializer);

			/**
			*  @brief
			*    Serializes the packet to the given qsf::BinarySerializer object.
			*
			*  @param[out] serializer
			*    qsf::BinarySerializer object to serialize into
			*/
			inline void serialize(BinarySerializer& serializer) const;


		//[--------------------------------------------------------]
		//[ Public virtual qsf::packet::BinaryPacketBase methods   ]
		//[--------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Returns the packet identifier that was specified as the template parameter.
			*
			*  @return
			*    The identifier of the implemented packet class
			*/
			virtual uint32 getPacketId() const = 0;

			/**
			*  @brief
			*    Method that implements the actual de/serialization.
			*
			*  @param[in] serializer
			*    The serializer object that the packet should de/serialize from/to
			*
			*  @note
			*    - Even though "this" is not const it is strictly prohibited to modify "this" if the serializer is in write mode
			*/
			inline virtual void serialize(BinarySerializer& serializer);


		};

		/**
		*  @brief
		*    TODO(co) Comment me
		*/
		template<class PacketClass>
		class BinaryPacket : public BinaryPacketBase
		{


		//[--------------------------------------------------------]
		//[ Public virtual qsf::packet::BinaryPacketBase methods   ]
		//[--------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Returns the packet identifier that was specified as the template parameter.
			*
			*  @return
			*    The identifier of the implemented packet class
			*/
			inline virtual uint32 getPacketId() const override;


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // packet
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/network/layered/packet/BinaryPacket-inl.h"

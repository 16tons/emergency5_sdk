// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/reflection/object/Object.h"
#include "qsf/reflection/type/CampUint64.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Map;
	class StreetGateway;
	class StreetComponent;
	class StreetSectionComponent;
	class StreetCrossingComponent;
	class BinarySerializer;
}


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
	*    Street gateway link class
	*/
	class QSF_API_EXPORT StreetGatewayLink : public Object
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		inline StreetGatewayLink();

		/**
		*  @brief
		*    Copy constructor
		*
		*  @param[in] source
		*    Source to copy from
		*/
		inline StreetGatewayLink(const StreetGatewayLink& source);

		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] entityId
		*    ID of the entity, "qsf::getUninitialized<uint64>()" if invalid
		*  @param[in] streetGatewayLink
		*    Street gateway link to copy the rest of the data from
		*/
		inline StreetGatewayLink(uint64 entityId, const StreetGatewayLink& streetGatewayLink);

		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] entityId
		*    ID of the entity, "qsf::getUninitialized<uint64>()" if invalid
		*  @param[in] streetGatewayIndex
		*    Street gateway index, "qsf::getUninitialized<uint32>()" if invalid
		*/
		inline StreetGatewayLink(uint64 entityId, uint32 streetGatewayIndex);

		/**
		*  @brief
		*    Return connected entity ID
		*
		*  @return
		*    ID of the entity, "qsf::getUninitialized<uint64>()" if invalid
		*/
		inline uint64 getConnectedEntityId() const;

		/**
		*  @brief
		*    Set connected entity ID
		*
		*  @param[in] entityId
		*    ID of the entity, "qsf::getUninitialized<uint64>()" if invalid
		*/
		inline void setConnectedEntityId(uint64 entityId);

		/**
		*  @brief
		*    Return the street gateway index inside the connected entity
		*
		*  @return
		*    The street gateway index inside the connected entity, "qsf::getUninitialized<uint32>()" if invalid
		*/
		inline uint32 getStreetGatewayIndex() const;

		/**
		*  @brief
		*    Set the street gateway index inside the connected entity
		*
		*  @param[in] streetGatewayIndex
		*    Street gateway index inside the connected entity, "qsf::getUninitialized<uint32>()" if invalid
		*/
		inline void setStreetGatewayIndex(uint32 streetGatewayIndex);

		//[-------------------------------------------------------]
		//[ Serialization                                         ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Serialize or deserialize the street gateway link using a binary serializer
		*
		*  @param[in] serializer
		*    The serializer, which can be either in read or write mode
		*/
		void serialize(BinarySerializer& serializer);

		//[-------------------------------------------------------]
		//[ Assignment operators                                  ]
		//[-------------------------------------------------------]
		inline StreetGatewayLink& operator =(const StreetGatewayLink& source);
		inline bool operator ==(const StreetGatewayLink& source) const;
		inline bool operator !=(const StreetGatewayLink& source) const;

		//[-------------------------------------------------------]
		//[ Ease of use                                           ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Return connected street crossing component inside the given map instance
		*
		*  @param[in] map
		*    Map instance to search for the connected entity in
		*
		*  @return
		*    The connected street crossing component inside the given map instance, null pointer on error
		*/
		StreetCrossingComponent* getConnectedStreetCrossingComponent(const Map& map) const;

		/**
		*  @brief
		*    Return connected street component inside the given map instance
		*
		*  @param[in] map
		*    Map instance to search for the connected entity in
		*
		*  @return
		*    The connected street component inside the given map instance, null pointer on error
		*/
		StreetComponent* getConnectedStreetComponent(const Map& map) const;

		/**
		*  @brief
		*    Return connected street section component inside the given map instance
		*
		*  @param[in] map
		*    Map instance to search for the connected entity in
		*
		*  @return
		*    The connected street section component inside the given map instance, null pointer on error
		*/
		StreetSectionComponent* getConnectedStreetSectionComponent(const Map& map) const;

		/**
		*  @brief
		*    Return connected street gatway inside the given map instance
		*
		*  @param[in] map
		*    Map instance to search for the connected entity in
		*
		*  @return
		*    The connected street gateway component inside the given map instance, null pointer on error
		*/
		StreetGateway* getConnectedStreetGateway(const Map& map) const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Connected to the CAMP reflection system
		uint64 mConnectedEntityId;		///< ID of the entity the link points to, "qsf::getUninitialized<uint64>()" if invalid
		uint32 mStreetGatewayIndex;		///< Gateway index inside the entity the link points to, "qsf::getUninitialized<uint32>()" if invalid


	//[-------------------------------------------------------]
	//[ CAMP reflection system                                ]
	//[-------------------------------------------------------]
	QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/component/street/StreetGatewayLink-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::StreetGatewayLink)

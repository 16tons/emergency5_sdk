// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/component/street/StreetGatewayLink.h"

#include <vector>


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
	*    Street gateway class
	*/
	class QSF_API_EXPORT StreetGateway : public Object
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const float RADIUS;	///< Gateway visualization and picking radius


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		inline StreetGateway();

		/**
		*  @brief
		*    Constructor to add getwayNodes
		*
		*  @param[in] gatewayNodes
		*    Nodes of gateway
		*/
		inline explicit StreetGateway(const std::vector<uint32>& gatewayNodes);

		/**
		*  @brief
		*    Copy constructor
		*
		*  @param[in] source
		*    Source to copy from
		*/
		inline StreetGateway(const StreetGateway& source);

		/**
		*  @brief
		*    Return street gateway nodes
		*
		*  @return
		*    Vector of gateway nodes
		*/
		inline const std::vector<uint32>& getGatewayNodes() const;

		/**
		*  @brief
		*    Set the gatewayNodes
		*
		*  @param[in] gatewayNodes
		*    The gateway nodes
		*/
		inline void setGatewayNodes(const std::vector<uint32>& gatewayNodes);

		/**
		*  @brief
		*    Return street gateway link
		*
		*  @return
		*    The street gateway link, do not destroy the instance
		*/
		inline const StreetGatewayLink& getStreetGatewayLink() const;

		/**
		*  @brief
		*    Set the street gateway link
		*
		*  @param[in] streetGatewayLink
		*    Street gateway link to set
		*/
		inline void setStreetGatewayLink(const StreetGatewayLink& streetGatewayLink);

		//[-------------------------------------------------------]
		//[ Serialization                                         ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Serialize or deserialize the street gateway using a binary serializer
		*
		*  @param[in] serializer
		*    The serializer, which can be either in read or write mode
		*/
		void serialize(BinarySerializer& serializer);

		//[-------------------------------------------------------]
		//[ Assignment operators                                  ]
		//[-------------------------------------------------------]
		inline StreetGateway& operator =(const StreetGateway& source);
		inline bool operator ==(const StreetGateway& source) const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Connected to the CAMP reflection system
		std::vector<uint32> mGatewayNodes;
		StreetGatewayLink	mStreetGatewayLink;


	//[-------------------------------------------------------]
	//[ CAMP reflection system                                ]
	//[-------------------------------------------------------]
	QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more
	QSF_CAMP_ARRAY_PROPERTY(GatewayNodes, StreetGateway, uint32)	// Adds public member variable "GatewayNodes" which is the middle part of the nodes <-> CAMP interconnection


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/component/street/StreetGateway-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::StreetGateway)
QSF_CAMP_ARRAY_MAPPER_DEFINITION(qsf::StreetGateway::GatewayNodesInterface)

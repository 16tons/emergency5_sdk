// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/map/serializer/MapSerializer.h"


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
	*    Binary map serializer
	*/
	class BinaryMapSerializer : public MapSerializer
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		inline BinaryMapSerializer();

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~BinaryMapSerializer();


	//[-------------------------------------------------------]
	//[ Public virtual qsf::MapSerializer methods             ]
	//[-------------------------------------------------------]
	public:
		virtual void deserialize(Map& map, std::istream& istream, const Map::SerializationOptions& serializationOptions) const override;
		virtual void serialize(const Map& map, std::ostream& ostream, const Map::SerializationOptions& serializationOptions) const override;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/map/serializer/BinaryMapSerializer-inl.h"

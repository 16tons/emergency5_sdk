// Copyright (C) 2012-2017 Promotion Software GmbH


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
	*    JSON map serializer
	*/
	class QSF_API_EXPORT JsonMapSerializer : public MapSerializer
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		inline JsonMapSerializer();

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~JsonMapSerializer();


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
#include "qsf/map/serializer/JsonMapSerializer-inl.h"
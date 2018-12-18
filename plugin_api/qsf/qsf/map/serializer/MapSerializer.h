// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/asset/AssetDataSerializer.h"
#include "qsf/map/Map.h"

#include <ostream>
#include <istream>


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
	*    Abstract map serializer interface
	*
	*  @note
	*    - Must work with exceptions in order to be able to interact with standard C++ streams
	*/
	class QSF_API_EXPORT MapSerializer : public AssetDataSerializer
	{


	//[-------------------------------------------------------]
	//[ Public virtual qsf::MapSerializer methods             ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Deserialize a map by using a given input stream
		*
		*  @param[in] map
		*    Map instance to fill
		*  @param[in] istream
		*    Input stream to deserialize the map from
		*  @param[in] serializationOptions
		*    Configuration for the serialization
		*
		*  @exception
		*    Throws "std::exception" in case on an error
		*/
		virtual void deserialize(Map& map, std::istream& istream, const Map::SerializationOptions& serializationOptions) const = 0;

		/**
		*  @brief
		*    Serialize the map into a given output stream
		*
		*  @param[in] map
		*    Map instance to serialize
		*  @param[in] ostream
		*    Output stream to serialize the map into
		*  @param[in] differenceToDefault
		*    Do not serialize properties which have the default value (meaning we already know its value without having to read it when deserializing)
		*
		*  @exception
		*    Throws "std::exception" in case on an error
		*/
		virtual void serialize(const Map& map, std::ostream& ostream, const Map::SerializationOptions& serializationOptions) const = 0;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf

// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/asset/AssetDataSerializer.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace logic
	{
		class Sequence;
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace logic
	{


		//[-------------------------------------------------------]
		//[ Classes                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Abstract sequence serializer interface
		*
		*  @note
		*    - Must work with exceptions in order to be able to interact with standard C++ streams
		*/
		class SequenceSerializer : public AssetDataSerializer
		{


		//[-------------------------------------------------------]
		//[ Public virtual qsf::logic::SequenceSerializer methods ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Deserialize a sequence by using a given input stream
			*
			*  @param[in] sequence
			*    Sequence instance to fill
			*  @param[in] istream
			*    Input stream to deserialize the sequence from
			*
			*  @exception
			*    Throws "std::exception" in case on an error
			*/
			virtual void deserialize(Sequence& sequence, std::istream& istream) const = 0;

			/**
			*  @brief
			*    Serialize the sequence into a given output stream
			*
			*  @param[in] sequence
			*   Sequence instance to serialize
			*  @param[in] ostream
			*    Output stream to serialize the sequence into
			*  @param[in] differenceToDefault
			*    Do not serialize properties which have the default value (meaning we already know its value without having to read it when deserializing)
			*
			*  @exception
			*    Throws "std::exception" in case on an error
			*
			*/
			virtual void serialize(Sequence& sequence, std::ostream& ostream, bool differenceToDefault = true) const = 0;


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // logic
} // qsf

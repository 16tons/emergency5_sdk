// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/asset/AssetDataSerializer.h"

#include <ostream>
#include <istream>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Translator;
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
	*    Abstract translator serializer interface
	*
	*  @note
	*    - Must work with exceptions in order to be able to interact with standard C++ streams
	*/
	class TranslatorSerializer : public AssetDataSerializer
	{


	//[-------------------------------------------------------]
	//[ Public virtual qsf::TranslatorSerializer methods      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Deserialize a translator by using a given input stream
		*
		*  @param[in] translator
		*    Translator instance to fill
		*  @param[in] istream
		*    Input stream to deserialize the translator from
		*
		*  @exception
		*    Throws "std::exception" in case on an error
		*/
		virtual void deserialize(Translator& translator, std::istream& istream) const = 0;

		/**
		*  @brief
		*    Serialize the translator into a given output stream
		*
		*  @param[in] translator
		*   Translator instance to serialize
		*  @param[in] ostream
		*    Output stream to serialize the translator into
		*
		*  @exception
		*    Throws "std::exception" in case on an error
		*/
		virtual void serialize(Translator& translator, std::ostream& ostream) const = 0;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf

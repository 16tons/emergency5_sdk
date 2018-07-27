// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/Export.h"


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
	*    Class used to pass memory size values around, supports automatic unit-agnostic formatting to std::ostream
	*/
	class ByteValue
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		inline ByteValue(uint64 value) : mValue(value) {}


	//[-------------------------------------------------------]
	//[ Public data                                           ]
	//[-------------------------------------------------------]
	public:
		uint64 mValue;	///< The size value measured in bytes


	};


	/**
	*  @brief
	*    OSTREAM formatter for ByteValue, will automatically format the value as b, kb, mb, gb
	*/
	QSF_GENERIC_API_EXPORT std::ostream& operator << (std::ostream& stream, const ByteValue& value);


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf

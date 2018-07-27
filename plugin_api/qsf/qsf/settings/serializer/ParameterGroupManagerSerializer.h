// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/serialization/Serializer.h"

#include <ostream>
#include <istream>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class ParameterGroupManager;
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
	*    Abstract parameter group manager serializer interface
	*
	*  @note
	*    - Must work with exceptions in order to be able to interact with standard C++ streams
	*/
	class ParameterGroupManagerSerializer : public Serializer
	{


	//[-------------------------------------------------------]
	//[ Public virtual qsf::ParameterGroupManagerSerializer methods ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Deserialize a parameter group manager by using a given input stream
		*
		*  @param[in] parameterGroupManager
		*    Parameter group manager instance to fill
		*  @param[in] istream
		*    Input stream to deserialize the parameter group manager from
		*
		*  @exception
		*    Throws "std::exception" in case on an error
		*/
		virtual void deserialize(ParameterGroupManager& parameterGroupManager, std::istream& istream) const = 0;

		/**
		*  @brief
		*    Serialize the parameter group manager into a given output stream
		*
		*  @param[in] parameterGroupManager
		*   Parameter group manager instance to serialize
		*  @param[in] ostream
		*    Output stream to serialize the parameter group manager into
		*  @param[in] differenceToDefault
		*    Do not serialize properties which have the default value (meaning we already know its value without having to read it when deserializing)
		*
		*  @exception
		*    Throws "std::exception" in case on an error
		*/
		virtual void serialize(ParameterGroupManager& parameterGroupManager, std::ostream& ostream, bool differenceToDefault = true) const = 0;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf

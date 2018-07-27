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
	class Project;
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
	*    Abstract project serializer interface
	*
	*  @note
	*    - Must work with exceptions in order to be able to interact with standard C++ streams
	*/
	class ProjectSerializer : public Serializer
	{


	//[-------------------------------------------------------]
	//[ Public virtual qsf::ProjectSerializer methods         ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Deserialize a project by using a given input stream
		*
		*  @param[in] project
		*    Project to fill
		*  @param[in] istream
		*    Input stream to deserialize the project from
		*
		*  @exception
		*    Throws "std::exception" in case on an error
		*/
		virtual void deserialize(Project& project, std::istream& istream) const = 0;

		/**
		*  @brief
		*    Serialize the project into a given output stream
		*
		*  @param[in] project
		*    Project instance to serialize
		*  @param[in] ostream
		*    Output stream to serialize the project into
		*
		*  @exception
		*    Throws "std::exception" in case on an error
		*/
		virtual void serialize(const Project& project, std::ostream& ostream) const = 0;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf

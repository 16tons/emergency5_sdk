// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/serialization/Serializer.h>

#include <ostream>
#include <istream>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace editor
	{
		namespace base
		{
			class UserManager;
		}
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace editor
	{
		namespace base
		{


			//[-------------------------------------------------------]
			//[ Classes                                               ]
			//[-------------------------------------------------------]
			/**
			*  @brief
			*    Abstract user manager serializer interface
			*
			*  @note
			*    - Must work with exceptions in order to be able to interact with standard C++ streams
			*/
			class UserManagerSerializer : public Serializer
			{


			//[-------------------------------------------------------]
			//[ Public virtual qsf::UserManagerSerializer methods     ]
			//[-------------------------------------------------------]
			public:
				/**
				*  @brief
				*    Deserialize a user manager by using a given input stream
				*
				*  @param[in] userManager
				*    User manager to fill
				*  @param[in] istream
				*    Input stream to deserialize the user manager from
				*
				*  @exception
				*    Throws "std::exception" in case on an error
				*/
				virtual void deserialize(UserManager& userManager, std::istream& istream) const = 0;

				/**
				*  @brief
				*    Serialize the user manager into a given output stream
				*
				*  @param[in] userManager
				*    User manager instance to serialize
				*  @param[in] ostream
				*    Output stream to serialize the user manager into
				*
				*  @exception
				*    Throws "std::exception" in case on an error
				*/
				virtual void serialize(const UserManager& userManager, std::ostream& ostream) const = 0;


			};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
		} // base
	} // editor
} // qsf

// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor_base/user/serializer/UserManagerSerializer.h"

#include <qsf/platform/PlatformTypes.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace boost
{
	namespace property_tree
	{
		template <typename U, typename V, typename W>
		class basic_ptree;
		typedef basic_ptree<std::string, std::string, std::less<std::string> > ptree;
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
			*    JSON user manager serializer
			*/
			class JsonUserManagerSerializer : public UserManagerSerializer
			{


			//[-------------------------------------------------------]
			//[ Public definitions                                    ]
			//[-------------------------------------------------------]
			public:
				static const char	FORMAT_TYPE[];
				static const uint32 FORMAT_VERSION;


			//[-------------------------------------------------------]
			//[ Public methods                                        ]
			//[-------------------------------------------------------]
			public:
				/**
				*  @brief
				*    Default constructor
				*/
				inline JsonUserManagerSerializer();

				/**
				*  @brief
				*    Destructor
				*/
				inline virtual ~JsonUserManagerSerializer();


			//[-------------------------------------------------------]
			//[ Public virtual qsf::UserManagerSerializer methods     ]
			//[-------------------------------------------------------]
			public:
				virtual void deserialize(UserManager& userManager, std::istream& istream) const override;
				virtual void serialize(const UserManager& userManager, std::ostream& ostream) const override;


			//[-------------------------------------------------------]
			//[ Private methods                                       ]
			//[-------------------------------------------------------]
			private:
				/**
				*  @brief
				*    Load a JSON user manager
				*
				*  @param[in] userManager
				*    User manager instance to fill
				*  @param[in] rootPTree
				*    Boost root ptree
				*
				*  @exception
				*    Throws "std::exception" in case on an error
				*/
				void loadJson(UserManager& userManager, boost::property_tree::ptree& rootPTree) const;

				/**
				*  @brief
				*    Load a user manager
				*
				*  @param[in] userManager
				*    User manager instance to fill
				*  @param[in] rootPTree
				*    Boost root ptree
				*
				*  @exception
				*    Throws "std::exception" in case on an error
				*/
				void loadUserManager(UserManager& userManager, boost::property_tree::ptree& rootPTree) const;


			};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
		} // base
	} // editor
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf_editor_base/user/serializer/JsonUserManagerSerializer-inl.h"

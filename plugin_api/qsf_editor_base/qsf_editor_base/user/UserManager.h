// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor_base/Export.h"

#include <qsf/base/manager/Manager.h>
#include <qsf/base/idgenerator/ConsecutiveIdGenerator.h>

#include <boost/container/flat_map.hpp>

#include <vector>
#include <string>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class RightsManager;
	namespace editor
	{
		namespace base
		{
			class User;
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
			*    User manager class
			*/
			class QSF_EDITOR_BASE_API_EXPORT UserManager : public Manager
			{


			//[-------------------------------------------------------]
			//[ Public definitions                                    ]
			//[-------------------------------------------------------]
			public:
				typedef std::vector<User*>						  UserList;
				typedef boost::container::flat_map<uint32, User*> UserMap;


			//[-------------------------------------------------------]
			//[ Public methods                                        ]
			//[-------------------------------------------------------]
			public:
				/**
				*  @brief
				*    Constructor
				*
				*  @param[in] rightsManager
				*    Rights manager, instance must stay valid as long as the user manager exists
				*/
				inline explicit UserManager(RightsManager& rightsManager);

				/**
				*  @brief
				*    Destructor
				*/
				inline virtual ~UserManager();

				/**
				*  @brief
				*    Return the rights manager the user manager is using
				*
				*  @return
				*    The rights manager the user manager is using, do not destroy the instance
				*/
				inline RightsManager& getRightsManager() const;

				//[-------------------------------------------------------]
				//[ User management                                       ]
				//[-------------------------------------------------------]
				/**
				*  @brief
				*    Create a new user instance
				*
				*  @return
				*    The created user instance, null pointer on error, do no destroy the returned
				*    instance and do need keep a reference to it outside the C runtime stack (use the unique user ID instead)
				*/
				User* createUser();

				/**
				*  @brief
				*    Create a new user instance with an enforced given unique user identifier
				*
				*  @param[in] id
				*    The unique user identifier
				*
				*  @return
				*    The created user instance, null pointer on error (e.g. ID already used), do no destroy the returned
				*    instance and do need keep a reference to it outside the C runtime stack (use the unique user ID instead)
				*
				*  @note
				*    - Use "createUser()" instead of "createUserWithId()" whenever possible, do not enforce unique user identifier if you can avoid it
				*/
				User* createUserWithId(uint32 id);

				/**
				*  @brief
				*    Destroy a user instance by using its unique identifier
				*
				*  @param[in] id
				*    The unique user identifier
				*
				*  @return
				*    "true" if all went fine, else "false" (unknown user identifier?)
				*/
				bool destroyUserById(uint32 id);

				/**
				*  @brief
				*    Destroy all users within this user manager
				*/
				void destroyAllUsers();

				/**
				*  @brief
				*    Return a user instance by using its unique identifier
				*
				*  @param[in] id
				*    The unique user identifier
				*
				*  @return
				*    The requested user instance, null pointer on error (maybe the ID is already used?), do not destroy the instance
				*/
				inline User* getUserById(uint32 id) const;

				/**
				*  @brief
				*    Return the user list
				*
				*  @return
				*    Reference to the internal user list, do not manipulate the list or destroy the users
				*/
				inline const UserList& getUsers() const;

				//[-------------------------------------------------------]
				//[ Serialization                                         ]
				//[-------------------------------------------------------]
				/**
				*  @brief
				*    Return the local filename of the user manager
				*
				*  @return
				*    The local UTF-8 filename in platform-independent notation of the user manager
				*/
				inline std::string getFilename() const;

				/**
				*  @brief
				*    Load a user manager by using a given filename
				*
				*  @param[in] filename
				*    UTF-8 filename in platform-independent notation of the user manager to load
				*
				*  @return
				*    "true" if all went fine, else "false" (file not found?)
				*
				*  @note
				*    - The current user manager will get lost, so, you might want to save a manipulated user manager before loading a new one
				*    - The local UTF-8 filename in platform-independent notation of the user manager is set automatically
				*
				*  @todo
				*    - TODO(co) For now, we only support JSON
				*/
				bool loadByFilename(const std::string& filename);

				/**
				*  @brief
				*    Save the user manager by using a given filename
				*
				*  @param[in] filename
				*    UTF-8 filename in platform-independent notation of the user manager to save
				*
				*  @return
				*    "true" if all went fine, else "false"
				*
				*  @note
				*    - The local UTF-8 filename in platform-independent notation of the user manager is not changed
				*
				*  @todo
				*    - TODO(co) For now, we only support JSON
				*/
				bool saveByFilename(const std::string& filename);


			//[-------------------------------------------------------]
			//[ Private data                                          ]
			//[-------------------------------------------------------]
			private:
				RightsManager*				   mRightsManager;	///< Rights manager, always valid, instance must stay valid as long as the user manager exists, do not destroy the instance
				ConsecutiveIdGenerator<uint32> mIdGenerator;	///< User ID generator
				UserList					   mUserList;		///< List of all users within the user manager
				UserMap						   mUserMap;		///< User map with unique user identifier as key and pointer to the user as value
				std::string					   mFilename;		///< Local UTF-8 filename in platform-independent notation of the user manager


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
#include "qsf_editor_base/user/UserManager-inl.h"

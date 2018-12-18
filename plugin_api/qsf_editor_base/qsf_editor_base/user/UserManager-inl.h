// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/base/error/BoostAssertHandler.h>


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
			//[ Public methods                                        ]
			//[-------------------------------------------------------]
			inline UserManager::UserManager(RightsManager& rightsManager) :
				mRightsManager(&rightsManager)
			{
				// Nothing to do in here
			}

			inline UserManager::~UserManager()
			{
				// Destroy all users within this user manager
				destroyAllUsers();
			}

			inline RightsManager& UserManager::getRightsManager() const
			{
				// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
				QSF_ASSERT(nullptr != mRightsManager, "The rights manager is invalid", QSF_REACT_THROW);
				return *mRightsManager;
			}

			inline User* UserManager::getUserById(uint32 id) const
			{
				const UserMap::const_iterator iterator = mUserMap.find(id);
				return (iterator != mUserMap.cend()) ? iterator->second : nullptr;
			}

			inline const UserManager::UserList& UserManager::getUsers() const
			{
				return mUserList;
			}

			inline std::string UserManager::getFilename() const
			{
				return mFilename;
			}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
		} // base
	} // editor
} // qsf

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


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		inline RightsManager& NetworkManager::getRightsManager() const
		{
			// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
			QSF_ASSERT(nullptr != mRightsManager, "The QSF editor rights manager instance is invalid", QSF_REACT_THROW);
			return *mRightsManager;
		}

		inline base::UserManager& NetworkManager::getUserManager() const
		{
			// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
			QSF_ASSERT(nullptr != mUserManager, "The QSF editor user manager instance is invalid", QSF_REACT_THROW);
			return *mUserManager;
		}

		inline base::User& NetworkManager::getUser() const
		{
			// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
			QSF_ASSERT(nullptr != mUser, "The QSF editor user is invalid", QSF_REACT_THROW);
			return *mUser;
		}

		inline AssetClient& NetworkManager::getAssetClient() const
		{
			// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
			QSF_ASSERT(nullptr != mAssetClient, "The QSF asset client instance is invalid", QSF_REACT_THROW);
			return *mAssetClient;
		}

		inline EditorClient& NetworkManager::getEditorClient() const
		{
			// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
			QSF_ASSERT(nullptr != mEditorClient, "The QSF editor client instance is invalid", QSF_REACT_THROW);
			return *mEditorClient;
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf

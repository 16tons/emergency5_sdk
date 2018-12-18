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
			inline User::~User()
			{
				// Nothing to do in here
			}

			inline UserManager& User::getUserManager() const
			{
				// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
				QSF_ASSERT(nullptr != mUserManager, "The user manager is invalid", QSF_REACT_THROW);
				return *mUserManager;
			}

			inline uint32 User::getId() const
			{
				return mId;
			}

			inline uint32 User::getSessionId() const
			{
				return mSessionId;
			}

			inline void User::setSessionId(uint32 sessionId)
			{
				mSessionId = sessionId;
			}

			inline bool User::hasRight(const StringHash& id) const
			{
				return std::find(mRightIds.begin(), mRightIds.end(), id) != mRightIds.end();
			}


			//[-------------------------------------------------------]
			//[ Private methods                                       ]
			//[-------------------------------------------------------]
			inline User::User(uint32 id, UserManager& userManager) :
				mId(id),
				mUserManager(&userManager),
				mSessionId(getUninitialized<uint32>())
			{
				// Nothing to do in here
			}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
		} // base
	} // editor
} // qsf

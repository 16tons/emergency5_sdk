// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/base/StringHash.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline BaseProject::BaseProject() :
		mAuthor("Unknown"),
		mVersion("0.0.0"),
		mRestrictedUserAccess(false)
	{
		// Nothing to do in here
	}

	inline BaseProject::BaseProject(const BaseProject& source) :
		mName(source.mName),
		mDescription(source.mDescription),
		mTags(source.mTags),
		mAuthor(source.mAuthor),
		mVersion(source.mVersion),
		mOwnerUserLogin(source.mOwnerUserLogin),
		mRestrictedUserAccess(source.mRestrictedUserAccess),
		mReadOnlyUserLogins(source.mReadOnlyUserLogins),
		mReadWriteUserLogins(source.mReadWriteUserLogins),
		mFlags(source.mFlags)
	{
		// Nothing to do in here
	}

	inline BaseProject::~BaseProject()
	{
		// Nothing to do in here
	}

	inline ProjectId BaseProject::getProjectId() const
	{
		return StringHash(mName);
	}

	inline const std::string& BaseProject::getName() const
	{
		return mName;
	}

	inline void BaseProject::setName(const std::string& name)
	{
		mName = name;
	}

	inline const std::string& BaseProject::getDescription() const
	{
		return mDescription;
	}

	inline void BaseProject::setDescription(const std::string& description)
	{
		mDescription = description;
	}

	inline const std::string& BaseProject::getTags() const
	{
		return mTags;
	}

	inline void BaseProject::setTags(const std::string& tags)
	{
		mTags = tags;
	}

	inline const std::string& BaseProject::getAuthor() const
	{
		return mAuthor;
	}

	inline void BaseProject::setAuthor(const std::string& author)
	{
		mAuthor = author;
	}

	inline const std::string& BaseProject::getVersion() const
	{
		return mVersion;
	}

	inline void BaseProject::setVersion(const std::string& version)
	{
		mVersion = version;
	}

	inline const std::string& BaseProject::getOwnerUserLogin() const
	{
		return mOwnerUserLogin;
	}

	inline void BaseProject::setOwnerUserLogin(const std::string& ownerUserLogin)
	{
		mOwnerUserLogin = ownerUserLogin;
	}

	inline bool BaseProject::getRestrictedUserAccess() const
	{
		return mRestrictedUserAccess;
	}

	inline void BaseProject::setRestrictedUserAccess(bool restrictedUserAccess)
	{
		mRestrictedUserAccess = restrictedUserAccess;
	}

	inline const std::string& BaseProject::getReadOnlyUserLogins() const
	{
		return mReadOnlyUserLogins;
	}

	inline void BaseProject::setReadOnlyUserLogins(const std::string& readOnlyUserLogins)
	{
		mReadOnlyUserLogins = readOnlyUserLogins;
	}

	inline const std::string& BaseProject::getReadWriteUserLogins() const
	{
		return mReadWriteUserLogins;
	}

	inline void BaseProject::setReadWriteUserLogins(const std::string& readWriteUserLogins)
	{
		mReadWriteUserLogins = readWriteUserLogins;
	}

	inline bool BaseProject::isApplicationCoreProject() const
	{
		return mFlags.isSet(APPLICATION_CORE_PROJECT);
	}

	inline void BaseProject::setApplicationCoreProject(bool enable)
	{
		mFlags.set(APPLICATION_CORE_PROJECT, enable);
	}

	inline void BaseProject::setLocked(bool locked)
	{
		mFlags.set(LOCKED, locked);
	}

	inline BaseProject& BaseProject::operator=(const BaseProject& source)
	{
		mName		 		  = source.mName;
		mDescription 		  = source.mDescription;
		mTags		 		  = source.mTags;
		mAuthor				  = source.mAuthor;
		mVersion			  = source.mVersion;
		mOwnerUserLogin		  = source.mOwnerUserLogin;
		mRestrictedUserAccess = source.mRestrictedUserAccess;
		mReadOnlyUserLogins	  = source.mReadOnlyUserLogins;
		mReadWriteUserLogins  = source.mReadWriteUserLogins;
		mFlags				  = source.mFlags;
		return *this;
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::BaseProject methods               ]
	//[-------------------------------------------------------]
	inline bool BaseProject::isLocked() const
	{
		return mFlags.isSet(LOCKED);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf

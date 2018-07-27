// Copyright (C) 2012-2017 Promotion Software GmbH


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
			inline UserBase::UserBase() :
				mColor(DEFAULT_COLOR),
				mEntitySelectionLimit(0),
				mEntityLockLimit(0)
			{
				// Nothing to do in here
			}

			inline UserBase::UserBase(const UserBase& x)
			{
				operator=(x);
			}

			inline UserBase::~UserBase()
			{
				// Nothing to do in here
			}

			inline UserBase& UserBase::operator=(const UserBase& x)
			{
				setLogin(x.mLogin);
				setSurname(x.mSurname);
				setName(x.mName);
				setEmail(x.mEmail);
				setRights(x.mRights);
				setDescription(x.mDescription);
				setTags(x.mTags);
				setColor(x.mColor);
				setEntitySelectionLimit(x.mEntitySelectionLimit);
				setEntityLockLimit(x.mEntityLockLimit);
				return *this;
			}

			inline std::string UserBase::getFullName() const
			{
				return mName.empty() ? mSurname : (mName + ' ' + mSurname);
			}

			inline std::string UserBase::getLogin() const
			{
				return mLogin;
			}

			inline void UserBase::setLogin(const std::string& login)
			{
				mLogin = login;
			}

			inline std::string UserBase::getSurname() const
			{
				return mSurname;
			}

			inline void UserBase::setSurname(const std::string& surname)
			{
				mSurname = surname;
			}

			inline std::string UserBase::getName() const
			{
				return mName;
			}

			inline void UserBase::setName(const std::string& name)
			{
				mName = name;
			}

			inline std::string UserBase::getEmail() const
			{
				return mEmail;
			}

			inline void UserBase::setEmail(const std::string& email)
			{
				mEmail = email;
			}

			inline std::string UserBase::getRights() const
			{
				return mRights;
			}

			inline void UserBase::setRights(const std::string& rights)
			{
				setRightsImpl(rights);
			}

			inline const std::string& UserBase::getDescription() const
			{
				return mDescription;
			}

			inline void UserBase::setDescription(const std::string& description)
			{
				mDescription = description;
			}

			inline const std::string& UserBase::getTags() const
			{
				return mTags;
			}

			inline void UserBase::setTags(const std::string& tags)
			{
				mTags = tags;
			}

			inline const Color4& UserBase::getColor() const
			{
				return mColor;
			}

			inline void UserBase::setColor(const Color4& color)
			{
				mColor = color;
			}

			inline uint32 UserBase::getEntitySelectionLimit() const
			{
				return mEntitySelectionLimit;
			}

			inline void UserBase::setEntitySelectionLimit(uint32 entitySelectionLimit)
			{
				mEntitySelectionLimit = entitySelectionLimit;
			}

			inline uint32 UserBase::getEntityLockLimit() const
			{
				return mEntityLockLimit;
			}

			inline void UserBase::setEntityLockLimit(uint32 entityLockLimit)
			{
				mEntityLockLimit = entityLockLimit;
			}


			//[-------------------------------------------------------]
			//[ Protected virtual qsf::editor::base::UserBase methods ]
			//[-------------------------------------------------------]
			inline void UserBase::setRightsImpl(const std::string& rights)
			{
				mRights = rights;
			}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
		} // base
	} // editor
} // qsf

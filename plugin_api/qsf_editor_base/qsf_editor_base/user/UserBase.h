// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor_base/Export.h"

#include <qsf/reflection/object/Object.h>
#include <qsf/reflection/type/CampQsfColor4.h>


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
			*    User class
			*/
			class QSF_EDITOR_BASE_API_EXPORT UserBase : public Object
			{


			//[-------------------------------------------------------]
			//[ Public definitions                                    ]
			//[-------------------------------------------------------]
			public:
				static const Color4 DEFAULT_COLOR;	///< Default user color


			//[-------------------------------------------------------]
			//[ Public methods                                        ]
			//[-------------------------------------------------------]
			public:
				/**
				*  @brief
				*    Constructor
				*/
				inline UserBase();

				/**
				*  @brief
				*    Copy constructor
				*/
				inline UserBase(const UserBase& x);

				/**
				*  @brief
				*    Destructor
				*/
				inline virtual ~UserBase();

				inline UserBase& operator=(const UserBase& x);

				/**
				*  @brief
				*    Return the full user name
				*
				*  @return
				*    The full user name (name + surname)
				*/
				inline std::string getFullName() const;

				/**
				*  @brief
				*    Return the full user name or the login as fallback
				*
				*  @return
				*    The full user name (name + surname) or the login as fallback in case there's no name as well as no surname and we would end up with an empty string
				*/
				std::string getBestFullName() const;

				//[-------------------------------------------------------]
				//[ Properties                                            ]
				//[-------------------------------------------------------]
				/**
				*  @brief
				*    Return the login of the user
				*
				*  @return
				*    The UTF-8 login of the user
				*
				*  @note
				*    - Connected to the CAMP reflection system
				*/
				inline std::string getLogin() const;

				/**
				*  @brief
				*    Set the login of the user
				*
				*  @param[in] login
				*    The UTF-8 login of the user
				*
				*  @note
				*    - Connected to the CAMP reflection system
				*/
				inline void setLogin(const std::string& login);

				/**
				*  @brief
				*    Return the surname of the user
				*
				*  @return
				*    The UTF-8 surname of the user
				*
				*  @note
				*    - Connected to the CAMP reflection system
				*/
				inline std::string getSurname() const;

				/**
				*  @brief
				*    Set the surname of the user
				*
				*  @param[in] surname
				*    The UTF-8 surname of the user
				*
				*  @note
				*    - Connected to the CAMP reflection system
				*/
				inline void setSurname(const std::string& surname);

				/**
				*  @brief
				*    Return the name of the user
				*
				*  @return
				*    The UTF-8 name of the user
				*
				*  @note
				*    - Connected to the CAMP reflection system
				*/
				inline std::string getName() const;

				/**
				*  @brief
				*    Set the name of the user
				*
				*  @param[in] name
				*    The UTF-8 name of the user
				*
				*  @note
				*    - Connected to the CAMP reflection system
				*/
				inline void setName(const std::string& name);

				/**
				*  @brief
				*    Return the electronic mail address of the user
				*
				*  @return
				*    The UTF-8 electronic mail address of the user
				*
				*  @note
				*    - Connected to the CAMP reflection system
				*/
				inline std::string getEmail() const;

				/**
				*  @brief
				*    Set the electronic mail address of the user
				*
				*  @param[in] email
				*    The UTF-8 electronic mail address of the user
				*
				*  @note
				*    - Connected to the CAMP reflection system
				*/
				inline void setEmail(const std::string& email);

				/**
				*  @brief
				*    Return the user right IDs as semicolon separated string
				*
				*  @return
				*    The user right IDs as semicolon separated ASCII string (e.g. "ServerManagement;AssetManagement")
				*
				*  @note
				*    - Connected to the CAMP reflection system
				*/
				inline std::string getRights() const;

				/**
				*  @brief
				*    Set the user right IDs as semicolon separated string
				*
				*  @param[in] rights
				*    The user right IDs as semicolon separated ASCII string (e.g. "ServerManagement;AssetManagement")
				*
				*  @note
				*    - Connected to the CAMP reflection system
				*/
				inline void setRights(const std::string& rights);

				/**
				*  @brief
				*    Return the description
				*
				*  @return
				*    The UTF-8 description, do no destroy the returned instance
				*
				*  @note
				*    - Connected to the CAMP reflection system
				*/
				inline const std::string& getDescription() const;

				/**
				*  @brief
				*    Set the description
				*
				*  @param[in] description
				*    The UTF-8 description
				*
				*  @note
				*    - Connected to the CAMP reflection system
				*/
				inline void setDescription(const std::string& description);

				/**
				*  @brief
				*    Return the tags as semicolon separated string
				*
				*  @return
				*    The UTF-8 tags as semicolon separated string (e.g. "external;gonzales"), tags are not case-sensitive
				*
				*  @note
				*    - Connected to the CAMP reflection system
				*/
				inline const std::string& getTags() const;

				/**
				*  @brief
				*    Set the tags as semicolon separated string
				*
				*  @param[in] tags
				*    The UTF-8 tags as semicolon separated string (e.g. "external;gonzales"), tags are not case-sensitive
				*
				*  @note
				*    - Connected to the CAMP reflection system
				*/
				inline void setTags(const std::string& tags);

				/**
				*  @brief
				*    Return whether or not a given tag is there
				*
				*  @param[in] tag
				*    The UTF-8 tag to check (e.g. "gonzales"), tags are not case-sensitive
				*
				*  @return
				*    "true" if the tag has been found, else "false"
				*/
				bool hasTag(const std::string& tag) const;

				/**
				*  @brief
				*    Return the user color
				*
				*  @return
				*    The user color used e.g. for entity lock visualization
				*
				*  @note
				*    - Connected to the CAMP reflection system
				*/
				inline const Color4& getColor() const;

				/**
				*  @brief
				*    Set the user color
				*
				*  @param[in] color
				*    The user color used e.g. for entity lock visualization
				*
				*  @note
				*    - Connected to the CAMP reflection system
				*/
				inline void setColor(const Color4& color);

				/**
				*  @brief
				*    Get the entity selection limit
				*
				*  @return
				*    The entity selection limit
				*/
				inline uint32 getEntitySelectionLimit() const;

				/**
				*  @brief
				*    Set the entity selection limit
				*
				*  @param[in] entitySelectionLimit
				*    New entity selection limit
				*/
				inline void setEntitySelectionLimit(uint32 entitySelectionLimit);

				/**
				*  @brief
				*    Get the entity lock limit
				*
				*  @return
				*    The entity lock limit
				*/
				inline uint32 getEntityLockLimit() const;

				/**
				*  @brief
				*    Set the entity lock limit
				*
				*  @param[in] entityLockLimit
				*    New entity lock limit
				*/
				inline void setEntityLockLimit(uint32 entityLockLimit);


			//[-------------------------------------------------------]
			//[ Protected virtual qsf::editor::base::UserBase methods ]
			//[-------------------------------------------------------]
			protected:
				virtual void setRightsImpl(const std::string& rights);


			//[-------------------------------------------------------]
			//[ Protected data                                        ]
			//[-------------------------------------------------------]
			protected:
				// Connected to the CAMP reflection system
				std::string mLogin;						///< The UTF-8 login of the user
				std::string mSurname;					///< The UTF-8 surname of the user
				std::string mName;						///< The UTF-8 name of the user
				std::string mEmail;						///< The UTF-8 electronic mail address of the user
				std::string mRights;					///< User right IDs as semicolon separated ASCII string (e.g. "ServerManagement;AssetManagement")
				std::string mDescription;				///< UTF-8 description
				std::string mTags;						///< The UTF-8 tags as semicolon separated string (e.g. "external;gonzales"), tags are not case-sensitive
				Color4		mColor;						///< The user color used e.g. for entity lock visualization
				uint32		mEntitySelectionLimit;		///< The entity selection limit (default: 0 = use server settings)
				uint32		mEntityLockLimit;			///< The entity lock limit (default: 0 = use server settings)


			//[-------------------------------------------------------]
			//[ CAMP reflection system                                ]
			//[-------------------------------------------------------]
			QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


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
#include "qsf_editor_base/user/UserBase-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::base::UserBase)

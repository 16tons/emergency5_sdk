// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/reflection/object/Object.h"
#include "qsf/asset/AssetSystemTypes.h"
#include "qsf/math/EnumBitmask.h"


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
	*    Project class
	*/
	class QSF_API_EXPORT BaseProject : public Object
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		enum Flags
		{
			APPLICATION_CORE_PROJECT,	///< If set, the project directly belongs to the application, otherwise it's user-defined (e.g. a "mod")
			LOCKED						///< If set, the project content is supposed to not be changed inside the editor
		};
		typedef EnumBitmask<uint8, Flags> FlagsSet;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		inline BaseProject();

		/**
		*  @brief
		*    Copy-constructor in case someone wants to have this copyable
		*
		*  @param[in] source
		*    Object to copy from
		*/
		inline explicit BaseProject(const BaseProject& source);

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~BaseProject();

		//[-------------------------------------------------------]
		//[ Properties                                            ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Return the project ID
		*
		*  @return
		*    The project ID which is the result of a project name string hash
		*/
		inline ProjectId getProjectId() const;

		/**
		*  @brief
		*    Return the name of the project
		*
		*  @return
		*    The UTF-8 name of the project
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*    - The technical project name, only lower case ASCII and no spaces, will also be the name of the project directory
		*    - Used in the construction of local asset names "<project name>/<asset type>/<asset category>/<asset name>.<cached asset type>" (e.g. "sample" -> "sample/mesh/house/house01.mesh")
		*/
		inline const std::string& getName() const;

		/**
		*  @brief
		*    Set the name of the project
		*
		*  @param[in] name
		*    The UTF-8 name of the project
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*
		*  @see
		*    - "qsf::BaseProject::getName()"
		*/
		inline void setName(const std::string& name);

		/**
		*  @brief
		*    Return the description of the project
		*
		*  @return
		*    The UTF-8 description of the project
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline const std::string& getDescription() const;

		/**
		*  @brief
		*    Set the description of the project
		*
		*  @param[in] description
		*    The UTF-8 description of the project
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
		*    The UTF-8 tags as semicolon separated string (e.g. "fire;water"), tags are not case-sensitive
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
		*    The UTF-8 tags as semicolon separated string (e.g. "fire;water"), tags are not case-sensitive
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline void setTags(const std::string& tags);

		/**
		*  @brief
		*    Return the author of the project
		*
		*  @return
		*    Name of the author of the project, if a project is no one-man-show this is usually the name of the team or the company
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline const std::string& getAuthor() const;

		/**
		*  @brief
		*    Set the author of the project
		*
		*  @param[in] author
		*    Name of the author of the project
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline void setAuthor(const std::string& author);

		/**
		*  @brief
		*    Return the version string of the project
		*
		*  @return
		*    Manually maintained version string of the project
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline const std::string& getVersion() const;

		/**
		*  @brief
		*    Set the version of the project
		*
		*  @param[in] version
		*    Manually maintained version string of the project
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline void setVersion(const std::string& version);

		//[-------------------------------------------------------]
		//[ User access related                                   ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Return the login of the project owning user
		*
		*  @return
		*    The login of the project owning user
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*    - This user has the control over the project user access restriction
		*/
		inline const std::string& getOwnerUserLogin() const;

		/**
		*  @brief
		*    Set the login of the project owning user
		*
		*  @param[in] ownerUserLogin
		*    Login of the project owning user
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*
		*  @see
		*    - "qsf::BaseProject::getOwnerUserLogin()"
		*/
		inline void setOwnerUserLogin(const std::string& ownerUserLogin);

		/**
		*  @brief
		*    Return whether or not the project has a user access restriction
		*
		*  @return
		*    "true" if the project has a user access restriction, else "false"
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*    - The project owning user is not affected by this user access restriction
		*/
		inline bool getRestrictedUserAccess() const;

		/**
		*  @brief
		*    Set whether or not the project has a user access restriction
		*
		*  @param[in] restrictedUserAccess
		*    "true" if the project has a user access restriction, else "false"
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*
		*  @see
		*    - "qsf::BaseProject::getRestrictedUserAccess()"
		*/
		inline void setRestrictedUserAccess(bool restrictedUserAccess);

		/**
		*  @brief
		*    Return a comma-separated list as string of user logins with read-only access to the project
		*
		*  @return
		*    Comma-separated list as string of user logins with read-only access to the project (example: "user42;mylogin")
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline const std::string& getReadOnlyUserLogins() const;

		/**
		*  @brief
		*    Set the comma-separated list as string of user logins with read-only access to the project
		*
		*  @param[in] readOnlyUserLogins
		*    Comma-separated list as string of user logins with read-only access to the project (example: "user42;mylogin")
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline void setReadOnlyUserLogins(const std::string& readOnlyUserLogins);

		/**
		*  @brief
		*    Return a comma-separated list as string of user logins with read-write access to the project
		*
		*  @return
		*    Comma-separated list as string of user logins with read-write access to the project (example: "user43;testlogin")
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline const std::string& getReadWriteUserLogins() const;

		/**
		*  @brief
		*    Set the comma-separated list as string of user logins with read-write access to the project
		*
		*  @param[in] readWriteUserLogins
		*    Comma-separated list as string of user logins with read-write access to the project (example: "user43;testlogin")
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline void setReadWriteUserLogins(const std::string& readWriteUserLogins);

		//[-------------------------------------------------------]
		//[ Flags                                                 ]
		//[-------------------------------------------------------]
		inline bool isApplicationCoreProject() const;
		inline void setApplicationCoreProject(bool enable);

		/**
		*  @brief
		*    Set whether or not the project is locked
		*
		*  @param[in] locked
		*    "true" if the project is locked, else "false"
		*/
		inline void setLocked(bool locked);

		//[-------------------------------------------------------]
		//[ Operators                                             ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Assignment operator
		*
		*  @param[in] source
		*    Object to copy from
		*/
		inline BaseProject& operator=(const BaseProject& source);


	//[-------------------------------------------------------]
	//[ Public virtual qsf::BaseProject methods               ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Return whether or not the project is locked
		*
		*  @return
		*    "true" if the project is locked, else "false"
		*
		*  @note
		*    - Locked projects are supposed to not be changed inside the editor
		*/
		inline virtual bool isLocked() const;


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		// Connected to the CAMP reflection system
		std::string mName;					///< The UTF-8 technical project name, only lower case ASCII and no spaces, will also be the name of the project directory
		std::string mDescription;			///< The UTF-8 description of the project
		std::string mTags;					///< The UTF-8 tags as semicolon separated string (e.g. "fire;water"), tags are not case-sensitive
		std::string mAuthor;				///< Project author name
		std::string mVersion;				///< User-defined project version, should be in format "1.0.0"
		std::string mOwnerUserLogin;		///< The login of the project owning user, this user has the control over the project user access restriction
		bool		mRestrictedUserAccess;	///< "true" if the project has a user access restriction, else "false"
		std::string mReadOnlyUserLogins;	///< Comma-separated list as string of user logins with read-only access to the project (example: "user42;mylogin")
		std::string mReadWriteUserLogins;	///< Comma-separated list as string of user logins with read-write access to the project (example: "user43;testlogin")
		FlagsSet	mFlags;					///< Project flags


	//[-------------------------------------------------------]
	//[ CAMP reflection system                                ]
	//[-------------------------------------------------------]
	QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/asset/project/BaseProject-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::BaseProject)

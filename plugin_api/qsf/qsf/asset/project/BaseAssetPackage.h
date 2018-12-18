// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/reflection/object/Object.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Project;
	class BinarySerializer;
}


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
	*    Asset package class
	*
	*  @remarks
	*    Each project is using 0-n assets which are grouped into asset packages in order to be able to keep an overview.
	*    When shipping products, each asset package can be e.g. zipped. Outside the project context asset packages are
	*    not directly visible to e.g. the asset system.
	*
	*    As soon as the project and the asset packages are loaded (!= mounted), the asset package is automatically mounted
	*    inside the QSF file system under a mount point which matches the physical file system structure. This is required
	*    to be able to load in the asset package information file "asset_package.json" in case the asset package is zipped.
	*
	*    The underlying QSF file system supports configuration of whether or not to append the asset package at the end of
	*    the virtual file system search path or  to prepend it (in case of overlapping files the new directory or archive
	*    is the preferred one). In order to keep it simple for our developer team and the mod-community, we don't expose
	*    the "append/prepend"-setting. Asset packages are automatically prepended, meaning asset packages will automatically
	*    "cover/override" other files inside the virtual file system in case of identical file locations.
	*/
	class QSF_API_EXPORT BaseAssetPackage : public Object
	{


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Return the default mount point for a given project
		*
		*  @param[in] project
		*    Project to get the default mount point for
		*
		*  @remarks
		*    When creating new projects inside the asset browser, one usually wants to create a mod-project. So, for ease-of-use,
		*    use as default mount point of new asset packages the name of the current project and not the name of the project the
		*    new asset package will be in.
		*/
		static std::string getDefaultMountPoint(const Project& project);


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		inline BaseAssetPackage();

		/**
		*  @brief
		*    Copy-constructor in case someone wants to have this copyable
		*
		*  @param[in] source
		*    Object to copy from
		*/
		inline explicit BaseAssetPackage(const BaseAssetPackage& source);

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~BaseAssetPackage();

		//[-------------------------------------------------------]
		//[ Properties                                            ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Return the name of the asset package
		*
		*  @return
		*    The UTF-8 name of the asset package
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline const std::string& getName() const;

		/**
		*  @brief
		*    Set the name of the asset package
		*
		*  @param[in] name
		*    The UTF-8 name of the asset package
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline void setName(const std::string& name);

		/**
		*  @brief
		*    Return the description of the asset package
		*
		*  @return
		*    The UTF-8 description of the asset package
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline const std::string& getDescription() const;

		/**
		*  @brief
		*    Set the description of the asset package
		*
		*  @param[in] description
		*    The UTF-8 description of the asset package
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline void setDescription(const std::string& description);

		/**
		*  @brief
		*    Return the location in the interpolated virtual file system tree that this asset package will be "mounted"
		*
		*  @return
		*    UTF-8 location in the interpolated virtual file system tree that this asset package will be "mounted", "" is equivalent to "/"
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline const std::string& getMountPoint() const;

		/**
		*  @brief
		*    Set the location in the interpolated virtual file system tree that this asset package will be "mounted"
		*
		*  @param[in] mountPoint
		*    UTF-8 location in the interpolated virtual file system tree that this asset package will be "mounted", "" is equivalent to "/"
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*    - Changing the mount point of the asset package is only possible if the asset package is currently not mounted
		*/
		inline void setMountPoint(const std::string& mountPoint);

		/**
		*  @brief
		*    Return the managed-state of the asset package (whether the contents of this asset package are controlled by the editor server when connected or not)
		*
		*  @return
		*    "true" if the asset package is a managed package, "false" otherwise
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline bool isManaged() const;

		/**
		*  @brief
		*    Set the managed-state of the asset package (whether the contents of this asset package are controlled by the editor server when connected or not)
		*
		*  @param[in] mountPoint
		*    "true" if the asset package is a managed package, "false" otherwise
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*    - Changing the managed state during runtime is not supported and will lead to undefined behaviour
		*    - Only AssetStorageManager will be allowed to change the state of this flag later on
		*/
		inline void setManaged(bool isManaged);

		/**
		*  @brief
		*    Return the mod-state of the asset package (whether the contents of this asset package are part of a game modification or not)
		*
		*  @return
		*    "true" if the asset package is a mod package, "false" otherwise
		*/
		inline bool isGameModification() const;

		/**
		*  @brief
		*    Return the multiplayer authenticated-state of the asset package (whether this asset package should be authenticated when a player joins a multiplayer game or not)
		*
		*  @return
		*    "true" if the asset package is a multiplayer authenticated package, "false" otherwise
		*
		*  @note
		*    - This flag will probably be ignored if the asset package is not packed to a single file (e.g. a zip-archive)
		*/
		inline bool isMultiplayerAuthenticated() const;

		/**
		*  @brief
		*    Assignment operator
		*
		*  @param[in] source
		*    Object to copy from
		*/
		inline BaseAssetPackage& operator=(const BaseAssetPackage& source);

		/**
		*  @brief
		*    Serialization of the asset package
		*/
		void serialize(BinarySerializer& serializer, uint32 formatVersion);


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		// Connected to the CAMP reflection system
		std::string mName;			///< The UTF-8 name of the asset package
		std::string mDescription;	///< The UTF-8 description of the asset package
		std::string mMountPoint;	///< UTF-8 location in the interpolated virtual file system tree that this asset package will be "mounted", "" is equivalent to "/"
		bool		mIsManaged;		///< The managed-state of the asset package (whether the contents of this asset package are controlled by the editor server when connected or not)


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
#include "qsf/asset/project/BaseAssetPackage-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::BaseAssetPackage)

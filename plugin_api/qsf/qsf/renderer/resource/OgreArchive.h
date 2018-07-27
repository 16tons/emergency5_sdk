// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <OGRE/OgreArchive.h>
#undef ERROR // OGRE includes an OS header resulting in the usual fancy definition-hell, undefine this MS Windows "ERROR"-definition


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace Ogre
{
	class MemoryDataStream;
}
namespace qsf
{
	class FileSystem;
	class AssetSystem;
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
	*    QSF OGRE archive implementation class
	*
	*  @note
	*    - Based on http://misterblue.com/programming/dynamic-ogre-resources/ogre-archive-wrapper
	*    - "qsf::OgreArchive" is an OGRE archive, meaning exceptions will be thrown in case of errors
	*/
	class OgreArchive : public Ogre::Archive
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class OgreArchiveFactory;	// QSF OGRE archive is QSF internal only


	//[-------------------------------------------------------]
	//[ Public virtual Ogre::Archive methods                  ]
	//[-------------------------------------------------------]
	public:
		virtual bool isCaseSensitive() const override;
		virtual void load() override;
		virtual void unload() override;
		virtual Ogre::DataStreamPtr open(const Ogre::String& filename, bool readOnly = true) override;
		virtual Ogre::StringVectorPtr list(bool recursive = true, bool directories = false) override;
		virtual Ogre::FileInfoListPtr listFileInfo(bool recursive = true, bool directories = false) override;
		virtual Ogre::StringVectorPtr find(const Ogre::String& pattern, bool recursive = true, bool directories = false) override;
		virtual bool exists(const Ogre::String& filename) override;
		virtual time_t getModifiedTime(const Ogre::String& filename) override;
		virtual Ogre::FileInfoListPtr findFileInfo(const Ogre::String& pattern, bool recursive = true, bool directories = false) override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] name
		*    Archive name
		*  @param[in] type
		*    Archive type
		*/
		OgreArchive(const Ogre::String& name, const Ogre::String& type);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~OgreArchive();

		// TODO(co) Comment
		void listInfoRecursive(const Ogre::String& base, bool recursive, bool directories, Ogre::FileInfoListPtr fileInfoList);
		void listRecursive(const Ogre::String& base, bool recursive, bool directories, Ogre::StringVectorPtr fileList) const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		const FileSystem&	mFileSystem;	///< File system instance, we're about to use it often in here so caching the reference is useful
		const AssetSystem&	mAssetSystem;	///< Asset system instance, we're about to use it often in here so caching the reference is useful
		Ogre::Archive*		mOgreArchive;	///< Internal OGRE archive, can be a null pointer when we're not loaded, the OGRE archive manager is reponsible for destroying the "mOgreArchive"-instance


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf

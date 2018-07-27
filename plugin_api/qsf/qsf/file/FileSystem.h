// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/base/System.h"

#include <string>
#include <vector>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class CacheManager;
	class FileCacheManager;
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
	*    File system class
	*
	*  @remarks
	*    The following example shows how to use "qsf::FileStream" for file handling:
	*    @code
	*    qsf::FileStream stream("test.txt", qsf::File::WRITE_MODE);
	*    stream << "Just a standard stream: " << 12345 << "\n";
	*    @endcode
	*    The resulting file will be written into the base directory. By using a standard file stream,
	*    Boost features like ptree can be used without any issues.
	*
	*    By using mount points, localized or new version of files can be realized. This functionality is also
	*    quite useful for community modding (see "qsf::FileSystem::mount()" for details). Imagine there are two compressed
	*    archives inside your base directory named "test.7z" and "test2.zip". Both archives contain a file
	*    named "test.map". The following example shows that, by default, the file of the last added mount point
	*    will be used:
	*    @code
	*    QSF_FILE.mount(QSF_FILE.getBaseDirectory() + '/' + "test.7z");
	*    QSF_FILE.mount(QSF_FILE.getBaseDirectory() + '/' + "test2.zip"); // Comment/uncomment this to get another "test.map" version
	*    boost::property_tree::ptree rootPTree;
	*    boost::property_tree::read_json(qsf::FileStream("test.map", qsf::File::READ_MODE), rootPTree);
	*    @endcode
	*
	*    The following useful PhysicsFS functions are not wrapped:
	*     - "PHYSFS_getDirSeparator()", use "qsf::PlatformSystem::getPathSeparator()"
	*     - "PHYSFS_getBaseDir()", use "qsf::PlatformSystem::getExecutableFilename()" and cut off the filename
	*     - "PHYSFS_getUserDir()", use "qsf::PlatformSystem::getUserHomeDirectory()"
	*     - "PHYSFS_setSaneConfig()" not supported
	*
	*  @note
	*    - Low level file access, use the asset system for high level resource handling
	*    - Have a look into the platform system for platform dependent directories
	*    - The external library "PhysicsFS" ( http://icculus.org/physfs/ ) is used internally, the PhysicsFS interface documentation was taken from PhysicsFS 2.0.3
	*    - Use "Boost Filesystem" http://www.boost.org/libs/filesystem/ for the rest if required, but please prefer "qsf::FileSystem"
	*    - Do not add a slash or a backslash at the end of directories, write for example "data/prefab/vehicle", not "data/prefab/vehicle/"
	*    - There are three types of filenames
	*       - Absolute filename example: "c:/test/data/qsf/content/mesh/test/sheep.mesh"
	*       - Relative filename example: "data/qsf/content/mesh/test/sheep.mesh"
	*       - Virtual filename example:  "qsf/mesh/test/sheep.mesh"
	*     - Use virtual filenames when you can because they assist modding, relative filenames if you have to and try to avoid absolute filenames
	*/
	class QSF_API_EXPORT FileSystem : public System
	{


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	public:
		static void normalizeFilename(std::string& filename);


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		FileSystem();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~FileSystem();

		/**
		*  @brief
		*    Return the cache manager instance
		*
		*  @return
		*    The cache manager; do not destroy the instance
		*/
		inline CacheManager& getCacheManager();

		/**
		*  @brief
		*    Return the file cache manager instance
		*
		*  @return
		*    The file cache manager; do not destroy the instance
		*/
		inline FileCacheManager& getFileCacheManager();

		/**
		*  @brief
		*    Return the absolute base directory of the application
		*
		*  @return
		*    The absolute UTF-8 base directory of the application in platform-independent notation (e.g. on Windows: "C:/MyApplication")
		*
		*  @note
		*    - The base directory can't be changed, use mounting instead
		*/
		inline std::string getBaseDirectory() const;

		//[-------------------------------------------------------]
		//[ Filename transformation                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Transform an absolute filename into a relative filename
		*
		*  @param[in]  absoluteFilename
		*    Absolute UTF-8 filename in platform-independent notation to convert to a relative filename
		*  @param[out] relativeFilename
		*    On success, receives the relative UTF-8 filename, not touched on error
		*
		*  @return
		*    "true" if all went fine, else "false"
		*
		*  @note
		*    - Example: The absolute filename "c:/test/data/qsf/content/mesh/test/sheep.mesh" will result in the relative filename "data/qsf/content/mesh/test/sheep.mesh"
		*/
		bool absoluteToRelativeFilename(const std::string& absoluteFilename, std::string& relativeFilename) const;

		/**
		*  @brief
		*    Transform a virtual filename into an absolute filename
		*
		*  @param[in]  virtualFilename
		*    Virtual UTF-8 filename in platform-independent notation to convert to an absolute filename
		*  @param[out] absoluteFilename
		*    On success, receives the absolute UTF-8 filename, not touched on error
		*  @param[out] guessLocation
		*    In case the file does not exist, should the method guess were the newly created file will probably be located at? (warning, use this with caution because the first match will be used)
		*
		*  @return
		*    "true" if all went fine, else "false"
		*
		*  @note
		*    - Example: The virtual filename "qsf/mesh/test/sheep.mesh" will result in the absolute filename "c:/test/data/qsf/content/mesh/test/sheep.mesh"
		*/
		bool virtualToAbsoluteFilename(const std::string& virtualFilename, std::string& absoluteFilename, bool guessLocation = false) const;

		/**
		*  @brief
		*    Transform a virtual filename into a relative filename
		*
		*  @param[in]  virtualFilename
		*    Virtual UTF-8 filename in platform-independent notation to convert to a relative filename
		*  @param[out] relativeFilename
		*    On success, receives the relative UTF-8 filename, not touched on error
		*  @param[out] guessLocation
		*    In case the file does not exist, should the method guess were the newly created file will probably be located at? (warning, use this with caution because the first match will be used)
		*
		*  @return
		*    "true" if all went fine, else "false"
		*
		*  @note
		*    - Example: The virtual filename "qsf/mesh/test/sheep.mesh" will result in the relative filename "data/qsf/content/mesh/test/sheep.mesh"
		*/
		bool virtualToRelativeFilename(const std::string& virtualFilename, std::string& relativeFilename, bool guessLocation = false) const;

		//[-------------------------------------------------------]
		//[ PhysicsFS interface                                   ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Get a list of supported archive types
		*
		*  @param[out] supportedArchiveTypes
		*    Will receive the list of supported archive types (UTF-8 file endings only, no description), list is not cleared before adding new entries
		*
		*  @remarks
		*    Get a list of archive types supported by the used implementation of PhysicsFS.
		*    These are the file formats usable for search path entries. This is for
		*    informational purposes only. Note that the extension listed is merely
		*    convention: if we list "ZIP", you can open a PkZip-compatible archive
		*    with an extension of "XYZ", if you like.
		*/
		void getSupportedArchiveTypes(std::vector<std::string>& supportedArchiveTypes) const;

		/**
		*  @brief
		*    Determine if the symbolic links are permitted
		*
		*  @return
		*    "true" if symlinks are permitted, else "false"
		*
		*  @remarks
		*    This reports the setting from the last call to "qsf::FileSystem::setPermitSymbolicLinks()".
		*    If "qsf::FileSystem::setPermitSymbolicLinks()" hasn't been called since the QSF file system was
		*    last started, symbolic links are implicitly disabled.
		*
		*  @see
		*    - "qsf::FileSystem::setPermitSymbolicLinks()"
		*/
		bool areSymbolicLinksPermitted() const;

		/**
		*  @brief
		*    Enable or disable following of symbolic links
		*
		*  @param[in] allow
		*    "true" if symlinks are permitted, else "false"
		*
		*  @remarks
		*    Some physical file systems and archives contain files that are just pointers
		*    to other files. On the physical file system, opening such a link will
		*    (transparently) open the file that is pointed to.
		*
		*    By default, PhysicsFS will check if a file is really a symlink during open
		*    calls and fail if it is. Otherwise, the link could take you outside the
		*    write and search paths, and compromise security.
		*
		*    If you want to take that risk, call this function with a non-zero parameter.
		*    Note that this is more for sandboxing a program's scripting language, in
		*    case untrusted scripts try to compromise the system. Generally speaking,
		*    a user could very well have a legitimate reason to set up a symlink, so
		*    unless you feel there's a specific danger in allowing them, you should
		*    permit them.
		*
		*    Symlinks are only explicitly checked when dealing with filenames
		*    in platform-independent notation. That is, when setting up your
		*    search and write paths, etc, symlinks are never checked for.
		*
		*    Symbolic link permission can be enabled or disabled at any time after
		*    the QSF file system was started, and is disabled by default.
		*/
		void setPermitSymbolicLinks(bool allow) const;

		/**
		*  @brief
		*    Get an array of paths to available CD-ROM drives
		*
		*  @param[out] cdRomDirectories
		*    Will receive the list of CD rom UTF-8 directories, list is not cleared before adding new entries
		*
		*  @remarks
		*    The directories returned are platform-dependent ("D:\" on Win32, "/cdrom" or
		*    whatnot on Unix). Directories are only returned if there is a disc ready and
		*    accessible in the drive. So if you've got two drives (D: and E:), and only
		*    E: has a disc in it, then that's all you get. If the user inserts a disc
		*    in D: and you call this function again, you get both drives. If, on a
		*    Unix box, the user unmounts a disc and remounts it elsewhere, the next
		*    call to this function will reflect that change.
		*
		*    This function refers to "CD-ROM" media, but it really means "inserted disc
		*    media," such as DVD-ROM, HD-DVD, CDRW, and Blu-Ray discs. It looks for
		*    file systems, and as such won't report an audio CD, unless there's a
		*    mounted file system track on it.
		*
		*  @note
		*    - This call may block while drives spin up. Be forewarned.
		*/
		void getCdRomDirectories(std::vector<std::string>& cdRomDirectories) const;

		/**
		*  @brief
		*    Add an archive or directory to the search path
		*
		*  @param[in] directory
		*    UTF-8 directory or archive to add to the path, in platform-dependent notation
		*  @param[in] mountPoint
		*    UTF-8 location in the interpolated tree that this archive will be "mounted", in platform-independent notation, "" is equivalent to "/"
		*  @param[in] appendToPath
		*    "true" to append at the end of the search path, "false" to prepend (in case of overlapping files the new directory or archive is the preferred one)
		*
		*  @return
		*    "true" if all went fine, else "false" (bogus archive, directory missing, etc)
		*
		*  @remarks
		*    If this is a duplicate, the entry is not added again, even though the
		*    function succeeds. You may not add the same archive to two different
		*    mount points: duplicate checking is done against the archive and not the
		*    mount point.
		*
		*    When you mount an archive, it is added to a virtual file system... all files
		*    in all of the archives are interpolated into a single hierarchical file
		*    tree. Two archives mounted at the same place (or an archive with files
		*    overlapping another mount point) may have overlapping files: in such a case,
		*    the file earliest in the search path is selected, and the other files are
		*    inaccessible to the application. This allows archives to be used to
		*    override previous revisions; you can use the mounting mechanism to place
		*    archives at a specific point in the file tree and prevent overlap; this
		*    is useful for downloadable mods that might trample over application data
		*    or each other, for example.
		*
		*    The mount point does not need to exist prior to mounting, which is different
		*    than those familiar with the Unix concept of "mounting" may not expect.
		*    As well, more than one archive can be mounted to the same mount point, or
		*    mount points and archive contents can overlap...the interpolation mechanism
		*    still functions as usual.
		*/
		bool mount(const std::string& directory, const std::string& mountPoint = "/", bool appendToPath = false) const;

		/**
		*  @brief
		*    Remove a directory or archive from the search path
		*
		*  @param[in] directory
		*    UTF-8 directory or archive to remove
		*
		*  @return
		*    "true" if all went fine, else "false"
		*
		*  @remarks
		*    This must be a (case-sensitive) match to a directory or archive already
		*    in the search path, specified in platform-dependent notation.
		*
		*    This call will fail (and fail to remove from the path) if the element still
		*    has files open in it.
		*/
		bool unmount(const std::string& directory) const;

		/**
		*  @brief
		*    Get a list of currently mounted directories or archives search paths
		*
		*  @param[out] directories
		*    Will receive the list of mounted UTF-8 directories, list is not cleared before adding new entries
		*
		*  @note
		*    - The default search path is an empty list
		*/
		void getMountedDirectories(std::vector<std::string>& directories) const;

		/**
		*  @brief
		*    Determine a mounted archive's mount point
		*
		*  @param[in] directory
		*    UTF-8 directory or archive previously added to the path, in
		*    platform-dependent notation. This must match the string
		*    used when adding, even if your string would also reference
		*    the same file with a different string of characters.
		*
		*  @return
		*    READ-ONLY string of mount point if added to path, empty string on failure (bogus archive, etc)
		*
		*  @remarks
		*    You give this function the name of an archive or directory you successfully
		*    added to the search path, and it reports the location in the interpolated
		*    tree where it is mounted. Files mounted with a "" mount point will report "/".
		*/
		std::string getMountPoint(const std::string& directory) const;

		//[-------------------------------------------------------]
		//[ PhysicsFS interface: Directory management             ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Create a directory
		*
		*  @param[in] directory
		*    UTF-8 filename of the new directory to create
		*
		*  @return
		*    "true" if all went fine, else "false"
		*
		*  @remarks
		*    This is specified in platform-independent notation in relation to the
		*    write directory. All missing parent directories are also created if they
		*    don't exist.
		*
		*    So if you've got the write directory set to "C:\mygame\writedir" and call
		*    qsf::FileSystem::createDirectory("downloads/maps") then the directories
		*    "C:\mygame\writedir\downloads" and "C:\mygame\writedir\downloads\maps"
		*    will be created if possible. If the creation of "maps" fails after we
		*    have successfully created "downloads", then the function leaves the
		*    created directory behind and reports failure.
		*/
		bool createDirectory(const std::string& directory) const;

		/**
		*  @brief
		*    Delete a file or directory
		*
		*  @param[in] filename
		*    UTF-8 filename of the file or directory to delete
		*
		*  @return
		*    "true" if all went fine, else "false"
		*
		*  @remarks
		*    The filename is specified in platform-independent notation in relation to the write directory.
		*
		*    A directory must be empty before this call can delete it.
		*
		*    Deleting a symlink will remove the link, not what it points to, regardless
		*    of whether you permit symlinks or not.
		*
		*    So if you've got the write directory set to "C:\mygame\writedir" and call
		*    qsf::FileSystem::deleteFileOrDirectory("downloads/maps/level1.map") then the file
		*    "C:\mygame\writedir\downloads\maps\level1.map" is removed from the
		*    physical file system, if it exists and the operating system permits the
		*    deletion.
		*
		*    Note that on Unix systems, deleting a file may be successful, but the
		*    actual file won't be removed until all processes that have an open
		*    file handle to it (including your program) close their handles.
		*
		*    Chances are, the bits that make up the file still exist, they are just
		*    made available to be written over at a later point. Don't consider this
		*    a security method or anything. :)
		*/
		bool deleteFileOrDirectory(const std::string& filename) const;

		/**
		*  @brief
		*    Figure out where in the search path a file resides
		*
		*  @param[in] filename
		*    UTF-8 filename of the file to look for
		*
		*  @return
		*    UTF-8 search path containing the file in question, empty string if not found
		*
		*  @remarks
		*    The file is specified in platform-independent notation. The returned
		*    filename will be the element of the search path where the file was found,
		*    which may be a directory, or an archive. Even if there are multiple
		*    matches in different parts of the search path, only the first one found
		*    is used, just like when opening a file.
		*
		*    So, if you look for "maps/level1.map", and C:\\mygame is in your search
		*    path and C:\\mygame\\maps\\level1.map exists, then "C:\mygame" is returned.
		*
		*    If a any part of a match is a symbolic link, and you've not explicitly
		*    permitted symlinks, then it will be ignored, and the search for a match
		*    will continue.
		*
		*    If you specify a fake directory that only exists as a mount point, it'll
		*    be associated with the first archive mounted there, even though that
		*    directory isn't necessarily contained in a real archive.
		*/
		std::string getRealDirectory(const std::string& filename) const;

		/**
		*  @brief
		*    Get a file listing of a search path's directory
		*
		*  @param[in] directory
		*    UTF-8 directory in platform-independent notation to enumerate
		*  @param[out] filenames
		*    Will receive the list of UTF-8 filenames of the files inside the directory, list is not cleared before adding new entries
		*
		*  @remarks
		*    Matching directories are interpolated. That is, if "C:\mydir" is in the
		*    search path and contains a directory "savegames" that contains "x.sav",
		*    "y.sav", and "z.sav", and there is also a "C:\userdir" in the search path
		*    that has a "savegames" subdirectory with "w.sav", then the following code:
		*    @code
		*    std::vector<std::string> filenames;
		*    QSF_FILE.enumerateFiles("savegames", filenames);
		*    for (const std::string& filename : filenames)
		*    {
		*      printf(" * We've got [%s].\n", filename.c_str());
		*    }
		*    @endcode
		*    ...will print:
		*    @verbatim
		*    We've got [x.sav].
		*    We've got [y.sav].
		*    We've got [z.sav].
		*    We've got [w.sav].
		*    @endverbatim
		*
		*    Another example: Enumerating the files inside a directory "data/prefab" might result in
		*   - "vehicle" (directory)
		*   - "person" (directory)
		*    Meaning there's no slash/backslash at the end of the directory name and there's only the
		*    local directory name, without the full path (e.g. "data/prefab/vehicle").
		*
		*    Feel free to sort the list however you like. We only promise there will
		*    be no duplicates, but not what order the final list will come back in.
		*/
		void enumerateFiles(const std::string& directory, std::vector<std::string>& filenames) const;

		/**
		*  @brief
		*    Get a file-only listing of a search path's directory
		*
		*  @param[in] directory
		*    UTF-8 directory in platform-independent notation to enumerate
		*  @param[out] filenames
		*    Will receive the list of UTF-8 filenames of the files inside the directory, list is not cleared before adding new entries
		*
		*  @note
		*    - Ease-of-use method
		*
		*  @see
		*    - "qsf::FileSystem::enumerateFiles"
		*/
		void enumerateOnlyFiles(const std::string& directory, std::vector<std::string>& filenames) const;

		/**
		*  @brief
		*    Get a directory-only listing of a search path's directory
		*
		*  @param[in] directory
		*    UTF-8 directory in platform-independent notation to enumerate
		*  @param[out] filenames
		*    Will receive the list of UTF-8 filenames of the directories inside the directory, list is not cleared before adding new entries
		*
		*  @note
		*    - Ease-of-use method
		*
		*  @see
		*    - "qsf::FileSystem::enumerateFiles"
		*/
		void enumerateOnlyDirectories(const std::string& directory, std::vector<std::string>& filenames) const;

		/**
		*  @brief
		*    Determine if a file exists in the search path
		*
		*  @param[in] filename
		*    UTF-8 filename in platform-independent notation
		*
		*  @return
		*    "true" if the filename exists, else "false"
		*
		*  @remarks
		*    Reports true if there is an entry anywhere in the search path by the
		*    name of the given filename.
		*
		*    Note that entries that are symlinks are ignored if
		*    "qsf::FileSystem::setPermitSymbolicLinks(true)" hasn't been called, so you
		*    might end up further down in the search path than expected.
		*/
		bool exists(const std::string& filename) const;

		/**
		*  @brief
		*    Determine if a file in the search path is really a directory
		*
		*  @param[in] filename
		*    UTF-8 filename in platform-independent notation
		*
		*  @return
		*    "true" if the filename exists and is a directory, else "false"
		*
		*  @remarks
		*    Determine if the first occurrence of the given filename in the search path is
		*    really a directory entry.
		*
		*    Note that entries that are symlinks are ignored if
		*    "qsf::FileSystem::setPermitSymbolicLinks(true)" hasn't been called, so you
		*    might end up further down in the search path than expected.
		*/
		bool isDirectory(const std::string& filename) const;

		/**
		*  @brief
		*    Determine if a file in the search path is really a symbolic link
		*
		*  @param[in] filename
		*    UTF-8 filename in platform-independent notation
		*
		*  @return
		*    "true" if the given filename exists and is a symlink, else "false"
		*
		*  @remarks
		*    Determine if the first occurrence of the given filename in the search path is
		*    really a symbolic link.
		*
		*    Note that entries that are symlinks are ignored if
		*    "qsf::FileSystem::setPermitSymbolicLinks(true)" hasn't been called, and as such,
		*    this function will always return 0 in that case.
		*/
		bool isSymbolicLink(const std::string& filename) const;

		/**
		*  @brief
		*    Get the last modification time of a file
		*
		*  @param[in] filename
		*    UTF-8 filename to check, in platform-independent notation
		*
		*  @return
		*    Last modified time of the file, -1 if it can't be determined
		*
		*  @remarks
		*    The modification time is returned as a number of seconds since the epoch
		*    (Jan 1, 1970). The exact derivation and accuracy of this time depends on
		*    the particular archiver. If there is no reasonable way to obtain this
		*    information for a particular archiver, or there was some sort of error,
		*    this function returns (-1).
		*/
		int64 getLastModificationTime(const std::string& filename) const;

		/**
		*  @brief
		*    Get the file size
		*
		*  @param[in] filename
		*    UTF-8 filename to check, in platform-independent notation
		*
		*  @return
		*    File size, -1 if it can't be determined
		*/
		int64 getFileSize(const std::string& filename) const;

		//[-------------------------------------------------------]
		//[ Additional ease-of-use methods                        ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Delete a directory including all files and directories inside it
		*
		*  @param[in] filename
		*    UTF-8 filename of the directory to delete, files or directories inside the directory are automatically deleted
		*
		*  @return
		*    "true" if all went fine, else "false"
		*
		*  @see
		*    - "qsf::FileSystem::deleteFileOrDirectory()"
		*/
		bool deleteDirectoryRecursive(const std::string& filename) const;

		/**
		*  @brief
		*    Delete all empty directories inside the given directory which will also be deleted in case it's empty
		*
		*  @param[in] filename
		*    UTF-8 filename of the directory to delete empty directories within
		*
		*  @see
		*    - "qsf::FileSystem::deleteFileOrDirectory()"
		*/
		void deleteEmptyDirectoriesRecursive(const std::string& filename) const;


	//[-------------------------------------------------------]
	//[ Public virtual qsf::System methods                    ]
	//[-------------------------------------------------------]
	public:
		inline virtual const char* getName() const override;
		//[-------------------------------------------------------]
		//[ Lifecycle                                             ]
		//[-------------------------------------------------------]
		virtual bool onStartup(bool serverMode) override;
		virtual void onShutdown() override;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		std::string	      mBaseDirectory;		///< The UTF-8 base directory of the application in platform-independent notation (e.g. on Windows: "C:/MyApplication")
		CacheManager&	  mCacheManager;		///< The cache manager instance; we're responsible for cleaning it up
		FileCacheManager& mFileCacheManager;	///< The file cache manager instance; we're responsible for cleaning it up


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/file/FileSystem-inl.h"

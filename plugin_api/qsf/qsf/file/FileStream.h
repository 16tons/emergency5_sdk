// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/base/manager/FastPodAllocator.h"

#include <boost/iostreams/stream.hpp>


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
	*    File stream class
	*
	*  @note
	*    - The external library "PhysicsFS" ( http://icculus.org/physfs/ ) is used internally and connected to "boost::iostreams"
	*    - This is not intended for direct use; instead, see the "qsf::FileStream" typedef below
	*    - "qsf::FileStream" is a standard C++ stream, meaning exceptions will be thrown in case of errors, the destructor and close are handled automatically
	*    - QSF must be initialized before it's possible to use "qsf::FileStream"
	*    - Lookout! When using "qsf::File" directly (not recommended), don't forget to call "qsf::File::close()" because it won't be called automatically inside the destructor.
	*/
	class QSF_API_EXPORT File : public FastPodAllocator<File>
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    File open mode
		*/
		enum OpenMode
		{
			READ_MODE,	///< Open file for read only
			WRITE_MODE,	///< Open file for write only
			APPEND_MODE	///< Open file for append only
		};


	//[-------------------------------------------------------]
	//[ Public Boost iostream definitions                     ]
	//[-------------------------------------------------------]
	public:
		typedef char char_type;
		struct category : boost::iostreams::seekable, boost::iostreams::device_tag, boost::iostreams::closable_tag, boost::iostreams::flushable_tag {};


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in]  filename
		*    UTF-8 filename in platform-independent notation
		*  @param[in]  openMode
		*    The mode to open the file in (read/write/append)
		*  @param[out] guessWriteLocation
		*    Only used for "qsf::File::WRITE_MODE". In case the file does not exist, should the method guess were the newly created file will probably be located at? (warning, use this with caution because the first match will be used)
		*/
		File(const std::string& filename, OpenMode openMode, bool guessWriteLocation = true);

		/**
		*  @brief
		*    Get total length of a file in bytes
		*
		*  @return
		*     File size in bytes of the file. <0 if can't be determined
		*
		*  @remarks
		*    Note that if the file size can't be determined (since the archive is
		*    "streamed" or whatnot) than this will report (-1). Also note that if
		*    another process/thread is writing to this file at the same time, then
		*    the information this function supplies could be incorrect before you
		*    get it. Use with caution, or better yet, don't use at all.
		*/
		int64 getFileLength() const;

		/**
		*  @brief
		*    Check for end-of-file state
		*
		*  @return
		*    "true" if the end-of-file has been reached, else "false"
		*/
		bool isEof() const;

		/**
		*  @brief
		*    Determine current position
		*
		*  @return
		*    Offset in bytes from start of file, <0 if error occurred
		*/
		int64 tell() const;


	//[-------------------------------------------------------]
	//[ Public Boost iostream methods                         ]
	//[-------------------------------------------------------]
	public:
		void close();
		bool flush();
		std::streamsize read(char* s, std::streamsize n);
		std::streamsize write(const char* s, std::streamsize n);
		std::streampos seek(std::streamoff off, std::ios_base::seekdir seekDirection);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		void* mPhysicsFsFile;	///< PhysicsFS file handle, can be a null pointer, close it if you no longer need it (we can't forward declare "PHYSFS_File")


	};


	//[-------------------------------------------------------]
	//[ Definitions                                           ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    "qsf::FileStream" is a standard C++ stream
	*
	*  @remarks
	*    The following example shows how to use "qsf::FileStream":
	*    @code
	*    qsf::FileStream stream("test.txt", qsf::File::WRITE_MODE);
	*    stream << "Just a standard stream: " << 12345 << "\n";
	*    @endcode
	*/
	typedef boost::iostreams::stream<File> FileStream;


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf

// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/Export.h"

#include <boost/iostreams/stream.hpp>

#include <vector>


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
	*    Paged memory buffer implementation
	*
	*  @note
	*    - Do not use this class for working with memory blocks >= 4 GiB, it only uses 32-bit integers
	*/
	class QSF_API_EXPORT PagedMemoryBuffer
	{


	//[-------------------------------------------------------]
	//[ Public Boost iostream definitions                     ]
	//[-------------------------------------------------------]
	public:
		typedef char char_type;
		struct category : boost::iostreams::seekable, boost::iostreams::device_tag {};


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		PagedMemoryBuffer();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~PagedMemoryBuffer();

		/**
		*  @brief
		*    Return whether the buffer is empty
		*/
		bool empty() const;

		/**
		*  @brief
		*    Return the size of the buffer
		*/
		uint32 size() const;

		/**
		*  @brief
		*    Clear the buffer
		*/
		void clear();

		/**
		*  @brief
		*    Copy contents from a stream
		*/
		void copyFromStream(std::istream& stream);

		/**
		*  @brief
		*    Copy contents to a stream
		*/
		void copyToStream(std::ostream& stream) const;


	//[-------------------------------------------------------]
	//[ Public Boost iostream methods                         ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Read a block of memory from the buffer to the given address in memory
		*/
		std::streamsize read(char* address, std::streamsize bytes);

		/**
		*  @brief
		*    Write a block of memory from the given address in memory into the buffer
		*/
		std::streamsize write(const char* address, std::streamsize bytes);

		/**
		*  @brief
		*    Set the cursor position in the buffer
		*/
		std::streampos seek(std::streamoff offset, std::ios_base::seekdir seekDirection);


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		struct Page
		{
			uint32			   mOffset;
			std::vector<uint8> mData;
		};
		typedef std::vector<Page*> PageArray;

		struct Cursor
		{
			uint32	mPosition;		///< Absolute position
			uint32	mPageIndex;		///< Index of the current page
		};

		struct PagePosition
		{
			Page*	mPage;
			uint32	mPositionInPage;
			uint32	mRemainingBytesInPage;
		};


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void reserve(uint32 minimumSize);
		Page& addPage(uint32 pageSize);

		void initializePagePosition(PagePosition& pagePosition);
		void checkPagePositionRemainingBytes(PagePosition& pagePosition);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		PageArray mPages;
		uint32 mSize;
		uint32 mReservedSize;
		Cursor mCursor;


	};


	//[-------------------------------------------------------]
	//[ Definitions                                           ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    "qsf::PagedMemoryStream" is a standard C++ stream
	*
	*  @remarks
	*    The following example shows how to use "qsf::FileStream":
	*    @code
	*    qsf::PagedMemoryBuffer buffer;
	*    qsf::PagedMemoryStream stream(buffer);
	*    stream << "Just a standard stream: " << 12345 << "\n";
	*    @endcode
	*/
	typedef boost::iostreams::stream<PagedMemoryBuffer> PagedMemoryStream;


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf

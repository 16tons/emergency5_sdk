// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/Export.h"

#include <string>
#include <vector>
#include <iosfwd>


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
	*    Static zlib packer class
	*/
	class QSF_API_EXPORT ZlibPacker
	{


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Compress data from a stream using zlib
		*
		*  @param[in] source
		*    Input stream to read data from
		*  @param[in] destination
		*    Output stream to write compressed data to
		*  @param[in] maxSourceBytes
		*    Limit to the number of bytes that will be read from source; use this in case you don't want to compress only a part of source
		*  @param[in] compressionLevel
		*    Zlib compression level between 1 (fastest) and 9 (best compression)
		*
		*  @return
		*    "true" if all went fine, "false" in case of an error
		*/
		static bool compressStream(std::istream& source, std::ostream& destination, uint32 maxSourceBytes = std::numeric_limits<uint32>::max(), int compressionLevel = 7);

		/**
		*  @brief
		*    Uncompress data from a stream using zlib
		*
		*  @param[in] source
		*    Input stream to read compressed data from
		*  @param[in] destination
		*    Output stream to write the uncompressed data to
		*  @param[in] maxSourceBytes
		*    Limit to the number of bytes that will be read from source; use this in case you don't want to decompress only a part of source
		*
		*  @return
		*    "true" if all went fine, "false" in case of an error
		*/
		static bool uncompressStream(std::istream& source, std::ostream& destination, uint32 maxSourceBytes = std::numeric_limits<uint32>::max());

	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf

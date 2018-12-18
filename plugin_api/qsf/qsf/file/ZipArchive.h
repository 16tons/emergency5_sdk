// Copyright (C) 2012-2018 Promotion Software GmbH


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
	*    Static ZIP archive class
	*/
	class QSF_API_EXPORT ZipArchive
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 BUFFER_SIZE;


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	public:
		static bool pack(const std::string& absoluteArchiveFilename, const std::string& absoluteSourcePath, bool recursive = true);
		static bool unpack(const std::string& absoluteArchiveFilename, const std::string& absoluteTargetPath, bool recursive = true);
		static bool deleteFile(const std::string& absoluteArchiveFilename, const std::string& pathInZip);
		static bool updateFile(const std::string& absoluteArchiveFilename, const std::string& pathInZip, const std::string& absoluteSourceFilename, bool tryRemoveFileInZip = true);
		static bool listFiles(const std::string& absoluteArchiveFilename, std::vector<std::string>& files, bool recursive = true);
		static bool unpackFile(const std::string& absoluteArchiveFilename, const std::string& pathInZip, const std::string& absoluteTargetFilename);
		static bool packFileFromStream(const std::string& absoluteArchiveFilename, const std::string& pathInZip, std::istream& srcStream, bool tryRemoveFileInZip = true);
		static bool unpackFileToStream(const std::string& absoluteArchiveFilename, const std::string& pathInZip, std::ostream& destStream);


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf

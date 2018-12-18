// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/forward/BoostPTree.h"
#include "qsf/file/FileStream.h"

#include <boost/filesystem.hpp>
#include <boost/nowide/fstream.hpp>

#include <string>


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
	*    Static file helper class
	*/
	class QSF_API_EXPORT FileHelper
	{


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	public:
		static bool copyDirectoryRecursive(const boost::filesystem::path& source, const boost::filesystem::path& destination, bool onlyNewer = false);
		static bool moveDirectoryRecursive(const boost::filesystem::path& source, const boost::filesystem::path& destination);

		/**
		*  @brief
		*    Usage of "boost::property_tree::read_json" increases the binary size significantly, so, do only use this wrapper
		*/
		static void readJson(std::istream& ifs, boost::property_tree::ptree& outPtree);
		static void readJson(boost::nowide::ifstream& ifs, boost::property_tree::ptree& outPtree);
		static void readJson(boost::nowide::wifstream& ifs, boost::property_tree::wptree& outPtree);
		static void readJson(FileStream& fileStream, boost::property_tree::ptree& outPtree);

		/**
		*  @brief
		*    Usage of "boost::property_tree::write_json" increases the binary size significantly, so, do only use this wrapper
		*/
		static void writeJson(std::ostream& ofs, const boost::property_tree::ptree& inPtree);
		static void writeJson(boost::nowide::ofstream& ofs, const boost::property_tree::ptree& inPtree);
		static void writeJson(boost::nowide::wofstream& ofs, const boost::property_tree::wptree& inPtree);
		static void writeJson(std::stringstream& ss, const boost::property_tree::ptree& inPtree);
		static void writeJson(std::wstringstream& wss, const boost::property_tree::wptree& inPtree);
		static void writeJson(FileStream& fileStream, const boost::property_tree::ptree& inPtree);

		/**
		*  @brief
		*    JSON format header check
		*
		*  @throw
		*   - Throws an exception in case of an error
		*/
		static void validateJsonFormatHeader(const boost::property_tree::ptree& rootPTree, const std::string& formatType, uint32 formatVersion);


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf

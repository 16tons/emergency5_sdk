// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/Export.h"

#include <boost/container/flat_map.hpp>

#include <string>
#include <vector>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class NamedIdentifier;
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
	*    Static string parser helper class
	*/
	class QSF_API_EXPORT StringParser
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		typedef boost::container::flat_map<std::string, std::string> StringMap;


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Checks if the given string contains an integer number, i.e. only characters '0'..'9'
		*
		*  @param[in] string
		*    The string to check
		*
		*  @return
		*    "true" if the string contains a number, "false" otherwise
		*/
		static bool isNumber(const std::string& string);

		/**
		*  @brief
		*    Checks if the given string contains an integer number, i.e. only characters '0'..'9'
		*
		*  @param[in] string
		*    The string to check
		*  @param[out] outNumber
		*    The number stored in the string; only set in case the method returns "true"
		*
		*  @return
		*    "true" if the string contains a number, "false" otherwise
		*/
		static bool isNumber(const std::string& string, uint32& outNumber);

		/**
		*  @brief
		*    Checks if the given string contains an integer number, i.e. only characters '0'..'9'
		*
		*  @param[in] string
		*    The string to check
		*  @param[out] outNumber
		*    The number stored in the string; only set in case the method returns "true"
		*
		*  @return
		*    "true" if the string contains a number, "false" otherwise
		*/
		static bool isNumber(const std::string& string, uint64& outNumber);

		/**
		*  @brief
		*    Parse a uint32 from the given string if possible, otherwise return the string's hash
		*
		*  @param[in] string
		*    The string to check
		*
		*  @return
		*    The string's hash or the number stored as a string if the string contains an integer number
		*/
		static uint32 parseStringHashFromString(const std::string& string);

		/**
		*  @brief
		*    Combine parameter key-value pairs to a string
		*
		*  @param[in] inParameters
		*    The key-values pairs that should be combined
		*
		*  @return
		*    UTF-8 key-values pairs as a string (e.g. "Name=\"Alice\" Description=\"This is the test entity Alice\"")
		*
		*  @note
		*    - use StringParser::parseValuesFromString() to parse the returned string
		*/
		static std::string combineValuesToString(const StringMap& inParameters);

		/**
		*  @brief
		*    Read parameter key-value pairs from a given string
		*
		*  @param[in] valuesString
		*    UTF-8 key-values pairs to set (e.g. "Name=\"Alice\" Description=\"This is the test entity Alice\"")
		*  @param[out] outParameters
		*    Gets filled with the resulting key-values pairs; the map is not automatically cleared
		*
		*  @note
		*    - use StringParser::combineValuesToString() to combine the string from a StringMap
		*/
		static void parseValuesFromString(const std::string& valuesString, StringMap& outParameters);

		/**
		*  @brief
		*    Delete all empty spaces and split string to parts
		*
		*  @param[in] input
		*    String to split
		*  @param[in] parts
		*    Receives the string parts, list is not cleared before new entries are added
		*  @param[in] trimSpaces
		*    If "true", remove spaces at the beginning and end of each string in output
		*/
		static void splitString(const std::string& input, std::vector<std::string>& parts, const std::string& separator = ",", bool trimSpaces = true);
		static void splitString(const std::string& input, std::vector<NamedIdentifier>& parts, const std::string& separator = ",", bool trimSpaces = true);

		/**
		*  @brief
		*    Combine the parts of a string array to one string and add the separator between the strings
		*
		*  @param[in] parts
		*    String parts to combine
		*
		*  @return
		*    The combined string
		*/
		static std::string combineString(const std::vector<std::string>& parts, const std::string& separator = ",");
		static std::string combineString(const std::vector<NamedIdentifier>& parts, const std::string& separator = ",");

		/**
		*  @brief
		*    Convert a number to string with separator
		*
		*  @param[in] number
		*    Number to convert to string with separator
		*
		*  @return
		*    The given number converted to string
		*/
		static std::string numberStringWithSeparator(uint32 number, const std::string& separator);

		/**
		*  @brief
		*    Strip C-style comments from source code
		*
		*  @param[in] sourceCode
		*    Source code to process
		*  @param[in] targetCode
		*    Resulting processed source code
		*/
		static void stripCommentsFromSourceCode(const std::string& sourceCode, std::string& targetCode);
		static void stripCommentsFromSourceCode(const std::wstring& sourceCode, std::wstring& targetCode);

		/**
		*  @brief
		*    Add a value to a comma-separated values string
		*
		*  @param[in] value
		*    Value to add
		*  @param[in] separator
		*    Separator
		*  @param[in, out] commaSeparatedValuesString
		*    Comma separated values string
		*/
		static void addValueToCommaSeparatedValuesString(const std::string& value, char separator, std::string& commaSeparatedValuesString);

		/**
		*  @brief
		*    Remove a value from a comma-separated values string
		*
		*  @param[in] value
		*    Value to remove
		*  @param[in] separator
		*    Separator
		*  @param[in, out] commaSeparatedValuesString
		*    Comma separated values string
		*/
		static void removeValueFromCommaSeparatedValuesString(const std::string& value, char separator, std::string& commaSeparatedValuesString);

		/**
		*  @brief
		*    Return whether or not a given tag in inside a given tags string
		*
		*  @param[in] tags
		*    The UTF-8 tags as semicolon separated string (e.g. "car;person"), tags are not case-sensitive
		*  @param[in] tag
		*    The UTF-8 tag to check (e.g. "car"), tags are not case-sensitive
		*
		*  @return
		*    "true" if the tag has been found, else "false"
		*/
		static bool hasTag(const std::string& tags, const std::string& tag);


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf

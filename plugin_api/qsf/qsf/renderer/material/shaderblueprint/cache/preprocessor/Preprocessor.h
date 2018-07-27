// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/platform/PlatformTypes.h"

#include <map>
#include <string>
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
	*    A simple interface for the shader preprocessor
	*/
	class Preprocessor
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		typedef std::map<uint32, std::string> IncludeContentCache;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Run a shader source string through the preprocessor
		*
		*  @param[in] source
		*    Source string
		*  @param[in] includePath
		*    Path to search for includes (that are included with #include)
		*  @param[in] definitions
		*    Macros to predefine (vector of strings of the format MACRO=value, or just MACRO to define it as 1)
		*  @param[in] name
		*    Name to use for error messages
		*  @param[out] result
		*    Receives the processed code
		*
		*  @return processed string
		*/
		void preprocess(std::string& source, const std::string& includePath, const std::vector<std::string>& definitions, const std::string& name, std::string& result);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		IncludeContentCache mIncludeContentCache;	///< Cached include content


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf

// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/Export.h"

#include <string>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace editor
	{


		//[-------------------------------------------------------]
		//[ Classes                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Static operation helper base class
		*
		*  @remarks
		*    An "operation helper" creates advanced compound operations which consist of atomic operations or other compound operations.
		*    All operation helpers should be derived from this class. This makes it easier to find all operation helpers within e.g.
		*    the automatic code documentation.
		*/
		class QSF_EDITOR_API_EXPORT OperationHelper
		{


		//[-------------------------------------------------------]
		//[ Public static methods                                 ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Helper method to build a string with either singular or plural form of a noun, depending on the given number
			*
			*  @param[in] formatString
			*    The format string, must contain placeholders %1% and %2% as denoted below (example: "You got %1% new %2% in your box")
			*  @param[in] number
			*    The number to insert for %1% in the format string
			*  @param[in] singularString
			*    The singular string, which replaces %2% in case number == 1
			*  @param[in] pluralString
			*    The plural string, which replaces %2% in case number != 1
			*
			*  @return
			*    The resulting string (example: "You got 3 new mails in your box")
			*/
			static std::string stringWithOptionalPlural(const std::string& formatString, uint32 number, const std::string& singularString, const std::string& pluralString);


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf

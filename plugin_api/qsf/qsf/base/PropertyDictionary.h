// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/Export.h"
#include "qsf/base/StringHash.h"
#include "qsf/reflection/CampClass.h"

#include <camp/value.hpp>
#include <camp/camptype.hpp>

#include <unordered_map>


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
	*    Generic key-value pair container
	*
	*  @remarks
	*    A property dictionary holds key-value pairs and is used for user defined parameter sets where neither keys nor types are known are priori.
	*    The usual way of accessing string encoded custom keys is to use the StringHash function and convert these to an uint32.
	*
	*  @note
	*    - (fw) I had to change the internal container type from boost::flat_map to std::unordered_map because of hard-to-resolve compiler errors
	*       when combining boost::flat_map, camp::Value and Ogre headers (as in the QSF precompiled header).
	*    - This class was formerly called "GenericObject", but that could be mixed up with "EGenericObject" from EM4.
	*/
	class QSF_API_EXPORT PropertyDictionary
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		inline PropertyDictionary();

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~PropertyDictionary();

		/**
		*  @brief
		*    Set a parameter value
		*
		*  @param[in] value
		*    The value to set; must be of a type connected to the CAMP reflection system
		*  @param[in] identifier
		*    Identifier or name of the parameter to set
		*/
		template<typename T>
		void set(const StringHash& identifier, const T& value);

		/**
		*  @brief
		*    Get a parameter value and write it to the given variable
		*
		*  @param[out] value
		*    Variable reference to write the parameter value to; must be of a type connected to the CAMP reflection system
		*  @param[in] identifier
		*    Identifier or name of the parameter to return
		*
		*  @return
		*    "true" on success, "false" if the parameter does not exist or could not be converted to the needed type
		*/
		template<typename T>
		bool get(const StringHash& identifier, T& value) const;

		/**
		*  @brief
		*    Return a parameter value
		*
		*  @param[in] identifier
		*    Identifier or name of the parameter to return
		*
		*  @return
		*    The parameter value
		*
		*  @note
		*    - Throws an exception in case the identifier could not be found
		*/
		template<typename T>
		T getSafe(const StringHash& identifier) const;

		/**
		*  @brief
		*    Get an optional parameter value and write it to the given variable
		*
		*  @param[in] identifier
		*    Identifier or name of the parameter to return
		*  @param[in] defaultValue
		*    Default value in case there is no entry for the given identifier; must be of a type connected to the CAMP reflection system
		*
		*  @return
		*    "true" on success, "false" if the parameter does not exist or could not be converted to the needed type
		*/
		template<typename T>
		T getOptional(const StringHash& identifier, const T& defaultValue) const;

		/**
		*  @brief
		*    Add parameter key-value pairs from a given string
		*
		*  @param[in] values
		*    UTF-8 key-values pairs to set (e.g. "Name=\"Alice\" Description=\"This is the test entity Alice\"")
		*
		*  @note
		*    - Don't use this generic method in highly performance critical situations
		*/
		void addValuesFromString(const std::string& values);


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		typedef std::unordered_map<uint32, camp::Value> ValueMap;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		ValueMap mValueMap;		///< Maps uint32 identifiers (usually hashed strings) to generic values


	//[-------------------------------------------------------]
	//[ CAMP reflection system                                ]
	//[-------------------------------------------------------]
	QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
CAMP_TYPE(qsf::PropertyDictionary)


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/base/PropertyDictionary-inl.h"

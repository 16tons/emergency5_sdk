// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/base/StringHash.h"


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
	*    Class combining strings with their hashes
	*
	*  @remarks
	*    In the special case of uninitialized named identifiers (e.g. by default constructor),
	*    the hash does not fit to the empty string, but is "qsf::getUninitialized<uint32>()".
	*/
	class NamedIdentifier
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		inline NamedIdentifier();

		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] name
		*    String to create the named identifier from
		*/
		inline NamedIdentifier(const std::string& name);

		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] name
		*    String to create the named identifier from
		*/
		// Without this something like 'static NamedIdentifier dummy = "name";' wouldn't compile'
		inline NamedIdentifier(const char* name);

		/**
		*  @brief
		*    Destructor
		*
		*  @note
		*    - Not virtual by intent
		*/
		inline ~NamedIdentifier();

		/**
		*  @brief
		*    Check if named identifier is empty
		*
		*  @return
		*    "true" if empty or uninitialized, "false" if not
		*/
		inline bool empty() const;

		/**
		*  @brief
		*    Return the name string
		*
		*  @return
		*    The identifier name
		*/
		inline const std::string& getName() const;

		/**
		*  @brief
		*    Return the generated hash value
		*
		*  @return
		*    The generated FNV-1a hash value
		*/
		inline uint32 getHash() const;

		/**
		*  @brief
		*    Return the name string
		*
		*  @return
		*    The identifier name
		*/
		inline operator const std::string&() const;

		/**
		*  @brief
		*    Return the generated hash value
		*
		*  @return
		*    The generated FNV-1a hash value
		*/
		inline operator uint32() const;

		/**
		*  @brief
		*    Return the identifier as a "qsf::StringHash"
		*
		*  @return
		*    The string hash instance, containing only the hash
		*/
		inline operator StringHash() const;

		/**
		*  @brief
		*    Compare two named identifiers
		*
		*  @param[in] other
		*    Named identifier to compare with
		*
		*  @return
		*    "true" if they are alike
		*/
		inline bool operator ==(const NamedIdentifier& other) const;

		/**
		*  @brief
		*    Compare named identifier with uint32
		*
		*  @param[in] other
		*    uint32 to compare with
		*
		*  @return
		*    "true" if they are alike
		*/
		inline bool operator ==(uint32 other) const;

		/**
		*  @brief
		*    Compare named identifier with string
		*
		*  @param[in] other
		*    String to compare with
		*
		*  @return
		*    "true" if they are alike
		*/
		inline bool operator ==(const std::string& other) const;

		/**
		*  @brief
		*    Reset to an empty string with uninitialized hash
		*/
		inline void reset();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		std::string mName;	///< The identifier name as a string
		uint32		mHash;	///< The identifier as FNV-1a hash value of the name


	};


	// When adding a const char* and a NamedIdentifier, the NamedIdentifier is interpreted as uint32 by default - not the expected behavior
	inline std::string operator +(const char* lhs, const NamedIdentifier& rhs);
	inline std::string operator +(const NamedIdentifier& lhs, const char* rhs);


	// Declare specialization for getUninitialized()
	template <>
	inline NamedIdentifier getUninitialized<NamedIdentifier>();


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/base/NamedIdentifier-inl.h"

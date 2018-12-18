// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/platform/PlatformTypes.h"

#include <string>
#undef DOMAIN	// "math.h" is defining a macro with this name making it impossible to use it as enum name


//[-------------------------------------------------------]
//[ Preprocessor definitions                              ]
//[-------------------------------------------------------]
// For improved debugging, we want to have the source string the string hash was calculated from as backup
#ifdef RETAIL
	#define STRING_HASH_SOURCE			   // Empty
	#define STRING_HASH_GET_SOURCE()	   // Empty
	#define STRING_HASH_SET_SOURCE(SOURCE) // Empty
#else
	#define STRING_HASH_SOURCE			   std::string mSourceString
	#define STRING_HASH_GET_SOURCE()	   inline const std::string& getSourceString() const { return mSourceString; }
	#define STRING_HASH_SET_SOURCE(SOURCE) mSourceString = SOURCE
#endif


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
	*    Compile time string hash class
	*
	*  @remarks
	*    The following example shows how to use the string hash class:
	*    @code
	*    uint32 hash = qsf::StringHash("qsf::MetadataComponent");	// Result will be 483975945
	*    @endcode
	*
	*  @note
	*    - Compile-time string hashing as described within the Gamasutra article "In-Depth: Quasi Compile-Time String Hashing"
	*      by Stefan Reinalter ( http://www.gamasutra.com/view/news/38198/InDepth_Quasi_CompileTime_String_Hashing.php#.UG1MpVFQbq4 )
	*    - As hash function FNV-1a is used ( http://isthe.com/chongo/tech/comp/fnv/ )
	*/
	class StringHash
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    "const char*"-wrapper enabling the compiler to distinguish between static and dynamic strings
		*/
		struct ConstCharWrapper
		{
			/**
			*  @brief
			*    "const char*"-wrapper constructor
			*
			*  @param[in] string
			*    Null terminated ASCII string to calculate the hash value for, must be valid
			*
			*  @note
			*    - Not explicit by intent
			*/
			QSF_FORCEINLINE ConstCharWrapper(const char* string) : mString(string) {}

			/**
			*  @brief
			*    "std::string"-wrapper constructor
			*
			*  @param[in] string
			*    Null terminated ASCII string to calculate the hash value for
			*
			*  @note
			*    - Not explicit by intent
			*/
			QSF_FORCEINLINE ConstCharWrapper(const std::string& string) : mString(string.c_str()) {}

			const char* mString;	///< Null terminated ASCII string to calculate the hash value for, must be valid
		};


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Calculate the hash value of the given string
		*
		*  @param[in] string
		*    Null terminated ASCII string to calculate the hash value for, must be valid
		*
		*  @return
		*    The hash value of the given string
		*/
		static QSF_FORCEINLINE uint32 calculateFNV(const char* string);


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor for calculating the hash value of a given static string
		*
		*  @param[in] string
		*    Static string to calculate the hash value for, must be valid
		*/
		template <uint32 N>
		QSF_FORCEINLINE StringHash(const char (&string)[N]);

		/**
		*  @brief
		*    Constructor for calculating the hash value of a given dynamic string
		*
		*  @param[in] string
		*    Dynamic string to calculate the hash value for, must be valid
		*/
		QSF_FORCEINLINE explicit StringHash(const ConstCharWrapper& string);

		/**
		*  @brief
		*    Constructor for directly setting an uint32 value as string hash
		*
		*  @param[in] hash
		*    Hash value to set
		*
		*  @note
		*    - By intent not explicit for best possible usability
		*/
		QSF_FORCEINLINE StringHash(uint32 hash);

		/**
		*  @brief
		*    Copy constructor
		*
		*  @param[in] stringHash
		*    String hash instance to copy from
		*
		*  @note
		*    - Don't make this explicit or you will loose the string debugging introspection functionality
		*/
		QSF_FORCEINLINE StringHash(const StringHash& stringHash);

		/**
		*  @brief
		*    Return the generated hash value
		*
		*  @return
		*    The generated FNV-1a hash value
		*/
		QSF_FORCEINLINE uint32 getHash() const;

		/**
		*  @brief
		*    Return the generated hash value
		*
		*  @return
		*    The generated FNV-1a hash value
		*/
		QSF_FORCEINLINE operator uint32() const;

		STRING_HASH_GET_SOURCE()


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		uint32 mHash;	///< The generated FNV-1a hash value
		STRING_HASH_SOURCE;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/base/StringHash-inl.h"

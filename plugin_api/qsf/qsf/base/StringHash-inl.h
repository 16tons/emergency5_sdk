// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Preprocessor definitions                              ]
//[-------------------------------------------------------]
#ifdef RETAIL
	#define STRING_HASH_MANAGER_REGISTRATION(string, hash)	// Empty
#else
	#define STRING_HASH_REGISTER_AT_MANAGER
	#define STRING_HASH_MANAGER_REGISTRATION(string, hash) StringHashManager::registerStringHash(string, mHash)
#endif


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/base/GetUninitialized.h"

#ifdef STRING_HASH_REGISTER_AT_MANAGER
	#include "qsf/base/manager/StringHashManager.h"
#endif

#include <string.h>	// For "strlen()"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Helper templates                                      ]
	//[-------------------------------------------------------]
	namespace StringHashTemplate
	{
		/**
		*  @brief
		*    String hash helper template
		*/
		template <uint32 N, uint32 I>
		struct FnvHash
		{
			/**
			*  @brief
			*    Calculate the hash value for the given static string
			*
			*  @param[in] string
			*    Static ASCII string to calculate the hash value for
			*
			*  @return
			*    The calculated hash value
			*/
			QSF_FORCEINLINE static uint32 hash(const char (&string)[N])
			{
				return (FnvHash<N, I-1>::hash(string) ^ string[I-1]) * 16777619u;
			}
		};

		/**
		*  @brief
		*    String hash helper template
		*/
		template <uint32 N>
		struct FnvHash<N, 1>
		{
			/**
			*  @brief
			*    Calculate the hash value for the given static string
			*/
			QSF_FORCEINLINE static uint32 hash(const char (&string)[N])
			{
				return (2166136261u ^ string[0]) * 16777619u;
			}
		};

		/**
		*  @brief
		*    String hash helper template
		*
		*  @remarks
		*    Specialization for empty strings, which we force to always use "qsf::getUninitialized<uint32>()" as string hash
		*/
		template <>
		struct FnvHash<1, 1>
		{
			/**
			*  @brief
			*    Calculate the hash value for the given static string
			*/
			QSF_FORCEINLINE static uint32 hash(const char (&)[1])
			{
				return getUninitialized<uint32>();
			}
		};
	}


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	QSF_FORCEINLINE uint32 StringHash::calculateFNV(const char* string)
	{
		if (string[0] == 0)
			return getUninitialized<uint32>();	// Force "uninitialized" value as hash for empty strings

		const size_t length = strlen(string) + 1;
		uint32 hash = 2166136261u;
		for (size_t i = 0; i < length; ++i)
		{
			hash ^= *string++;
			hash *= 16777619u;
		}
		return hash;
	}


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	template <uint32 N>
	QSF_FORCEINLINE StringHash::StringHash(const char (&string)[N]) :
		mHash(StringHashTemplate::FnvHash<N, N>::hash(string))
	{
		// Register this string hash at the manager
		// -> This is only done in debug builds, otherwise this line is removed by the compiler
		STRING_HASH_MANAGER_REGISTRATION(string, hash);

		// Backup the source string the string hash was calculated from
		// -> This is only done in debug builds, otherwise this line is removed by the compiler
		STRING_HASH_SET_SOURCE(string);
	}

	QSF_FORCEINLINE StringHash::StringHash(const ConstCharWrapper& string) :
		mHash(calculateFNV(string.mString))
	{
		// Register this string hash at the manager
		// -> This is only done in debug builds, otherwise this line is removed by the compiler
		STRING_HASH_MANAGER_REGISTRATION(string.mString, hash);

		// Backup the source string the string hash was calculated from
		// -> This is only done in debug builds, otherwise this line is removed by the compiler
		STRING_HASH_SET_SOURCE(string.mString);
	}

	QSF_FORCEINLINE StringHash::StringHash(uint32 hash) :
		mHash(hash)
	{
		// Nothing do to in here
	}

	QSF_FORCEINLINE StringHash::StringHash(const StringHash& stringHash) :
		mHash(stringHash.mHash)
	{
		// Backup the source string the string hash was calculated from
		// -> This is only done in debug builds, otherwise this line is removed by the compiler
		STRING_HASH_SET_SOURCE(stringHash.mSourceString);
	}

	QSF_FORCEINLINE uint32 StringHash::getHash() const
	{
		return mHash;
	}

	QSF_FORCEINLINE StringHash::operator uint32() const
	{
		return mHash;
	}

	// Specialization for uninitialized values
	template <>
	QSF_FORCEINLINE StringHash getUninitialized<StringHash>()
	{
		return std::numeric_limits<uint32>::max();
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf

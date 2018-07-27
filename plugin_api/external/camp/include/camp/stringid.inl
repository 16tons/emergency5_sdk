/*********************************************************\
 * Copyright (c) 2012-2015 Christian Ofenberg
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 * and associated documentation files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 * BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
\*********************************************************/


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <string.h>	// For "strlen()", we could get rid of this include by reimplementing this tiny function, on the other hand, it's not really worth it because this header is usally already included indirectly by other headers


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace camp
{


	//[-------------------------------------------------------]
	//[ Helper templates                                      ]
	//[-------------------------------------------------------]
	namespace detail
	{
		namespace StringHashTemplate
		{
			/**
			*  @brief
			*    String hash helper template
			*/
			template <uint32_t N, uint32_t I>
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
				FORCEINLINE static uint32_t hash(const char (&string)[N])
				{
					return (FnvHash<N, I-1>::hash(string) ^ string[I-1]) * 16777619u;
				}
			};

			/**
			*  @brief
			*    String hash helper template
			*/
			template <uint32_t N>
			struct FnvHash<N, 1>
			{
				/**
				*  @brief
				*    Calculate the hash value for the given static string
				*/
				FORCEINLINE static uint32_t hash(const char (&string)[N])
				{
					return (2166136261u ^ string[0]) * 16777619u;
				}
			};

			/**
			*  @brief
			*    String hash helper template
			*
			*  @remarks
			*    Specialization for empty strings, which we force to always use "MAXIMUM_UINT32_T_VALUE" as string hash
			*/
			template <>
			struct FnvHash<1, 1>
			{
				/**
				*  @brief
				*    Calculate the hash value for the given static string
				*/
				FORCEINLINE static uint32_t hash(const char (&)[1])
				{
					return StringId::MAXIMUM_UINT32_T_VALUE;
				}
			};
		}
	}


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	inline uint32_t StringId::calculateFNV(const char* string)
	{
		if (string[0] == 0)
			return MAXIMUM_UINT32_T_VALUE;	// Force "uninitialized" value as hash for empty strings

		const size_t length = strlen(string) + 1;
		uint32_t hash = 2166136261u;
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
	FORCEINLINE StringId::StringId() :
		mId(MAXIMUM_UINT32_T_VALUE)
	{
		// Nothing here
	}

	template <uint32_t N>
	FORCEINLINE StringId::StringId(const char (&string)[N]) :
		mId(detail::StringHashTemplate::FnvHash<N, N>::hash(string))
	{
		// Nothing here
	}

	FORCEINLINE StringId::StringId(const ConstCharWrapper& string) :
		mId(calculateFNV(string.mString))
	{
		// Nothing here
	}

	FORCEINLINE StringId::StringId(uint32_t id) :
		mId(id)
	{
		// Nothing do to in here
	}

	FORCEINLINE StringId::StringId(const StringId& stringId) :
		mId(stringId.mId)
	{
		// Nothing here
	}

	FORCEINLINE uint32_t StringId::getId() const
	{
		return mId;
	}

	FORCEINLINE StringId::operator uint32_t() const
	{
		return mId;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // camp

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
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <stdint.h>	// For uint32_t, uint64_t etc.


//[-------------------------------------------------------]
//[ C++ compiler keywords                                 ]
//[-------------------------------------------------------]
#if defined(_MSC_VER)
	/**
	*  @brief
	*    Force the compiler to inline something
	*
	*  @note
	*    - Do only use this when you really have to, usually it's best to let the compiler decide by using the standard keyword "inline"
	*/
	#define FORCEINLINE __forceinline
#elif defined(__clang__) || defined(__GNUG__)
	/**
	*  @brief
	*    Force the compiler to inline something
	*
	*  @note
	*    - Do only use this when you really have to, usually it's best to let the compiler decide by using the standard keyword "inline"
	*/
	#define FORCEINLINE inline __attribute__((always_inline))
#elif __APPLE__
	/**
	*  @brief
	*    Force the compiler to inline something
	*
	*  @note
	*    - Do only use this when you really have to, usually it's best to let the compiler decide by using the standard keyword "inline"
	*/
	#if __has_attribute(always_inline)
		#define FORCEINLINE inline __attribute__((always_inline))
	#else
		#define FORCEINLINE inline
	#endif
#else
	#error "Unsupported platform"
#endif


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace camp
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Compile time string ID class generating an identifier by hashing a given string
	*
	*  @remarks
	*    The following example shows how to use the string ID class:
	*    @code
	*    uint32_t id = camp::StringId("Example/Font/Default/LinBiolinum_R");	// Result will be 64363173
	*    @endcode
	*
	*  @note
	*    - Is using compile-time string hashing as described within the Gamasutra article "In-Depth: Quasi Compile-Time String Hashing"
	*      by Stefan Reinalter ( http://www.gamasutra.com/view/news/38198/InDepth_Quasi_CompileTime_String_Hashing.php#.UG1MpVFQbq4 )
	*    - As hash function FNV-1a is used ( http://isthe.com/chongo/tech/comp/fnv/ )
	*/
	class StringId
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32_t MAXIMUM_UINT32_T_VALUE = 4294967295;	///< We don't want to include "<limits>" in this lightweight core header just to be able to use "std::numeric_limits<uint32_t>::max()"

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
			inline ConstCharWrapper(const char* string) :
				mString(string)
			{
				// Nothing here
			}

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
		static inline uint32_t calculateFNV(const char* string);


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		FORCEINLINE StringId();

		/**
		*  @brief
		*    Constructor for calculating the hash value of a given static string
		*
		*  @param[in] string
		*    Static string to calculate the hash value for, must be valid
		*/
		template <uint32_t N>
		FORCEINLINE StringId(const char (&string)[N]);

		/**
		*  @brief
		*    Constructor for calculating the hash value of a given dynamic string
		*
		*  @param[in] string
		*    Dynamic string to calculate the hash value for, must be valid
		*/
		FORCEINLINE explicit StringId(const ConstCharWrapper& string);

		/**
		*  @brief
		*    Constructor for directly setting an uint32_t value as string ID
		*
		*  @param[in] id
		*    ID value to set
		*
		*  @note
		*    - By intent not explicit for best possible usability
		*/
		FORCEINLINE StringId(uint32_t id);

		/**
		*  @brief
		*    Copy constructor
		*
		*  @param[in] stringId
		*    String ID instance to copy from
		*/
		FORCEINLINE StringId(const StringId& stringId);

		/**
		*  @brief
		*    Return the generated ID
		*
		*  @return
		*    The generated FNV-1a hash value used as identifier
		*/
		FORCEINLINE uint32_t getId() const;

		/**
		*  @brief
		*    Return the generated ID
		*
		*  @return
		*    The generated FNV-1a hash value used as identifier
		*/
		FORCEINLINE operator uint32_t() const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		uint32_t mId;	///< The generated FNV-1a hash value which is used as identifier


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // camp


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include <camp/stringid.inl>

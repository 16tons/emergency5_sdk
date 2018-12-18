// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


// Macros used later on in this file
#define QSF_STRINGIFY(x) _QSF_STRINGIFY_IMPL(x)
#define _QSF_STRINGIFY_IMPL(x) #x


//[-------------------------------------------------------]
//[ C++11 definitions                                     ]
//[-------------------------------------------------------]
#ifdef WIN32
	// Microsoft Visual Studio 2010: Have a look at http://blogs.msdn.com/b/vcblog/archive/2010/04/06/c-0x-core-language-features-in-vc10-the-table.aspx see which C++11 features are supported

	// "nullptr"-definition
	#if defined(__INTEL_COMPILER) || !defined(_MSC_VER) || _MSC_VER < 1600	// The Intel C++ compiler has no support for nullptr, 1600 = Microsoft Visual Studio 2010
		/**
		*  @brief
		*    nullptr definition for compilers don't supporting this C++11 feature
		*
		*  @note
		*    - The implementation comes from the "A name for the null pointer: nullptr"-document (http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2007/n2431.pdf)
		*/
		const class
		{
			public:
				template<class T> operator T*() const  { return 0; }
				template<class C, class T> operator T C::*() const  { return 0; }
			private:
				void operator&() const;
		} nullptr = {};
	#endif
#elif LINUX
	// GCC: Have a look at http://gcc.gnu.org/projects/cxx0x.html see which C++11 features are supported

	// "nullptr"-definition
	#if (__GNUC__ == 4 && __GNUC_MINOR__ == 5 && __GNUC_PATCHLEVEL__ < 3)
		// There's a bug in GCC 4.5.x ... 4.5.2 causing compiler errors when using the nullptr-class solution from below:
		//   "Bug 45383 - [4.5 Regression] Implicit conversion to pointer does no longer automatically generate operator== and operator!=."
		//   (http://gcc.gnu.org/bugzilla/show_bug.cgi?id=45383)
		// ... looked a few minutes for a solution, without success... (just defining it as 0 is no solution and will introduce other compiler errors!)
		#error "Due to a bug in GCC 4.5.x ... 4.5.2 this GCC version is not supported. Please use a newer or older GCC version instead."
	#elif ((__GNUC__ < 4 || (__GNUC__ == 4 && __GNUC_MINOR__ < 6)) && !defined(__clang__))
		/**
		*  @brief
		*    nullptr definition for compilers don't supporting this C++11 feature
		*
		*  @note
		*    - The implementation comes from the "A name for the null pointer: nullptr"-document (http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2007/n2431.pdf)
		*/
		const class
		{
			public:
				template<class T> operator T*() const  { return 0; }
				template<class C, class T> operator T C::*() const  { return 0; }
			private:
				void operator&() const;
		} nullptr = {};
	#endif

	// "override"-definition, see e.g. http://www2.research.att.com/~bs/C++0xFAQ.html#override (C++11 keyword is introduced in GCC 4.7 or clang 2.9)
	#if ((__GNUC__ < 4 || __GNUC_MINOR__ < 7) && !defined(__clang__))
		#define override
	#endif
#elif __APPLE__
	// Clang: Have a look at http://clang.llvm.org/cxx_status.html to see which C++11 features are supported

	// Since we only support OS X Yosemite and newer we use the current stable Xcode 6.1.
	// This version of Xcode ships a modified clang 3.5 version, which includes full support for all C++11 features
	#if !defined(__clang__) || __clang_major__ < 3 || (__clang_major__ == 3 && __clang_minor__ < 5)
		#error "Usupported clang version"
	#endif

	// Use __has_builtin(x) and similar macros as described here for feature-checking: http://clang.llvm.org/docs/LanguageExtensions.html#feature-checking-macros
#else
	#error "Unsupported platform"
#endif


//[-------------------------------------------------------]
//[ C++ compiler keywords                                 ]
//[-------------------------------------------------------]
#ifdef WIN32
	/**
	*  @brief
	*    Force the compiler to inline something
	*
	*  @note
	*    - Do only use this when you really have to, usually it's best to let the compiler decide by using the standard keyword "inline"
	*/
	#define QSF_FORCEINLINE __forceinline

	/**
	*  @brief
	*    No operation macro ("_asm nop"/"__nop()")
	*/
	#define QSF_NOP __nop()
#elif LINUX
	/**
	*  @brief
	*    Force the compiler to inline something
	*
	*  @note
	*    - Do only use this when you really have to, usually it's best to let the compiler decide by using the standard keyword "inline"
	*/
	#define QSF_FORCEINLINE __attribute__((always_inline))

	/**
	*  @brief
	*    No operation macro ("_asm nop"/__nop())
	*/
	#define QSF_NOP __nop()	// TODO(co) This will probably not work, when it's time for Unix ports, review this
#elif __APPLE__
	/**
	*  @brief
	*    Force the compiler to inline something
	*
	*  @note
	*    - Do only use this when you really have to, usually it's best to let the compiler decide by using the standard keyword "inline"
	*/
	#if __has_attribute(always_inline)
		#define QSF_FORCEINLINE inline __attribute__((always_inline))
	#else
		#define QSF_FORCEINLINE inline
	#endif

	/**
	*  @brief
	*    No operation macro (dead instruction taken from http://stackoverflow.com/questions/8188045/llvm-no-op-instruction)
	*/
	#define QSF_NOP
#else
	#error "Unsupported platform"
#endif


//[-------------------------------------------------------]
//[ Data types                                            ]
//[-------------------------------------------------------]
#ifdef WIN32
	// Standard types
	typedef signed __int8			int8;
	typedef unsigned __int8			uint8;

	typedef signed __int16			int16;
	typedef unsigned __int16		uint16;

	typedef signed __int32			int32;
	typedef unsigned __int32		uint32;

	typedef signed __int64			int64;
	typedef unsigned __int64		uint64;

	#if QSF_X64
		typedef unsigned __int64	uint_ptr;
		typedef unsigned __int64	handle;
	#else
		typedef unsigned __int32	uint_ptr;
		typedef unsigned __int32	handle;
	#endif

	#ifndef NULL_HANDLE
		#define NULL_HANDLE 0
	#endif
#else
	#include <stdint.h>

	typedef signed char			int8;
	typedef unsigned char		uint8;

	typedef signed short		int16;
	typedef unsigned short		uint16;

	typedef signed int			int32;
	typedef unsigned int		uint32;

	typedef int64_t 			int64;
	typedef uint64_t			uint64;

	typedef uintptr_t			uint_ptr;
	typedef uintptr_t			handle;
	#ifndef NULL_HANDLE
		#define NULL_HANDLE 0
	#endif
#endif


//[-------------------------------------------------------]
//[ Pragma macros                                         ]
//[-------------------------------------------------------]
/**
*  @brief
*    Backup the current warning state for all warnings
*/
#ifdef WIN32
	#ifdef __MINGW32__
		#define QSF_WARNING_PUSH
	#else
		#define QSF_WARNING_PUSH __pragma(warning(push))
	#endif
#elif LINUX
	#define QSF_WARNING_PUSH
#elif __APPLE__
	#ifdef __clang__
		#define QSF_WARNING_PUSH _Pragma("clang diagnostic push")
	#else
		#define QSF_WARNING_PUSH
	#endif
#else
	#error "Unsupported platform"
#endif

/**
*  @brief
*    Disable a MSVC warning
*
*  @param[in] WarningID
*    ID of the MSVC warning to disable
*
*  @note
*    - Not every compiler has the same warning numbers, so, use this macro instead of using the pragma directly
*    - Do only disable warnings for certain code parts, not in general, warnings are your friend and help you to maintain a decend quality
*    - Use "QSF_WARNING_PUSH" before changing warning states and "QSF_WARNING_POP" to restore the previous settings
*    - Do always add a semicolon at the end of the macro usage or Doxygen (automatic code documentation generation) get's confused, write
*      "QSF_WARNING_DISABLE_MSVC(4275);" instead of "QSF_WARNING_DISABLE_MSVC(4275)"
*    - Do always write as comment what the warning number means, e.g.
*      QSF_WARNING_DISABLE_MSVC(4275);	// warning C4275: non dll-interface class '<x>' used as base for dll-interface class '<y>'
*/
#ifdef WIN32
	#ifdef __MINGW32__
		#define QSF_WARNING_DISABLE_MSVC(WarningID)
	#else
		#define QSF_WARNING_DISABLE_MSVC(WarningID) __pragma(warning(disable: WarningID))
	#endif
#elif LINUX
	#define QSF_WARNING_DISABLE_MSVC(WarningID)
#elif __APPLE__
	#define QSF_WARNING_DISABLE_MSVC(WarningID)
#else
	#error "Unsupported platform"
#endif

/**
*  @brief
*    Set a MSVC warning to its default behaviour
*
*  @param[in] WarningID
*    ID of the MSVC warning to set to its default behaviour
*/
#ifdef WIN32
	#ifdef __MINGW32__
		#define QSF_WARNING_DEFAULT_MSVC(WarningID)
	#else
		#define QSF_WARNING_DEFAULT_MSVC(WarningID) __pragma(warning(default: WarningID))
	#endif
#elif LINUX
	#define QSF_WARNING_DEFAULT_MSVC(WarningID)
#elif __APPLE__
	#define QSF_WARNING_DISABLE_MSVC(WarningID)
#else
	#error "Unsupported platform"
#endif

/**
 *  @brief
 *    Disable a clang warning
 *
 *  @param[in] WarningFlag
 *    "-W"-flag of the clang warning to disable
 *
 *  @note
 *    - Not every compiler has the same warning flags, so, use this macro instead of using the pragma directly
 *    - Do only disable warnings for certain code parts, not in general, warnings are your friend and help you to maintain a decend quality
 *    - Use "QSF_WARNING_PUSH" before changing warning states and "QSF_WARNING_POP" to restore the previous settings
 *    - Do always add a semicolon at the end of the macro usage or Doxygen (automatic code documentation generation) get's confused, write
 *      'QSF_WARNING_DISABLE_CLANG("-Wbuiltin-macro-redefined");' instead of 'QSF_WARNING_DISABLE_CLANG("-Wbuiltin-macro-redefined")'
 *    - Do always write as comment what the warning flag means, e.g.
 *      QSF_WARNING_DISABLE_CLANG("-Wbuiltin-macro-redefined");	// warning -Wbuiltin-macro-redefined: suppress builtin macro redefined warnings
 */
#ifdef WIN32
	#define QSF_WARNING_DISABLE_CLANG(WarningFlag)
#elif LINUX
	#define QSF_WARNING_DISABLE_CLANG(WarningFlag)
#elif __APPLE__
	#ifdef __clang__
		#define _QSF_WARNING_DISABLE_CLANG_HELPER(WarningFlagString) QSF_STRINGIFY(clang diagnostic ignored WarningFlagString)
		#define QSF_WARNING_DISABLE_CLANG(WarningFlag) _Pragma(_QSF_WARNING_DISABLE_CLANG_HELPER(QSF_STRINGIFY(WarningFlag)))
	#else
		#define QSF_WARNING_DISABLE_CLANG(WarningFlag)
	#endif
#else
	#error "Unsupported platform"
#endif

/**
*  @brief
*    Restore the last warning state pushed onto the stack
*/
#ifdef WIN32
	#ifdef __MINGW32__
		#define QSF_WARNING_POP
	#else
		#define QSF_WARNING_POP __pragma(warning(pop))
	#endif
#elif LINUX
	#define QSF_WARNING_POP
#elif __APPLE__
	#ifdef __clang__
		#define QSF_WARNING_POP _Pragma("clang diagnostic pop")
	#else
		#define QSF_WARNING_POP
	#endif
#else
	#error "Unsupported platform"
#endif


//[-------------------------------------------------------]
//[ Project independent generic export                    ]
//[-------------------------------------------------------]
#ifdef WIN32
	// To export classes, methods and variables
	#define QSF_GENERIC_API_EXPORT	__declspec(dllexport)

	// To export functions
	#define QSF_GENERIC_FUNCTION_EXPORT	extern "C" __declspec(dllexport)
#elif LINUX
	// To export classes, methods and variables
	#if defined(HAVE_VISIBILITY_ATTR)
		#define QSF_GENERIC_API_EXPORT __attribute__ ((visibility("default")))
	#else
		#define QSF_GENERIC_API_EXPORT
	#endif

	// To export functions
	#define QSF_GENERIC_FUNCTION_EXPORT	__attribute__ ((visibility("default")))
#elif __APPLE__
	// To export classes, methods and variables
	#define QSF_GENERIC_API_EXPORT __attribute__ ((visibility("default")))

	// To export functions
	#define QSF_GENERIC_FUNCTION_EXPORT	extern "C" __attribute__ ((visibility("default")))
#else
	#error "Unsupported platform"
#endif


//[-------------------------------------------------------]
//[ Project independent generic import                    ]
//[-------------------------------------------------------]
#ifdef WIN32
	// To import classes, methods and variables
	#define QSF_GENERIC_API_IMPORT	__declspec(dllimport)
#elif LINUX
	// To import classes, methods and variables
	#define QSF_GENERIC_API_IMPORT
#elif __APPLE__
	// To import classes, methods and variables
	#define QSF_GENERIC_API_IMPORT
#else
	#error "Unsupported platform"
#endif

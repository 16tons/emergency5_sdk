// From http://alfps.wordpress.com/2010/06/05/cppx-w4-no-warnings-part-ii-disabling-msvc-sillywarnings/
// Copyright (c) Alf P. Steinbach, 2010.
// #include <progrock/cppx/compiler_specific/msvc/no_sillywarnings_please.h>

#ifndef PROGROCK_CPPX_COMPILERSPECIFIC_MSVC_NOSILLYWARNINGSPLEASE_H
#define PROGROCK_CPPX_COMPILERSPECIFIC_MSVC_NOSILLYWARNINGSPLEASE_H

#ifndef _MSC_VER
#   error This file is specific to the MSVC (Microsoft Visual C++) compiler.
#endif

#ifndef CPPX_ALLOW_WP64
#   // The /Wp64 option generates spurious warnings when a __w64 type argument selects
#   // a correct overload with non-__w64 formal argument type, i.e. for <<. In newer
#   // versions of MSVC this option is deprecated. It Really Annoyed a lot of people!
#   ifdef  _Wp64
#       error Do not use the /Wp64 option: use a 64-bit compiler to detect 64-bit portability issues.
#   endif
#endif

#pragma warning( disable: 4250 )    // inherits (implements) some member via dominance
#pragma warning( disable: 4251 )    // needs to have dll-interface to be used by clients
#pragma warning( disable: 4275 )    // exported class derived from non-exported class
#pragma warning( disable: 4996 )    // 'XXX': Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators' (XXX)
#pragma warning( disable: 4520 )    // 'XXX' : multiple default constructors specified
#pragma warning( disable: 4316 )    // 'XXX' : object allocated on the heap may not be aligned 16
#pragma warning( disable: 4503 )    // warning C: 'XXX': decorated name length exceeded, name was truncated

#endif

Boost 1.57.0 (November 3rd, 2014)
- License: "Boost Software License" ( http://www.boost.org/users/license.html )
- Download from: http://www.boost.org/


= Also Contains the Addons =
Boost.Hash 2013-Aug-20 18:01:03
- License: "Boost Software License" ( http://www.boost.org/users/license.html )
- Download from: http://svn.boost.org/svn/boost/sandbox/hash/
- More information: http://www.cs.mcgill.ca/~smcmur/hash/

Boost.Nowide 2012-Jun-02 18:01:03
- License: "Boost Software License" ( http://www.boost.org/users/license.html )
- Download from: http://cppcms.com/files/nowide/
- More information: http://cppcms.com/files/nowide/html/


== Build Boost ==
Build bjam:	bootstrap.bat
x86:		bjam --toolset=msvc-12.0 --build-type=complete stage link=static runtime-link=shared debug release
x64:		bjam --toolset=msvc-12.0 --build-type=complete architecture=x86 address-model=64 stage link=static runtime-link=shared debug release

Or instead of --build-type=complete specify the version to build by combining the options below to speed up the build time.
The options are (no guarantee for completeness)

debug					builds debug versions (can be specified additionally to release)
release					builds relase versions (can be specified additionally to debug)
link=shared				builds shared libraries that are linked dynamically
link=static				builds static libraries that are linked statically
runtime-link=static		uses a static version of the runtime
runtime-link=shared 	uses a shared version of the runtime
--with=X				builds only the library boost_X


== Boost 1.57.0 Visual Studio 2013 Issues ==
- "qsf::TsTranslatorSerializer::serialize()": Broken in Boost 1.57.0, see (lucky us that this QSF method is not critical)
	- "property_tree commit 8af8b6b breaks VS2013 64-bit build" - https://svn.boost.org/trac/boost/ticket/10272
	- https://groups.google.com/forum/#!topic/boost-devel-archive/amznx_SpZVk
	- https://github.com/sanderbog/property_tree/commit/4e16d57fa634fc9ddb64cde1c687e11e23b6e34a
- A C4267 "possible loss of data" warning when compiling on VS2013 x64 with /W4
	- https://svn.boost.org/trac/boost/ticket/10818
	- Made the minor header fix described in the link above

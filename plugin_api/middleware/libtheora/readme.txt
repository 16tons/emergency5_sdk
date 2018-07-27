libtheora-1.1.1
- License: "BSD-STYLE"
- Used by the QSF OGG/Theora Video backend (no additional external dependencies)
- Download from: http://xiph.org/downloads/


How to build
============

- Unpack the original libtheora-1.1.1.tar.gz (WinRAR: Choose "unpack here")
- Copy the include directory "ogg" of libogg into the libtheora include directory libtheora-1.1.1\include so that the ogg headers can be found from the theora include directories
- Unpack the libtheora-1.1.1-VS2012.zip (WinRAR: Choose "unpack here") which will put VS2012 adjusted project files + solution into the libtheora-1.1.1\win32 folder
- Open libtheora-1.1.1\win32\VS2012\libtheora_static.sln
- Ignore warnings about missing platforms (e.g. windows phone)
- Do a batch build of all available configurations
- Copy the output libraries in the libtheora-1.1.1\win32\VS2012\Win32 and libtheora-1.1.1\win32\VS2012\x64 folders to the corresponding external library folders of QSF for each platform
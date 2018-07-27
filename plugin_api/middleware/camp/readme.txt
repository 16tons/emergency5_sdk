CAMP 0.7.1
- Was changed to license: "MIT" (see https://github.com/tegesoft/camp/blob/master/COPYING.txt)
	- Was license: "LGPL3" ( https://github.com/tegesoft/camp/blob/master/LICENSE.LGPL3.txt )
- Reflection system
- Download from: https://projects.tegesoft.com/pm/projects/camp/
- Rewrote several parts of the implementation for better performance: https://github.com/cofenberg/camp


== BUILD ==
- Add system environment variable "BOOST_ROOT" pointing to "<qsf>\external\boost\include"
- Build solution using CMake
	- Build as static library, i.e. BUILD_SHARED_LIBS is off
	- Use configuration "Visual Studio 14" for VS 2015
- Build the project "camp" inside the resulting VS solution, once in Debug and once in Release configuration
- Copy the created libs to "<qsf>\external\_windows_x86\camp\lib"
- Copy the created headers to "<qsf>\external\camp\include"
- Do the same once again for the x64 build
	- Use CMake configuration "Visual Studio 14 x64" for VS 2015
	- Copy the libs to "qsf\external\_windows_x64\camp\lib"


== CHANGED ==
2013-01-26 (co)
- When using the shared library version of CAMP there's an issue with the CAMP class manager singleton destruction (in static build all is fine)
- Changed the CAMP class manager singleton to resolve this issue
- Added "singleton.hpp"
- Added method "camp::ClassBuilder::getClass()"
- Added method "camp::Class::constructorCount()"
- Changed the "classmanager"-singleton to a less problematic implementation which does not introduce e.g. a memory leak and gives us control when to create/destroy the instance
	- Search for "[CHANGED]"
- "tegesoft-camp-0.7.1-17-g00152fb.zip" is the original, "tegesoft-camp-0.7.1-17-g00152fb_[CHANGED].zip" the changed version

2013-03-02 (fw)
- Added method "camp::ArrayProperty::provideDefault()"

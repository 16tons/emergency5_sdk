nvidia-texture-tools r1396 (Dec 2, 2014)
- License: "MIT license"
- Online: http://code.google.com/p/nvidia-texture-tools/
- Use the SVN patch "enabled_tiff_support.patch" to enable tiff support


== Build Information ==
- In order to build with Visual Studio 2015 in the configuration we required, following was updated:
	- Apply "enabled_tiff_support.patch" to enabled tif-support
	- "nvimage"-project: Add "$(SolutionDir)\..\..\extern\gnuwin32\include" as include path
	- Made "nvtt" to a static library
	- When choosing a configuration, choose a CUDA configuration

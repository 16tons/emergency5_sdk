#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "camp::camp" for configuration "Debug"
set_property(TARGET camp::camp APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(camp::camp PROPERTIES
  IMPORTED_IMPLIB_DEBUG "${_IMPORT_PREFIX}/lib/campd.lib"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/bin/campd.dll"
  )

list(APPEND _IMPORT_CHECK_TARGETS camp::camp )
list(APPEND _IMPORT_CHECK_FILES_FOR_camp::camp "${_IMPORT_PREFIX}/lib/campd.lib" "${_IMPORT_PREFIX}/bin/campd.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)

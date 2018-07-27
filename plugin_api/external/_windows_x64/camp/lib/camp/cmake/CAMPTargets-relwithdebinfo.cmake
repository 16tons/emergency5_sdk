#----------------------------------------------------------------
# Generated CMake target import file for configuration "RelWithDebInfo".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "camp::camp" for configuration "RelWithDebInfo"
set_property(TARGET camp::camp APPEND PROPERTY IMPORTED_CONFIGURATIONS RELWITHDEBINFO)
set_target_properties(camp::camp PROPERTIES
  IMPORTED_IMPLIB_RELWITHDEBINFO "${_IMPORT_PREFIX}/lib/camp.lib"
  IMPORTED_LOCATION_RELWITHDEBINFO "${_IMPORT_PREFIX}/bin/camp.dll"
  )

list(APPEND _IMPORT_CHECK_TARGETS camp::camp )
list(APPEND _IMPORT_CHECK_FILES_FOR_camp::camp "${_IMPORT_PREFIX}/lib/camp.lib" "${_IMPORT_PREFIX}/bin/camp.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)

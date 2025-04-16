#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "FlowDataSystem::FlowDataSystem" for configuration "Debug"
set_property(TARGET FlowDataSystem::FlowDataSystem APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(FlowDataSystem::FlowDataSystem PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "CXX"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/libFlowDataSystem.a"
  )

list(APPEND _cmake_import_check_targets FlowDataSystem::FlowDataSystem )
list(APPEND _cmake_import_check_files_for_FlowDataSystem::FlowDataSystem "${_IMPORT_PREFIX}/lib/libFlowDataSystem.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)

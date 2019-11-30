cmake_minimum_required(VERSION 3.6)

set(lib_name ogg)

project(${lib_name})

include(../cmake/CocosExternalConfig.cmake)

# -----macro: set ogg sub target-----
macro(set_ogg_sub_target sub_lib_name)
  
  set(sub_target_name ext_${sub_lib_name})
  if(WINDOWS)
    add_library(${sub_target_name} SHARED IMPORTED GLOBAL)
  else()
    add_library(${sub_target_name} STATIC IMPORTED GLOBAL)
  endif()

  set_target_properties(${sub_target_name} PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/include"
  )
  if(WINDOWS)
    set_target_properties(${sub_target_name} PROPERTIES
      IMPORTED_LOCATION "${platform_spec_path}/lib${sub_lib_name}.dll"
      IMPORTED_IMPLIB "${platform_spec_path}/lib${sub_lib_name}.lib"
    )
  else()
    set_target_properties(${sub_target_name} PROPERTIES
      IMPORTED_LOCATION "${platform_spec_path}/lib${sub_lib_name}.a"
    )
  endif()
endmacro()

# -----sub target 1: vorbis-----
set(target_name vorbis)
set_ogg_sub_target(${target_name})

# -----sub target 2: vorbisfile-----
set(target_name vorbisfile)
set_ogg_sub_target(${target_name})


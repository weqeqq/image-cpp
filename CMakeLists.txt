cmake_minimum_required(VERSION 3.23)
project(image C CXX)

set(PROJECT_VERSION "0.1.0")
set(PROJECT_VERSION_MAJOR 0)
set(PROJECT_VERSION_MINOR 1)
set(PROJECT_VERSION_PATCH 0)

option(IMAGE_FETCH_PUBLIC "Fetch public dependencies." OFF)
if(IMAGE_FETCH_PUBLIC) 
  set(IMAGE_FETCH_FILE     ON)
  set(IMAGE_FETCH_ITERABLE ON)
else()
  option(IMAGE_FETCH_FILE     "Fetch 'file' dependency."     OFF)
  option(IMAGE_FETCH_ITERABLE "Fetch 'iterable' dependency." OFF)
endif()

include(FetchContent) 
FetchContent_Declare(
  dependency-cmake 
  GIT_REPOSITORY https://github.com/weqeqq/dependency-cmake
  GIT_TAG        1d103e5cff768665a750915500b7dc826220333c
)
FetchContent_MakeAvailable(
  dependency-cmake
)
if(IMAGE_FETCH_FILE)
  Dependency(file::file
    REPOSITORY 
      weqeqq/file-cpp
    TAG        
      d7eda176f32ec5b45eac172e2cddb08d562d3ebb
  )
else()
  find_package(file REQUIRED)
endif()
if(IMAGE_FETCH_ITERABLE)
  Dependency(iterable::iterable
    REPOSITORY 
      weqeqq/iterable-cpp 
    TAG
      a6e65c0297c8d430b1f4ca1a59ce4f493593fc7a
  )
else()
  find_package(iterable REQUIRED)
endif()

include(GenerateExportHeader)
function(GenerateExportHeader target file)
  set(export_header_dir 
    ${CMAKE_BINARY_DIR}/export-header PARENT_SCOPE)

  set(export_header_file 
    ${CMAKE_BINARY_DIR}/export-header/${file})

  generate_export_header(
    ${target} 
    EXPORT_FILE_NAME 
    ${CMAKE_BINARY_DIR}/export-header/${file}
  ) 
endfunction()

add_subdirectory(module/core)
add_subdirectory(module/codec)
add_subdirectory(module/io)
add_subdirectory(lib)

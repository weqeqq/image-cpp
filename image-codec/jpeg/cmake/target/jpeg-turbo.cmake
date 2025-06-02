
include(ExternalProject)

ExternalProject_Add(
  jpeg-turbo-project

  GIT_REPOSITORY https://github.com/libjpeg-turbo/libjpeg-turbo.git
  GIT_TAG        36ac5b847047b27b90b459f5d44154773880196f
  GIT_PROGRESS   ON

  PREFIX ${CMAKE_BINARY_DIR}/jpeg-turbo-project

  CMAKE_ARGS
    -DCMAKE_BUILD_TYPE=Release
    -DENABLE_SHARED=OFF
    -DENABLE_STATIC=ON
    -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}
    -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>
)
add_library(
  target-jpeg-turbo STATIC IMPORTED
)
add_library(
  target::jpeg-turbo ALIAS target-jpeg-turbo
)
ExternalProject_Get_Property(
  jpeg-turbo-project INSTALL_DIR
)
set(TARGET_INCLUDE_DIR ${INSTALL_DIR}/include)

if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
  set(TARGET_LIB_DIR ${INSTALL_DIR}/lib)
else() 
  set(TARGET_LIB_DIR ${INSTALL_DIR}/lib64)
endif()
file(MAKE_DIRECTORY ${TARGET_INCLUDE_DIR})

include(GNUInstallDirs)

set_target_properties(target-jpeg-turbo PROPERTIES
  IMPORTED_LOCATION             
  ${TARGET_LIB_DIR}/${CMAKE_STATIC_LIBRARY_PREFIX}turbojpeg${CMAKE_STATIC_LIBRARY_SUFFIX}

  INTERFACE_INCLUDE_DIRECTORIES 
  ${TARGET_INCLUDE_DIR}
)
add_dependencies(target-jpeg-turbo jpeg-turbo-project)


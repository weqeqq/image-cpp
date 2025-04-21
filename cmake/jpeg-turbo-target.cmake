include(ExternalProject)

ExternalProject_Add(
  jpeg-turbo-project
  GIT_REPOSITORY https://github.com/libjpeg-turbo/libjpeg-turbo.git
  GIT_TAG 36ac5b847047b27b90b459f5d44154773880196f
  GIT_PROGRESS true
  PREFIX ${CMAKE_BINARY_DIR}/jpeg-turbo

  CMAKE_ARGS
    -DCMAKE_BUILD_TYPE=Release
    -DENABLE_SHARED=OFF
    -DENABLE_STATIC=ON
    -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}
    -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>
)
add_library(jpeg-turbo STATIC IMPORTED)

ExternalProject_Get_Property(jpeg-turbo-project INSTALL_DIR)

file(MAKE_DIRECTORY ${INSTALL_DIR}/include)

include(GNUInstallDirs)

if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
  set_target_properties(jpeg-turbo PROPERTIES
    IMPORTED_LOCATION             ${INSTALL_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}turbojpeg${CMAKE_STATIC_LIBRARY_SUFFIX}
    INTERFACE_INCLUDE_DIRECTORIES ${INSTALL_DIR}/include
  )
else()
  set_target_properties(jpeg-turbo PROPERTIES
    IMPORTED_LOCATION             ${INSTALL_DIR}/lib64/${CMAKE_STATIC_LIBRARY_PREFIX}turbojpeg${CMAKE_STATIC_LIBRARY_SUFFIX}
    INTERFACE_INCLUDE_DIRECTORIES ${INSTALL_DIR}/include
  )
endif()
add_dependencies(jpeg-turbo jpeg-turbo-project)

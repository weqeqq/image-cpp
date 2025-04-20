include(ExternalProject)

ExternalProject_Add(
  jpeg-turbo-project
  GIT_REPOSITORY https://github.com/libjpeg-turbo/libjpeg-turbo.git
  GIT_TAG        36ac5b847047b27b90b459f5d44154773880196f
  GIT_PROGRESS true
  PREFIX ${CMAKE_BINARY_DIR}/jpeg-turbo

  CONFIGURE_COMMAND ${CMAKE_COMMAND} 
    -S <SOURCE_DIR> 
    -B <BINARY_DIR> 
    -DCMAKE_BUILD_TYPE=Release 
    -DENABLE_SHARED=OFF
    -DENABLE_STATIC=ON
    -DCMAKE_TOOLCHAIN_FILE=${CMAKE_CURRENT_SOURCE_DIR}/${CMAKE_TOOLCHAIN_FILE}

  BUILD_COMMAND     ${CMAKE_COMMAND} --build   <BINARY_DIR> --config Release
  INSTALL_COMMAND   ${CMAKE_COMMAND} --install <BINARY_DIR> --prefix <INSTALL_DIR> --config Release)

ExternalProject_Get_Property(jpeg-turbo-project INSTALL_DIR)

file(MAKE_DIRECTORY ${INSTALL_DIR}/include)

add_library(jpeg-turbo STATIC IMPORTED)
set_target_properties(jpeg-turbo PROPERTIES
  IMPORTED_LOCATION             ${INSTALL_DIR}/lib64/libturbojpeg.a
  INTERFACE_INCLUDE_DIRECTORIES ${INSTALL_DIR}/include)

add_dependencies(jpeg-turbo jpeg-turbo-project)

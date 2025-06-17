
include(ExternalProject)

ExternalProject_Add(
  jpeg-turbo

  GIT_REPOSITORY https://github.com/libjpeg-turbo/libjpeg-turbo.git
  GIT_TAG        36ac5b847047b27b90b459f5d44154773880196f

  CMAKE_ARGS
    -DCMAKE_BUILD_TYPE=Release
    -DENABLE_SHARED=OFF
    -DENABLE_STATIC=ON
    -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}
    -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>

  BUILD_BYPRODUCTS
    <INSTALL_DIR>/lib64/libjpeg.a 
    <INSTALL_DIR>/lib64/libturbojpeg.a 
)
add_library(
  target::jpeg-turbo INTERFACE IMPORTED
)
ExternalProject_Get_Property(
  jpeg-turbo INSTALL_DIR
)
set(TARGET_INCLUDE_DIR ${INSTALL_DIR}/include)

if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
  set(TARGET_LIB_DIR ${INSTALL_DIR}/lib)
else() 
  set(TARGET_LIB_DIR ${INSTALL_DIR}/lib64)
endif()
file(MAKE_DIRECTORY ${TARGET_INCLUDE_DIR})

target_include_directories(
  target::jpeg-turbo 
  INTERFACE 
  ${TARGET_INCLUDE_DIR}
)
target_link_libraries(
  target::jpeg-turbo
  INTERFACE 
  ${TARGET_LIB_DIR}/libjpeg.a
  ${TARGET_LIB_DIR}/libturbojpeg.a
)
add_dependencies(target::jpeg-turbo jpeg-turbo)

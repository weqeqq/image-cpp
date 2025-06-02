
include(ExternalProject)

ExternalProject_Add(
  fpng-project
  GIT_REPOSITORY https://github.com/richgel999/fpng.git 
  GIT_TAG        925796543b9d26b8edfcdcecd94c1dac280f29fc
  GIT_PROGRESS   ON
  PREFIX ${CMAKE_BINARY_DIR}/fpng-project

  PATCH_COMMAND     
    ${CMAKE_COMMAND} 
    -E copy
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/target/fpng/CMakeLists.txt
    <SOURCE_DIR>/CMakeLists.txt

  CMAKE_ARGS
    -DCMAKE_BUILD_TYPE=Release
    -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR> 
    -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}
)

add_library(
  target_fpng STATIC IMPORTED
)
add_library(
  target::fpng ALIAS target_fpng
)
ExternalProject_Get_Property(
  fpng-project INSTALL_DIR
)
set(TARGET_LIB_DIR     ${INSTALL_DIR}/lib)
set(TARGET_INCLUDE_DIR ${INSTALL_DIR}/include)

file(MAKE_DIRECTORY ${TARGET_INCLUDE_DIR})

set_target_properties(target_fpng PROPERTIES
  IMPORTED_LOCATION             ${TARGET_LIB_DIR}/libfpng.a
  INTERFACE_INCLUDE_DIRECTORIES ${TARGET_INCLUDE_DIR})

add_dependencies(target_fpng fpng-project)

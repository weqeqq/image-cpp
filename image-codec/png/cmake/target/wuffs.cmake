
include(ExternalProject)

ExternalProject_Add(
  wuffs-project
  GIT_REPOSITORY https://github.com/google/wuffs.git 
  GIT_TAG        f483ef0849186b176fb275fc19a1375fc21c763d
  GIT_PROGRESS   ON

  PREFIX ${CMAKE_BINARY_DIR}/wuffs-project

  PATCH_COMMAND 
    ${CMAKE_COMMAND} 
    -E copy 
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/target/wuffs/CMakeLists.txt 
    <SOURCE_DIR>/CMakeLists.txt

  CMAKE_ARGS
    -DCMAKE_BUILD_TYPE=Release
    -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>
    -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}
)
add_library(
  target_wuffs STATIC IMPORTED
)
add_library(
  target::wuffs ALIAS target_wuffs
)
ExternalProject_Get_Property(
  wuffs-project INSTALL_DIR
)
set(TARGET_INCLUDE_DIR ${INSTALL_DIR}/include)
set(TARGET_LIB_DIR     ${INSTALL_DIR}/lib)

file(MAKE_DIRECTORY ${TARGET_INCLUDE_DIR})

set_target_properties(target_wuffs PROPERTIES
  IMPORTED_LOCATION             ${TARGET_LIB_DIR}/libwuffs.a
  INTERFACE_INCLUDE_DIRECTORIES ${TARGET_INCLUDE_DIR})

add_dependencies(target_wuffs wuffs-project)

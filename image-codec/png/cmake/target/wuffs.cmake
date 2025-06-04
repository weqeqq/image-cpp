
include(ExternalProject)

ExternalProject_Add(
  wuffs
  GIT_REPOSITORY https://github.com/google/wuffs.git 
  GIT_TAG        f483ef0849186b176fb275fc19a1375fc21c763d

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
  target::wuffs INTERFACE IMPORTED
)
ExternalProject_Get_Property(
  wuffs INSTALL_DIR
)
set(TARGET_INCLUDE_DIR ${INSTALL_DIR}/include)
set(TARGET_LIB_DIR     ${INSTALL_DIR}/lib)

file(MAKE_DIRECTORY ${TARGET_INCLUDE_DIR})

target_include_directories(
  target::wuffs
  INTERFACE 
  ${TARGET_INCLUDE_DIR}
)
target_link_directories(
  target::wuffs
  INTERFACE 
  ${TARGET_LIB_DIR}
)
add_dependencies(target::wuffs wuffs)

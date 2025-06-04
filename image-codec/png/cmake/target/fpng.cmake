
include(ExternalProject)

ExternalProject_Add(
  fpng
  GIT_REPOSITORY https://github.com/richgel999/fpng.git 
  GIT_TAG        925796543b9d26b8edfcdcecd94c1dac280f29fc

  PATCH_COMMAND     
    ${CMAKE_COMMAND} 
    -E copy
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/target/fpng/CMakeLists.txt
    <SOURCE_DIR>/CMakeLists.txt

  CMAKE_ARGS
    -DCMAKE_BUILD_TYPE=Release
    -DBUILD_SHARED_LIBS=OFF
    -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR> 
    -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}
)
add_library(
  target::fpng INTERFACE IMPORTED
)
ExternalProject_Get_Property(
  fpng INSTALL_DIR
)
set(TARGET_LIB_DIR     ${INSTALL_DIR}/lib)
set(TARGET_INCLUDE_DIR ${INSTALL_DIR}/include)

file(MAKE_DIRECTORY ${TARGET_INCLUDE_DIR})

target_include_directories(
  target::fpng 
  INTERFACE 
  ${TARGET_INCLUDE_DIR}
)
target_link_directories(
  target::fpng 
  INTERFACE
  ${TARGET_LIB_DIR}
)
add_dependencies(target::fpng fpng)

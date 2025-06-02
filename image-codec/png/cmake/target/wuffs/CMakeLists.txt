
cmake_minimum_required(VERSION 3.21)

project(
  wuffs LANGUAGES C)
add_library(
  wuffs STATIC)
add_library(
  wuffs::wuffs ALIAS wuffs)

file(COPY_FILE release/c/wuffs-v0.4.c ${CMAKE_CURRENT_SOURCE_DIR}/wuffs.c ONLY_IF_DIFFERENT)
file(COPY_FILE release/c/wuffs-v0.4.c ${CMAKE_CURRENT_SOURCE_DIR}/wuffs.h ONLY_IF_DIFFERENT)

target_sources(wuffs PUBLIC 
  FILE_SET  HEADERS
  BASE_DIRS .
  FILES
    wuffs.h

  PRIVATE
    wuffs.c
)
target_compile_options(
  wuffs PRIVATE -O3 -DWUFFS_IMPLEMENTATION
)
install(TARGETS wuffs FILE_SET HEADERS)

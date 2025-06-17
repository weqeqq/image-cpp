cmake_minimum_required(VERSION 3.21)

project(fpng LANGUAGES CXX)

add_library(fpng STATIC)

target_sources(fpng PUBLIC 
  FILE_SET  HEADERS 
  BASE_DIRS src
  FILES 
  src/fpng.h

  PRIVATE
  src/fpng.cpp
)
target_compile_options(fpng PRIVATE 
  -O3 
  -msse4.1 
  -mpclmul 
  -fno-strict-aliasing
)
install(TARGETS fpng FILE_SET HEADERS) 

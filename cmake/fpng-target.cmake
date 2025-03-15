include(ExternalProject)

ExternalProject_Add(
  fpng-project
  GIT_REPOSITORY https://github.com/richgel999/fpng.git 
  GIT_TAG        925796543b9d26b8edfcdcecd94c1dac280f29fc
  GIT_PROGRESS true
  PREFIX ${CMAKE_BINARY_DIR}/fpng

  PATCH_COMMAND     ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/cmake/fpng/CMakeLists.txt <SOURCE_DIR>/CMakeLists.txt
  CONFIGURE_COMMAND ${CMAKE_COMMAND} -S <SOURCE_DIR> -B <BINARY_DIR> -DCMAKE_BUILD_TYPE=Release
  BUILD_COMMAND     ${CMAKE_COMMAND} --build <BINARY_DIR> --config Release 
  INSTALL_COMMAND   ${CMAKE_COMMAND} --install <BINARY_DIR> --prefix <INSTALL_DIR> --config Release)

ExternalProject_Get_Property(fpng-project INSTALL_DIR)
ExternalProject_Add_StepTargets(fpng-project install)

file(MAKE_DIRECTORY ${INSTALL_DIR}/include)

add_library(fpng STATIC IMPORTED)
set_target_properties(fpng PROPERTIES
  IMPORTED_LOCATION             ${INSTALL_DIR}/lib/libfpng.a
  INTERFACE_INCLUDE_DIRECTORIES ${INSTALL_DIR}/include)

add_dependencies(fpng fpng-project)

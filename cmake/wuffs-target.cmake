include(ExternalProject)

ExternalProject_Add(
  wuffs-project
  GIT_REPOSITORY https://github.com/google/wuffs.git 
  GIT_TAG        f483ef0849186b176fb275fc19a1375fc21c763d
  GIT_PROGRESS true
  PREFIX ${CMAKE_BINARY_DIR}/wuffs 

  PATCH_COMMAND     ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/cmake/wuffs/CMakeLists.txt <SOURCE_DIR>/CMakeLists.txt
  CONFIGURE_COMMAND ${CMAKE_COMMAND} -S <SOURCE_DIR> -B <BINARY_DIR> -DCMAKE_BUILD_TYPE=Release
  BUILD_COMMAND     ${CMAKE_COMMAND} --build <BINARY_DIR> --config Release 
  INSTALL_COMMAND   ${CMAKE_COMMAND} --install <BINARY_DIR> --prefix <INSTALL_DIR> --config Release)

ExternalProject_Get_Property(wuffs-project INSTALL_DIR)

file(MAKE_DIRECTORY ${INSTALL_DIR}/include)

add_library(wuffs STATIC IMPORTED)
set_target_properties(wuffs PROPERTIES
  IMPORTED_LOCATION             ${INSTALL_DIR}/lib/libwuffs.a
  INTERFACE_INCLUDE_DIRECTORIES ${INSTALL_DIR}/include)

add_dependencies(wuffs wuffs-project)

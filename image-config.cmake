@PACKAGE_INIT@

set_and_check(IMAGE_INCLUDE_DIR @PACKAGE_IMAGE_INCLUDE_DIR@)
set_and_check(IMAGE_LIBRARY_DIR @PACKAGE_IMAGE_LIBRARY_DIR@)

include(${CMAKE_CURRENT_LIST_DIR}/image-targets.cmake)
check_required_components(image)

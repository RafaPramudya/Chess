# CMakeList.txt : CMake project for Chess, include source and define
# project specific logic here.
#

file(GLOB_RECURSE SRC_FILES "*.cpp" "*.h")
add_executable (Chess ${SRC_FILES})

target_link_libraries(Chess PRIVATE SDL3::SDL3 SDL3_image::SDL3_image SDL3_ttf::SDL3_ttf)

if (CMAKE_VERSION VERSION_GREATER 3.16)
  set_property(TARGET Chess PROPERTY CXX_STANDARD 23)
endif()

# TODO: Add tests and install targets if needed.

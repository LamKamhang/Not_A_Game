# +----------------------------------------------+
# | project settings                             |
# +--------------------------------------------- +
cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(not_a_game)

# +----------------------------------------------+
# | Complilation flags                           |
# +--------------------------------------------- +
list(APPEND CMAKE_CXX_FLAGS "-std=c++11")
add_definitions(-Wall)

# +----------------------------------------------+
# | Load third libraries                         |
# +--------------------------------------------- +
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(${PROJECT_SOURCE_DIR}/OpenGL/glfw-3.2.1)
include_directories(${PROJECT_SOURCE_DIR}/OpenGL/Includes)
link_directories(${PROJECT_SOURCE_DIR}/OpenGL/Libs)
# +----------------------------------------------+
# | Build executable                             |
# +--------------------------------------------- +
file(GLOB_RECURSE source_files ${PROJECT_SOURCE_DIR}/src/*.cpp)

link_libraries(libassimp.dll.a)

add_executable(not_a_game
    ${source_files}
    OpenGL/glad.c
	OpenGL/stb_image.cpp
)
target_link_libraries(not_a_game glfw)
target_link_libraries(not_a_game libassimp.dll.a)
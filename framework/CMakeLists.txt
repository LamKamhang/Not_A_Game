# +----------------------------------------------+
# | project settings                             |
# +--------------------------------------------- +
cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(not_a_game)

# +----------------------------------------------+
# | Complilation flags                           |
# +--------------------------------------------- +
list(APPEND CMAKE_CXX_FLAGS "-std=c++11")


# +----------------------------------------------+
# | Load third libraries                         |
# +--------------------------------------------- +
include_directories(${PROJECT_SOURCE_DIR}/OpenGL/Includes)
link_directories(${PROJECT_SOURCE_DIR}/OpenGL/Libs)
# +----------------------------------------------+
# | Build executable                             |
# +--------------------------------------------- +
file(GLOB_RECURSE source_files ${PROJECT_SOURCE_DIR}/src/*.cpp)

link_libraries(assimp-vc140-mt.lib)
link_libraries(freetyped.lib)
link_libraries(glfw3.lib)

add_executable(not_a_game
    ${source_files}
    OpenGL/glad.c
	OpenGL/stb_image.cpp
)
target_link_libraries(not_a_game glfw3.lib)
target_link_libraries(not_a_game assimp-vc140-mt.lib)
link_libraries(freetyped.lib)
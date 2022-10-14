cmake_minimum_required(VERSION 3.6)

# set the project name and version
project(GameEngine VERSION 0.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# hide ALL_BUILD and ZERO_CHECK
 set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Add sub-directories
add_subdirectory(thirdparty)

add_subdirectory(Engine)
add_subdirectory(Game)
#######
#GLFW
#######

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(${PROJECT_SOURCE_DIR}/thirdparty/glfw3)

include_directories(GameEngine ${GLFW_INCLUDE_DIR})

#######

# Set game as startup directory
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Game)
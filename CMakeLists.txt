cmake_minimum_required(VERSION 3.6)

# Options. Turn on with 'cmake -Dmyvarname=ON'.
option(test "Build all tests." OFF) # Makes boolean 'test' available.

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

# Add gtests
# Add libs with predefined CmakeLists

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


# Set game as startup directory
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Game)

 ##############
  # Unit Tests
  ##############
  set(gtest_force_shared_crt on)
  add_subdirectory(${PROJECT_SOURCE_DIR}/thirdparty/gtest)
if (test)
	enable_testing()

	# Include the gtest library. gtest_SOURCE_DIR is available due to
	# 'project(gtest)' above.
	include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

	##############
	# Unit Tests
	##############
	add_executable(BDTests BDTests/src/test_engine.cpp)

	# Standard linking to gtest stuff.
	target_link_libraries(BDTests gtest gtest_main)

	# Extra linking for the project.
	target_link_libraries(BDTests Engine)

	# This is so you can do 'make test' to see all your tests run, instead of
	# manually running the executable runUnitTests to see those specific tests.
	#Example:
	#add_test(NAME that-test-I-made COMMAND BDTests)

	# You can also omit NAME and COMMAND. The second argument could be some other
	# test executable.
	#Example:
	#add_test(that-other-test-I-made BDTests)
endif()
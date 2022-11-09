add_executable(Game src/GameApp.cpp)
target_link_libraries(Game Engine spdlog)
add_library(bdScripts INTERFACE)
target_include_directories(bdScripts
                           INTERFACE
                            ${CMAKE_CURRENT_SOURCE_DIR}/Assets/Scripts
                           )

################
##POST Process##
################
add_custom_command(
        TARGET Game POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E copy_directory
                "${CMAKE_BINARY_DIR}/Game/Assets"
                "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Assets")
add_custom_command(
        TARGET Game POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E copy_directory
                "${CMAKE_BINARY_DIR}/Engine/EngineAssets"
                "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/EngineAssets")
#######
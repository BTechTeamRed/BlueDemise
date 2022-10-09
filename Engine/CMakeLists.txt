add_library(Engine SHARED src/Engine/Application.cpp src/Engine/Log.cpp  "src/Engine/Audio.cpp" "src/Engine/Audio.h")

#preprocessor definitions
add_compile_definitions(GE_BUILD_DLL)

find_package(OpenGL REQUIRED)
target_link_libraries(Engine spdlog glfw OpenGL::GL glm glad entt ${irrKlang} irrKlang)
target_include_directories(Engine PUBLIC ${CMAKE_CURRENT_LIST_DIR}/src)

####################
#Copy pre-built dlls
####################
add_custom_command(TARGET Engine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/thirdparty/irrKlang/dlls"
        $<TARGET_FILE_DIR:Engine>)
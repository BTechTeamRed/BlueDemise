file (GLOB_RECURSE sources CONFIGURE_DEPENDS "*.cpp" "*.h")
add_library(Engine SHARED ${sources} )

#preprocessor definitions
add_compile_definitions(GE_BUILD_DLL)

find_package(OpenGL REQUIRED)
target_link_libraries(Engine spdlog glfw OpenGL::GL glm glad entt imgui ${irrKlang} irrKlang stb json)
target_include_directories(Engine PUBLIC ${CMAKE_CURRENT_LIST_DIR}/src)

####################
#Script Reflection##
####################

find_package (Python COMPONENTS Interpreter Development REQUIRED)

execute_process(COMMAND ${PYTHON_EXECUTABLE} ${PROJECT_SOURCE_DIR}/Scripts/ReflectScripts.py ${PROJECT_SOURCE_DIR}
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})


add_custom_command(TARGET Engine PRE_BUILD
 COMMAND ${PYTHON_EXECUTABLE} ${PROJECT_SOURCE_DIR}/Scripts/ReflectScripts.py ${PROJECT_SOURCE_DIR}
 COMMENT "Generating script files"
 BYPRODUCTS ${CMAKE_CURRENT_LIST_DIR}/src/Engine/ResourceManagement/ScriptSerializer.cpp
)

####################
#Copy pre-built dlls
####################
add_custom_command(TARGET Engine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/thirdparty/irrKlang/dlls"
        $<TARGET_FILE_DIR:Engine>)

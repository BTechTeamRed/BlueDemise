add_library(Engine SHARED src/Engine/Application.cpp src/Engine/Log.cpp )

#preprocessor definitions
add_compile_definitions(GE_BUILD_DLL)

target_link_libraries(Engine spdlog)
target_include_directories(Engine PUBLIC ${CMAKE_CURRENT_LIST_DIR}/src)
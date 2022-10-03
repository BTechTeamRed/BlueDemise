## add the library that runs
add_library(spdlog INTERFACE)
add_library(glad ${CMAKE_CURRENT_SOURCE_DIR}/glad/include/glad/glad.h ${CMAKE_CURRENT_SOURCE_DIR}/glad/src/glad.c)
add_library(glm INTERFACE)
add_library(entt INTERFACE)

file(GLOB IMGUI_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/imgui/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/imgui/*.h)
add_library(imgui STATIC ${IMGUI_SOURCES})

# state that anybody linking to us needs to include the current source dir
# to find third party libs, while we don't.
target_include_directories(spdlog
                           INTERFACE
                            ${CMAKE_CURRENT_SOURCE_DIR}/spdlog/include
                           )

target_include_directories(glad
                   PUBLIC
                    ${CMAKE_CURRENT_SOURCE_DIR}/glad/include
                   )

target_include_directories(glm
                           INTERFACE
                            ${CMAKE_CURRENT_SOURCE_DIR}/glm
                           )

target_include_directories(entt
                           INTERFACE
                            ${CMAKE_CURRENT_SOURCE_DIR}/entt
                           )
target_include_directories(imgui
                           PUBLIC
                            ${CMAKE_CURRENT_SOURCE_DIR}/imgui
                           )
## add the library that runs
add_library(spdlog INTERFACE)
add_library(glad INTERFACE)
add_library(glm INTERFACE)
add_library(entt INTERFACE)

# state that anybody linking to us needs to include the current source dir
# to find third party libs, while we don't.
target_include_directories(spdlog
                           INTERFACE
                            ${CMAKE_CURRENT_SOURCE_DIR}/spdlog/include
                           )

target_include_directories(glad
                           INTERFACE
                            ${CMAKE_CURRENT_SOURCE_DIR}/glad/include
                           )

target_include_directories(glm
                           INTERFACE
                            ${CMAKE_CURRENT_SOURCE_DIR}/glm/glm
                           )

target_include_directories(entt
                           INTERFACE
                            ${CMAKE_CURRENT_SOURCE_DIR}/entt
                           )
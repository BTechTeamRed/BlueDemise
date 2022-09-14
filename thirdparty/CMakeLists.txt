## add the library that runs
add_library(spdlog INTERFACE)
add_library(glfw INTERFACE)
add_library(glad INTERFACE)
add_library(glm INTERFACE)

# state that anybody linking to us needs to include the current source dir
# to find third party libs, while we don't.
target_include_directories(spdlog
                           INTERFACE
                            ${CMAKE_CURRENT_SOURCE_DIR}/spdlog/include
				    ${CMAKE_CURRENT_SOURCE_DIR}/glfw/include
				    ${CMAKE_CURRENT_SOURCE_DIR}/glad/include
				    ${CMAKE_CURRENT_SOURCE_DIR}/glfw/include
                           )
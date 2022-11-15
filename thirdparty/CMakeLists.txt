## add the library that runs
add_library(spdlog INTERFACE)
add_library(glad ${CMAKE_CURRENT_SOURCE_DIR}/glad/include/glad/glad.h ${CMAKE_CURRENT_SOURCE_DIR}/glad/src/glad.c)
add_library(glm INTERFACE)
add_library(entt INTERFACE)
find_library(irrKlang 
	NAMES irrKlang ikpMP3.dll ikpFlac.dll irrKlang.dll
	PATHS "${CMAKE_CURRENT_SOURCE_DIR}/irrKlang"
)
add_library(irrKlang INTERFACE)
add_library(stb INTERFACE)
add_library(json INTERFACE)
add_library(freetype INTERFACE)

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

target_include_directories(irrKlang
                           INTERFACE
                            ${CMAKE_CURRENT_SOURCE_DIR}/irrKlang
                           )

target_include_directories(stb
                           INTERFACE
			${CMAKE_CURRENT_SOURCE_DIR}/stb
                           )

target_include_directories(json
                           INTERFACE
                            ${CMAKE_CURRENT_SOURCE_DIR}/json
                           )
target_include_directories(freetype
                           INTERFACE
                            ${CMAKE_CURRENT_SOURCE_DIR}/freetype
                           )
target_include_directories(imgui
                           PUBLIC
                            ${CMAKE_CURRENT_SOURCE_DIR}/imgui
                           )
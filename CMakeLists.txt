cmake_minimum_required(VERSION 3.16)
project(SDL_Engine VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Source files
file(GLOB_RECURSE SOURCES src/*.cpp)

# OpenGL
find_package(OpenGL REQUIRED)

# Prebuilt SDL3
set(SDL3_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/external/SDL/include/SDL3")
set(SDL3_LIBRARY_DEBUG "${CMAKE_SOURCE_DIR}/external/SDL/libs/Debug/SDL3.lib")
set(SDL3_LIBRARY_RELEASE "${CMAKE_SOURCE_DIR}/external/SDL/libs/Release/SDL3.lib")
set(SDL3_DLL_DEBUG "${CMAKE_SOURCE_DIR}/external/SDL/libs/Debug/SDL3.dll")
# Add ImGui
add_subdirectory(external/imgui)
add_subdirectory(external/glm)

# Create executable
add_executable(SDL_Engine ${SOURCES})

# Include directories
target_include_directories(SDL_Engine PRIVATE
    include
    external/imgui
    external/glm
    ${SDL3_INCLUDE_DIR}
)

# Link libraries
target_link_libraries(SDL_Engine PRIVATE
    imgui
    glm
    OpenGL::GL
    debug ${SDL3_LIBRARY_DEBUG}
    optimized ${SDL3_LIBRARY_RELEASE}
)

add_custom_command(TARGET SDL_Engine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<$<CONFIG:Debug>:${SDL3_DLL_DEBUG}>
        $<$<CONFIG:Release>:${SDL3_DLL_RELEASE}>
        $<TARGET_FILE_DIR:SDL_Engine>
)
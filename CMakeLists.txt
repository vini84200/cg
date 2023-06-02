cmake_minimum_required(VERSION 3.25)
project(triangle)

set(CMAKE_CXX_STANDARD 17)

#
# Prevent building in the source directory
#
if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
    message(FATAL_ERROR "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there.\n")
endif()


add_executable(triangle src/main.cpp src/LoadShaders.cpp src/Window.cpp include/triangle/Window.h src/Object.cpp src/Renderer.cpp include/triangle/Renderer.h src/Camera.cpp include/triangle/Camera.h src/RendererSimple.cpp include/triangle/RendererSimple.h src/RenderTarget.cpp include/triangle/RenderTarget.h src/Scene.cpp include/Scene.h src/TriangleObject.cpp src/TriangleObject.h)
target_include_directories(triangle PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(triangle PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/triangle)


add_compile_definitions(_DEBUG)

# --------------
# DEPENDENCIES
# --------------

set(EXT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external)
message(STATUS "EXT_DIR: ${EXT_DIR}")

# OpenGL
#find_package(OpenGL REQUIRED)

# GLAD
add_library(glad ${EXT_DIR}/include/glad/glad.h ${EXT_DIR}/glad/glad.c src/MeshBundle.cpp include/triangle/MeshBundle.h)
target_include_directories(glad PUBLIC ${EXT_DIR}/include)

# GLFW
add_subdirectory("${EXT_DIR}/glfw")
message(STATUS "GLFW3_INCLUDE_DIR: ${GLFW3_INCLUDE_DIR}")

# GLM
add_subdirectory("${EXT_DIR}/glm")

if (WIN32)
    target_link_libraries(${PROJECT_NAME} PUBLIC glad glfw glm)

else()
    target_link_libraries(${PROJECT_NAME} PUBLIC glad glfw glm)
endif()



cmake_minimum_required(VERSION 3.25)

include(FetchContent)

project(triangle)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
#
# Prevent building in the source directory
#
if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
  message(
    FATAL_ERROR
      "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there.\n"
  )
endif()

add_executable(
  triangle
  src/main.cpp
  src/LoadShaders.cpp
  src/Window.cpp
  include/triangle/Window.h
  src/Object.cpp
  src/Renderer.cpp
  include/triangle/Renderer.h
  src/Camera.cpp
  include/triangle/Camera.h
  src/RendererSimple.cpp
  include/triangle/RendererSimple.h
  src/RenderTarget.cpp
  include/triangle/RenderTarget.h
  src/Scene.cpp
  include/triangle/Scene.h
  src/TriangleObject.cpp
  include/triangle/TriangleObject.h
  src/ImGuiPlugin.cpp
  include/triangle/ImGuiPlugin.h
  src/ObjectFromFileIn.cpp
  include/triangle/ObjectFromFileIn.h
  src/RendererCloseToGl.cpp
  include/triangle/RendererCloseToGl.h
  src/RendererCloseToGlWithRasterizer.cpp
  src/Rasterizer.cpp
  include/triangle/Rasterizer.h
  src/C2GLProgram.cpp
  src/PhongProgram.cpp
  src/GouradProgram.cpp
  src/Texture.cpp
  )
target_include_directories(triangle PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(triangle
                           PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/triangle)

add_compile_definitions(_DEBUG)

# --------------
# DEPENDENCIES
# --------------

set(EXT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external)
message(STATUS "EXT_DIR: ${EXT_DIR}")

# OpenGL find_package(OpenGL REQUIRED)

# GLAD
add_library(glad ${EXT_DIR}/include/glad/glad.h ${EXT_DIR}/glad/glad.c
                 src/MeshBundle.cpp include/triangle/MeshBundle.h)
target_include_directories(glad PUBLIC ${EXT_DIR}/include)

# GLFW
add_subdirectory("${EXT_DIR}/glfw")
message(STATUS "GLFW3_INCLUDE_DIR: ${GLFW3_INCLUDE_DIR}")

# GLM
add_subdirectory("${EXT_DIR}/glm")

# IMGUI
set(IMGUI_DIR ${EXT_DIR}/imgui)
add_library(
  imgui
  ${IMGUI_DIR}/imgui.h
  ${IMGUI_DIR}/imgui.cpp
  ${IMGUI_DIR}/imgui_draw.cpp
  ${IMGUI_DIR}/imgui_widgets.cpp
  ${IMGUI_DIR}/imgui_tables.cpp
  ${IMGUI_DIR}/imgui_demo.cpp
  ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
  ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
  ${EXT_DIR}/glfw)
include_directories(${IMGUI_DIR} ${IMGUI_DIR}/backends)
target_link_libraries(imgui PUBLIC glfw)

# NativeFileDialog Extended
add_subdirectory("${EXT_DIR}/nfd-extended")

# if (WIN32) target_link_libraries(${PROJECT_NAME} PUBLIC glad glfw glm imgui)

# else() endif()

FetchContent_Declare(
  tracy
  GIT_REPOSITORY https://github.com/wolfpld/tracy.git
  GIT_TAG master
  GIT_SHALLOW TRUE
  GIT_PROGRESS TRUE)
FetchContent_MakeAvailable(tracy)
option(TRACY_ENABLE "Enable Tracy" OFF)

target_include_directories(${PROJECT_NAME} PUBLIC ${TRACY_INCLUDE_DIR})
message(STATUS "TRACY_INCLUDE_DIR: ${TRACY_INCLUDE_DIR}")


FetchContent_Declare(
    stb
    GIT_REPOSITORY https://github.com/nothings/stb.git
    GIT_TAG master
    GIT_SHALLOW TRUE
    GIT_PROGRESS TRUE
    )
FetchContent_MakeAvailable(stb)
include_directories(${stb_SOURCE_DIR})


target_link_libraries(${PROJECT_NAME} PUBLIC glad glfw glm imgui nfd
                                             TracyClient)

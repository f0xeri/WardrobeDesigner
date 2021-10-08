cmake_minimum_required(VERSION 3.8 FATAL_ERROR)
project(WardrobeDesigner VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(THREADS_PREFER_PTHREAD_FLAG ON)

set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)
find_package(Threads REQUIRED)

set(OpenGL_GL_PREFERENCE GLVND)
set(glew-cmake_BUILD_SHARED OFF CACHE BOOL "disable glew shared" FORCE)
add_subdirectory(${PROJECT_SOURCE_DIR}/glew-cmake)
add_subdirectory(${PROJECT_SOURCE_DIR}/glfw)

set(SRC ${PROJECT_SOURCE_DIR}/src/main.cpp
        ${PROJECT_SOURCE_DIR}/src/Window.cpp
        ${PROJECT_SOURCE_DIR}/src/Texture.cpp
        ${PROJECT_SOURCE_DIR}/src/ObjectLoader.cpp
        ${PROJECT_SOURCE_DIR}/src/Camera.cpp
        ${PROJECT_SOURCE_DIR}/src/Shader.cpp
        ${PROJECT_SOURCE_DIR}/src/Texture.cpp
        ${PROJECT_SOURCE_DIR}/src/Controls.cpp
        ${PROJECT_SOURCE_DIR}/src/Object/IObject.cpp
        ${PROJECT_SOURCE_DIR}/src/Object/Cube.cpp
        ${PROJECT_SOURCE_DIR}/src/Object/Sphere.cpp
        ${PROJECT_SOURCE_DIR}/src/Scene.cpp
        ${PROJECT_SOURCE_DIR}/src/GUIRenderer.cpp
        ${PROJECT_SOURCE_DIR}/src/WardrobeGenerator.cpp
        ${PROJECT_SOURCE_DIR}/src/Object/WardrobeElements/WardrobeEdge.cpp
        ${PROJECT_SOURCE_DIR}/src/Object/WardrobeElements/WardrobeHorizontalShelf.cpp
        ${PROJECT_SOURCE_DIR}/src/Object/WardrobeElements/WardrobeVerticalElement.cpp
        ${PROJECT_SOURCE_DIR}/src/Tools.cpp
        ${PROJECT_SOURCE_DIR}/src/ArcBallCamera.cpp)

add_executable(${CMAKE_PROJECT_NAME} ${SRC})

set(CMAKE_CXX_STANDARD 20)

target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/glew-cmake/include)
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/glfw/include)
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/glm)

#imgui integration
set(IMGUI_INCLUDE   ${PROJECT_SOURCE_DIR}/imgui/backends
        ${PROJECT_SOURCE_DIR}/imgui)



set(IMGUI_SRC       ${PROJECT_SOURCE_DIR}/imgui/imgui_draw.cpp
        ${PROJECT_SOURCE_DIR}/imgui/imgui_tables.cpp
        ${PROJECT_SOURCE_DIR}/imgui/imgui_widgets.cpp
        ${PROJECT_SOURCE_DIR}/imgui/imgui.cpp
        ${PROJECT_SOURCE_DIR}/imgui/backends/imgui_impl_glfw.cpp
        ${PROJECT_SOURCE_DIR}/imgui/backends/imgui_impl_opengl3.cpp)


add_library(imgui STATIC ${IMGUI_SRC})
target_link_libraries(imgui glfw)
target_include_directories(imgui PUBLIC ${PROJECT_SOURCE_DIR}/glfw/include})
target_include_directories(imgui PUBLIC ${IMGUI_INCLUDE})


target_link_libraries(${CMAKE_PROJECT_NAME} Threads::Threads)
target_link_libraries(${CMAKE_PROJECT_NAME} ${GLEW_LIBRARIES})
target_link_libraries(${CMAKE_PROJECT_NAME} ${GLFW_LIBRARIES})
target_link_libraries(${CMAKE_PROJECT_NAME} ${OPENGL_LIBRARY})
target_link_libraries(${CMAKE_PROJECT_NAME} glfw)
target_link_libraries(${CMAKE_PROJECT_NAME} libglew_static)

target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC ${IMGUI_INCLUDE})
target_link_libraries(${CMAKE_PROJECT_NAME} imgui)

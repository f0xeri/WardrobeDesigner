cmake_minimum_required(VERSION 3.8 FATAL_ERROR)
project(FoxEngine VERSION 1.0 LANGUAGES CXX)

add_compile_options(
    -Werror
    -Wall
    -Wextra
    -Wpedantic
    -Wcast-align
    -Wcast-qual
    -Wconversion
    -Wctor-dtor-privacy
    -Wenum-compare
    -Wfloat-equal
    -Wnon-virtual-dtor
    -Wold-style-cast
    -Woverloaded-virtual
    -Wredundant-decls
    -Wsign-conversion
    -Wsign-promo
)

set(THREADS_PREFER_PTHREAD_FLAG ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -std=c++11 -Wno-expansion-to-defined")

set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)
find_package(Threads REQUIRED)

set(OpenGL_GL_PREFERENCE GLVND)
add_subdirectory(${PROJECT_SOURCE_DIR}/glew-cmake)
add_subdirectory(${PROJECT_SOURCE_DIR}/glfw)

set(SRC ${PROJECT_SOURCE_DIR}/src/main.cpp
        ${PROJECT_SOURCE_DIR}/src/Window.cpp
        ${PROJECT_SOURCE_DIR}/src/Texture.cpp
        ${PROJECT_SOURCE_DIR}/src/ObjectLoader.cpp
        ${PROJECT_SOURCE_DIR}/src/Camera.cpp
        ${PROJECT_SOURCE_DIR}/src/Program.cpp
        ${PROJECT_SOURCE_DIR}/src/Texture.cpp
        ${PROJECT_SOURCE_DIR}/src/Controls.cpp
        ${PROJECT_SOURCE_DIR}/src/Object/IObject.cpp
        ${PROJECT_SOURCE_DIR}/src/Object/Cube.cpp
        ${PROJECT_SOURCE_DIR}/src/Object/Sphere.cpp
        ${PROJECT_SOURCE_DIR}/src/Scene.cpp)

add_executable(${CMAKE_PROJECT_NAME} ${SRC})

target_compile_features(${CMAKE_PROJECT_NAME} INTERFACE cxx_std_17)

target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC ${GLEW_INCLUDE_DIRS})
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/glfw/include)
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/glm)

target_link_libraries(${CMAKE_PROJECT_NAME} Threads::Threads)
target_link_libraries(${CMAKE_PROJECT_NAME} ${GLEW_LIBRARIES})
target_link_libraries(${CMAKE_PROJECT_NAME} ${GLFW_LIBRARIES})
target_link_libraries(${CMAKE_PROJECT_NAME} ${OPENGL_LIBRARY})
target_link_libraries(${CMAKE_PROJECT_NAME} glfw)
target_link_libraries(${CMAKE_PROJECT_NAME} libglew_static)




cmake_minimum_required(VERSION 3.17)
project(FoxEngine)

set(CMAKE_CXX_STANDARD 17)

INCLUDE_DIRECTORIES(
        ${GL_INCLUDE_DIR}
        ${GLFW_INCLUDE_DIR})

add_executable(FoxEngine src/main.cpp
        src/Window/Window.cpp src/Window/Window.h
        src/ObjectLoader/ObjectLoader.cpp src/ObjectLoader/ObjectLoader.h
        src/Camera/Camera.cpp src/Camera/Camera.h
        src/Program/Program.cpp src/Program/Program.hpp
        src/Texture/Texture.cpp src/Texture/Texture.hpp
        src/Controls/Controls.cpp src/Controls/Controls.h
        src/State.hpp
        src/Model.h src/Object/IObject.cpp src/Object/IObject.hpp src/Object/Cube.cpp src/Object/Cube.hpp src/Object/Sphere.cpp src/Object/Sphere.hpp src/Scene/Scene.cpp src/Scene/Scene.hpp)
target_link_libraries(FoxEngine glfw3 opengl32 glew32)

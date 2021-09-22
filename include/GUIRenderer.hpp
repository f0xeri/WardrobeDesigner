//
// Created by Yaroslav on 05.04.2021.
//

#ifndef MINE_GUIRENDERER_HPP
#define MINE_GUIRENDERER_HPP

#include <sstream>
#include <typeinfo>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include "State.hpp"
#include "Window.h"
#include "Tools.hpp"

class GUIRenderer
{
public:
    GUIRenderer(GLFWwindow *window);
    void render(State *state);

    void renderDebugInfo(State *state);
    void renderSettings(State *state);
    void renderWardrobeMenu(State *state);
    void renderDebugString(const std::string &string, float x = 0.0f, float y = 0.0f);
};

#endif //MINE_GUIRENDERER_HPP

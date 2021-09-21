//
// Created by Yaroslav on 05.04.2021.
//

#ifndef MINE_GUIRENDERER_HPP
#define MINE_GUIRENDERER_HPP

#include <sstream>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include "State.hpp"
#include "Window.h"

class GUIRenderer
{
public:
    GUIRenderer(GLFWwindow *window);
    void render(State *state);

    void renderDebugInfo(State *state)
    {
        ImGuiWindowFlags window_flags = 0;
        window_flags |= ImGuiWindowFlags_NoBackground;
        window_flags |= ImGuiWindowFlags_NoTitleBar;
        //window_flags |= ImGuiWindowFlags_NoResize;
        ImGui::SetNextWindowPos({0, 0}, ImGuiCond_Once);
        ImGui::Begin("Debug222", &state->showDebug, window_flags);
        std::stringstream debugSS;
        debugSS << "Camera position:\n" << "X: " << state->camera->pos.x << " Y: " << state->camera->pos.y << " Z: "
                << state->camera->pos.z << "\n\n";
        ImGui::Text("%s", debugSS.str().c_str());
        /*if (ImGui::Button("Close (F3)"))
            show_debug = false;*/
        std::stringstream resSS;

        ImGui::Text("%s", resSS.str().c_str());
        ImGui::End();
    }

    void renderSettings(State *state)
    {
        float settingsWinX, settingsWinY, settingsWinW, settingsWinH;
        float objWinX, objWinY, objWinW, objWinH;

        settingsWinX = objWinX = Window::_width - Window::_width / 6.0f;
        settingsWinY = 0;
        settingsWinW = objWinW = Window::_width / 6.0f;
        settingsWinH = state->pickedObject == -1 ? Window::_height : Window::_height - 2 * Window::_height / 3;

        objWinY = Window::_height - 2 * Window::_height / 3;
        objWinH = Window::_height - Window::_height / 3;

        ImGuiWindowFlags window_flags = 0;
        ImGui::SetNextWindowPos({settingsWinX, settingsWinY}, ImGuiCond_Always);
        ImGui::SetNextWindowSize({settingsWinW, settingsWinH});
        window_flags |= ImGuiWindowFlags_NoScrollbar;
        window_flags |= ImGuiWindowFlags_NoCollapse;
        window_flags |= ImGuiWindowFlags_NoResize;

        ImGui::Begin("Настройки", nullptr, window_flags);
        ImGui::Text("Стенки шкафа");
        ImGui::Checkbox("Основание", &state->wardrobeGenerator->bottomSide->enabled);
        ImGui::Checkbox("Задняя", &state->wardrobeGenerator->backSide->enabled);
        ImGui::Checkbox("Верхняя", &state->wardrobeGenerator->topSide->enabled);
        ImGui::Checkbox("Левая", &state->wardrobeGenerator->leftSide->enabled);
        ImGui::Checkbox("Правая", &state->wardrobeGenerator->rightSide->enabled);

        if (state->pickedObject != -1)
        {
            ImGui::SetNextWindowPos({objWinX, objWinY}, ImGuiCond_Always);
            ImGui::SetNextWindowSize({objWinW, objWinH});
            std::stringstream stringstream;
            stringstream << "Настройки детали #" << state->pickedObject;

            ImGui::Begin(stringstream.str().c_str(), nullptr, window_flags);
            ImGui::Text("Позиция");
            ImGui::InputFloat("x", &state->scene->objects[state->pickedObject]->position.x, 0.01f);
            ImGui::InputFloat("y", &state->scene->objects[state->pickedObject]->position.y, 0.01f);
            ImGui::InputFloat("z", &state->scene->objects[state->pickedObject]->position.z, 0.01f);

            ImGui::End();
        }
        ImGui::End();
    }

    void renderDebugString(const std::string &string, float x = 0.0f, float y = 0.0f);
};

#endif //MINE_GUIRENDERER_HPP

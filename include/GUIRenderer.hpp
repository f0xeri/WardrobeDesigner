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
        ImGui::Begin("Debug", &state->showDebug, window_flags);
        std::stringstream debugSS;
        debugSS << "Global player position:\n" << "X: " << state->camera->pos.x << " Y: " << state->camera->pos.y << " Z: "
                << state->camera->pos.z << "\n\n";
        ImGui::Text("%s", debugSS.str().c_str());
        /*if (ImGui::Button("Close (F3)"))
            show_debug = false;*/
        std::stringstream resSS;

        ImGui::Text("%s", resSS.str().c_str());
        ImGui::End();
    }

    /*void renderInventory(State *state)
    {
        ImGuiWindowFlags window_flags = 0;
        window_flags |= ImGuiWindowFlags_NoBackground;
        window_flags |= ImGuiWindowFlags_NoTitleBar;
        window_flags |= ImGuiWindowFlags_NoResize;
        ImGui::SetNextWindowPos({Window::_width / 2.0f - 80, Window::_height - (Window::_height / 12.0f)});
        ImGui::SetNextWindowSize({160, 60}, ImGuiCond_Once);
        ImGui::Begin("Inventory", &state->showInventory, window_flags);
        std::stringstream ss;
        switch (state->currentBlockId)
        {
            case 0:
                ss << "Air";
                break;
            case 1:
                ss << "Stone";
                break;
            case 2:
                ss << "Sand";
                break;
            case 3:
                ss << "Grass";
                break;
            case 6:
                ss << "Bricks";
                break;
        }
        ImGui::SetWindowFontScale(1.8f);
        float font_size = ImGui::GetFontSize() * ss.str().size() / 2;
        ImGui::SameLine(ImGui::GetWindowSize().x / 2 - font_size + (font_size / 2));
        ImGui::Text("%s", ss.str().c_str());
        ImGui::End();
    }*/

    /*void renderRendererSettings(State *state)
    {
        ImGui::Begin("Renderer Settings");
        ImGui::Text("Shadows");
        ImGui::Checkbox("On", &state->useShadows);
        ImGui::Text("Light direction");
        ImGui::SliderFloat("lightDir.x", &state->lightDir.x, -10.0f, 10.0f);
        ImGui::SliderFloat("lightDir.y", &state->lightDir.y, -10.0f, 10.0f);
        ImGui::SliderFloat("lightDir.z", &state->lightDir.z, -10.0f, 10.0f);
        std::stringstream ss;
        ss << "Calculated light position: " << state->calculatedLightPosition.x << " " << state->calculatedLightPosition.y << " " << state->calculatedLightPosition.z;
        ImGui::Text("%s", ss.str().c_str());

        ImGui::Text("Render distance (chunks)");
        ImGui::SliderInt("Chunks", &state->viewDistance, 0, 10);
        ImGui::End();
    }*/

    void renderDebugString(const std::string &string, float x = 0.0f, float y = 0.0f);
};

#endif //MINE_GUIRENDERER_HPP

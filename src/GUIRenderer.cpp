//
// Created by Yaroslav on 05.04.2021.
//

#include "GUIRenderer.hpp"

GUIRenderer::GUIRenderer(GLFWwindow *window)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    //ImGuiIO &io = ImGui::GetIO();
    ImFontConfig font_config;
    font_config.OversampleH = 2; //or 2 is the same
    font_config.OversampleV = 1;
    font_config.PixelSnapH = 1;

    static const ImWchar ranges[] =
            {
                    0x0020, 0x00FF, // Basic Latin + Latin Supplement
                    0x0400, 0x044F, // Cyrillic
                    0,
            };
    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->AddFontFromFileTTF("res/fonts/NotoSans-Regular.ttf", 18.0f, &font_config, ranges);
    ImGui::StyleColorsLight();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    const char* glslVersion = "#version 330";
    ImGui_ImplOpenGL3_Init(glslVersion);
}

void GUIRenderer::render(State *state)
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    //if (state->showDebug) renderDebugInfo(state);
    renderSettings(state);

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    ImGui::EndFrame();
}

void GUIRenderer::renderDebugString(const std::string &string, float x, float y) {
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::SetNextWindowPos({x, y});
        ImGui::Begin("");
        ImGui::Text("%s", string.c_str());
        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        ImGui::EndFrame();
    }
}

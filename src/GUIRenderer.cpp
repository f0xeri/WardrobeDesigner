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

void GUIRenderer::renderDebugInfo(State *state)
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
std::string current_item;
int selected_item_index;
void GUIRenderer::renderSettings(State *state)
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

        ImGui::Text("Текстура");
        current_item = state->scene->objects[state->pickedObject]->texture->name;
        ImGui::SetNextItemWidth(settingsWinW - 14);

        float prevItemSpacingY = ImGui::GetStyle().ItemSpacing.y;
        ImGui::GetStyle().ItemSpacing.y = 0;
        /* HACK: I don't know how to set Combo height properly, ImGui calculates height like "fontsize + framepadding * 2"
         * so i just change framepadding.y for Combo and then revert it back */
        ImGui::GetStyle().FramePadding.y = 7;
        if(ImGui::BeginCombo("##texture", (std::string("           ") + current_item).c_str()))
        {
            ImGui::GetStyle().FramePadding.y = 3;
            for(const auto& [name, texture] : state->wardrobeTextures)
            {
                bool is_selected = (current_item == name);
                auto drawList = ImGui::GetWindowDrawList();
                ImGuiSelectableFlags flags = 0;
                if(ImGui::Selectable((std::string("           ") + name).c_str(), is_selected, flags, {settingsWinW, 32}))
                {
                    //selected_item_index = n;
                    current_item = name;
                    state->scene->objects[state->pickedObject]->texture = state->wardrobeTextures.at(current_item);
                }
                auto rect_min = ImGui::GetItemRectMin();
                auto rect_max = ImGui::GetItemRectMax();
                rect_min.x += 2;
                rect_max.x = rect_min.x + 32;
                rect_max.y = rect_min.y + 32;
                drawList->AddImage(reinterpret_cast<ImTextureID>(state->wardrobeTextures.at(name)->texture), rect_min, rect_max, ImVec2(0, 0), ImVec2(1, 1), IM_COL32(255, 255, 255, 255));
                if(is_selected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }
        else
        {
            auto rect_min = ImGui::GetItemRectMin();
            auto rect_max = ImGui::GetItemRectMax();
            rect_max.x = rect_min.x + 32;
            rect_max.y = rect_min.y + 32;
            auto drawList = ImGui::GetWindowDrawList();
            drawList->AddImage(reinterpret_cast<ImTextureID>(state->wardrobeTextures.at(current_item)->texture), rect_min, rect_max, ImVec2(0,0), ImVec2(1,1), IM_COL32(255,255,255,255));
        }
        ImGui::GetStyle().FramePadding.y = 3;
        ImGui::GetStyle().ItemSpacing.y = prevItemSpacingY;
        ImGui::End();
    }
    ImGui::End();
}


void GUIRenderer::renderWardrobeMenu(State *state)
{

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

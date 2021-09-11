//
// Created by Yaroslav on 10.02.2021.
//

#include <thread>
#include <iostream>
#include "Controls.h"
#include "Window.h"

State *localState;

Controls::Controls(State *s) {
    localState = s;
}

void toggleCursor(GLFWwindow *window)
{
    localState->cursor_locked = !localState->cursor_locked;
    glfwSetInputMode(window, GLFW_CURSOR, localState->cursor_locked ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
}

void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        toggleCursor(window);
    }
    if (key == GLFW_KEY_SPACE)
        switch (action)
        {
            case GLFW_PRESS:
            {
                localState->speed = localState->speed * 20;
                break;
            }
            case GLFW_RELEASE:
            {
                localState->speed = localState->speed / 20;
                break;
            }
        }

    if (key == GLFW_KEY_P && action == GLFW_PRESS)
    {
        localState->vsync = !localState->vsync;
        glfwSwapInterval(localState->vsync);
        //LOG("[INFO] VSync " << (localState->vsync ? "on" : "off") << ".");
    }
    if (key == GLFW_KEY_O && action == GLFW_PRESS)
    {
        localState->showPolygons = !localState->showPolygons;
    }
}

void updateInputs(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) localState->camera->pos += localState->camera->front * localState->speed * localState->deltaTime;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) localState->camera->pos -= localState->camera->right * localState->speed * localState->deltaTime;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) localState->camera->pos -= localState->camera->front * localState->speed * localState->deltaTime;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) localState->camera->pos += localState->camera->right * localState->speed * localState->deltaTime;
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) localState->camera->pos += localState->camera->up * localState->speed * localState->deltaTime;
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) localState->camera->pos -= localState->camera->up * localState->speed * localState->deltaTime;
}

void cursorCallback(GLFWwindow *window, double xpos, double ypos)
{
    if (localState->cursor_started){
        localState->deltaX += xpos - localState->x;
        localState->deltaY += ypos - localState->y;
    }
    else localState->cursor_started = true;
    localState->x = xpos;
    localState->y = ypos;
    if (localState->cursor_locked)
    {
        localState->camY += -localState->deltaY / Window::_height * 2;
        localState->camX += -localState->deltaX / Window::_height * 2;

        if (localState->camY < -radians(89.0f)){
            localState->camY = -radians(89.0f);
        }
        if (localState->camY > radians(89.0f)){
            localState->camY = radians(89.0f);
        }

        localState->camera->rotation = mat4(1.0f);
        localState->camera->rotate(localState->camY, localState->camX, 0);
    }
    localState->deltaX = localState->deltaY = 0.0f;
}

void resizeCallback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
    Window::_width = width;
    Window::_height = height;
}

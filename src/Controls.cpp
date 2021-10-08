//
// Created by Yaroslav on 10.02.2021.
//

#include <thread>
#include <iostream>
#include <glm/ext/matrix_projection.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/intersect.hpp>

#include "Controls.h"
#include "Window.h"
#include "Logger.hpp"

#include <Object/WardrobeElements/WardrobeVerticalElement.hpp>
#include <Object/WardrobeElements/WardrobeHorizontalShelf.hpp>

State *localState;

Controls::Controls(State *s)
{
    localState = s;
}

void toggleCursor(GLFWwindow *window)
{
    localState->cursorLocked = !localState->cursorLocked;
    glfwSetInputMode(window, GLFW_CURSOR, localState->cursorLocked ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
}

void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
    /*if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
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
    */
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
    /*if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) localState->camera->pos += localState->camera->front * localState->speed * localState->deltaTime;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) localState->camera->pos -= localState->camera->right * localState->speed * localState->deltaTime;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) localState->camera->pos -= localState->camera->front * localState->speed * localState->deltaTime;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) localState->camera->pos += localState->camera->right * localState->speed * localState->deltaTime;
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) localState->camera->pos += localState->camera->up * localState->speed * localState->deltaTime;
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) localState->camera->pos -= localState->camera->up * localState->speed * localState->deltaTime;*/
}

glm::vec2 transform_mouse(glm::vec2 in)
{
    return glm::vec2(in.x * 2.f / Window::_width - 1.f, 1.f - 2.f * in.y / Window::_height);
}

glm::vec2 prev_mouse(-2.f);
template <typename T>
int sgn(T val)
{
    return (T(0) < val) - (val < T(0));
}

void cursorCallback(GLFWwindow *window, double xpos, double ypos)
{

    localState->deltaX = localState->deltaY = 0.0f;
    if (localState->cursorStarted)
    {
        localState->deltaX += xpos - localState->x;
        localState->deltaY += ypos - localState->y;
    }
    else
        localState->cursorStarted = true;
    localState->x = xpos;
    localState->y = ypos;
    if (localState->cursorLocked)
    {
        localState->camY += -localState->deltaY / Window::_height * 2;
        localState->camX += -localState->deltaX / Window::_height * 2;

        /*if (localState->camY < -radians(89.0f)){
            localState->camY = -radians(89.0f);
        }
        if (localState->camY > radians(89.0f)){
            localState->camY = radians(89.0f);
        }*/

        localState->camera->rotation = mat4(1.0f);
        //localState->camera->rotate(localState->camY, localState->camX, 0);
    }

    glm::vec4 position(localState->arcBallCamera->pos.x, localState->arcBallCamera->pos.y, localState->arcBallCamera->pos.z, 1);
    glm::vec4 pivot(localState->arcBallCamera->lookat.x, localState->arcBallCamera->lookat.y, localState->arcBallCamera->lookat.z, 1);

    float deltaAngleX = (2 * M_PI / Window::_height);
    float deltaAngleY = (M_PI / Window::_height);
    float xAngle = (prev_mouse.x - xpos) * deltaAngleX;
    float yAngle = (prev_mouse.y - ypos) * deltaAngleY;

    const glm::vec2 cur_mouse = transform_mouse(glm::vec2(xpos, ypos));

    float cosAngle = dot(localState->arcBallCamera->getViewDir(), localState->arcBallCamera->up);
    if (cosAngle * sgn(deltaAngleY) > 0.99f)
        deltaAngleY = 0;

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && localState->pickedObject == -1)
    {
        glm::mat4 rotationMatrixX(1.0f);
        rotationMatrixX = glm::rotate(rotationMatrixX, xAngle, localState->arcBallCamera->up);
        position = (rotationMatrixX * (position - pivot)) + pivot;

        glm::mat4 rotationMatrixY(1.0f);
        rotationMatrixY = glm::rotate(rotationMatrixY, yAngle, localState->arcBallCamera->getRightVector());
        glm::vec3 finalPosition = (rotationMatrixY * (position - pivot)) + pivot;

        localState->arcBallCamera->setCameraView(finalPosition, localState->arcBallCamera->lookat, localState->arcBallCamera->up);
    }

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS && localState->pickedObject == -1)
    {
        glm::vec2 click = glm::vec2(xpos, ypos) - prev_mouse;
        glm::vec3 look = localState->arcBallCamera->pos - localState->arcBallCamera->lookat;
        float length = glm::length(look);
        glm::vec3 right = localState->arcBallCamera->getRightVector();
        glm::vec3 pan = (localState->arcBallCamera->up * -click.y + right * click.x) * 0.005f * 1.0f * length;
        localState->arcBallCamera->setCameraView(localState->arcBallCamera->pos - pan, localState->arcBallCamera->lookat - pan, localState->arcBallCamera->up);
    }

    prev_mouse = {xpos, ypos};
}

void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods)
{
    if (localState->cursorLocked)
        return;

    glm::vec3 ray_start = localState->arcBallCamera->pos;
    glm::vec3 ray_dir = localState->arcBallCamera->raycastFromViewportCoords(localState->dx, localState->dy);
    glm::vec3 ray_end = ray_start + ray_dir * 1000.0f;

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        unsigned char data[4];
        double x, y;
        glfwGetCursorPos(window, &x, &y);
        // Do nothing if user clicked on GUI
        if (x < Window::_width / 6.0f || x > Window::_width - Window::_width / 6.0f)
            return;

        int pickedID = -1;
        for (int i = 0; i < localState->scene->objects.size(); i++)
        {
            Cube *cube = dynamic_cast<Cube *>(localState->scene->objects[i]);
            if (cube != nullptr)
            {
                float t;
                if (cube->intersect(ray_start, ray_end, t))
                    pickedID = i;
            }
        }

        localState->pickedObject = pickedID;

        if (pickedID > localState->scene->objects.size())
        {
            localState->pickedObject = -1;
        }
        else
        {
            if (!localState->toInsert)
            {
                localState->pickedObject = pickedID;
                localState->scene->objects[pickedID]->start_move(localState);
            }
        }

        if (localState->toInsert)
        {
            float intersect_distance;
            glm::intersectRayPlane(ray_start, ray_dir, glm::vec3(0, 0, 0), glm::vec3(0, 0, 1), intersect_distance);
            glm::vec3 intersect_pos = ray_start + ray_dir * intersect_distance;

            Cluster *c = localState->cm.trySplit(intersect_pos, localState->wardrobeGenerator->boardThickness,
                                                 localState->toInsert == 1);
            if (c)
            {
                IObject *obj;
                if (localState->toInsert == 1)
                {
                    obj = new WardrobeVerticalElement(c, &localState->cm);
                }
                else if (localState->toInsert == 2)
                {
                    obj = new WardrobeHorizontalShelf(c,&localState->cm);
                }
                obj->texture = localState->wardrobeTextures.at("res/textures/woodTexture.jpg");
                obj->generateVAO();
                localState->scene->addObject(obj);
                localState->toInsert = 0;
            }
        }
    }
    else
    {
        if (localState->pickedObject != -1)
        {
            localState->scene->objects[localState->pickedObject]->end_move();
        }

        // If we set pickedObject to -1 here, object will be unpicked on left mouse button release
        //localState->pickedObject = -1;
    }
}

void scrollCallback(GLFWwindow *window, double xoffset, double yoffset)
{
    localState->arcBallCamera->setCameraView(localState->arcBallCamera->pos + localState->arcBallCamera->getViewDir() * (float)yoffset, localState->arcBallCamera->lookat + localState->arcBallCamera->getViewDir() * (float)yoffset, localState->arcBallCamera->up);
    localState->arcBallCamera->zoom += yoffset;
}

void resizeCallback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
    Window::_width = width;
    Window::_height = height;
}

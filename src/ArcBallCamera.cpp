//
// Created by Yaroslav on 30.07.2020.
//

#include <glm/ext.hpp>
#include "ArcBallCamera.hpp"
#include "Window.h"
#include "Logger.hpp"

ArcBallCamera::ArcBallCamera(glm::vec3 pos, glm::vec3 lookat, glm::vec3 up, float FOV) : pos(pos), lookat(lookat), up(up), FOV(FOV), rotation(1.0f)
{
    updateViewMatrix();
    updateVectors();
}

ArcBallCamera::~ArcBallCamera()
{

}

void ArcBallCamera::updateViewMatrix()
{
    view = glm::lookAt(pos, lookat, up);
}

glm::mat4 ArcBallCamera::getProjectionMatrix()
{
    return glm::perspective(FOV, float(Window::_width) / float(Window::_height), 0.1f, 5000.0f);
}

glm::mat4 ArcBallCamera::getViewMatrix()
{
    return view;
}

void ArcBallCamera::updateVectors()
{
    front = -glm::transpose(view)[2];
    //up = up;
    right = glm::transpose(view)[0];
}

glm::vec3 ArcBallCamera::getViewDir() {
    return -glm::transpose(view)[2];
}

glm::vec3 ArcBallCamera::getRightVector() {
    return glm::transpose(view)[0];
}

void ArcBallCamera::setCameraView(glm::vec3 eye, glm::vec3 _lookat, glm::vec3 _up)
{
    pos = eye;
    lookat = _lookat;
    up = _up;
    updateViewMatrix();
}

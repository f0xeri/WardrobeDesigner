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

glm::vec3 ArcBallCamera::raycastFromViewportCoords(double x, double y)
{
    float nx = (2.0f * x) / Window::_width - 1.0f;
    float ny = (2.0f * y) / Window::_height - 1;
    ny = -ny;
    float nz = 1.0f;
    glm::vec3 ray_nds = glm::vec3(nx, ny, nz);
    glm::vec4 ray_clip = glm::vec4(ray_nds.x, ray_nds.y, -1.0, 1.0);
    glm::vec4 ray_eye = inverse(getProjectionMatrix()) * ray_clip;
    ray_eye = glm::vec4(ray_eye.x, ray_eye.y, -1.0, 0.0);
    glm::vec3 ray_wor = (inverse(getViewMatrix()) * ray_eye);
    ray_wor = normalize(ray_wor);
    return ray_wor;
}


//
// Created by Yaroslav on 30.07.2020.
//

#include <glm/ext.hpp>
#include "Camera.h"
#include "Window.h"
#include "Logger.hpp"

Camera::Camera(vec3 pos, float FOV) : pos(pos), FOV(FOV), rotation(1.0f)
{
    updateVectors();
}

Camera::~Camera()
{

}

mat4 Camera::getProjectionMatrix()
{
    return perspective(FOV, float(Window::_width) / float(Window::_height), 0.1f, 5000.0f);
}

mat4 Camera::getViewMatrix()
{
    return lookAt(pos, pos + front, up);
}

void Camera::updateVectors()
{
    front = vec3(rotation * vec4(0, 0, -1, 1));
    up = vec3(rotation * vec4(0, 1, 0, 1));
    right = vec3(rotation * vec4(1, 0, 0, 1));
}

void Camera::rotate(float x, float y, float z)
{
    rotation = glm::rotate(rotation, z, vec3(0, 0, 1));
    rotation = glm::rotate(rotation, y, vec3(0, 1, 0));
    rotation = glm::rotate(rotation, x, vec3(1, 0, 0));

    updateVectors();
}

vec3 Camera::raycastFromViewportCoords(double x, double y) {
    float nx = (2.0f * x) / Window::_width - 1.0f;
    float ny = (2.0f * y) / Window::_height - 1;
    ny = -ny;
    float nz = 1.0f;
    vec3 ray_nds = vec3(nx, ny, nz);
    vec4 ray_clip = vec4(ray_nds.x, ray_nds.y, -1.0, 1.0);
    vec4 ray_eye = inverse(getProjectionMatrix()) * ray_clip;
    ray_eye = vec4(ray_eye.x, ray_eye.y, -1.0, 0.0);
    vec3 ray_wor = (inverse(getViewMatrix()) * ray_eye);
    ray_wor = normalize(ray_wor);
    //vec3 ray_start = pos;
    //vec3 ray_end = pos + ray_wor * vec3(50, 50, 50);
    //LOG(ray_start.x << " " << ray_start.y << " " << ray_start.z);
    //LOG(ray_end.x << " " << ray_end.y << " " << ray_end.z);
    //LOG("")

    /*mat4 toWorld = inverse(getProjectionMatrix() * getViewMatrix());
    vec4 from = toWorld * vec4(nx, ny, -1.0f, 1.0f);
    vec4 to = toWorld * vec4(nx, ny, 1.0f, 1.0f);
    LOG(from.x << " " << from.y << " " << from.z);
    LOG(to.x << " " << to.y << " " << to.z);
    LOG("")*/

    //LOG("")
    //vec3 ray_end2 = glm::unProject(vec3(x, Window::_height - y, nz), getViewMatrix(), getProjectionMatrix(), vec4(0, 0, Window::_width, Window::_height));
    //LOG(ray_end.x << " " << ray_end.y << " " << ray_end.z);
    //LOG(ray_end2.x << " " << ray_end2.y << " " << ray_end2.z);
    return ray_wor;
}

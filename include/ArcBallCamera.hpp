//
// Created by Yaroslav on 30.07.2020.
//

#ifndef OPENGLTEST_ARCBALLCAMERA_H
#define OPENGLTEST_ARCBALLCAMERA_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>


class ArcBallCamera
{
private:
    void updateVectors();
public:
    float FOV;
    glm::vec3 pos;
    glm::vec3 lookat;
    glm::mat4 rotation;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::mat4 view;
    double zoom;

    void updateViewMatrix();

    ArcBallCamera(glm::vec3 pos, glm::vec3 lookAt, glm::vec3 up, float FOV);
    ~ArcBallCamera();

    glm::mat4 getProjectionMatrix();
    glm::mat4 getViewMatrix();
    glm::vec3 getViewDir();
    glm::vec3 getRightVector();
    void setCameraView(glm::vec3 eye, glm::vec3 _lookat, glm::vec3 _up);
    glm::vec3 raycastFromViewportCoords(double x, double y);
};


#endif //OPENGLTEST_ARCBALLCAMERA_H

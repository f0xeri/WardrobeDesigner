//
// Created by Yaroslav on 13.02.2021.
//

#ifndef MINE_STATE_HPP
#define MINE_STATE_HPP

#include <map>
#include "Camera.h"
#include "Scene.hpp"
#include "WardrobeGenerator.hpp"
#include "ArcBallCamera.hpp"
#include "Cluster.h"

struct State
{
    
    float deltaTime;
    float speed = 12.f;
    GLFWwindow *window;
    Camera *camera;
    ArcBallCamera *arcBallCamera;
    Scene *scene;
    WardrobeGenerator *wardrobeGenerator;
    ClusterManager cm;
    std::map<std::string, Texture*> wardrobeTextures;

    float deltaX = 0.0f;
    float deltaY = 0.0f;

    double dx, dy;

    float x = 0.0f;
    float y = 0.0f;
    unsigned int idBuffer;
    long long pickedObject = -1;

    bool dragEnabled = false;
    bool cursorLocked = true;
    bool cursorStarted = false;
    bool showDebug = true;

    int toInsert = 0;

    float camX = 0.0f;
    float camY = 0.0f;
    bool vsync = true;

    bool showPolygons = false;

    
};


#endif //MINE_STATE_HPP

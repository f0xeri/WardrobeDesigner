//
// Created by Yaroslav on 30.10.2020.
//

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <tuple>
#include <glm/ext.hpp>
#include <sstream>
#include <GUIRenderer.hpp>
#include "Window.h"
#include "Logger.hpp"
#include "ObjectLoader.h"
#include "Camera.h"
#include "Shader.hpp"
#include "Texture.hpp"
#include "Controls.h"
#include "Object/Cube.hpp"
#include "Object/Sphere.hpp"
#include "Scene.hpp"

State *state;
Controls *controls;

unsigned int nbFrames = 0;
double lastTime;

float skyboxVertices[] = {
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f,  1.0f
};

void showFPS(GLFWwindow *pWindow)
{
    double currentTime = glfwGetTime();
    double delta = currentTime - lastTime;
    nbFrames++;
    if (delta >= 1.0)
    {
        double fps = double(nbFrames) / delta;
        std::stringstream ss;
        ss << "FoxEngine " << " [" << fps << " FPS]";
        glfwSetWindowTitle(pWindow, ss.str().c_str());
        nbFrames = 0;
        lastTime = currentTime;
    }
}

Window::Window(const char *title, int width, int height)
{
    if (!glfwInit())
    {
        LOG("[ERROR] Failed to init GLFW!");
        throw std::runtime_error("GLFW init failed.");
    }
    else LOG("[INFO] GLFW inited.");
    glfwWindowHint(GLFW_SAMPLES, 8);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_POLYGON_SMOOTH);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

    mainWindow = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (mainWindow)
    {
        LOG("[INFO] Window opened.");
    }
    else
    {
        LOG("[ERROR] Window open failed!");
        throw std::runtime_error("Window init failed.");
    }
    makeContextCurrent();
    glfwSwapInterval(0);
    glewExperimental = true;
    if (glewInit() != GLEW_OK)
    {
        LOG("[ERROR] Failed to init GLEW!");
        throw std::runtime_error("GLEW init failed.");
    }
    else LOG("[INFO] GLEW inited.");
    Window::_width = width;
    Window::_height = height;

    state = new State();
    state->camera = new Camera({-20, 20, 0}, 60.0f);
    state->window = mainWindow;
    glfwSwapInterval(state->vsync);
    controls = new Controls(state);

    glfwSetWindowSizeCallback(mainWindow, resizeCallback);
    glfwSetCursorPosCallback(mainWindow, cursorCallback);
    glfwSetKeyCallback(mainWindow, keyCallback);
    glfwSetMouseButtonCallback(mainWindow, mouseButtonCallback);
    toggleCursor(mainWindow);
}

Window::~Window()
{
    glfwDestroyWindow(mainWindow);
    LOG("[INFO] Window closed.");
}

GLuint genModelVAO(VertexVector &model)
{
    std::vector<glm::fvec3> vertexPositions = std::get<0>(model);
    std::vector<glm::fvec2> vertexTexCoords = std::get<1>(model);
    std::vector<glm::fvec3> vertexNormals = std::get<2>(model);

    GLuint verticesVBO;
    GLuint textCoordsVBO;
    GLuint normalsVBO;
    GLuint VAO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &verticesVBO);
    glGenBuffers(1, &textCoordsVBO);
    glGenBuffers(1, &normalsVBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, verticesVBO);
    glBufferData(GL_ARRAY_BUFFER, vertexPositions.size() * sizeof(glm::vec3), vertexPositions.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)nullptr);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, textCoordsVBO);
    glBufferData(GL_ARRAY_BUFFER, vertexTexCoords.size() * sizeof(glm::vec2), vertexTexCoords.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void *)nullptr);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, normalsVBO);
    glBufferData(GL_ARRAY_BUFFER, vertexNormals.size() * sizeof(glm::vec3), vertexNormals.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void *)nullptr);
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    return VAO;
}

GLuint genSkyboxVAO()
{
    unsigned int skyboxVAO, skyboxVBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)nullptr);
    return skyboxVAO;
}

inline float squared(float v) { return v * v; }
bool doesCubeIntersectSphere(Cube *cube, Sphere *sphere)
{
    if (!cube->collisionEnabled || !sphere->collisionEnabled) return false;

    float dist_squared = sphere->radius * sphere->radius;
    if (sphere->position.x < cube->Bmin.x) dist_squared -= squared(sphere->position.x - cube->Bmin.x);
    else if (sphere->position.x > cube->Bmax.x) dist_squared -= squared(sphere->position.x - cube->Bmax.x);
    if (sphere->position.y < cube->Bmin.y) dist_squared -= squared(sphere->position.y - cube->Bmin.y);
    else if (sphere->position.y > cube->Bmax.y) dist_squared -= squared(sphere->position.y - cube->Bmax.y);
    if (sphere->position.z < cube->Bmin.z) dist_squared -= squared(sphere->position.z - cube->Bmin.z);
    else if (sphere->position.z > cube->Bmax.z) dist_squared -= squared(sphere->position.z - cube->Bmax.z);
    return dist_squared > 0;
}

bool doesRayIntersectCube(glm::vec3 ray_start, glm::vec3 dir, IObject *cube, float &htmin, float &htmax)
{
    Cube *ncube = dynamic_cast<Cube *>(cube);
    //glm::vec3 dir = ray_end - ray_start;
    glm::vec3 invDir = -dir;
    glm::vec3 tbot = invDir * (ncube->Bmin - ray_start);
    glm::vec3 ttop = invDir * (ncube->Bmax - ray_start);
    glm::vec3 tmin = min(ttop, tbot);
    glm::vec3 tmax = max(ttop, tbot);
    glm::vec2 t = max(vec2(tmin.x, tmin.x), vec2(tmin.y, tmin.z));
    float t0 = max(t.x, t.y);
    t = min(vec2(tmax.x, tmax.x), vec2(tmax.y, tmax.z));
    float t1 = min(t.x, t.y);
    htmin = t0;
    htmax = t1;
    return t1 > max(t0, 0.0f);
}

void renderDemoScene(Shader &shader, Cube *cube, Cube *cube2, Sphere *sphere)
{
    glActiveTexture(GL_TEXTURE0);
    cube->texture->bind();
    glm::mat4 model = glm::mat4(1.0f);
    shader.uniformMatrix(model, "model");
    if (state->pickedObject == 0)
        shader.setInt(1, "isPicked");
    else
        shader.setInt(0, "isPicked");
    cube->draw();

    glActiveTexture(GL_TEXTURE0);
    cube2->texture->bind();
    model = glm::mat4(1.0f);
    shader.uniformMatrix(model, "model");
    if (state->pickedObject == 1)
        shader.setInt(1, "isPicked");
    else
        shader.setInt(0, "isPicked");
    cube2->draw();

    if (doesCubeIntersectSphere(cube, sphere))
    {
        sphere->update(state->deltaTime, true, cube->position.y);
    }

    if (doesCubeIntersectSphere(cube2, sphere))
    {
        sphere->update(state->deltaTime, true, cube2->position.y);
    }
    else
    {
        sphere->update(state->deltaTime, false, 0.0);
    }

    vec3 sphereTranslate = sphere->position - sphere->startPosition;
    model = glm::mat4(1.0f);
    model = glm::translate(model, sphereTranslate);
    shader.uniformMatrix(model, "model");
    if (state->pickedObject == 2)
    {
        // с этим условием получаются рывки, если пытаться сделать норм скорость
        // без - объект продолжает двигаться даже если не двигать мышкой, ибо сохраняется последние данные deltaX и deltaY
        if (state->x - state->dx != 0 || state->y - state->dy != 0)
        {
            sphere->position.x += state->deltaX * state->deltaTime;
            sphere->position.z += state->deltaY * state->deltaTime;

            sphereTranslate = sphere->position - sphere->startPosition;
            model = glm::mat4(1.0f);
            model = glm::translate(model, sphereTranslate);
            shader.uniformMatrix(model, "model");
        }
        shader.setInt(1, "isPicked");
    }
    else
        shader.setInt(0, "isPicked");
    sphere->draw();
}

void renderScene(Shader &shader, Scene *scene)
{
    glActiveTexture(GL_TEXTURE0);
    glm::vec3 ray_start = state->camera->pos;
    glm::vec3 ray_dir = state->camera->raycastFromViewportCoords(state->dx, state->dy);
    glm::vec3 ray_end = ray_start + ray_dir * 50.0f;
    glm::vec3 hit;

    glLineWidth(2.5);
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_LINES);
    glVertex3f(ray_start.x, ray_start.y, ray_start.z);
    glVertex3f(ray_end.x, ray_end.y, ray_end.z);
    glEnd();

    for (size_t i = 0; i < scene->objects.size(); i++)
    {
        scene->objects[i]->update(state, i);
        shader.uniformMatrix(scene->objects[i]->model, "model");
        if (state->pickedObject == i)
            shader.setInt(1, "isPicked");
        else
            shader.setInt(0, "isPicked");
        scene->objects[i]->texture->bind();
        scene->objects[i]->draw();
    }
    //LOG("")
}

void renderSceneId(Shader &shader, Scene *scene)
{
    for (int i = 0; i < scene->objects.size(); i++)
    {
        /*vec3 objPosTranslate = scene->objects[i]->position - scene->objects[i]->startPosition;
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, objPosTranslate);*/
        shader.uniformMatrix(scene->objects[i]->model, "model");
        int r = (i & 0x000000FF) >>  0;
        int g = (i & 0x0000FF00) >>  8;
        int b = (i & 0x00FF0000) >> 16;
        glUniform4f(glGetUniformLocation(shader.mProgram, "color"), r / 255.0f, g / 255.0f, b / 255.0f, 1.0f);
        scene->objects[i]->draw();
    }
}

unsigned int quadVAO = 0;
unsigned int quadVBO;
void renderQuad()
{
    if (quadVAO == 0)
    {
        float quadVertices[] = {
                // positions        // texture Coords
                -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
                -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
                1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
                1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        };
        // setup plane VAO
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    }
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}

long double operator "" _mm(long double mm)
{
    return mm / 320.0f;
}

long double operator "" _mm(unsigned long long mm)
{
    return mm / 320.0f;
}


void Window::startLoop()
{
    glEnable(GL_DEPTH_TEST);
    ObjectLoader loader;
    GUIRenderer gui(mainWindow);

    Texture *texture = new Texture("res/textures/cube.jpg");

    Texture *grassAlbedo = new Texture("res/textures/grass_material/grass1-albedo3.png");
    grassAlbedo->loadTexture();

    Texture *floorTexture = new Texture("res/textures/floor.jpg");
    floorTexture->loadTexture();

    Texture *woodTexture = new Texture("res/textures/woodTexture.jpg");
    woodTexture->loadTexture();

    Texture *skyboxTexture = new Texture("skybox");
    std::vector<std::string> faces
            {
                    "res/textures/right.jpg",
                    "res/textures/left.jpg",
                    "res/textures/top.jpg",
                    "res/textures/bottom.jpg",
                    "res/textures/front.jpg",
                    "res/textures/back.jpg"
            };

    /*Cube *cube = new Cube({0, 0, 0}, {2048, 1, 2048});
    cube->physicsEnabled = true;
    cube->collisionEnabled = true;
    cube->texture = grassAlbedo;
    cube->generateVAO();
    Cube *cube2 = new Cube({750, 3, 750}, {5, 1, 5});
    cube2->physicsEnabled = true;
    cube2->collisionEnabled = true;
    cube2->texture = texture;
    cube2->generateVAO();
    glm::vec3 startPos{748, 15, 748};
    Sphere *sphere = new Sphere(startPos, 1.0f);
    sphere->physicsEnabled = true;
    sphere->collisionEnabled = true;
    sphere->velocity = {2, 0, 2};
    sphere->texture = texture;
    sphere->generateVAO();*/

    Cube *floor = new Cube({-10, -1, -10}, {20, 1, 20});
    floor->texture = floorTexture;
    floor->texScaleX = floor->texScaleY = 8;
    floor->generateVAO();

    Cube *bottomSide = new Cube({0, 0, -600_mm}, {1500_mm, 132_mm, 600_mm});
    bottomSide->texture = woodTexture;
    bottomSide->texScaleX = 4;
    bottomSide->texScaleY = 1;
    bottomSide->generateVAO();

    Cube *backSide = new Cube({0, 0, 0}, {1500_mm, 2400_mm, 16_mm});
    backSide->texture = woodTexture;
    backSide->texScaleX = 4;
    backSide->texScaleY = 1;
    backSide->generateVAO();

    Cube *topSide = new Cube({16_mm, 2384_mm, -600_mm}, {1468_mm, 16_mm, 600_mm});
    topSide->texture = woodTexture;
    topSide->texScaleX = 2;
    topSide->texScaleY = 2;
    topSide->generateVAO();

    Cube *leftSide = new Cube({0, 132_mm, -600_mm}, {16_mm, 2268_mm, 600_mm});
    leftSide->texture = woodTexture;
    leftSide->texScaleX = 2;
    leftSide->texScaleY = 2;
    leftSide->generateVAO();

    Cube *rightSide = new Cube({1484_mm, 132_mm, -600_mm}, {16_mm, 2268_mm, 600_mm});
    rightSide->texture = woodTexture;
    rightSide->texScaleX = 2;
    rightSide->texScaleY = 2;
    rightSide->generateVAO();

    Cube *centerSide = new Cube({726_mm, 132_mm, -600_mm}, {16_mm, 1851_mm, 600_mm});
    centerSide->texture = woodTexture;
    centerSide->texScaleX = 2;
    centerSide->texScaleY = 2;
    centerSide->generateVAO();

    Cube *shellMainLow = new Cube({16_mm, 2.4, -600_mm}, {3.9, 16_mm, 600_mm});
    shellMainLow->texture = woodTexture;
    shellMainLow->texScaleX = 2;
    shellMainLow->texScaleY = 2;
    shellMainLow->generateVAO();

    Cube *shellMainHigh = new Cube({16_mm, 1983_mm, -600_mm}, {1468_mm, 16_mm, 600_mm});
    shellMainHigh->texture = woodTexture;
    shellMainHigh->texScaleX = 2;
    shellMainHigh->texScaleY = 2;
    shellMainHigh->generateVAO();

    Cube *shellHalfLeft1 = new Cube({0, 3.4, -1.45}, {1.95, 16_mm, 600_mm});
    shellHalfLeft1->texture = woodTexture;
    shellHalfLeft1->texScaleX = 2;
    shellHalfLeft1->texScaleY = 2;
    shellHalfLeft1->generateVAO();

    //vec3 lightPos(730.0f, 15.0f, 730.0f);
    vec3 lightPos(15.0f, 25.0f, -20.0f);

    auto skybox = genSkyboxVAO();

    Scene *scene = new Scene();
    /*scene->addObject(cube);
    scene->addObject(cube2);
    scene->addObject(sphere);*/
    scene->addObject(floor);
    scene->addObject(bottomSide);
    scene->addObject(backSide);
    scene->addObject(topSide);
    scene->addObject(leftSide);
    scene->addObject(rightSide);
    scene->addObject(centerSide);
    //scene->addObject(shellMainLow);
    scene->addObject(shellMainHigh);
    //scene->addObject(shellHalfLeft1);
    state->scene = scene;

    glEnable(GL_DEPTH_TEST);
    state->camera = new Camera(glm::vec3(-20.0f, 15.0f, -20.0f), radians(60.0f));

    unsigned int cubemapTexture = skyboxTexture->loadCubemap(faces);
    texture->loadTexture();

    const unsigned int SHADOW_WIDTH = 2048, SHADOW_HEIGHT = 2048;
    unsigned int depthMapFBO;
    glGenFramebuffers(1, &depthMapFBO);
    // create depth texture
    unsigned int depthMap;
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    // attach depth texture as FBO's depth buffer
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    //unsigned int idBuffer;
    glGenFramebuffers(1, &state->idBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, state->idBuffer);
    unsigned int idColor;
    glGenTextures(1, &idColor);
    glBindTexture(GL_TEXTURE_2D, idColor);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Window::_width, Window::_height, 0, GL_RGBA, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, idColor, 0);
    unsigned int attachments[1] = { GL_COLOR_ATTACHMENT0 };
    glDrawBuffers(1, attachments);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "Framebuffer not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    Shader shader("vert", "frag");
    shader.link();

    Shader colorIdShader("colorPickVert", "colorPickFrag");
    colorIdShader.link();

    Shader skyboxShader("vertSkybox", "fragSkybox");
    skyboxShader.link();

    Shader simpleDepthShader("vertDepthShader", "fragDepthShader");
    simpleDepthShader.link();

    Shader debugQuad("vertDebugQuad", "fragDebugQuad");
    debugQuad.setInt(0, "depthMap");
    debugQuad.link();

    double lastTime = glfwGetTime();

    while (!glfwWindowShouldClose(mainWindow))
    {
        showFPS(mainWindow);
        double currentTime = glfwGetTime();
        state->deltaTime = glfwGetTime() - lastTime;
        lastTime = currentTime;
        glfwGetCursorPos(mainWindow, &state->dx, &state->dy);
        //state->deltaX = state->deltaY = 0;
        updateInputs(mainWindow);

        glm::mat4 lightProjection, lightView;
        glm::mat4 lightSpaceMatrix;
        float near_plane = 1.0f, far_plane = 80.5f;
        lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
        lightView = glm::lookAt(lightPos, glm::vec3(-2.0f, 0.0f, -2.0f), glm::vec3(0.0, 1.0, 0.0));
        lightSpaceMatrix = lightProjection * lightView;
        // render scene from light's point of view
        simpleDepthShader.use();
        simpleDepthShader.uniformMatrix(lightSpaceMatrix, "lightSpaceMatrix");

        glViewport(0, 0,  SHADOW_WIDTH, SHADOW_HEIGHT);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
            glClear(GL_DEPTH_BUFFER_BIT);
            glActiveTexture(GL_TEXTURE0);
            texture->bind();
        renderScene(simpleDepthShader, scene);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // color id
        glBindFramebuffer(GL_FRAMEBUFFER, state->idBuffer);
        glViewport(0, 0, Window::_width, Window::_height);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        colorIdShader.use();
        colorIdShader.uniformMatrix(state->camera->getProjectionMatrix() * state->camera->getViewMatrix(), "projView");
        renderSceneId(colorIdShader, scene);
        //glFlush();
        //glFinish();
        //glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        //glReadBuffer(GL_COLOR_ATTACHMENT0);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        // reset viewport
        glViewport(0, 0, Window::_width, Window::_height);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        if (state->showPolygons) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        shader.use();
        shader.uniformMatrix(state->camera->getProjectionMatrix() * state->camera->getViewMatrix(), "projView");
        glUniform3f(glGetUniformLocation(shader.mProgram, "viewPos"), state->camera->pos.x, state->camera->pos.y, state->camera->pos.z);
        glUniform3f(glGetUniformLocation(shader.mProgram, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
        shader.uniformMatrix(lightSpaceMatrix, "lightSpaceMatrix");
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, depthMap);
        renderScene(shader, scene);


        debugQuad.use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, depthMap);
        //renderQuad();

        glDepthFunc(GL_LEQUAL);
        skyboxShader.use();
        skyboxTexture->bind();
        glm::mat4 model2 = glm::mat4(1.f);
        skyboxShader.uniformMatrix(model2, "view");
        skyboxShader.uniformMatrix(state->camera->getProjectionMatrix() * glm::mat4(glm::mat3(state->camera->getViewMatrix())), "projection");
        glBindVertexArray(skybox);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glDepthFunc(GL_LESS);

        state->lmbClicked = false;

        gui.render(state);

        glfwSwapBuffers(mainWindow);
        glfwPollEvents();
    }
}

void Window::makeContextCurrent()
{
    glfwMakeContextCurrent(mainWindow);
}

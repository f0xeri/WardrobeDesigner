//
// Created by Yaroslav on 30.10.2020.
//

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <tuple>
#include <glm/ext.hpp>
#include <sstream>
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

void renderScene(Shader &shader, Cube *cube, Cube *cube2, Sphere *sphere, vec3 startPos, Texture *texture, Texture *grassAlbedo, bool colorIdPass)
{
    glActiveTexture(GL_TEXTURE0);
    grassAlbedo->bind();
    glm::mat4 model = glm::mat4(1.0f);
    shader.uniformMatrix(model, "model");
    if (state->pickedObject == 0)
        shader.setInt(1, "isPicked");
    else
        shader.setInt(0, "isPicked");
    cube->draw();

    glActiveTexture(GL_TEXTURE0);
    texture->bind();
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

    vec3 sphereTranslate = sphere->position - startPos;
    model = glm::mat4(1.0f);
    model = glm::translate(model, sphereTranslate);
    shader.uniformMatrix(model, "model");
    if (state->pickedObject == 2)
        shader.setInt(1, "isPicked");
    else
        shader.setInt(0, "isPicked");
    sphere->draw();
}

void renderSceneId(Shader &shader, Scene *scene, Cube *cube, Cube *cube2, Sphere *sphere, vec3 startPos)
{
    /*for (int i = 0; i < scene->objects.size(); i++)
    {
        glm::mat4 model = glm::mat4(1.0f);
        shader.uniformMatrix(model, "model");
        int r = (i & 0x000000FF) >>  0;
        int g = (i & 0x0000FF00) >>  8;
        int b = (i & 0x00FF0000) >> 16;
        glUniform4f(glGetUniformLocation(shader.mProgram, "color"), r / 255.0f, g / 255.0f, b / 255.0f, 1.0f);
        scene->objects[i]->draw();
    }*/

    glm::mat4 model = glm::mat4(1.0f);
    shader.uniformMatrix(model, "model");
    int r = (0 & 0x000000FF) >>  0;
    int g = (0 & 0x0000FF00) >>  8;
    int b = (0 & 0x00FF0000) >> 16;
    glUniform4f(glGetUniformLocation(shader.mProgram, "color"), r / 255.0f, g / 255.0f, b / 255.0f, 1.0f);
    cube->draw();

    model = glm::mat4(1.0f);
    shader.uniformMatrix(model, "model");
    r = (1 & 0x000000FF) >>  0;
    g = (1 & 0x0000FF00) >>  8;
    b = (1 & 0x00FF0000) >> 16;
    glUniform4f(glGetUniformLocation(shader.mProgram, "color"), r / 255.0f, g / 255.0f, b / 255.0f, 1.0f);
    cube2->draw();

    model = glm::mat4(1.0f);
    vec3 sphereTranslate = sphere->position - startPos;
    model = glm::mat4(1.0f);
    model = glm::translate(model, sphereTranslate);
    shader.uniformMatrix(model, "model");
    r = (2 & 0x000000FF) >>  0;
    g = (2 & 0x0000FF00) >>  8;
    b = (2 & 0x00FF0000) >> 16;
    glUniform4f(glGetUniformLocation(shader.mProgram, "color"), r / 255.0f, g / 255.0f, b / 255.0f, 1.0f);
    sphere->draw();
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

void Window::startLoop()
{
    glEnable(GL_DEPTH_TEST);
    ObjectLoader loader;

    Cube *cube = new Cube({0, 0, 0}, {2048, 1, 2048});
    cube->physicsEnabled = true;
    cube->collisionEnabled = true;
    cube->generateVAO();

    Cube *cube2 = new Cube({750, 3, 750}, {5, 1, 5});
    cube2->physicsEnabled = true;
    cube2->collisionEnabled = true;
    cube2->generateVAO();

    glm::vec3 startPos{748, 15, 748};
    Sphere *sphere = new Sphere(startPos, 1.0f);
    sphere->physicsEnabled = true;
    sphere->collisionEnabled = true;
    sphere->velocity = {2, 0, 2};
    sphere->generateVAO();

    vec3 lightPos(730.0f, 15.0f, 730.0f);

    auto skybox = genSkyboxVAO();

    Scene *scene = new Scene();
    scene->addObject(cube);
    scene->addObject(cube2);
    scene->addObject(sphere);
    state->scene = scene;

    glEnable(GL_DEPTH_TEST);
    state->camera = new Camera(glm::vec3(750.0f, 15.0f, 800.0f), radians(60.0f));

    Texture *texture = new Texture("res/textures/cube.jpg");

    Texture *grassAlbedo = new Texture("res/textures/grass_material/grass1-albedo3.png");
    grassAlbedo->loadTexture();


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
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Window::_width, Window::_height, 0, GL_RGBA, GL_FLOAT, NULL);
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

        updateInputs(mainWindow);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 lightProjection, lightView;
        glm::mat4 lightSpaceMatrix;
        float near_plane = 1.0f, far_plane = 80.5f;
        lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
        lightView = glm::lookAt(lightPos, glm::vec3(750.0f, 0.0f, 750.0f), glm::vec3(0.0, 1.0, 0.0));
        lightSpaceMatrix = lightProjection * lightView;
        // render scene from light's point of view
        simpleDepthShader.use();
        simpleDepthShader.uniformMatrix(lightSpaceMatrix, "lightSpaceMatrix");

        glViewport(0, 0,  SHADOW_WIDTH, SHADOW_HEIGHT);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
            glClear(GL_DEPTH_BUFFER_BIT);

            glActiveTexture(GL_TEXTURE0);
            texture->bind();
            renderScene(simpleDepthShader, cube, cube2, sphere, startPos, texture, grassAlbedo, false);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // color id
        glBindFramebuffer(GL_FRAMEBUFFER, state->idBuffer);
        glViewport(0, 0, Window::_width, Window::_height);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        colorIdShader.use();
        colorIdShader.uniformMatrix(state->camera->getProjectionMatrix() * state->camera->getViewMatrix(), "projView");
        renderSceneId(colorIdShader, scene, cube, cube2, sphere, startPos);
        glFlush();
        glFinish();
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        glReadBuffer(GL_COLOR_ATTACHMENT0);

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
        renderScene(shader, cube, cube2, sphere, startPos, texture, grassAlbedo, false);

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

        glfwSwapBuffers(mainWindow);
        glfwPollEvents();
    }
}

void Window::makeContextCurrent()
{
    glfwMakeContextCurrent(mainWindow);
}

//
// Created by Yaroslav on 30.10.2020.
//

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <tuple>
#include <glm/ext.hpp>
#include <sstream>
#include <GUIRenderer.hpp>
#include <WardrobeGenerator.hpp>
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
#include "Tools.hpp"


#include <Object/WardrobeElements/WardrobeVerticalElement.hpp>
#include <Object/WardrobeElements/WardrobeHorizontalShelf.hpp>


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
        ss << "WardrobeDesigner " << " [" << fps << " FPS]";
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
    glfwSetScrollCallback(mainWindow, scrollCallback);
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

void renderScene(Shader &shader, Scene *scene)
{
    glActiveTexture(GL_TEXTURE0);

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

    GUIRenderer gui(mainWindow);

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

    state->wardrobeTextures.insert(std::make_pair(floorTexture->name, floorTexture));
    state->wardrobeTextures.insert(std::make_pair(woodTexture->name, woodTexture));

    Cube *floor = new Cube({-10, -1, -10}, {20, 1, 20});
    floor->texture = floorTexture;
    floor->texScaleX = floor->texScaleY = 8;
    floor->generateVAO();

    state->wardrobeGenerator = new WardrobeGenerator({0, 0, 0}, 1500_mm, 2400_mm, 600_mm, 16_mm, 132_mm, woodTexture);
    
    state->cm = ClusterManager(
        glm::vec3(  state->wardrobeGenerator->width - state->wardrobeGenerator->boardThickness * 2,
                    state->wardrobeGenerator->height - state->wardrobeGenerator->baseHeight - state->wardrobeGenerator->boardThickness,
                    state->wardrobeGenerator->depth),
        state->wardrobeGenerator->origin);

    vec3 lightPos(15.0f, 25.0f, -20.0f);

    auto skybox = genSkyboxVAO();

    Scene *scene = new Scene();

    scene->addObject(floor);
    scene->addObject(state->wardrobeGenerator->bottomSide);
    scene->addObject(state->wardrobeGenerator->backSide);
    scene->addObject(state->wardrobeGenerator->topSide);
    scene->addObject(state->wardrobeGenerator->leftSide);
    scene->addObject(state->wardrobeGenerator->rightSide);
    state->scene = scene;

    glEnable(GL_DEPTH_TEST);
    state->camera = new Camera(glm::vec3(-20.0f, 15.0f, -20.0f), radians(60.0f));

    unsigned int cubemapTexture = skyboxTexture->loadCubemap(faces);

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

    glm::vec3 eye(10, 10, 10);
    glm::vec3 center(0);
    glm::vec3 up(0, 1, 0);
    state->arcBallCamera = new ArcBallCamera(eye, center, up, radians(60.0f));
    double lastTime = glfwGetTime();

    while (!glfwWindowShouldClose(mainWindow))
    {
        showFPS(mainWindow);
        double currentTime = glfwGetTime();
        state->deltaTime = glfwGetTime() - lastTime;
        lastTime = currentTime;
        glfwGetCursorPos(mainWindow, &state->dx, &state->dy);

        //updateInputs(mainWindow);

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
        renderScene(simpleDepthShader, scene);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        // reset viewport
        glViewport(0, 0, Window::_width, Window::_height);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        if (state->showPolygons) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        shader.use();
        shader.uniformMatrix(state->arcBallCamera->getProjectionMatrix() * state->arcBallCamera->getViewMatrix(), "projView");
        glUniform3f(glGetUniformLocation(shader.mProgram, "viewPos"), state->arcBallCamera->pos.x, state->arcBallCamera->pos.y, state->arcBallCamera->pos.z);
        glUniform3f(glGetUniformLocation(shader.mProgram, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
        shader.uniformMatrix(lightSpaceMatrix, "lightSpaceMatrix");
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, depthMap);
        renderScene(shader, scene);

        /*debugQuad.use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, idColor);
        renderQuad();*/

        glDepthFunc(GL_LEQUAL);
        skyboxShader.use();
        skyboxTexture->bind();
        glm::mat4 model2 = glm::mat4(1.f);
        skyboxShader.uniformMatrix(model2, "view");
        skyboxShader.uniformMatrix(state->arcBallCamera->getProjectionMatrix() * glm::mat4(glm::mat3(state->arcBallCamera->getViewMatrix())), "projection");
        glBindVertexArray(skybox);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glDepthFunc(GL_LESS);

        gui.render(state);

        glfwSwapBuffers(mainWindow);
        glfwPollEvents();
    }
}

void Window::makeContextCurrent()
{
    glfwMakeContextCurrent(mainWindow);
}
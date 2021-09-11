//
// Created by Yaroslav on 30.10.2020.
//

#ifndef LAB4B_WINDOW_H
#define LAB4B_WINDOW_H

#include <string>
#include "GLFW/glfw3.h"

class Window
{
private:
    GLFWwindow *mainWindow;
public:
    inline static int _width, _height;
    Window(const char *title, int width, int height);
    ~Window();

    void startLoop();
    void makeContextCurrent();

};

#endif //LAB4B_WINDOW_H

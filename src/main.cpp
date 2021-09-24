#include <GLFW/glfw3.h>
#include "Window.h"

int main() {
    Window mainWindow("WardrobeDesigner", 1280, 720);
    mainWindow.makeContextCurrent();
    mainWindow.startLoop();
    glfwTerminate();
    return 0;
}

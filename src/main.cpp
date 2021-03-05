#include <GLFW/glfw3.h>
#include "Window/Window.h"

int main() {
    Window mainWindow("FoxEngine", 1280, 720);
    mainWindow.makeContextCurrent();
    mainWindow.startLoop();
    glfwTerminate();
    return 0;
}

#define GL_SILENCE_DEPRECATION


#include <GLFW/glfw3.h>
#include <iostream>
#include "Application.h"
#include "checkError.h"


void onResize(int width, int height) { std::cout << "onResize: " << width << " " << height << std::endl; }


void onKeyBoard(int key, int action, int mods) {
    std::cout << "onKey: " << key << " " << action << " " << mods << std::endl;
}


int main() {
    Application &app = Application::getInstance();
    if (!app.init(800, 600)) {
        std::cerr << "Failed to initialize application" << std::endl;
        return -1;
    }
    app.setKeyCallback(onKeyBoard);
    app.setResizeCallback(onResize);


    GL_CALL(glViewport(0, 0, 800, 600));
    GL_CALL(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));

    while (app.update()) {
        // 渲染
        GL_CALL(glClear(GL_COLOR_BUFFER_BIT));
    }
    app.destroy();
    return 0;
}

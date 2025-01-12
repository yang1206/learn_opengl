#define GL_SILENCE_DEPRECATION
// glad引入要在 glfw 前面
#include <glad/glad.h>


#include <GLFW/glfw3.h>
#include <iostream>

// 窗口大小改变回调函数
void frameBufferSizeCallback(GLFWwindow *window, int width, int height) {
    std::cout << "frameBufferSizeCallback" << std::endl;
    std::cout << "width: " << width << ", height: " << height << std::endl;
    glViewport(0, 0, width, height);
}

// 键盘回调函数
void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    std::cout << "keyCallback" << std::endl;
    std::cout << "key: " << key << ", scancode: " << scancode << ", action: " << action << ", mods: " << mods
              << std::endl;
}


int main() {
    // 初始化 GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }
    // 设置窗口属性, macos最高支持OpenGL 4.1
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    // 设置核心渲染模式
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // 创建窗口（用于创建 OpenGL 上下文）
    GLFWwindow *window = glfwCreateWindow(800, 600, "OpenGL 学习", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // 设置当前上下文 设置当前窗口为 opengl 的绘制舞台
    glfwMakeContextCurrent(window);

    // 设置窗口大小改变回调函数
    glfwSetFramebufferSizeCallback(window, frameBufferSizeCallback);

    glfwSetKeyCallback(window, keyCallback);

    // 初始化 glad
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cerr << "Failed to initialize glad" << std::endl;
        glfwTerminate();
        return -1;
    }

    glViewport(0, 0, 800, 600);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    // 主循环
    while (!glfwWindowShouldClose(window)) {

        // 检查消息队列是否有消息需要处理
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT);
        // 切换双缓冲区
        glfwSwapBuffers(window);
    }

    // 清理
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

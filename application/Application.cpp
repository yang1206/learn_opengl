#include "Application.h"
// glad引入要在 glfw 前面
#include <glad/glad.h>


#include <GLFW/glfw3.h>
#include <iostream>
Application *Application::instance = nullptr;

Application &Application::getInstance() {
    if (instance == nullptr) {
        instance = new Application();
    }
    return *instance;
}

Application::Application() { std::cout << "Application constructor" << std::endl; }

Application::~Application() { std::cout << "Application destructor" << std::endl; }

bool Application::init(const int width, const int height) {
    mWidth = width;
    mHeight = height;
    // 初始化 GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return false;
    }

    // 设置窗口属性, macos最高支持OpenGL 4.1
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    // 设置核心渲染模式
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // 创建窗口（用于创建 OpenGL 上下文）
    mWindow = glfwCreateWindow(mWidth, mHeight, "OpenGL 学习", nullptr, nullptr);
    if (!mWindow) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }

    // 设置当前上下文 设置当前窗口为 opengl 的绘制舞台
    glfwMakeContextCurrent(mWindow);
    // 初始化 glad
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cerr << "Failed to initialize glad" << std::endl;
        glfwTerminate();
        return false;
    }
    glfwSetWindowUserPointer(mWindow, this);

    // 设置窗口大小改变回调函数
    glfwSetFramebufferSizeCallback(mWindow, frameBufferSizeCallback);

    // 设置键盘回调函数
    glfwSetKeyCallback(mWindow, keyCallback);


    return true;
}


bool Application::update() {
    if (glfwWindowShouldClose(mWindow))
        return false;

    // 检查消息队列是否有消息需要处理
    glfwPollEvents();
    // 切换双缓冲区
    glfwSwapBuffers(mWindow);
    return true;
}

void Application::destroy() { glfwTerminate(); }

void Application::setResizeCallback(ResizeCallback callback) { mResizeCallback = callback; }

void Application::setKeyCallback(KeyCallback callback) { mKeyCallback = callback; }

void Application::frameBufferSizeCallback(GLFWwindow *window, int width, int height) {
    Application *app = static_cast<Application *>(glfwGetWindowUserPointer(window));
    if (app->mResizeCallback != nullptr) {
        app->mResizeCallback(width, height);
    }
}

void Application::keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    Application *app = static_cast<Application *>(glfwGetWindowUserPointer(window));
    if (app->mKeyCallback != nullptr) {
        app->mKeyCallback(key, action, mods);
    }
}

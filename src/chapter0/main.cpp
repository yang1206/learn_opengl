#include "imgui.h"
#define GL_SILENCE_DEPRECATION
// glad引入要在 glfw 前面
#include <glad/glad.h>


#include <GLFW/glfw3.h>
#include <cmath>
#include <iostream>
#include "ImGuiLayer.h"


void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

const char *vertexShaderSource = "#version 330 core\n"
                                 "layout (location = 0) in vec3 aPos;\n"
                                 "layout (location = 1) in vec3 aColor;\n"
                                 "out vec3 ourColor;\n"
                                 "uniform float xOffset;\n"
                                 "uniform float yOffset;\n"
                                 "uniform float colorMix;\n"
                                 "void main()\n"
                                 "{\n"
                                 "   gl_Position = vec4(aPos.x + xOffset, aPos.y + yOffset, aPos.z, 1.0);\n"
                                 "   vec3 nextColor = vec3(aColor.b, aColor.r, aColor.g);\n"
                                 "   ourColor = mix(aColor, nextColor, colorMix);\n"
                                 "}\0";

const char *fragmentShaderSource = "#version 330 core\n"
                                   "out vec4 FragColor;\n"
                                   "in vec3 ourColor;\n"
                                   "void main()\n"
                                   "{\n"
                                   "   FragColor = vec4(ourColor, 1.0f);\n"
                                   "}\n\0";
int main() {
    // 初始化 GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
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
    GLFWwindow *window = glfwCreateWindow(800, 600, "OpenGL 学习", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // 设置当前上下文 设置当前窗口为 opengl 的绘制舞台
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // 初始化 glad
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cerr << "Failed to initialize glad" << std::endl;
        glfwTerminate();
        return -1;
    }

    ImGuiLayer::init(window);

    // build and compile our shader program
    // ------------------------------------
    // vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);

    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // link shaders

    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    float vertices[] = {
            0.5f,  -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // 右下
            -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // 左下
            0.0f,  0.5f,  0.0f, 0.0f, 0.0f, 1.0f // 顶部
    };

    // 创建 VBO 和 VAO
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // 首先绑定顶点数组对象，然后绑定并设置顶点缓冲区，然后配置顶点属性。
    glBindVertexArray(VAO);

    // 绑定 VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);
    // 颜色属性
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);


    // glBindBuffer(GL_ARRAY_BUFFER, 0);
    // glBindVertexArray(0);

    // 添加控制变量
    float xOffset = 0.0f;
    float yOffset = 0.0f;
    float colorMix = 0.0f;
    bool autoRotate = true; // 控制是否自动旋转
    float rotateSpeed = 1.0f; // 控制旋转速度
    float color[3] = {1.0f, 1.0f, 1.0f}; // 额外添加的颜色控制


    // 主循环
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        // ImGui新帧
        ImGuiLayer::begin();

        // ImGui控制面板
        ImGui::Begin("控制面板");

        ImGui::Checkbox("自动旋转", &autoRotate);
        ImGui::SliderFloat("旋转速度", &rotateSpeed, 0.1f, 5.0f);

        if (autoRotate) {
            float timeValue = glfwGetTime() * rotateSpeed;
            xOffset = cos(timeValue) * 0.5f;
            yOffset = sin(timeValue) * 0.5f;
            colorMix = (sin(timeValue) + 1.0f) * 0.5f;
        } else {
            ImGui::SliderFloat("X偏移", &xOffset, -0.5f, 0.5f);
            ImGui::SliderFloat("Y偏移", &yOffset, -0.5f, 0.5f);
            ImGui::SliderFloat("颜色混合", &colorMix, 0.0f, 1.0f);
        }

        ImGui::ColorEdit3("三角形颜色", color);
        ImGui::Text("应用平均 %.3f ms/帧 (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

        ImGui::End();

        glUseProgram(shaderProgram);

        int xOffsetLocation = glGetUniformLocation(shaderProgram, "xOffset");
        int yOffsetLocation = glGetUniformLocation(shaderProgram, "yOffset");
        int colorMixLocation = glGetUniformLocation(shaderProgram, "colorMix");

        glUniform1f(xOffsetLocation, xOffset);
        glUniform1f(yOffsetLocation, yOffset);
        glUniform1f(colorMixLocation, colorMix);

        glDrawArrays(GL_TRIANGLES, 0, 3);
        // 渲染ImGui
        ImGuiLayer::end();

        glfwSwapBuffers(window);
        // 检查消息队列是否有消息需要处理
        glfwPollEvents();
    }
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
    // 清理ImGui
    ImGuiLayer::destroy();
    ImGui::DestroyContext();
    // 清理
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

#define GL_SILENCE_DEPRECATION
#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <iostream>
#include "Application.h"
#include "checkError.h"


void onResize(int width, int height) {
    std::cout << "onResize: " << width << " " << height << std::endl;
    GL_CALL(glViewport(0, 0, width, height));
}


void onKeyBoard(int key, int action, int mods) {
    std::cout << "onKey: " << key << " " << action << " " << mods << std::endl;
}

GLuint vao = 0;
GLuint shaderProgram = 0;


void prepareVAOForGLTriangle() {
    float positions[] = {
            -0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f, 0.0f, 0.5f, 0.0f,
            // 0.5f,  0.5f,  0.0f, 0.8f, 0.8f,  0.0f, 0.8f, 0.0f, 0.0f,
    };

    GLuint posVbo;
    glGenBuffers(1, &posVbo);
    glBindBuffer(GL_ARRAY_BUFFER, posVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, posVbo);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void prepareShader() {

    const char *vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
void main()
{
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}
)";

    const char *fragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;
void main()
{
    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
}
)";

    // 创建 shader 对象

    GLuint vertex, fragment;
    vertex = glCreateShader(GL_VERTEX_SHADER);
    fragment = glCreateShader(GL_FRAGMENT_SHADER);


    // 为 shader 对象加载源码
    glShaderSource(vertex, 1, &vertexShaderSource, nullptr);
    glShaderSource(fragment, 1, &fragmentShaderSource, nullptr);
    // 编译 shader 对象
    int success = 0;
    glCompileShader(vertex);
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(vertex, 512, nullptr, infoLog);
        std::cout << "vertex shader compile error: " << infoLog << std::endl;
    }
    glCompileShader(fragment);
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[1024];
        glGetShaderInfoLog(fragment, 1024, nullptr, infoLog);
        std::cout << "fragment shader compile error: " << infoLog << std::endl;
    }

    // 创建 shader 程序对象
    shaderProgram = glCreateProgram();
    // 将 shader 对象附加到 shader 程序对象中
    glAttachShader(shaderProgram, vertex);
    glAttachShader(shaderProgram, fragment);
    // 链接 shader 程序对象
    glLinkProgram(shaderProgram);
    // 检查链接状态
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (success == GL_FALSE) {
        char infoLog[1024];
        glGetProgramInfoLog(shaderProgram, 1024, nullptr, infoLog);
        std::cout << "shader program link error: " << infoLog << std::endl;
    }


    // 清理
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void render() {
    // 渲染
    GL_CALL(glClear(GL_COLOR_BUFFER_BIT));
    GL_CALL(glUseProgram(shaderProgram));
    GL_CALL(glBindVertexArray(vao));
    GL_CALL(glDrawArrays(GL_TRIANGLES, 0, 3));
}


int main() {
    Application &app = Application::getInstance();
    if (!app.init(800, 600)) {
        std::cerr << "Failed to initialize application" << std::endl;
        return -1;
    }
    app.setKeyCallback(onKeyBoard);
    app.setResizeCallback(onResize);


    GL_CALL(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));
    prepareShader();
    prepareVAOForGLTriangle();

    while (app.update()) {
        glClear(GL_COLOR_BUFFER_BIT);
        render();
    }
    app.destroy();
    return 0;
}

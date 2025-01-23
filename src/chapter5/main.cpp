#define GL_SILENCE_DEPRECATION
#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <iostream>
#include "Application.h"
#include "checkError.h"


void onResize(int width, int height) { 
    glViewport(0, 0, width, height);
 }


void onKeyBoard(int key, int action, int mods) {
    std::cout << "onKey: " << key << " " << action << " " << mods << std::endl;
}

GLuint positionVbo = 0;
GLuint shaderProgram;
void prepareSingleBuffer() {
    // 1. 准备顶点位置数据和颜色数据
    float positions[] = {
            -0.5f, -0.5f, 0.0f, // 左下角
            0.5f,  -0.5f, 0.0f, // 右下角
            0.0f,  0.5f,  0.0f, // 上角
    };


    GL_CALL(glGenBuffers(1, &positionVbo));

    // position填充数据
    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, positionVbo));
    GL_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW));


    // 3. 生成 vao并且绑定
    GLuint vao = 0;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // 4.1 将位置信息加入到 vao 中
    glBindBuffer(GL_ARRAY_BUFFER, positionVbo);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);


    // 5. 解绑 VBO 和 VAO
    glBindVertexArray(0);
}

void prepareShader() {

    const char *vertexShaderSource = R"(
#version 420 core
layout (location = 0) in vec3 aPos;
void main()
{
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}
)";

    const char *fragmentShaderSource = R"(
#version 420 core
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
        char infoLog[512];
        glGetShaderInfoLog(fragment, 512, nullptr, infoLog);
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
        char infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
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
    GL_CALL(glBindVertexArray(positionVbo));
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
    prepareSingleBuffer();

    while (app.update()) {
        render();
    }
    app.destroy();
    return 0;
}

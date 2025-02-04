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

unsigned int VBO, VAO, EBO;
GLuint shaderProgram = 0;


void prepareObjects() {
    float positions[] = {
            0.5f,  0.5f,  0.0f, // 右上角
            0.5f,  -0.5f, 0.0f, // 右下角
            -0.5f, -0.5f, 0.0f, // 左下角
            -0.5f, 0.5f,  0.0f // 左上角
    };
    unsigned int indices[] = {
            // 注意索引从0开始!
            // 此例的索引(0,1,2,3)就是顶点数组vertices的下标，
            // 这样可以由下标代表顶点组合成矩形

            0, 1, 3, // 第一个三角形
            1, 2, 3, // 第二个三角形
    };

    // 生成一个顶点数组对象，一个顶点缓冲对象和一个索引缓冲对象
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // 绑定顶点数组对象
    glBindVertexArray(VAO);
    // 绑定顶点缓冲区
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);

    // 绑定索引缓冲区
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // 启用顶点属性
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);

    // 解绑顶点数组对象 和 缓冲区
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void prepareShader() {

    const char *vertexShaderSource = R"(
#version 410 core
layout (location = 0) in vec3 aPos;
void main()
{
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}
)";

    const char *fragmentShaderSource = R"(
#version 410 core
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
    GL_CALL(glBindVertexArray(VAO));
    //线框模式，可以看出是使用了两个三角形组合成的矩形
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    GL_CALL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0););
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
    prepareObjects();

    while (app.update()) {
        glClear(GL_COLOR_BUFFER_BIT);
        render();
    }
    app.destroy();
    return 0;
}

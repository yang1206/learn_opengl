#define GL_SILENCE_DEPRECATION
#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <iostream>
#include "Application.h"
#include "Shader.h"
#include "checkError.h"


void onResize(int width, int height) {
    std::cout << "onResize: " << width << " " << height << std::endl;
    GL_CALL(glViewport(0, 0, width, height));
}


void onKeyBoard(int key, int action, int mods) {
    std::cout << "onKey: " << key << " " << action << " " << mods << std::endl;
}

unsigned int VBO, VAO;



void prepareVAO() {
    float vertices[] = {
            // positions         // colors
            0.5f,  -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom right
            -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom left
            0.0f,  0.5f,  0.0f, 0.0f, 0.0f, 1.0f // top
    };


    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // 解除绑定vao
    // glBindVertexArray(0);
    // glBindBuffer(GL_ARRAY_BUFFER, 0);
}


void render() {
    // 渲染
    GL_CALL(glClear(GL_COLOR_BUFFER_BIT));
    GL_CALL(glDrawArrays(GL_TRIANGLES, 0, 6););
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


    Shader ourShader("../../shaders/chapter8/vertex.glsl", "../../shaders/chapter8/fragment.glsl");


    prepareVAO();

    while (app.update()) {
        ourShader.use();
        render();
    }
    app.destroy();
    return 0;
}

#define GL_SILENCE_DEPRECATION
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <glad/glad.h>

#include "Application.h"
#include "Shader.h"
#include "checkError.h"
#include <GLFW/glfw3.h>
#include <iostream>

void onResize(int width, int height) {
  std::cout << "onResize: " << width << " " << height << std::endl;
  GL_CALL(glViewport(0, 0, width, height));
}

void onKeyBoard(int key, int action, int mods) {
  std::cout << "onKey: " << key << " " << action << " " << mods << std::endl;
}

unsigned int VBO, VAO, EBO, texture;

void prepareVAO() {
  float vertices[] = {
      //     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
      0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // 右上
      0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // 右下
      -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 左下
      -0.5f, 0.5f,  0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f  // 左上
  };
  unsigned int indices[] = {
      0, 1, 3, // first triangle
      1, 2, 3  // second triangle
  };
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);

  // position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  // color attribute
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  // texture coord attribute
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void *)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);

  // load and create a texture
  // -------------------------
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D,
                texture); // all upcoming GL_TEXTURE_2D operations now have
                          // effect on this texture object
  // set the texture wrapping parameters
  glTexParameteri(
      GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
      GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  // set texture filtering parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  // load image, create texture and generate mipmaps
  int width, height, nrChannels;
  // The FileSystem::getPath(...) is part of the GitHub repository so we can
  // find files on any IDE/platform; replace it with your own image path.
  unsigned char *data = stbi_load("../../assets/textures/wall.jpg", &width,
                                  &height, &nrChannels, 0);
  if (data) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    std::cout << "Failed to load texture" << std::endl;
  }
  stbi_image_free(data);
}

void render(Shader &shader) {

  float timeValue = glfwGetTime();
  float redValue = (sin(timeValue) / 2.0f) + 0.5f;
  float greenValue = (sin(timeValue + 2.0f) / 2.0f) + 0.5f;
  float blueValue = (sin(timeValue + 4.0f) / 2.0f) + 0.5f;

  shader.setVec3("uColor", redValue, greenValue, blueValue);
  // 渲染
  GL_CALL(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));
  GL_CALL(glClear(GL_COLOR_BUFFER_BIT));
  glBindTexture(GL_TEXTURE_2D, texture);
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

  Shader ourShader("../../shaders/chapter9/vertex_tex.glsl",
                   "../../shaders/chapter9/fragment_tex.glsl");

  prepareVAO();

  while (app.update()) {
    ourShader.use();
    render(ourShader);
  }
  app.destroy();
  return 0;
}

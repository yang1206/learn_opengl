#define GL_SILENCE_DEPRECATION
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <glad/glad.h>

#include "Application.h"
#include "ImGuiLayer.h"
#include "Shader.h"
#include "checkError.h"
#include "imgui.h"
#include <GLFW/glfw3.h>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void onResize(int width, int height) {
  GL_CALL(glViewport(0, 0, width, height));
}

void onKeyBoard(int key, int action, int mods) {
  std::cout << "onKey: " << key << " " << action << " " << mods << std::endl;
}

unsigned int VBO, VAO, EBO, texture1, texture2;

void prepare() {
  float vertices[] = {
      // positions          // colors           // texture coords
      0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top right
      0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom right
      -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
      -0.5f, 0.5f,  0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f  // top left
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
}

void loadTexture() {

  // load and create a texture
  // -------------------------
  glGenTextures(1, &texture1);
  glBindTexture(GL_TEXTURE_2D, texture1);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  // load image, create texture and generate mipmaps
  int width, height, nrChannels;
  stbi_set_flip_vertically_on_load(true);
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

  glGenTextures(1, &texture2);
  glBindTexture(GL_TEXTURE_2D, texture2);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  data = stbi_load("../../assets/textures/awesomeface.png", &width, &height,
                   &nrChannels, 0);
  if (data) {
    // note that the awesomeface.png has transparency and thus an alpha channel,
    // so make sure to tell OpenGL the data type is of GL_RGBA
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    std::cout << "Failed to load texture" << std::endl;
  }
  stbi_image_free(data);
}

float mixValue = 0.2f;
float translateX = 0.1f;
float translateY = -0.1f;
float rotateAngle = 0.0f;
float scaleX = 1.0f;
float scaleY = 1.0f;
bool autoRotate = false;

int main() {
  Application &app = Application::getInstance();
  if (!app.init(800, 600)) {
    std::cerr << "Failed to initialize application" << std::endl;
    return -1;
  }
  const GLubyte *version = glGetString(GL_VERSION);
  const GLubyte *vendor = glGetString(GL_VENDOR);
  const GLubyte *renderer = glGetString(GL_RENDERER);
  std::cout << "OpenGL Version: " << version << std::endl;
  std::cout << "Vendor: " << vendor << std::endl;
  std::cout << "Renderer: " << renderer << std::endl;
  ImGuiLayer::init(app.getWindow());

  app.setKeyCallback(onKeyBoard);
  app.setResizeCallback(onResize);

  Shader ourShader("../../shaders/chapter11/vertex.glsl",
                   "../../shaders/chapter11/fragment.glsl");

  prepare();
  loadTexture();

  while (app.update()) {
    // 渲染
    GL_CALL(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));
    GL_CALL(glClear(GL_COLOR_BUFFER_BIT));

    ImGuiLayer::begin();
    // ImGui控制面板
    ImGui::Begin("控制面板");
    ImGui::SliderFloat("纹理可见度", &mixValue, 0.0f, 1.0f);
    // 添加变换控制
    if (ImGui::TreeNode("变换控制")) {
      ImGui::SliderFloat("X平移", &translateX, -1.0f, 1.0f);
      ImGui::SliderFloat("Y平移", &translateY, -1.0f, 1.0f);
      ImGui::SliderFloat("旋转角度", &rotateAngle, 0.0f, 360.0f);
      ImGui::Checkbox("自动旋转", &autoRotate);
      ImGui::SliderFloat("X缩放", &scaleX, 0.1f, 2.0f);
      ImGui::SliderFloat("Y缩放", &scaleY, 0.1f, 2.0f);
      ImGui::TreePop();
    }
    ImGui::End();

    // bind textures on corresponding texture units
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);

    // create transformations
    glm::mat4 transform = glm::mat4(1.0f);
    transform =
        glm::translate(transform, glm::vec3(translateX, translateY, 0.0f));

    // 从中心点进行旋转
    transform = glm::translate(transform, glm::vec3(0.1f, 0.1f, 0.0f));
    if (autoRotate) {
      transform = glm::rotate(transform, (float)glfwGetTime(),
                              glm::vec3(0.0f, 0.0f, 1.0f));
    } else {
      transform = glm::rotate(transform, glm::radians(rotateAngle),
                              glm::vec3(0.0f, 0.0f, 1.0f));
    }
    transform = glm::translate(transform, glm::vec3(-0.1f, -0.1f, 0.0f));

    // 应用缩放
    transform = glm::scale(transform, glm::vec3(scaleX, scaleY, 1.0f));

    ourShader.use();
    // set transformation uniforms
    ourShader.setMat4("transform", transform);
    // either set it manually like so:
    glUniform1i(glGetUniformLocation(ourShader.ID, "texture1"), 0);
    // or set it via the texture class
    ourShader.setInt("texture2", 1);

    ourShader.setFloat("mixValue", mixValue);
    glBindVertexArray(VAO);
    GL_CALL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0););

    // second transformation
    // ---------------------
    transform = glm::mat4(1.0f); // reset it to identity matrix
    transform = glm::translate(transform, glm::vec3(-0.5f, 0.5f, 0.0f));
    float scaleAmount = static_cast<float>(sin(glfwGetTime()));
    transform =
        glm::scale(transform, glm::vec3(scaleAmount, scaleAmount, scaleAmount));
    ourShader.setMat4("transform", transform);

    // now with the uniform matrix being replaced with new transformations, draw
    // it again.
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // 渲染ImGui
    ImGuiLayer::end();
  }
  app.destroy();
  // 清理ImGui
  ImGuiLayer::destroy();
  ImGui::DestroyContext();
  return 0;
}

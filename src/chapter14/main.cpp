#include "imgui_impl_glfw.h"
#define GL_SILENCE_DEPRECATION
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <glad/glad.h>

#include "Application.h"
#include "Camera.h"
#include "ImGuiLayer.h"
#include "Shader.h"
#include "checkError.h"
#include "imgui.h"
#include <GLFW/glfw3.h>
#include <iostream>

float mixValue = 0.2f;
float fov = 45.0f;
float nearPlane = 0.1f;
float farPlane = 100.0f;
bool wireframeMode = false;
glm::vec4 tintColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
glm::vec4 bgColor = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
glm::vec3 translateVec = glm::vec3(0.0f);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// timing
float deltaTime = 0.0f; // time between current frame and last frame
float lastFrame = 0.0f;

bool imguiWantCaptureMouse = false;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

void onResize(int width, int height) {
  GL_CALL(glViewport(0, 0, width, height));
  camera.SetAspectRatio(static_cast<float>(width) / static_cast<float>(height));
}

void processInput(GLFWwindow *window) {
  ImGuiIO &io = ImGui::GetIO();
  // 添加这个检查，当ImGui需要键盘输入时不处理相机移动
  if (io.WantCaptureKeyboard) {
    return;
  }

  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);

  // 使用相机类处理键盘输入
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    camera.ProcessKeyboard(FORWARD, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    camera.ProcessKeyboard(BACKWARD, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    camera.ProcessKeyboard(LEFT, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    camera.ProcessKeyboard(RIGHT, deltaTime);
}

void mouse_callback(GLFWwindow *window, double xposIn, double yposIn) {
  float xpos = static_cast<float>(xposIn);
  float ypos = static_cast<float>(yposIn);
  camera.ProcessMouseMovement(xpos, ypos);
}

void mouse_button_callback(GLFWwindow *window, int button, int action,
                           int mods) {
  ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);
  ImGuiIO &io = ImGui::GetIO();
  if (io.WantCaptureMouse)
    return;

  if (button == GLFW_MOUSE_BUTTON_RIGHT) {
    if (action == GLFW_PRESS) {
      camera.SetMouseControl(true);
      glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
      double xpos, ypos;
      glfwGetCursorPos(window, &xpos, &ypos);
      camera.ResetMouseState(static_cast<float>(xpos),
                             static_cast<float>(ypos));
    } else if (action == GLFW_RELEASE) {
      camera.SetMouseControl(false);
      glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
  }
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
  ImGuiIO &io = ImGui::GetIO();
  if (io.WantCaptureMouse) {
    return; // 如果ImGui需要鼠标，直接返回
  }

  camera.ProcessMouseScroll(static_cast<float>(yoffset));
  fov = camera.GetFov();
}

unsigned int VBO, VAO, texture1, texture2;

void prepare() {

  float vertices[] = {
      -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.5f,  -0.5f, -0.5f, 1.0f, 0.0f,
      0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
      -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

      -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, 0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,
      0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
      -0.5f, 0.5f,  0.5f,  0.0f, 1.0f, -0.5f, -0.5f, 0.5f,  0.0f, 0.0f,

      -0.5f, 0.5f,  0.5f,  1.0f, 0.0f, -0.5f, 0.5f,  -0.5f, 1.0f, 1.0f,
      -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
      -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, -0.5f, 0.5f,  0.5f,  1.0f, 0.0f,

      0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
      0.5f,  -0.5f, -0.5f, 0.0f, 1.0f, 0.5f,  -0.5f, -0.5f, 0.0f, 1.0f,
      0.5f,  -0.5f, 0.5f,  0.0f, 0.0f, 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

      -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.5f,  -0.5f, -0.5f, 1.0f, 1.0f,
      0.5f,  -0.5f, 0.5f,  1.0f, 0.0f, 0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,
      -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

      -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
      0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
      -0.5f, 0.5f,  0.5f,  0.0f, 0.0f, -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f};

  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  // texture coord attribute
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
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

glm::vec3 cubePositions[] = {
    glm::vec3(0.0f, 0.0f, 0.0f),    glm::vec3(2.0f, 5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f), glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3(2.4f, -0.4f, -3.5f),  glm::vec3(-1.7f, 3.0f, -7.5f),
    glm::vec3(1.3f, -2.0f, -2.5f),  glm::vec3(1.5f, 2.0f, -2.5f),
    glm::vec3(1.5f, 0.2f, -1.5f),   glm::vec3(-1.3f, 1.0f, -1.5f)};

int main() {
  Application &app = Application::getInstance();
  if (!app.init(SCR_WIDTH, SCR_HEIGHT, "Chapter14控制相机")) {
    std::cerr << "Failed to initialize application" << std::endl;
    return -1;
  }
  ImGuiLayer::init(app.getWindow());
  ImGui::GetIO().ConfigFlags |=
      ImGuiConfigFlags_NavEnableKeyboard; // 启用键盘导航

  // 开启opengl深度测试 解决立方体重的叠问题 z-fighting
  glEnable(GL_DEPTH_TEST);
  Shader ourShader("../../shaders/chapter12/vertex.glsl",
                   "../../shaders/chapter12/fragment.glsl");

  prepare();
  loadTexture();

  app.setResizeCallback(onResize);

  glfwSetCursorPosCallback(app.getWindow(), mouse_callback);
  glfwSetScrollCallback(app.getWindow(), scroll_callback);
  glfwSetMouseButtonCallback(app.getWindow(), mouse_button_callback);

  camera.SetAspectRatio(static_cast<float>(SCR_WIDTH) /
                        static_cast<float>(SCR_HEIGHT));
  camera.SetNearPlane(nearPlane);
  camera.SetFarPlane(farPlane);
  camera.SetFov(fov);

  while (app.update()) {
    float currentFrame = static_cast<float>(glfwGetTime());
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    ImGuiLayer::begin();

    processInput(app.getWindow());

    GL_CALL(glClearColor(bgColor.r, bgColor.g, bgColor.b, bgColor.a));
    GL_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

    // 设置线框模式
    if (wireframeMode) {
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    } else {
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    // 1. 先激活着色器
    ourShader.use();
    ourShader.setVec4("tintColor", tintColor);
    // 2. 设置纹理
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);
    // 3. 设置uniform变量
    ourShader.setInt("texture1", 0);
    ourShader.setInt("texture2", 1);
    ourShader.setFloat("mixValue", mixValue);

    // 4. 设置变换矩阵

    // 5. 使用Shader类的方法设置矩阵
    // camera/view transformation
    glm::mat4 view = camera.GetViewMatrix();
    glm::mat4 projection = camera.GetProjectionMatrix();
    ourShader.setMat4("view", view);
    ourShader.setMat4("projection", projection);
    // 6. 绘制
    glBindVertexArray(VAO);
    for (unsigned int i = 0; i < 10; i++) {
      glm::mat4 model = glm::mat4(
          1.0f); // make sure to initialize matrix to identity matrix first
      model = glm::translate(model, cubePositions[i] + translateVec);
      float angle = 20.0f * i;
      model =
          glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));

      ourShader.setMat4("model", model);

      glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    // 左上角窗口
    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_Once);
    ImGui::SetNextWindowSize(ImVec2(200, 150), ImGuiCond_Once);
    ImGui::Begin("Render Settings");
    ImGui::Checkbox("Wire Mode", &wireframeMode);
    ImGui::ColorEdit4("Tint", &tintColor.x);
    ImGui::ColorEdit4("Background", &bgColor.x);
    ImGui::End();

    ImGui::Begin("Camera Settings");
    static float moveSpeed = SPEED;
    static float sensitivity = SENSITIVITY;
    if (ImGui::SliderFloat("Move Speed", &moveSpeed, 0.1f, 10.0f)) {
      camera.SetMovementSpeed(moveSpeed);
    }
    if (ImGui::SliderFloat("Mouse Sensitivity", &sensitivity, 0.01f, 1.0f)) {
      camera.SetMouseSensitivity(sensitivity);
    }

    // 显示相机位置信息
    glm::vec3 pos = camera.GetPosition();
    ImGui::Text("Position: (%.2f, %.2f, %.2f)", pos.x, pos.y, pos.z);
    ImGui::End();

    // ImGui控制面板
    ImGui::SetNextWindowPos(ImVec2(10, SCR_HEIGHT - 160), ImGuiCond_Once);
    ImGui::SetNextWindowSize(ImVec2(200, 150), ImGuiCond_Once);
    ImGui::Begin("Transform");
    ImGui::SliderFloat3("Translate", &translateVec.x, -3.0f, 3.0f);
    if (ImGui::SliderFloat("FOV", &fov, 1.0f, 90.0f)) {
      camera.SetFov(fov); // 当 FOV 改变时更新相机
    }
    ImGui::SliderFloat("纹理可见度", &mixValue, 0.0f, 1.0f);
    ImGui::End();
    // 渲染ImGui
    ImGuiLayer::end();
  }
  app.destroy();
  // 清理ImGui
  ImGuiLayer::destroy();
  ImGui::DestroyContext();
  return 0;
}

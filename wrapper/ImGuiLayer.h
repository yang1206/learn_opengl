#pragma once
#include <glad/glad.h>

#include <GLFW/glfw3.h>

class ImGuiLayer {
public:
  static void init(GLFWwindow *window);
  static void destroy();
  static void begin();
  static void end();
};

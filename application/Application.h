#pragma once
#include <cstdint>

class GLFWwindow;

using ResizeCallback = void (*)(int width, int height);
using KeyCallback = void (*)(int key, int action, int mods);

class Application {

private:
  static void frameBufferSizeCallback(GLFWwindow *window, int width,
                                      int height);
  static void keyCallback(GLFWwindow *window, int key, int scancode, int action,
                          int mods);

public:
  ~Application();

  static Application &getInstance();

  uint32_t getWidth() const { return mWidth; }
  uint32_t getHeight() const { return mHeight; }

  bool init(const int width = 800, const int height = 600,
            const char *title = "OpenGL 学习");
  bool update();
  void destroy();
  void setResizeCallback(ResizeCallback callback);
  void setKeyCallback(KeyCallback callback);

  GLFWwindow *getWindow() const { return mWindow; }

private:
  static Application *instance;
  Application();

  uint32_t mWidth{0};
  uint32_t mHeight{0};

  GLFWwindow *mWindow{nullptr};

  ResizeCallback mResizeCallback{nullptr};
  KeyCallback mKeyCallback{nullptr};
};

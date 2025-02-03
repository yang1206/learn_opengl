#include "ImGuiLayer.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <cstdio>

void ImGuiLayer::init(GLFWwindow *window) {
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();

  // 配置ImGui风格
  ImGui::StyleColorsDark();
  ImGuiStyle &style = ImGui::GetStyle();

  // 获取显示器缩放比例
  float xscale, yscale;
  glfwGetWindowContentScale(window, &xscale, &yscale);

  // 设置基础字体大小和窗口最小尺寸
  float baseFontSize = 12.0f; // 基础字体大小
  style.WindowMinSize = ImVec2(100.0f, 100.0f);

  // 加载字体
  const char *fontPath = "../../assets/fonts/SourceHanSansCN-Normal.otf";
  FILE *file = fopen(fontPath, "rb");
  if (file) {
    fclose(file);
    // 字体大小已经考虑DPI缩放
    io.Fonts->AddFontFromFileTTF(fontPath, baseFontSize * xscale, nullptr,
                                 io.Fonts->GetGlyphRangesChineseFull());
  }

  // 确保至少有一个字体可用
  if (io.Fonts->Fonts.empty()) {
    io.Fonts->AddFontDefault();
  }

  // 初始化平台/渲染器绑定
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init("#version 410");
}

void ImGuiLayer::begin() {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
}

void ImGuiLayer::end() {
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ImGuiLayer::destroy() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}

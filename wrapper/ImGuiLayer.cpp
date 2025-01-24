#include "ImGuiLayer.h"
#include <cstdio>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

void ImGuiLayer::init(GLFWwindow *window) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void) io;

    // 配置ImGui风格
    ImGui::StyleColorsDark();
    // 或者使用浅色主题
    // ImGui::StyleColorsLight();

    // 设置中文字体
    loadChineseFont();

    // 初始化平台/渲染器绑定
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
}

void ImGuiLayer::loadChineseFont() {
    ImGuiIO &io = ImGui::GetIO();
    // 尝试加载中文字体
    const char *fontPath = "../../assets/fonts/SourceHanSansCN-Normal.otf";
    FILE *file = fopen(fontPath, "rb");
    if (file) {
        fclose(file);
        // 字体文件存在，加载中文字体
        io.Fonts->AddFontFromFileTTF(fontPath, 16.0f, nullptr, io.Fonts->GetGlyphRangesChineseFull());
    }

    // 确保至少有一个字体可用（如果中文字体加载失败，使用默认字体）
    if (io.Fonts->Fonts.empty()) {
        io.Fonts->AddFontDefault();
    }
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

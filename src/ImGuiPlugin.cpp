//
// Created by vini84200 on 6/1/23.
//

#include "ImGuiPlugin.h"
#include "Window.h"
#include "GLFW/glfw3.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

ImGuiPlugin::ImGuiPlugin() {

}

void ImGuiPlugin::init() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void) io;

    ImGui::StyleColorsDark();

    GLFWwindow *window = glfwGetCurrentContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
}

void ImGuiPlugin::render() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    if (demo_) {
        ImGui::ShowDemoWindow(&demo_);
    }

    // Window GUI
    if (window_ && window_->show_gui_) {
        window_->renderImGuiMainWindow();
    }


    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(glfwGetCurrentContext(), &display_w, &display_h);
//    glViewport(0, 0, display_w, display_h);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ImGuiPlugin::shutdown() {

}

void ImGuiPlugin::setWindow(Window *window) {
    window_ = window;
}

void ImGuiPlugin::update(float dt) {

}

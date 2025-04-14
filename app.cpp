#include "App.h"
#include "NodeUI.h"
#include <vector>
#include <string>
#include "OutputNode.h"
#include <opencv2/opencv.hpp>
#include "BaseNode.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <iostream>
#include "ImageInputNode.h"
GLFWwindow* window;
NodeUIManager uiManager;
ImageInputNode inputNode(1);


OutputNode outputNode(2); // ID = 2
cv::Mat testImage;

// Sample node list (for testing visual layout)

std::vector<BaseNode*> nodes = {
     &inputNode,
    &outputNode  // add more nodes as needed
};

void SetupTestConnections() {
    uiManager.connections = {
        {1, 2}  // Connect Image Input → Output Node
    };
}
bool App::Init()
{
    if (!glfwInit())
        return false;

    // OpenGL settings
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(1280, 720, "Node-Based Image Editor", NULL, NULL);
    if (!window) return false;

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW\n";
        return false;
    }

    // ImGui setup
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    // ⬇️ Add this after ImGui setup
    inputNode.SetImagePath("D:\\55.jpg");
    outputNode.SetInput(inputNode.outputImage);

    SetupTestConnections(); // keep this at the end

    return true;
}


void App::Run()
{
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // 🌟 Toolbar
        ImGui::Begin("Toolbar");
        if (ImGui::Button("Open Image")) { /* TODO */ }
        ImGui::SameLine();
        if (ImGui::Button("Save Image")) { /* TODO */ }
        ImGui::End();

        // 🎨 Canvas
        ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize); // Make canvas fullscreen
        ImGui::SetNextWindowPos(ImVec2(0, 0));

        ImGui::Begin("Canvas", nullptr,
            ImGuiWindowFlags_NoCollapse |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoScrollbar |
            ImGuiWindowFlags_NoBringToFrontOnFocus);

        ImDrawList* drawList = ImGui::GetWindowDrawList();

        // Draw nodes
        for (BaseNode* node : nodes) {
            uiManager.RenderNode(*node);
        }

        uiManager.RenderConnections();
        // Render image preview from OutputNode
        



        ImGui::End();



        // ⚙️ Properties
        ImGui::Begin("Properties");
        ImGui::Text("Adjust node parameters here.");
        ImGui::End();

        // Render
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.2f, 0.2f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }
}

void App::Cleanup()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
}

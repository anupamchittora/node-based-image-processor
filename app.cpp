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
#include "NodeGraph.h"
#include "filters/GrayscaleNode.h"
#include "filters/BrightnessContrastNode.h"
#include "filters/ColorChannelSplitterNode.h"
#include "filters/BlurNode.h"
#include "filters/ThresholdNode.h"

GrayscaleNode grayNode(3);
ImageInputNode inputNode(1);
OutputNode outputNode(2);
NodeGraph graph;
GLFWwindow* window;
NodeUIManager uiManager;
BrightnessContrastNode bcNode(4);
ColorChannelSplitterNode channelNode(5);  // Use unique ID
BlurNode blurNode(6);
ThresholdNode thresholdNode(7); // Use a unique ID

std::vector<BaseNode*> nodes = {
    &inputNode,
    &grayNode,
    &bcNode,
    &channelNode,
    &blurNode,
    &thresholdNode,
    &outputNode
};

bool App::Init()
{
    if (!glfwInit())
        return false;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(1280, 720, "Node-Based Image Editor", NULL, NULL);
    if (!window) return false;

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW\n";
        return false;
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    // NodeGraph setup
    graph.AddNode(&inputNode);
    graph.AddNode(&grayNode);
    graph.AddNode(&outputNode);
    graph.AddNode(&bcNode);
    graph.AddNode(&channelNode);
    graph.AddNode(&blurNode);
    graph.AddNode(&thresholdNode);

    // Load image
    inputNode.SetImagePath("D:\\55.jpg");

    // Clear any default UI connections
    uiManager.connections.clear();
    graph.connections.clear();

    graph.ProcessAll();

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

        ImGui::Begin("Toolbar");
        if (ImGui::Button("Open Image")) { /* TODO */ }
        ImGui::SameLine();
        if (ImGui::Button("Save Image")) { /* TODO */ }
        ImGui::End();

        ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
        ImGui::SetNextWindowPos(ImVec2(0, 0));

        ImGui::Begin("Canvas", nullptr,
            ImGuiWindowFlags_NoCollapse |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoScrollbar |
            ImGuiWindowFlags_NoBringToFrontOnFocus);

        ImDrawList* drawList = ImGui::GetWindowDrawList();

        for (BaseNode* node : nodes) {
            uiManager.RenderNode(*node);
        }

        uiManager.RenderConnections();

        ImGui::End();

        ImGui::Begin("Properties");
        ImGui::Text("Adjust node parameters here.");
        ImGui::End();

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
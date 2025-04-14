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
#include "filters/EdgeDetectionNode.h"
#include "filters/BlendNode.h"
#include "filters/NoiseNode.h"
#include "filters/ConvolutionNode.h"

GrayscaleNode grayNode(3);
ImageInputNode inputNode(1);
OutputNode outputNode(2);
NodeGraph graph;
GLFWwindow* window;
NodeUIManager uiManager;
BrightnessContrastNode bcNode(4);
ColorChannelSplitterNode channelNode(5);
BlurNode blurNode(6);
ThresholdNode thresholdNode(7);
EdgeDetectionNode edgeNode(8);
BlendNode blendNode(9);
NoiseNode noiseNode(10);
ConvolutionNode convNode(11);

std::vector<BaseNode*> nodes = {
    &inputNode,
    &grayNode,
    &bcNode,
    &channelNode,
    &blurNode,
    &thresholdNode,
    &edgeNode,
    &blendNode,
    &noiseNode,
    &convNode,
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

    graph.AddNode(&inputNode);
    graph.AddNode(&grayNode);
    graph.AddNode(&outputNode);
    graph.AddNode(&bcNode);
    graph.AddNode(&channelNode);
    graph.AddNode(&blurNode);
    graph.AddNode(&thresholdNode);
    graph.AddNode(&edgeNode);
    graph.AddNode(&blendNode);
    graph.AddNode(&noiseNode);
    graph.AddNode(&convNode);

    inputNode.SetImagePath("D:\\55.jpg");

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
            ImGuiWindowFlags_NoBringToFrontOnFocus);

        ImGui::BeginChild("NodeCanvas", ImVec2(0, 0), false,
            ImGuiWindowFlags_AlwaysVerticalScrollbar | ImGuiWindowFlags_AlwaysHorizontalScrollbar);

        // 🖱️ Optional: right-click pan
        if (ImGui::IsWindowHovered() && ImGui::IsMouseDragging(ImGuiMouseButton_Right)) {
            ImVec2 delta = ImGui::GetIO().MouseDelta;
            for (auto& [id, state] : uiManager.nodeStates) {
                state.position.x += delta.x;
                state.position.y += delta.y;
            }
        }

        ImDrawList* drawList = ImGui::GetWindowDrawList();

        for (BaseNode* node : nodes) {
            uiManager.RenderNode(*node);
        }

        uiManager.RenderConnections();

        // ✅ Simulate large canvas for scroll
        ImGui::SetCursorScreenPos(ImVec2(3000, 2000));
        ImGui::InvisibleButton("dummy_scroll", ImVec2(1, 1));


        ImGui::EndChild();
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

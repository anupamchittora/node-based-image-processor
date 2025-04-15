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
#include "NodeFactory.h"

#include "tinyfiledialogs/tinyfiledialogs.h"
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
static int nextID = 1000;

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

    //inputNode.SetImagePath("D:\\55.jpg");

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
        ImGui::SetNextWindowSize(ImVec2(250, 100), ImGuiCond_Always);
        ImGui::Begin("Toolbox", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

        static const char* nodeTypes[] = {
            "Image Input", "Output Node", "Grayscale", "Brightness/Contrast",
            "Channel Splitter", "Gaussian Blur", "Threshold",
            "Edge Detection", "Blend", "Noise Generator"
        };

        static int selected = 0;
        ImGui::Combo("Node Type", &selected, nodeTypes, IM_ARRAYSIZE(nodeTypes));

        if (ImGui::Button("Add Node")) {
            uiManager.SpawnNode(nodeTypes[selected]);  // let SpawnNode handle it

        }

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
        ImGui::SetNextWindowSize(ImVec2(300, 200), ImGuiCond_Always);
        ImGui::Begin("Properties", nullptr, ImGuiWindowFlags_AlwaysAutoResize);


        ImGui::Text("Adjust node parameters here.");

        int selectedID = uiManager.selectedNodeID;
        if (selectedID != -1 && graph.nodes.count(selectedID)) {
            BaseNode* node = graph.nodes[selectedID];

            if (BrightnessContrastNode* bc = dynamic_cast<BrightnessContrastNode*>(node)) {
                ImGui::Text("Brightness/Contrast Settings");
                ImGui::SliderFloat("Brightness", &bc->brightness, -100.0f, 100.0f);
                ImGui::SliderFloat("Contrast", &bc->contrast, 0.0f, 3.0f);
                if (ImGui::Button("Reset Brightness")) bc->brightness = 0.0f;
                if (ImGui::Button("Reset Contrast")) bc->contrast = 1.0f;
                bc->Process(); graph.ProcessAll();
            }

            if (BlurNode* blur = dynamic_cast<BlurNode*>(node)) {
                ImGui::Text("Gaussian Blur Settings");
                if (ImGui::SliderInt("Radius", &blur->radius, 1, 20)) {
                    blur->Process(); graph.ProcessAll();
                }
            }

            if (ThresholdNode* th = dynamic_cast<ThresholdNode*>(node)) {
                ImGui::Text("Threshold Settings");
                if (ImGui::SliderInt("Threshold", &th->thresholdValue, 0, 255)) {
                    th->Process(); graph.ProcessAll();
                }
            }

            if (EdgeDetectionNode* edge = dynamic_cast<EdgeDetectionNode*>(node)) {
                ImGui::Text("Edge Detection Settings");
                const char* modes[] = { "Sobel", "Canny" };
                int current = (edge->mode == EdgeMode::Sobel) ? 0 : 1;
                if (ImGui::Combo("Mode", &current, modes, IM_ARRAYSIZE(modes))) {
                    edge->mode = (current == 0) ? EdgeMode::Sobel : EdgeMode::Canny;
                    edge->Process(); graph.ProcessAll();
                }

                if (edge->mode == EdgeMode::Sobel) {
                    if (ImGui::SliderInt("Kernel Size", &edge->kernelSize, 1, 7)) {
                        if (edge->kernelSize % 2 == 0) edge->kernelSize += 1;
                        edge->Process(); graph.ProcessAll();
                    }
                }
                else {
                    if (ImGui::SliderInt("Canny Thresh 1", &edge->cannyThreshold1, 0, 255)) {
                        edge->Process(); graph.ProcessAll();
                    }
                    if (ImGui::SliderInt("Canny Thresh 2", &edge->cannyThreshold2, 0, 255)) {
                        edge->Process(); graph.ProcessAll();
                    }
                }
            }

            if (BlendNode* blend = dynamic_cast<BlendNode*>(node)) {
                ImGui::Text("Blend Settings");
                const char* blendModes[] = { "Normal", "Multiply", "Screen", "Overlay", "Difference" };
                int current = static_cast<int>(blend->mode);
                if (ImGui::Combo("Blend Mode", &current, blendModes, IM_ARRAYSIZE(blendModes))) {
                    blend->mode = static_cast<BlendMode>(current);
                    blend->Process(); graph.ProcessAll();
                }

                if (ImGui::SliderFloat("Opacity", &blend->opacity, 0.0f, 1.0f)) {
                    blend->Process(); graph.ProcessAll();
                }
            }

            if (NoiseNode* noise = dynamic_cast<NoiseNode*>(node)) {
                ImGui::Text("Noise Generator Settings");
                ImGui::SliderInt("Width", &noise->width, 64, 1024);
                ImGui::SliderInt("Height", &noise->height, 64, 1024);
                ImGui::SliderFloat("Scale", &noise->scale, 1.0f, 100.0f);
                ImGui::SliderInt("Seed", &noise->seed, 0, 1000);

                if (ImGui::Button("Generate")) {
                    noise->Process();
                    graph.ProcessAll();
                }
            }

            if (ConvolutionNode* conv = dynamic_cast<ConvolutionNode*>(node)) {
                ImGui::Text("Convolution Filter");
                const char* presets[] = { "Sharpen", "Edge", "Emboss" };
                static int selectedPreset = 0;
                if (ImGui::Combo("Preset", &selectedPreset, presets, IM_ARRAYSIZE(presets))) {
                    conv->SetPreset(presets[selectedPreset]);
                    conv->Process(); graph.ProcessAll();
                }

                ImGui::Text("Kernel:");
                for (int i = 0; i < conv->kernel.size(); ++i) {
                    if (i % conv->kernelSize != 0) ImGui::SameLine();
                    ImGui::PushItemWidth(40);
                    if (ImGui::InputFloat(("##k" + std::to_string(i)).c_str(), &conv->kernel[i], 0.1f, 1.0f, "%.1f")) {
                        conv->Process(); graph.ProcessAll();
                    }
                    ImGui::PopItemWidth();
                }
            }
        }


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

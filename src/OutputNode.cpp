#include "OutputNode.h"
#include <GL/glew.h>
#include <iostream>

OutputNode::OutputNode(int id)
    : BaseNode(id, "Output Node") {
}

void OutputNode::SetInput(const cv::Mat& image) {
    inputImage = image;
    Process();
    std::cout << "[OutputNode] Received image via connection!\n";

}

void OutputNode::Process() {
    if (inputImage.empty()) return;
    CreateTexture();
}

void OutputNode::CreateTexture() {
    FreeTexture();

    if (inputImage.empty()) return;

    cv::Mat imageRGBA;

    if (inputImage.channels() == 1) {
        cv::cvtColor(inputImage, imageRGBA, cv::COLOR_GRAY2RGBA);
    }
    else if (inputImage.channels() == 3) {
        cv::cvtColor(inputImage, imageRGBA, cv::COLOR_BGR2RGBA);
    }
    else if (inputImage.channels() == 4) {
        imageRGBA = inputImage.clone(); // already good
    }
    else {
        std::cerr << "Unsupported image format!\n";
        return;
    }

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageRGBA.cols, imageRGBA.rows, 0,
        GL_RGBA, GL_UNSIGNED_BYTE, imageRGBA.data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
}


void OutputNode::FreeTexture() {
    if (textureID) {
        glDeleteTextures(1, &textureID);
        textureID = 0;
    }
}

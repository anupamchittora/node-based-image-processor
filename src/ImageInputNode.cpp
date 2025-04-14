// ImageInputNode.cpp
#include "ImageInputNode.h"
#include <iostream>
#include <GL/glew.h>
#include <opencv2/opencv.hpp>
ImageInputNode::ImageInputNode(int id)
    : BaseNode(id, "Image Input") {
}

void ImageInputNode::SetImagePath(const std::string& path) {
    imagePath = path;
    outputImage = cv::imread(path);

    if (outputImage.empty()) {
        std::cerr << "[ImageInputNode] Failed to load image: " << path << std::endl;
        return;
    }

    std::cout << "[ImageInputNode] Loaded image: " << path << "\n";
    CreateTexture();  // 
}

void ImageInputNode::Process() {
    outputImage = cv::imread(imagePath);
    if (outputImage.empty()) {
        std::cerr << "[ImageInputNode] Failed to load image: " << imagePath << std::endl;
    }
    else {
        std::cout << "[ImageInputNode] Loaded image: " << imagePath << std::endl;
    }
}
void ImageInputNode::CreateTexture() {
    if (textureID)
        glDeleteTextures(1, &textureID);

    GLenum format, internalFormat;

    if (outputImage.channels() == 1) {
        format = GL_RED;
        internalFormat = GL_RED;
    }
    else if (outputImage.channels() == 3) {
        format = GL_BGR;
        internalFormat = GL_RGB;
    }
    else if (outputImage.channels() == 4) {
        format = GL_BGRA;
        internalFormat = GL_RGBA;
    }
    else {
        std::cerr << "Unsupported channel count: " << outputImage.channels() << "\n";
        return;
    }

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); 

    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, outputImage.cols, outputImage.rows, 0, format, GL_UNSIGNED_BYTE, outputImage.data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
}



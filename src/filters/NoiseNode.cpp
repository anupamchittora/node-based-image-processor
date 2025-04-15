#include "filters/NoiseNode.h"
#include <opencv2/opencv.hpp>
#include <random>
#include <iostream>
#include <GL/glew.h>

NoiseNode::NoiseNode(int id)
    : BaseNode(id, "Noise Generator") {
}

void NoiseNode::Process() {
    outputImage = cv::Mat(height, width, CV_8UC1);

    std::mt19937 rng(seed);
    std::uniform_int_distribution<int> dist(0, 255);

    for (int y = 0; y < outputImage.rows; ++y) {
        for (int x = 0; x < outputImage.cols; ++x) {
            outputImage.at<uchar>(y, x) = static_cast<uchar>(dist(rng));
        }
    }

    std::cout << "[NoiseNode] Generated " << width << "x" << height << " noise with seed " << seed << "\n";

    CreateTexture();
}

void NoiseNode::CreateTexture() {
    if (textureID)
        glDeleteTextures(1, &textureID);

    if (outputImage.empty()) return;

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, outputImage.cols, outputImage.rows, 0,
        GL_RED, GL_UNSIGNED_BYTE, outputImage.data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
}

NoiseNode::~NoiseNode() {
    if (textureID)
        glDeleteTextures(1, &textureID);
}

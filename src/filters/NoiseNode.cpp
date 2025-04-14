#include "filters/NoiseNode.h"
#include <opencv2/core.hpp>
#include <random>
#include <iostream>

NoiseNode::NoiseNode(int id)
    : BaseNode(id, "Noise Generator") {
}

cv::Mat NoiseNode::GenerateNoise() {
    cv::Mat noise(height, width, CV_8UC1);

    std::mt19937 rng(seed);
    std::uniform_int_distribution<int> dist(0, 255);

    for (int y = 0; y < height; ++y)
        for (int x = 0; x < width; ++x)
            noise.at<uchar>(y, x) = static_cast<uchar>(dist(rng));

    return noise;
}

void NoiseNode::Process() {
    outputImage = GenerateNoise();
    std::cout << "[NoiseNode] Generated procedural noise " << width << "x" << height << "\n";
}

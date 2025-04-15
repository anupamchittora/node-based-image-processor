#pragma once
#include <string>
#include <vector>
#include <map>
#include <opencv2/opencv.hpp>

class BaseNode {
public:
    int id;
    std::string name;
    std::map<std::string, float> floatParams;
    std::vector<int> inputIDs;
    std::vector<int> outputIDs;

    BaseNode(int id, const std::string& name);
    virtual ~BaseNode() {}
    virtual cv::Mat GetOutput() const { return cv::Mat(); }
    virtual void SetInput(const cv::Mat&) {
        std::cerr << "[" << name << "] SetInput not implemented.\n";
    }

    virtual void Process() = 0;
};

#pragma once
#include <string>
#include <vector>
#include <map>

class BaseNode {
public:
    int id;
    std::string name;
    std::map<std::string, float> floatParams;
    std::vector<int> inputIDs;
    std::vector<int> outputIDs;

    BaseNode(int id, const std::string& name);
    virtual ~BaseNode() {}

    virtual void Process() = 0;
};

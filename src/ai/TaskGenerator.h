#pragma once
#include "../core/Task.h"
#include <vector>
#include <string>

class TaskGenerator {
public:
    TaskGenerator(int rackW,int rackH,int floorW,int floorH);
    // create tasks from a high-level string (mock LLM)
    std::vector<Task> generate(const std::string& hl, int &nextId);
private:
    int rackW, rackH, floorW, floorH;
};

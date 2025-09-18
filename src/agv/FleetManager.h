#pragma once
#include "AGV.h"
#include "PathPlanner.h"
#include "../core/Task.h"
#include <vector>
#include <mutex>

class FleetManager {
public:
    FleetManager(int gridW, int gridH, int agvCount);
    bool assignTaskToFleet(const Task& t); // returns true if accepted
    void update();
    const std::vector<AGV>& getAGVs() const;
    Grid getGrid() const; // grid used for planning (0 free, 1 obstacle)
    void setObstacles(const Grid& g);
private:
    std::vector<AGV> agvs;
    PathPlanner planner;
    Grid grid; // planner grid
    std::mutex mtx;
};

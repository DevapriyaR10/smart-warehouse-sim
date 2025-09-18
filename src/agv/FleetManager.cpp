#include "FleetManager.h"
#include "../core/Utils.h"

FleetManager::FleetManager(int gridW, int gridH, int agvCount)
    : planner(gridW, gridH), grid(gridH, std::vector<int>(gridW, 0)) {
    for (int i=0;i<agvCount;++i) {
        // place AGVs at bottom-left area
        agvs.emplace_back(i, 0, gridH-1 - i%2);
    }
}

bool FleetManager::assignTaskToFleet(const Task& t) {
    std::lock_guard<std::mutex> g(mtx);
    // find idle AGV
    for (auto &agv : agvs) {
        if (agv.getState() == AGVState::AGV_IDLE) {
            // find path from agv pos to t.x,t.y then to t.x2,t.y2 (concatenate)
            Grid useGrid = grid;
            auto path1 = planner.findPath(useGrid, agv.getX(), agv.getY(), t.x, t.y);
            if (path1.empty()) continue;
            auto path2 = planner.findPath(useGrid, t.x, t.y, t.x2, t.y2);
            if (path2.empty()) continue;
            // merge (skip duplicate point)
            std::vector<std::pair<int,int>> full = path1;
            if (!path2.empty()) {
                full.insert(full.end(), path2.begin(), path2.end());
            }
            agv.assignTask(t, full);
            return true;
        }
    }
    return false;
}

void FleetManager::update() {
    std::lock_guard<std::mutex> g(mtx);
    for (auto &agv : agvs) agv.update();
}

const std::vector<AGV>& FleetManager::getAGVs() const { return agvs; }

FleetManager::Grid FleetManager::getGrid() const { return grid; }

void FleetManager::setObstacles(const Grid& g) {
    std::lock_guard<std::mutex> g2(mtx);
    grid = g;
}

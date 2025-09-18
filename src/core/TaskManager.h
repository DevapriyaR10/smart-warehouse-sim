#pragma once
#include "../core/Task.h"
#include "../asrs/Crane.h"
#include "../asrs/Rack.h"
#include "../agv/FleetManager.h"
#include <vector>
#include <mutex>

class TaskManager {
public:
    TaskManager(Crane* crane, Rack* rack, FleetManager* fleet);
    void pushTask(const Task& t);
    void update();
    int pendingCount() const;
private:
    Crane* crane;
    Rack* rack;
    FleetManager* fleet;
    std::vector<Task> asrsQueue; // tasks for crane
    std::vector<Task> agvQueue;  // tasks for AGVs
    mutable std::mutex mtx;
    int nextAssignId = 0;
};

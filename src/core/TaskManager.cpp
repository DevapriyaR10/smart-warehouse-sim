#include "TaskManager.h"
#include "../core/Utils.h"
#include <algorithm>
#include <optional>

TaskManager::TaskManager(Crane* crane_, Rack* rack_, FleetManager* fleet_)
    : crane(crane_), rack(rack_), fleet(fleet_) {}

void TaskManager::pushTask(const Task& t) {
    std::lock_guard<std::mutex> g(mtx);
    if (t.kind == TaskKind::ASRS_STORE || t.kind == TaskKind::ASRS_RETRIEVE) {
        asrsQueue.push_back(t);
        log_info("TaskManager: pushed ASRS task id=" + std::to_string(t.id));
    } else if (t.kind == TaskKind::AGV_MOVE) {
        agvQueue.push_back(t);
        log_info("TaskManager: pushed AGV task id=" + std::to_string(t.id));
    } else {
        log_warn("TaskManager: pushed unknown task type");
    }
}

int TaskManager::pendingCount() const {
    std::lock_guard<std::mutex> g(mtx);
    return (int)asrsQueue.size() + (int)agvQueue.size();
}

void TaskManager::update() {
    // assign ASRS tasks if crane idle
    if (crane && !crane->activeTask()) {
        std::lock_guard<std::mutex> g(mtx);
        if (!asrsQueue.empty()) {
            Task t = asrsQueue.front();
            asrsQueue.erase(asrsQueue.begin());
            // Convert to Crane's Task object and assign
            // Crane expects Task (we reuse Task struct differences)
            // For simplicity pass through
            crane->assignTask(t);
        }
    }

    // assign AGV tasks to fleet
    if (fleet) {
        std::lock_guard<std::mutex> g(mtx);
        if (!agvQueue.empty()) {
            // give first to fleet; FleetManager will pop/claim
            Task t = agvQueue.front();
            bool accepted = fleet->assignTaskToFleet(t);
            if (accepted) agvQueue.erase(agvQueue.begin());
        }
    }
}

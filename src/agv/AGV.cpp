#include "AGV.h"
#include "../core/Utils.h"

AGV::AGV(int id_, int sx, int sy) : id_(id_), x(sx), y(sy), state(AGVState::AGV_IDLE), pathIdx(0) {}

int AGV::id() const { return id_; }
int AGV::getX() const { return x; }
int AGV::getY() const { return y; }
AGVState AGV::getState() const { return state; }

void AGV::assignTask(const Task& t, const std::vector<std::pair<int,int>>& p) {
    if (state != AGVState::AGV_IDLE) {
        log_warn("AGV busy");
        return;
    }
    current = t;
    path = p;
    pathIdx = 0;
    if (!path.empty()) state = AGVState::AGV_NAVIGATING;
    log_info("AGV " + std::to_string(id_) + " assigned task id=" + std::to_string(t.id));
}

void AGV::update() {
    if (state == AGVState::AGV_NAVIGATING) {
        if (pathIdx < path.size()) {
            x = path[pathIdx].first;
            y = path[pathIdx].second;
            ++pathIdx;
        } else {
            // reached target
            state = AGVState::AGV_DELIVERING;
            log_info("AGV " + std::to_string(id_) + " arrived for task id=" + std::to_string(current.id));
        }
    } else if (state == AGVState::AGV_DELIVERING) {
        // simulate drop/pick time, then become idle
        state = AGVState::AGV_IDLE;
        log_info("AGV " + std::to_string(id_) + " completed task id=" + std::to_string(current.id));
    }
}

std::optional<Task> AGV::activeTask() const {
    if (state == AGVState::AGV_IDLE) return std::nullopt;
    return current;
}

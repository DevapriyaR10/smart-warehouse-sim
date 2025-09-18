#include "Crane.h"
#include "../core/Utils.h"
#include <cmath>

Crane::Crane(int sx,int sy, Rack* rackPtr)
    : x(sx), y(sy), targetX(sx), targetY(sy), state(CraneState::IDLE), rack(rackPtr) {}

void Crane::assignTask(const Task& t) {
    if (current) {
        log_warn("Crane already busy");
        return;
    }
    if (t.kind != TaskKind::ASRS_STORE && t.kind != TaskKind::ASRS_RETRIEVE) {
        log_warn("Crane: received non-ASRS task");
        return;
    }
    current = t;
    targetX = t.x;
    targetY = t.y;
    state = CraneState::MOVING;
    log_info("Crane assigned task id=" + std::to_string(t.id));
}

void Crane::stepToward() {
    if (x < targetX) ++x;
    else if (x > targetX) --x;
    else if (y < targetY) ++y;
    else if (y > targetY) --y;
}

void Crane::update() {
    if (!current) { state = CraneState::IDLE; return; }
    switch (state) {
        case CraneState::MOVING:
            if (x==targetX && y==targetY) {
                if (current->kind == TaskKind::ASRS_STORE) state = CraneState::PLACING;
                else state = CraneState::PICKING;
            } else stepToward();
            break;
        case CraneState::PICKING: {
            bool ok = rack->retrieveAt(x,y);
            if (ok) log_info("Crane picked at ("+std::to_string(x)+","+std::to_string(y)+")");
            else log_warn("Crane pick failed at ("+std::to_string(x)+","+std::to_string(y)+")");
            // after picking, deliver to dropzone (x2,y2) if provided, else home (0, rackHeight)
            if (current->x2!=current->x || current->y2!=current->y) {
                targetX = current->x2; targetY = current->y2;
            } else { targetX = 0; targetY = 0; }
            state = CraneState::MOVING;
            break;
        }
        case CraneState::PLACING: {
            bool ok = rack->storeAt(x,y);
            if (ok) log_info("Crane placed at ("+std::to_string(x)+","+std::to_string(y)+")");
            else log_warn("Crane place failed at ("+std::to_string(x)+","+std::to_string(y)+")");
            current.reset();
            state = CraneState::IDLE;
            break;
        }
        case CraneState::IDLE:
            break;
    }
}

int Crane::getX() const { return x; }
int Crane::getY() const { return y; }
CraneState Crane::getState() const { return state; }
std::optional<Task> Crane::activeTask() const { return current; }

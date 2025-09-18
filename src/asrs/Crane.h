#pragma once
#include "../core/Task.h"
#include "../asrs/Rack.h"
#include <optional>
#include <atomic>

enum class CraneState { IDLE, MOVING, PICKING, PLACING };

class Crane {
public:
    Crane(int sx, int sy, Rack* rackPtr);
    void assignTask(const Task& t); // accepts ASRS tasks
    void update();
    int getX() const;
    int getY() const;
    CraneState getState() const;
    std::optional<Task> activeTask() const;
private:
    int x,y;
    int targetX, targetY;
    CraneState state;
    Rack* rack;
    std::optional<Task> current;
    void stepToward();
};

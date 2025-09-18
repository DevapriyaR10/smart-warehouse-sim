#pragma once
#include <vector>
#include <utility>
#include "../core/Task.h"
#include <optional>

enum class AGVState { AGV_IDLE, AGV_NAVIGATING, AGV_WAITING, AGV_DELIVERING };

class AGV {
public:
    AGV(int id_, int sx, int sy);
    int id() const;
    int getX() const;
    int getY() const;
    AGVState getState() const;
    void assignTask(const Task& t, const std::vector<std::pair<int,int>>& path);
    void update(); // step along path
    std::optional<Task> activeTask() const;
private:
    int id_;
    int x,y;
    AGVState state;
    Task current;
    std::vector<std::pair<int,int>> path;
    size_t pathIdx;
};

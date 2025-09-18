```cpp
#pragma once
#include <string>

enum class TaskKind {
    ASRS_STORE,
    ASRS_RETRIEVE,
    AGV_MOVE,       // AGV pickup at X,Y -> deliver to X2,Y2
    UNKNOWN
};

struct Task {
    int id;
    TaskKind kind;
    int x, y;       // primary coords (rack slot or pickup)
    int x2, y2;     // secondary coords (delivery/drop)
    std::string meta; // optional
    Task(int _id = 0, TaskKind k = TaskKind::UNKNOWN, int _x=0,int _y=0,int _x2=0,int _y2=0)
      : id(_id), kind(k), x(_x), y(_y), x2(_x2), y2(_y2) {}
};

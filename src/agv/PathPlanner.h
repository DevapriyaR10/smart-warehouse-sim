#pragma once
#include <vector>
#include <utility>

using Grid = std::vector<std::vector<int>>; // 0 free, 1 obstacle

struct PathPlanner {
    PathPlanner(int w, int h);
    std::vector<std::pair<int,int>> findPath(const Grid& grid, int sx,int sy,int tx,int ty);
    int width, height;
private:
    int idx(int x,int y) const { return y*width + x; }
};

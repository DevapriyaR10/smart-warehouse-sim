#include "Rack.h"
#include <algorithm>

Rack::Rack(int width_, int height_) : width(width_), height(height_) {
    grid.assign(height, std::vector<bool>(width, false));
}

bool Rack::isValid(int x,int y) const {
    return x>=0 && x<width && y>=0 && y<height;
}

bool Rack::isOccupied(int x,int y) const {
    if (!isValid(x,y)) return false;
    return grid[y][x];
}

bool Rack::storeAt(int x,int y) {
    if (!isValid(x,y)) return false;
    if (grid[y][x]) return false;
    grid[y][x] = true;
    return true;
}

bool Rack::retrieveAt(int x,int y) {
    if (!isValid(x,y)) return false;
    if (!grid[y][x]) return false;
    grid[y][x] = false;
    return true;
}

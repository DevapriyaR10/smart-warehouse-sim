#pragma once
#include <vector>

class Rack {
public:
    Rack(int width, int height);
    bool isValid(int x, int y) const;
    bool isOccupied(int x, int y) const;
    bool storeAt(int x, int y);
    bool retrieveAt(int x, int y);
    int getWidth() const { return width; }
    int getHeight() const { return height; }
private:
    int width, height;
    std::vector<std::vector<bool>> grid;
};

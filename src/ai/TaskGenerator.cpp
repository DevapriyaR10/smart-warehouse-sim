#include "TaskGenerator.h"
#include <sstream>
#include <cstdlib>

TaskGenerator::TaskGenerator(int rw,int rh,int fw,int fh) : rackW(rw), rackH(rh), floorW(fw), floorH(fh) {}

std::vector<Task> TaskGenerator::generate(const std::string& hl, int &nextId) {
    // Simple rule-based break-down:
    // If hl contains "order" or "pick", generate a sequence: ASRS_RETRIEVE -> AGV_MOVE to shipping
    std::vector<Task> out;
    std::string s = hl;
    for (auto &c : s) c = tolower(c);
    if (s.find("order")!=std::string::npos || s.find("pick")!=std::string::npos) {
        // generate 1-3 items
        int items = 1 + (rand()%3);
        for (int i=0;i<items;++i) {
            int rx = rand()%rackW;
            int ry = rand()%rackH;
            Task t1(nextId++, TaskKind::ASRS_RETRIEVE, rx, ry, 0, 0);
            // designate delivery at a buffer point (floor coords)
            int bx = rand()%(floorW-2)+1;
            int by = rand()%(floorH-2)+1;
            // We put drop coordinates in x2,y2 so crane moves item to buffer
            t1.x2 = bx; t1.y2 = by;
            out.push_back(t1);
            // Then add an AGV move to pick from buffer (bx,by) to shipping (0,0)
            Task t2(nextId++, TaskKind::AGV_MOVE, bx, by, 0, 0);
            out.push_back(t2);
        }
    } else {
        // fallback: create a random store task
        int rx = rand()%rackW;
        int ry = rand()%rackH;
        out.push_back(Task(nextId++, TaskKind::ASRS_STORE, rx, ry, 0,0));
    }
    return out;
}

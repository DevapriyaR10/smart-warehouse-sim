#include "PathPlanner.h"
#include <queue>
#include <limits>
#include <cmath>
#include <algorithm>

PathPlanner::PathPlanner(int w,int h) : width(w), height(h) {}

std::vector<std::pair<int,int>> PathPlanner::findPath(const Grid& grid, int sx,int sy,int tx,int ty) {
    std::vector<int> dist(width*height, std::numeric_limits<int>::max());
    std::vector<int> prev(width*height, -1);
    auto valid = [&](int x,int y){ return x>=0 && x<width && y>=0 && y<height && grid[y][x]==0; };
    using Node = std::pair<int,int>; // dist, idx
    auto heuristic = [&](int x,int y){ return abs(x-tx)+abs(y-ty); };

    struct QN { int f, g, x, y; };
    struct Cmp { bool operator()(const QN&a,const QN&b) const { return a.f> b.f; } };
    std::priority_queue<QN, std::vector<QN>, Cmp> pq;
    if (!valid(sx,sy) || !valid(tx,ty)) return {};

    int sidx = idx(sx,sy);
    dist[sidx]=0;
    pq.push({heuristic(sx,sy),0,sx,sy});

    int dx[4]={1,-1,0,0};
    int dy[4]={0,0,1,-1};

    while(!pq.empty()){
        auto cur = pq.top(); pq.pop();
        int x=cur.x,y=cur.y;
        int g = cur.g;
        if (x==tx && y==ty) break;
        if (g>dist[idx(x,y)]) continue;
        for (int k=0;k<4;++k){
            int nx=x+dx[k], ny=y+dy[k];
            if (!valid(nx,ny)) continue;
            int nidx = idx(nx,ny);
            int ng = g+1;
            if (ng < dist[nidx]) {
                dist[nidx]=ng;
                prev[nidx]=idx(x,y);
                int f = ng + heuristic(nx,ny);
                pq.push({f, ng, nx, ny});
            }
        }
    }

    int tidx = idx(tx,ty);
    if (dist[tidx]==std::numeric_limits<int>::max()) return {};
    std::vector<std::pair<int,int>> path;
    int cur = tidx;
    while (cur!= -1) {
        int cx = cur % width;
        int cy = cur / width;
        path.emplace_back(cx,cy);
        cur = prev[cur];
    }
    std::reverse(path.begin(), path.end());
    return path;
}

#include <SFML/Graphics.hpp>
#include <thread>
#include <chrono>
#include <sstream>
#include "core/Task.h"
#include "core/TaskManager.h"
#include "asrs/Rack.h"
#include "asrs/Crane.h"
#include "agv/FleetManager.h"
#include "ai/TaskGenerator.h"
#include "core/Utils.h"
#include <cstdlib>
#include <ctime>

int main() {
    srand((unsigned)time(nullptr));
    // config
    const int rackW = 8, rackH = 5;
    const int floorW = 16, floorH = 12; // AGV planning grid
    const int agvCount = 3;

    // instantiate modules
    Rack rack(rackW, rackH);
    Crane crane(0, rackH-1, &rack); // crane positioned adjacent to rack area
    FleetManager fleet(floorW, floorH, agvCount);

    TaskManager tm(&crane, &rack, &fleet);
    TaskGenerator tg(rackW, rackH, floorW, floorH);
    int nextId = 1;

    // SFML window
    const int cellSize = 40;
    const int margin = 20;
    int winW = std::max(rackW, floorW) * cellSize + margin*2;
    int winH = (rackH + floorH) * cellSize + margin*3 + 100;
    sf::RenderWindow window(sf::VideoMode(winW, winH), "Smart Warehouse Simulator");
    window.setFramerateLimit(30);

    sf::Font font;
    bool hasFont = false;
    if (font.loadFromFile("arial.ttf")) hasFont = true;

    // Prepare obstacle grid for fleet: mark rack area as obstacles (top-left region)
    FleetManager::Grid grid = fleet.getGrid();
    // mark top-left rackW x rackH area as obstacles so AGVs avoid racks
    for (int y=0;y<floorH;++y) for (int x=0;x<floorW;++x) grid[y][x]=0;
    for (int ry=0; ry<rackH; ++ry) {
        for (int rx=0; rx<rackW; ++rx) {
            if (ry < floorH && rx < floorW) grid[ry][rx] = 1; // obstacle
        }
    }
    fleet.setObstacles(grid);

    // HUD instructions
    log_info("Controls: 1 -> AI generate order; S -> add STORE; R -> add RETRIEVE; P -> add AGV move");

    while (window.isOpen()) {
        sf::Event ev;
        while (window.pollEvent(ev)) {
            if (ev.type == sf::Event::Closed) window.close();
            if (ev.type == sf::Event::KeyPressed) {
                if (ev.key.code == sf::Keyboard::Num1) {
                    // AI generate
                    std::string hl = "Prepare order";
                    auto tasks = tg.generate(hl, nextId);
                    for (auto &t : tasks) tm.pushTask(t);
                } else if (ev.key.code == sf::Keyboard::S) {
                    int rx = rand()%rackW, ry = rand()%rackH;
                    Task t(nextId++, TaskKind::ASRS_STORE, rx, ry, 0,0);
                    tm.pushTask(t);
                } else if (ev.key.code == sf::Keyboard::R) {
                    int rx = rand()%rackW, ry = rand()%rackH;
                    Task t(nextId++, TaskKind::ASRS_RETRIEVE, rx, ry, 0,0);
                    tm.pushTask(t);
                } else if (ev.key.code == sf::Keyboard::P) {
                    int sx = rand()%(floorW-2)+1, sy = rand()%(floorH-2)+1;
                    Task t(nextId++, TaskKind::AGV_MOVE, sx, sy, 0, 0);
                    tm.pushTask(t);
                }
            }
        }

        // update logic
        tm.update();
        crane.update();
        fleet.update();

        // rendering
        window.clear(sf::Color(245,245,245));

        // draw rack region (top-left)
        for (int ry=0; ry<rackH; ++ry) {
            for (int rx=0; rx<rackW; ++rx) {
                sf::RectangleShape cell(sf::Vector2f(cellSize-2, cellSize-2));
                int px = margin + rx*cellSize;
                int py = margin + ry*cellSize;
                cell.setPosition(px, py);
                cell.setOutlineThickness(1);
                cell.setOutlineColor(sf::Color::Black);
                if (rack.isOccupied(rx,ry)) cell.setFillColor(sf::Color(160,220,160));
                else cell.setFillColor(sf::Color(220,220,220));
                window.draw(cell);
            }
        }

        // Draw Crane
        sf::RectangleShape craneRect(sf::Vector2f(cellSize-6, cellSize-6));
        int cpx = margin + crane.getX()*cellSize + 3;
        int cpy = margin + crane.getY()*cellSize + 3;
        craneRect.setPosition(cpx, cpy);
        craneRect.setFillColor(sf::Color(200,80,80));
        window.draw(craneRect);

        // draw floor grid below the rack region
        int floorOriginY = margin + rackH*cellSize + margin;
        for (int fy=0; fy<floorH; ++fy) {
            for (int fx=0; fx<floorW; ++fx) {
                sf::RectangleShape cell(sf::Vector2f(cellSize-2, cellSize-2));
                int px = margin + fx*cellSize;
                int py = floorOriginY + fy*cellSize;
                cell.setPosition(px, py);
                cell.setOutlineThickness(1);
                cell.setOutlineColor(sf::Color(200,200,200));
                // obstacle?
                if (grid[fy][fx]==1) cell.setFillColor(sf::Color(60,60,60)); // rack obstacle
                else cell.setFillColor(sf::Color(250,250,250));
                window.draw(cell);
            }
        }

        // draw AGVs
        for (auto &agv : fleet.getAGVs()) {
            sf::CircleShape dot(cellSize/3);
            int px = margin + agv.getX()*cellSize + cellSize/2 - dot.getRadius();
            int py = floorOriginY + agv.getY()*cellSize + cellSize/2 - dot.getRadius();
            dot.setPosition(px, py);
            dot.setFillColor(sf::Color(80,120,220));
            window.draw(dot);
            if (hasFont) {
                sf::Text idt;
                idt.setFont(font);
                idt.setCharacterSize(12);
                idt.setString(std::to_string(agv.id()));
                idt.setPosition(px+2, py+2);
                idt.setFillColor(sf::Color::White);
                window.draw(idt);
            }
        }

        // HUD
        if (hasFont) {
            sf::Text hud;
            hud.setFont(font);
            hud.setCharacterSize(14);
            std::stringstream ss;
            ss << "Pending: " << tm.pendingCount();
            ss << "   Controls: 1(AI order) S STORE R RETRIEVE P AGV_MOVE";
            hud.setString(ss.str());
            hud.setPosition(10, winH-50);
            hud.setFillColor(sf::Color::Black);
            window.draw(hud);
        }

        window.display();

        std::this_thread::sleep_for(std::chrono::milliseconds(120));
    }

    return 0;
}

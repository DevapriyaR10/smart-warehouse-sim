cmake_minimum_required(VERSION 3.10)
project(smart_warehouse_sim LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

file(GLOB SRC_FILES
    src/*.cpp
    src/*/*.cpp
    src/*/*/*.cpp
    src/*/*.h
    src/*/*/*.h
)

add_executable(smart_warehouse_sim
    src/main.cpp
    src/core/Task.cpp
    src/core/TaskManager.cpp
    src/asrs/Rack.cpp
    src/asrs/Crane.cpp
    src/agv/PathPlanner.cpp
    src/agv/AGV.cpp
    src/agv/FleetManager.cpp
    src/ai/TaskGenerator.cpp
)

target_include_directories(smart_warehouse_sim PRIVATE src)
target_link_libraries(smart_warehouse_sim PRIVATE sfml-graphics sfml-window sfml-system)

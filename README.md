# Smart Warehouse Automation Simulator

This project is an **end-to-end warehouse automation simulator** built in **C++17** using **SFML** for visualization.  
It integrates three major modules that reflect real-world warehouse systems:

1. **AS/RS (Automated Storage and Retrieval System)**
   - Simulates racks, storage slots, and a crane robot.
   - Crane moves along the rack to **store** or **retrieve** items.
   - State machine: `IDLE → MOVING → PICKING/PLACING → IDLE`.

2. **AGV Fleet (Automated Guided Vehicles)**
   - Multiple AGVs navigate a warehouse floor.
   - Uses **A\* path planning** on a grid to avoid obstacles (racks).
   - Each AGV has states: `IDLE → NAVIGATING → DELIVERING → IDLE`.
   - FleetManager handles task assignment and coordination.

3. **Generative AI TaskGenerator**
   - Mock “AI” that converts high-level instructions (e.g. *"Prepare an order"*) into low-level tasks:
     - **AS/RS_RETRIEVE** → retrieve items from rack.
     - **AGV_MOVE** → AGV delivers items from buffer to shipping zone.
   - Designed to be easily replaced with a real LLM API.

Together, these modules create a **Smart Warehouse** simulation:
- Orders are broken down into crane + AGV subtasks.
- Crane picks/stores at rack.
- AGV fleet delivers items to shipping or staging areas.
- Visualization shows racks, crane, floor grid, and AGVs moving in real time.

---

## Features
- **TaskManager**: Central brain that schedules AS/RS and AGV tasks.
- **Rack**: 2D grid with occupancy tracking.
- **Crane**: State-based robot operating on the rack grid.
- **AGVs**: Small mobile robots moving with path planning.
- **FleetManager**: Assigns tasks to idle AGVs, prevents collisions via obstacle grid.
- **TaskGenerator**: Mock AI that simulates generative planning.
- **Visualization (SFML)**:
  - Racks (gray cells, green if occupied).
  - Crane (red block).
  - AGVs (blue circles).
  - Obstacles (dark gray).
  - HUD with pending tasks and controls.

---

## Controls
Once running:
- `1` → Generate a **high-level AI order** (creates AS/RS + AGV tasks).
- `S` → Add a **STORE task** (crane places an item into rack).
- `R` → Add a **RETRIEVE task** (crane removes an item from rack).
- `P` → Add a **manual AGV move task** (AGV picks/delivers on floor grid).
- Close window → Exit simulator.

---

## Requirements
- **Compiler**: g++/clang with **C++17 support**
- **CMake**: Version 3.10+
- **SFML**: Version 2.5 or later
  - Linux: `sudo apt install libsfml-dev`
  - macOS: `brew install sfml`
  - Windows: Download from [https://www.sfml-dev.org/download.php](https://www.sfml-dev.org/download.php)

---

## Build & Run

```bash
# Clone or copy project
git clone https://github.com/yourname/smart-warehouse-sim.git
cd smart-warehouse-sim

# Create build directory
mkdir build
cd build

# Generate build files
cmake ..

# Compile
cmake --build .

# Run simulator
./smart_warehouse_sim

# Flocking System 2D

A real-time 2D flocking simulation built with [raylib](https://www.raylib.com/) and [raygui](https://github.com/raysan5/raygui), implementing Craig Reynolds' classic Boids algorithm. Boids self-organize into flocks using three simple steering rules — separation, alignment, and cohesion — with live controls to tune the behaviour at runtime.

---

## Demo

- 300 boids spawn at the center and naturally disperse into flocks
- Each distinct flock is colored differently in real time
- Sliders on the right panel let you reshape flocking behaviour without restarting

---

## Features

- **Three flocking rules** — separation, alignment, cohesion
- **Live parameter sliders** — adjust weights and radii while the simulation runs
- **Dynamic flock coloring** — connected components detected each frame via BFS; each flock gets a unique color from a 10-color palette
- **Wrapping borders** — boids wrap around all four screen edges
- **Dark themed UI** — raygui style loaded from an `.rgs` file
- **Split viewport** — left side is the simulation, right side is the control panel

---

## Project Structure

```
.
├── main.cpp        # Entry point, window, game loop, UI
├── boid.h/.cpp     # Boid struct — steering, physics, drawing
├── flock.h/.cpp    # Flock struct — manages boids, BFS color assignment
├── params.h        # FlockParams — shared tuning values
├── config.h        # Window dimensions, viewport/panel sizes
├── raygui.h        # raygui single-header UI library
└── assets/
    └── style_dark.rgs  # Dark theme for raygui
```

---

## How It Works

### The Three Rules

Each boid looks at its neighbours every frame and computes three steering forces:

| Rule | Radius used | What it does |
|------|-------------|--------------|
| **Separation** | `desiredSeparation` | Steers away from boids that are too close, weighted by inverse distance |
| **Alignment** | `neighborDistance` | Steers toward the average velocity direction of nearby boids |
| **Cohesion** | `neighborDistance` | Steers toward the average position of nearby boids |

The three forces are weighted and summed each frame:

```
totalForce = sep * 1.5  +  ali * alignmentWeight  +  coh * 1.0
```

### Flock Detection and Coloring

Every frame after the physics update, a BFS runs over all boids. Two boids are considered connected if their distance is less than `neighborDistance`. Each connected component is a flock and gets assigned a color from the palette. Colors update live as flocks merge and split.

### Update / Draw Separation

Physics (`flock`, `update`, `borders`) runs before `BeginDrawing()`. Rendering runs inside `BeginDrawing()` / `EndDrawing()`. This keeps simulation logic cleanly separated from rendering.

---

## Controls

| Slider | Range | Effect |
|--------|-------|--------|
| Alignment Weight | 0.0 – 3.0 | How strongly boids match neighbour direction. 0 = no alignment, chaos. 3 = tight coordinated movement |
| Separation Radius | 5.0 – 80.0 | Personal space per boid. Increase to spread the flock out, decrease to allow tighter packing |
| Neighbour Distance | 10.0 – 150.0 | Perception radius for alignment and cohesion. Larger values form bigger, slower-reacting flocks |

### Suggested Presets

| Behaviour | Alignment | Separation | Neighbour |
|-----------|-----------|------------|-----------|
| Natural flocking | 1.0 | 25.0 | 50.0 |
| Tight schooling | 1.5 | 20.0 | 75.0 |
| Scattered swarm | 0.3 | 40.0 | 30.0 |
| Single mega-flock | 2.0 | 15.0 | 120.0 |

---

## Building

### Requirements

- [raylib](https://www.raylib.com/) 4.5 or later
- A C++17 compiler (g++, clang++, MSVC)
- `raygui.h` in the project root
- `assets/style_dark.rgs` in the working directory

### Linux / macOS

```bash
g++ -std=c++17 main.cpp boid.cpp flock.cpp -o flocking -lraylib -lm
./flocking
```

### Windows (MinGW)

```bash
g++ -std=c++17 main.cpp boid.cpp flock.cpp -o flocking.exe -lraylib -lopengl32 -lgdi32 -lwinmm
flocking.exe
```

---

## Configuration

Edit `config.h` to change window and panel dimensions:

```cpp
const int PANEL_WIDTH   = 280;   // right panel width in pixels
const int width         = 1800;  // total window width
const int height        = 700;   // window height
const int viewportWidth = width - PANEL_WIDTH;  // simulation area
```

Edit `params.h` to change the default starting values of the sliders:

```cpp
struct FlockParams {
    float alignmentWeight   = 1.0f;
    float desiredSeparation = 25.0f;
    float neighborDistance  = 50.0f;
};
```

---

## Known Limitations

- BFS color assignment is O(n²) per frame — works fine up to ~500 boids; above that consider spatial hashing
- All boids share the same `maxspeed` and `maxforce` — per-boid variation would make movement more organic
- No mouse interaction — clicking to scatter or attract boids would be a natural next feature

---

## References

- Craig Reynolds, [Flocks, Herds, and Schools: A Distributed Behavioral Model](https://www.red3d.com/cwr/papers/1987/boids.html), SIGGRAPH 1987
- Daniel Shiffman, [The Nature of Code — Autonomous Agents](https://natureofcode.com/autonomous-agents/)
- [raylib documentation](https://www.raylib.com/cheatsheet/cheatsheet.html)
- [raygui documentation](https://github.com/raysan5/raygui)
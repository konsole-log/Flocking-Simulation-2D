#include "flock.h"
#include <queue>

// palette — add more if you expect many small flocks
static const Color PALETTE[] = {
    {235, 85, 85, 255},   // red
    {85, 200, 85, 255},   // green
    {85, 145, 235, 255},  // blue
    {235, 195, 60, 255},  // yellow
    {175, 95, 235, 255},  // purple
    {235, 145, 55, 255},  // orange
    {75, 215, 200, 255},  // teal
    {235, 120, 185, 255}, // pink
    {140, 220, 90, 255},  // lime
    {90, 185, 235, 255},  // sky blue
};
static const int PALETTE_SIZE = sizeof(PALETTE) / sizeof(PALETTE[0]);

Flock::Flock() {
  boids = {};
}

void Flock::addBoid(const Boid &b) {
  boids.push_back(b);
}

void Flock::assignColors() {
  int n = (int)boids.size();
  std::vector<int> group(n, -1);
  int groupCount = 0;

  for (int i = 0; i < n; i++) {
    if (group[i] != -1)
      continue;
    std::queue<int> q;
    q.push(i);
    group[i] = groupCount;

    while (!q.empty()) {
      int cur = q.front();
      q.pop();
      for (int j = 0; j < n; j++) {
        if (group[j] == -1 &&
            Vector2Distance(boids[cur].position, boids[j].position) <
                params.neighborDistance) {
          group[j] = groupCount;
          q.push(j);
        }
      }
    }
    groupCount++;
  }
  for (int i = 0; i < n; i++)
    boids[i].color = PALETTE[group[i] % PALETTE_SIZE];
}

void Flock::run() {
  for (auto &boid : boids)
    boid.run(boids, params);

  // assignColors();
}

void Flock::draw() const {
  for (const auto &boid : boids)
    boid.draw();
}
#pragma once
#include "boid.h"
#include "params.h"
#include <vector>

struct Flock {
    std::vector<Boid> boids;
    FlockParams params;

    Flock();
    void addBoid(const Boid &b);
    void run();
    void draw() const;

  private:
    void assignColors();
};
#pragma once
#include "config.h"
#include "params.h"
#include <raylib.h>
#include <raymath.h>
#include <vector>

struct Boid {
    Vector2 position, velocity, acceleration;
    float size, maxspeed, maxforce;
    Color color;

    Boid(Vector2 pos);
    void run(std::vector<Boid> &boids, const FlockParams &params);
    void applyForce(Vector2 force);
    void flock(std::vector<Boid> &boids, const FlockParams &params);
    void update();
    Vector2 seek(Vector2 &target);
    void draw() const;
    void borders();

    Vector2 separate(std::vector<Boid> &boids, float desiredSeparation);
    Vector2 align(std::vector<Boid> &boids, float neighborDistance);
    Vector2 cohere(std::vector<Boid> &boids, float neighborDistance);
};
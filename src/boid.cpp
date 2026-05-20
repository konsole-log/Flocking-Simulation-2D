#include "boid.h"

static inline Vector2 rotate(Vector2 v, float a) {
  return {v.x * cosf(a) - v.y * sinf(a), v.x * sinf(a) + v.y * cosf(a)};
}

Boid::Boid(Vector2 pos) {
  size = 6.0f;
  maxspeed = 3.0f;
  maxforce = 0.05f;
  position = pos;
  acceleration = {0, 0};
  color = WHITE;
  Vector2 vel = {(float)GetRandomValue(-100, 100),
                 (float)GetRandomValue(-100, 100)};
  velocity = Vector2Normalize(vel);
  velocity = Vector2Scale(velocity, maxspeed);
}

void Boid::applyForce(Vector2 force) {
  acceleration = Vector2Add(acceleration, force);
}

void Boid::flock(std::vector<Boid> &boids, const FlockParams &params) {
  Vector2 sep = separate(boids, params.desiredSeparation);
  Vector2 ali = align(boids, params.neighborDistance);
  Vector2 coh = cohere(boids, params.neighborDistance);

  sep = Vector2Scale(sep, 1.5f);
  ali = Vector2Scale(ali, params.alignmentWeight); // slider-controlled
  coh = Vector2Scale(coh, 1.0f);

  applyForce(sep);
  applyForce(ali);
  applyForce(coh);
}

void Boid::update() {
  velocity = Vector2Add(velocity, acceleration);
  if (Vector2Length(velocity) > maxspeed) {
    velocity = Vector2Normalize(velocity);
    velocity = Vector2Scale(velocity, maxspeed);
  }
  position = Vector2Add(position, velocity);
  acceleration = {0, 0};
}

Vector2 Boid::seek(Vector2 &target) {
  Vector2 desired = Vector2Subtract(target, position);
  desired = Vector2Normalize(desired);
  desired = Vector2Scale(desired, maxspeed);
  Vector2 steer = Vector2Subtract(desired, velocity);
  if (Vector2Length(steer) > maxforce) {
    steer = Vector2Normalize(steer);
    steer = Vector2Scale(steer, maxforce);
  }
  return steer;
}

void Boid::draw() const {
  float angle = atan2f(velocity.y, velocity.x);
  Vector2 p1 = {size * 2, 0};
  Vector2 p2 = {-size * 2, -size};
  Vector2 p3 = {-size * 2, size};

  p1 = rotate(p1, angle);
  p2 = rotate(p2, angle);
  p3 = rotate(p3, angle);

  p1 = Vector2Add(p1, position);
  p2 = Vector2Add(p2, position);
  p3 = Vector2Add(p3, position);

  DrawTriangle(p1, p2, p3, color);
}

void Boid::borders() {
  if (position.x < -size)
    position.x = width + size;
  if (position.y < -size)
    position.y = height + size;
  if (position.x > width + size)
    position.x = -size;
  if (position.y > height + size)
    position.y = -size;
}

Vector2 Boid::separate(std::vector<Boid> &boids, float desiredSeparation) {
  Vector2 steer = {0, 0};
  int count = 0;
  for (size_t i = 0; i < boids.size(); i++) {
    float dist = Vector2Distance(position, boids[i].position);
    if (dist > 0 && dist < desiredSeparation) {
      Vector2 diff = Vector2Subtract(position, boids[i].position);
      diff = Vector2Normalize(diff);
      diff = Vector2Scale(diff, 1.0f / dist);
      steer = Vector2Add(steer, diff);
      count++;
    }
  }
  if (count > 0)
    steer = Vector2Scale(steer, 1.0f / count);

  if (Vector2Length(steer) > 0) {
    steer = Vector2Normalize(steer);
    steer = Vector2Scale(steer, maxspeed);
    steer = Vector2Subtract(steer, velocity);
    if (Vector2Length(steer) > maxforce) {
      steer = Vector2Normalize(steer);
      steer = Vector2Scale(steer, maxforce);
    }
  }
  return steer;
}

Vector2 Boid::align(std::vector<Boid> &boids, float neighborDistance) {
  Vector2 sum = {0, 0};
  int count = 0;
  for (size_t i = 0; i < boids.size(); i++) {
    float dist = Vector2Distance(position, boids[i].position);
    if (dist > 0 && dist < neighborDistance) {
      sum = Vector2Add(sum, boids[i].velocity);
      count++;
    }
  }
  if (count > 0) {
    sum = Vector2Scale(sum, 1.0f / count);
    sum = Vector2Normalize(sum);
    sum = Vector2Scale(sum, maxspeed);
    Vector2 steer = Vector2Subtract(sum, velocity);
    if (Vector2Length(steer) > maxforce) {
      steer = Vector2Normalize(steer);
      steer = Vector2Scale(steer, maxforce);
    }
    return steer;
  }
  return {0, 0};
}

Vector2 Boid::cohere(std::vector<Boid> &boids, float neighborDistance) {
  Vector2 sum = {0, 0};
  int count = 0;
  for (size_t i = 0; i < boids.size(); i++) {
    float dist = Vector2Distance(position, boids[i].position);
    if (dist > 0 && dist < neighborDistance) {
      sum = Vector2Add(sum, boids[i].position);
      count++;
    }
  }
  if (count > 0) {
    sum = Vector2Scale(sum, 1.0f / count);
    return seek(sum);
  }
  return {0, 0};
}

void Boid::run(std::vector<Boid> &boids, const FlockParams &params) {
  flock(boids, params);
  update();
  borders();
}
#include "../include/Xlib_Engine.h"

GameEngine::GameEngine(int windowWidth, int windowHeight, int borderWidth,
                       double gravitationalPull, double jumpImpulse,
                       double walkingSpeed, int frameDuration,
                       bool collisions) {
  MockCollisionEngine collisionEngine;
  displayManager = new XManager(windowWidth, windowHeight, borderWidth);
  physicsEngine = new XPhysicsEngine(
      gravitationalPull, jumpImpulse, walkingSpeed, windowWidth, windowHeight,
      frameDuration, collisionEngine, collisions);
}

GameEngine::~GameEngine() {
    delete displayManager;
    delete physicsEngine;
}



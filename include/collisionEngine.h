#ifndef COLLISION_ENGINE_H
#define COLLISION_ENGINE_H

#define GRID_COLUMNS 10
#define GRID_ROWS 10
#define GRID_SIZE GRID_COLUMNS *GRID_ROWS

#include "gameObjects.h"
#include <vector>

struct Collision {
  std::shared_ptr<GameObject> collider1, collider2;

  Collision(std::shared_ptr<GameObject> c1, std::shared_ptr<GameObject> c2)
      : collider1(c1), collider2(c2) {}
};

struct CollisionEngine {
  /**
   * Get all collisions between game objects
   *
   * @return vector containing all collisions
   */
  virtual std::vector<Collision> getAllCollisions() = 0;

  /**
   * Get all game objects that are colliding with the given object
   *
   * @param gameObject object against which all collisions are checked
   * @return vector containing all game objects that are colliding with
   * gameObject
   */
  virtual std::vector<std::shared_ptr<GameObject>>
  getCollisionsWithObject(std::shared_ptr<GameObject> &gameObject) = 0;

  virtual bool objectsCollided(const std::shared_ptr<GameObject> &o1,
                               const std::shared_ptr<GameObject> &o2) = 0;

  virtual void addGameObject(std::shared_ptr<GameObject> gameObject) = 0;

  virtual void removeGameObject(std::shared_ptr<GameObject> &gameObject) = 0;

  /**
   * Put the game object in the correct quadrants, and remove it from the wrong
   * quadrants
   *
   * @param previousState game object with the state the object was in on the
   * last update
   * @param newState game object with the object's current state
   */
  virtual void updateObjectQuadrants(std::shared_ptr<GameObject> &previousState,
                                     std::shared_ptr<GameObject> &newState) = 0;
};

struct MockCollisionEngine : CollisionEngine {
  std::vector<Collision> getAllCollisions() override;

  std::vector<std::shared_ptr<GameObject>>
  getCollisionsWithObject(std::shared_ptr<GameObject> &gameObject) override;

  bool objectsCollided(const std::shared_ptr<GameObject> &o1,
                       const std::shared_ptr<GameObject> &o2) override;

  void addGameObject(std::shared_ptr<GameObject> gameObject) override;

  void removeGameObject(std::shared_ptr<GameObject> &gameObject) override;

  void updateObjectQuadrants(std::shared_ptr<GameObject> &previousState,
                             std::shared_ptr<GameObject> &newState) override;
};

// typedef std::vector<std::shared_ptr<GameObject>> Quadrant;

///**
// * Engine that detects collisions between game objects
// */
// struct XCollisionEngine : CollisionEngine {
//  Quadrant gameGrid[GRID_SIZE];
//
//  int width, height;
//
//  int rows = GRID_ROWS;
//  int columns = GRID_COLUMNS;
//
//  XCollisionEngine(int width, int height) : width(width), height(height) {}
//
//  XCollisionEngine(int width, int height, int rows, int columns)
//      : width(width), height(height), rows(rows), columns(columns) {}
//
//  std::vector<Collision> getAllCollisions() override;
//
//  std::vector<std::shared_ptr<GameObject>>
//  getCollisionsWithObject(std::shared_ptr<GameObject> &gameObject) override;
//
//  bool objectsCollided(const std::shared_ptr<GameObject> &o1,
//                       const std::shared_ptr<GameObject> &o2) override;
//
//  void addGameObject(std::shared_ptr<GameObject> gameObject) override;
//
//  void removeGameObject(std::shared_ptr<GameObject> &gameObject) override;
//
//  void updateObjectQuadrants(std::shared_ptr<GameObject> &previousState,
//                             std::shared_ptr<GameObject> &newState) override;
//
// private:
//  /**
//   * Get all quadrants that the game object is in
//   *
//   * @param gameObject reference game object
//   * @return vector containing all the quadrants that the gameObject is in
//   * */
//  const std::vector<Quadrant> *
//  getObjectQuadrants(const std::shared_ptr<GameObject> &gameObject);
//
//  /**
//   * Get all game objects that are colliding with the given object, inside the
//   * given quadrant
//   *
//   * @param gameObject object against which all collisions are checked
//   * @param quadrant quadrant to search possible colliding objects in
//   * @return vector containing all game objects that are colliding with
//   * gameObject inside quadrant
//   */
//  std::vector<std::shared_ptr<GameObject>>
//  getCollisionsInQuadrant(const std::shared_ptr<GameObject> &gameObject,
//                          const Quadrant &quadrant);
//  /**
//   * Get the indexes of the quadrants that the game object is in
//   *
//   * @param gameObject refernece game object
//   * @return vector containing the quadrant indexes
//   */
//  std::vector<int>
//  getObjectQuadrantsIndexes(std::shared_ptr<GameObject> &gameObject);
//
//  /**
//   * Convert a row and column pair into an index in the game grid
//   *
//   * @param row
//   * @param column
//   * @return index equivalent
//   */
//  int getQuadrantIndex(int row, int column);
//};
//
#endif // !COLLISION_ENGINE_H

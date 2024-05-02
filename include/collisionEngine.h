#ifndef COLLISION_ENGINE_H
#define COLLISION_ENGINE_H

#define GRID_COLUMNS 10
#define GRID_ROWS 10
#define GRID_SIZE GRID_COLUMNS *GRID_ROWS

#include "gameObjects.h"
#include <vector>

using std::vector; 

typedef vector<GameObject *> Quadrant;

/**
 * Engine that detects collisions between game objects
 */
struct CollisionEngine {
  Quadrant gameGrid[GRID_SIZE];

  int width, height;

  CollisionEngine(int width, int height) : width(width), height(height) {}

  /**
   * Get all game objects that are colliding with the given object
   *
   * @param gameObject object against which all collisions are checked
   * @return vector containing all game objects that are colliding with
   * gameObject
   */
  vector<GameObject> getCollisions(GameObject gameObject);

  /**
   * Get all quadrants that the game object is in
   *
   * @param gameObject reference game object
   * @return vector containing all the quadrants that the gameObject is in
   * */
  const vector<Quadrant> *getObjectQuadrants(const GameObject &gameObject);

  /**
   * Get all game objects that are colliding with the given object, inside the
   * given quadrant
   *
   * @param gameObject object against which all collisions are checked
   * @param quadrant quadrant to search possible colliding objects in
   * @return vector containing all game objects that are colliding with
   * gameObject inside quadrant
   */
  vector<GameObject> getCollisionsInQuadrant(const GameObject &gameObject,
                                             const Quadrant &quadrant);
  /**
   * Determine if two game objects have collided
   *
   * @param o1 game object 1
   * @param o2 game object 2
   * @return True if there is a collision between the objects, False if
   * otherwise
   */
  bool objectsCollided(const GameObject &o1, const GameObject &o2);

  /**
   * Put the game object in the correct quadrants, and remove it from the wrong
   * quadrants
   *
   * @param previousState game object with the state the object was in on the
   * last update
   * @param newState game object with the object's current state
   */
  void updateObjectQuadrants(GameObject &previousState, GameObject &newState);

  /**
   * Get the indexes of the quadrants that the game object is in
   *
   * @param gameObject refernece game object
   * @return vector containing the quadrant indexes
   */
  vector<int> getObjectQuadrantsIndexes(GameObject &gameObject);

  /**
   * Convert a row and column pair into an index in the game grid
   *
   * @param row
   * @param column
   * @return index equivalent
   */
  int getQuadrantIndex(int row, int column);

  /**
   * Add game object to the collision engine
   *
   * @param gameObject object to be added
   */
  void addGameObject(GameObject *gameObject);

  /**
   * Remove game object from collision engine
   *
   * @param gameObject object to be removed
   */
  void removeGameObject(GameObject &gameObject);
};

#endif // !COLLISION_ENGINE_H

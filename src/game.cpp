#include "../include/Xlib_Engine.h"

#define INITIAL_WINDOW_WIDTH 400
#define INITIAL_WINDOW_HEIGHT 300
#define INITIAL_BORDER_WIDTH 5
#define INITIAL_GRAVITY 1.0
#define INITIAL_JUMP_IMPULSE 10.0
#define INITIAL_WALKING_SPEED 150.0
#define INITIAL_FRAME_DURATION 30
#define PLAYER_WIDTH 30
#define PLAYER_HEIGHT 30
#define PLAYER_MASS 1

int main(int argc, char *argv[]) {
  GameEngine engine(INITIAL_WINDOW_WIDTH, INITIAL_WINDOW_HEIGHT,
                    INITIAL_BORDER_WIDTH, INITIAL_GRAVITY, INITIAL_JUMP_IMPULSE,
                    INITIAL_WALKING_SPEED, INITIAL_FRAME_DURATION, false);

  engine.setNewPlayer(RECTANGLE, 0, 10, PLAYER_WIDTH, PLAYER_HEIGHT,
                      PLAYER_MASS);

  engine.onKeyPressed(
      KEY_Q, [](GameEngine &gameEngine) -> void { gameEngine.exit(); });

  engine.onKeyPressed(KEY_SPACE, [](GameEngine &gameEngine) -> void {
    gameEngine.playerJump();
  });

  engine.onKeyPressed(KEY_LEFT, [](GameEngine &gameEngine) -> void {
    gameEngine.playerSetWalkingLeft();
  });

  engine.onKeyPressed(RELEASE_LEFT, [](GameEngine &gameEngine) -> void {
    gameEngine.playerUnsetWalkingLeft();
  });

  engine.onKeyPressed(KEY_RIGHT, [](GameEngine &gameEngine) -> void {
    gameEngine.playerSetWalkingRight();
  });

  engine.onKeyPressed(RELEASE_RIGHT, [](GameEngine &gameEngine) -> void {
    gameEngine.playerUnsetWalkingRight();
  });

  engine.onKeyPressed(NO_KEY, [](GameEngine &gameEngine) -> void {});

  engine.run();

  return 0;
}

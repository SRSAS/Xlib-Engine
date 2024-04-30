#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <ostream>
#include <tuple>

// WINDOW CONSTANTS

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 400

#define WINDOW_X 0
#define WINDOW_Y 0

#define BORDER_WIDTH 5

// OBJECTS CONSTANTS

#define FLOOR_HEIGHT 30
#define FLOOR_Y WINDOW_HEIGHT - FLOOR_HEIGHT

#define RECTANGLE_WIDTH 30
#define RECTANGLE_HEIGHT 30

#define RECTANGLE_X 30
#define RECTANGLE_Y FLOOR_Y - RECTANGLE_HEIGHT

// PHYSICS CONSTANTS

struct Force {
  int x;
  int y;

  Force(int x, int y) : x(x), y(y) {}
};

// Global Variables
bool running;
bool leftPressed = false, rightPressed = false;
const Force gravity(0, 1);
const Force leftMovement(-1, 0);
const Force rightMovement(1, 0);
int windowWidth = WINDOW_WIDTH, windowHeight = WINDOW_HEIGHT;

struct Rectangle {
  int x;
  int y;
  int width;
  int height;

  int speedX = 0;
  int speedY = 0;

  Rectangle(int x, int y, int width, int height)
      : x(x), y(y), width(width), height(height) {}

  Rectangle &updateCoordinates() {
    x += speedX;
    y += speedY;

    if (x <= 0) {
      x = 0;
    } else if (x >= windowWidth - width) {
      x = windowWidth - width;
    }

    if (y >= windowHeight - height) {
      y = windowHeight - height;
    } else if (y <= 0) {
      y = 0;
    }

    return *this;
  }

  Rectangle &applyForce(Force force) {
    speedX += force.x;
    speedY += force.y;

    if (y >= windowHeight - height && speedY > 0) {
      speedY = 0;
    }

    return *this;
  }
};

// Function Declarations
std::tuple<Display *, Window, GC, int> setupWindow();
void handleEvents(Display *display, Window &window, Rectangle &player);
void animate(Display *display, Window &window, GC &gc, int screen,
             Rectangle &player);
void deleteRectangle(Display *display, Window &window, GC &gc, int screen,
                     Rectangle &player);
void bounceBack(Rectangle &player);
bool isKeyDown(Display *display, KeySym key);
void updateWindowSize(Display *display, Window &window);

int main(int argc, char *argv[]) {
  auto player =
      Rectangle(RECTANGLE_X, RECTANGLE_Y, RECTANGLE_WIDTH, RECTANGLE_HEIGHT);

  Display *display;
  Window window;
  GC gc;
  int screen;
  std::tie(display, window, gc, screen) = setupWindow();

  running = true;
  auto frameStartTime = std::chrono::high_resolution_clock::now();

  while (running) {
    auto frameEndTime = std::chrono::high_resolution_clock::now();
    auto frameTimeElapsed =
        std::chrono::duration_cast<std::chrono::milliseconds>(frameEndTime -
                                                              frameStartTime);
    handleEvents(display, window, player);

    if (frameTimeElapsed.count() >= 30) {
      frameStartTime = std::chrono::high_resolution_clock::now();
      animate(display, window, gc, screen, player);
    }
  }

  XFreeGC(display, gc);
  XDestroyWindow(display, window);
  XCloseDisplay(display);

  return EXIT_SUCCESS;
}

std::tuple<Display *, Window, GC, int> setupWindow() {
  Display *display = XOpenDisplay(NULL);
  if (display == NULL) {
    std::cout << "Could not open display." << std::endl;
    exit(EXIT_FAILURE);
  }

  int screen = DefaultScreen(display);
  Window window = XCreateSimpleWindow(
      display, RootWindow(display, screen), WINDOW_X, WINDOW_Y, WINDOW_WIDTH,
      WINDOW_HEIGHT, BORDER_WIDTH, BlackPixel(display, screen),
      BlackPixel(display, screen));

  XSelectInput(display, window,
               KeyPressMask | KeyReleaseMask | StructureNotifyMask);
  XMapWindow(display, window);

  GC gc = XCreateGC(display, window, 0, nullptr);

  return std::make_tuple(display, window, gc, screen);
}

void handleEvents(Display *display, Window &window, Rectangle &player) {
  Force jump(0, -20);
  XEvent event;
  while (XPending(display) > 0) {
    XNextEvent(display, &event);

    switch (event.type) {
    case KeyPress: {
      switch (XLookupKeysym(&event.xkey, 0)) {
      case XK_space:
        player.applyForce(jump);
        break;
      case XK_q:
        running = false;
        break;
      case XK_Right:
        rightPressed = true;
        break;
      case XK_Left:
        leftPressed = true;
        break;
      default:
        break;
      }
      break;
    }

    case KeyRelease: {
      switch (XLookupKeysym(&event.xkey, 0)) {
      case XK_Right:
        rightPressed = false;
        break;
      case XK_Left:
        leftPressed = false;
        break;
      default:
        break;
      }
      break;
    }

    case ConfigureNotify: {
      updateWindowSize(display, window);
      break;
    }

    default:
      break;
    }
  }
}

void updateWindowSize(Display *display, Window &window) {
  XWindowAttributes attributes;
  XGetWindowAttributes(display, window, &attributes);
  windowWidth = attributes.width;
  windowHeight = attributes.height;
}

void animate(Display *display, Window &window, GC &gc, int screen,
             Rectangle &player) {
  player.applyForce(gravity);
  auto tempPlayer = Rectangle(player.x, player.y, player.width, player.height);

  if (player.y <= 0) {
    bounceBack(player);
  }

  if (leftPressed) {
    player.x -= 10;
  }

  if (rightPressed) {
    player.x += 10;
  }

  deleteRectangle(display, window, gc, screen, tempPlayer);
  player.updateCoordinates();
  XFillRectangle(display, window, gc, player.x, player.y, player.width,
                 player.height);
}

void bounceBack(Rectangle &player) { player.speedY = (-player.speedY * 2) / 3; }

void deleteRectangle(Display *display, Window &window, GC &gc, int screen,
                     Rectangle &player) {
  XSetForeground(display, gc, BlackPixel(display, screen));
  XFillRectangle(display, window, gc, player.x, player.y, player.width,
                 player.height);
  XSetForeground(display, gc, WhitePixel(display, screen));
}

#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <ostream>
#include <tuple>

// WINDOW CONSTANTS

#define WINDOW_WIDTH 400
#define WINDOW_HEIGHT 450

#define WINDOW_X 0
#define WINDOW_Y 0

#define BORDER_WIDTH 5

// OBJECTS CONSTANTS

#define FLOOR_HEIGHT 0
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

    if (y >= RECTANGLE_Y) {
      y = RECTANGLE_Y;
    }

    return *this;
  }

  Rectangle &applyForce(Force force) {
    speedX += force.x;
    speedY += force.y;

    if (y >= RECTANGLE_Y && speedY > 0) {
      speedY = 0;
    }

    return *this;
  }
};

// Function Declarations
std::tuple<Display *, Window, GC, int> setupWindow();
void handleEvents(Display *display, Rectangle &player);
void animate(Display *display, Window &window, GC &gc, int screen,
             Rectangle &player);
void deleteRectangle(Display *display, Window &window, GC &gc, int screen,
                     Rectangle &player);

// Global Variables
bool running;
const Force gravity(0, 1);

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

    handleEvents(display, player);

    if (frameTimeElapsed.count() >= 40) {
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

  XSelectInput(display, window, ExposureMask | KeyPressMask);
  XMapWindow(display, window);

  GC gc = XCreateGC(display, window, 0, nullptr);

  return std::make_tuple(display, window, gc, screen);
}

void handleEvents(Display *display, Rectangle &player) {
  Force jump(0, -15);
  XEvent event;
  while (XPending(display) > 0) {
    XNextEvent(display, &event);

    switch (event.type) {
    case Expose:
      // Handle expose event
      break;
    case KeyPress: {
      // Handle key press event
      KeySym key = XLookupKeysym(&event.xkey, 0);
      switch (key) {
      case XK_space:
        player.applyForce(jump);
        break;
      case XK_q:
        running = false;
        break;
      default:
        break;
      }
      break;
    }
    default:
      break;
    }
  }
}

void animate(Display *display, Window &window, GC &gc, int screen,
             Rectangle &player) {

  deleteRectangle(display, window, gc, screen, player);

  player.applyForce(gravity).updateCoordinates();

  XFillRectangle(display, window, gc, player.x, player.y, player.width,
                 player.height);
}

void deleteRectangle(Display *display, Window &window, GC &gc, int screen,
                     Rectangle &player) {
  XSetForeground(display, gc, BlackPixel(display, screen));
  XFillRectangle(display, window, gc, player.x, player.y, player.width,
                 player.height);
  XSetForeground(display, gc, WhitePixel(display, screen));
}

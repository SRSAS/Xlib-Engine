#ifndef DESIGN_PATERNS_H
#define DESIGN_PATERNS_H

#include <memory>
struct Rectangle;

struct Observer {
  virtual void onNotified() = 0;
};

struct Observable {
  virtual void notifyAll() = 0;
  virtual void addObserver(std::shared_ptr<Observer> observer) = 0;
  virtual void removeObserver(std::shared_ptr<Observer> &observer) = 0;
};

struct VisitorDisplay {
  virtual void visitRectangle(const Rectangle &rectangle) = 0;
};

struct DisplayVisitable {
  virtual void accept(VisitorDisplay& visitor) = 0;
};

#endif // !DESIGN_PATERNS_H

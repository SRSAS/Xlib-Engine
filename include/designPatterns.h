#ifndef DESIGN_PATERNS_H
#define DESIGN_PATERNS_H

#include <memory>
struct Rectangle;

struct Observer {
  virtual void onNotified();
};

struct Observable {
  virtual void notifyAll();
  virtual void addObserver(std::shared_ptr<Observer> observer);
  virtual void removeObserver(std::shared_ptr<Observer> &observer);
};

struct VisitorDisplay {
  virtual void visitRectangle(const Rectangle &rectangle);
};

struct DisplayVisitable {
  virtual void accept(VisitorDisplay visitor);
};

#endif // !DESIGN_PATERNS_H

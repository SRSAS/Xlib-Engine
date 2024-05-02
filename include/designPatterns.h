#ifndef DESIGN_PATERNS_H
#define DESIGN_PATERNS_H

struct Rectangle;

struct Observer {
  virtual void onNotified();
};

struct Observable {
  virtual void notifyAll();
  virtual void addObserver(Observer *observer);
  virtual void removeObserver(Observer *observer);
};

struct VisitorDisplay {
    virtual void visitRectangle(Rectangle& rectangle);
};

struct DisplayVisitable {
    virtual void accept(VisitorDisplay visitor);
};

#endif // !DESIGN_PATERNS_H

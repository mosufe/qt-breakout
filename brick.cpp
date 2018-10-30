#include <iostream>
#include "resources.h"

Brick::Brick(int x, int y, int size) {

  if (size == 0)
    image.load(":/images/brickie.png");
  else if (size == 1)
    image.load(":/images/brickiesmall.png");
  destroyed = false;
  rect = image.rect();
  rect.translate(x, y);
}

Brick::~Brick() {

  std::cout << ("Brick deleted") << std::endl;
}

QRect Brick::getRect() {
  return rect;
}

void Brick::setRect(QRect rct) {

  rect = rct;
}

QImage & Brick::getImage() {

  return image;
}

bool Brick::isDestroyed() {

  return destroyed;
}

void Brick::setDestroyed(bool destr) {

  destroyed = destr;
}

#include <iostream>
#include "resources.h"

ComputerPaddle::ComputerPaddle() {
  dy = 0;
  imagex.load("://images/LeftPaddle.png");
  rect = imagex.rect();
  resetState();
}

ComputerPaddle::~ComputerPaddle() {

 std::cout << ("Paddle deleted") << std::endl;
}

void ComputerPaddle::setDy(int y) {
  dy = y;
}

void ComputerPaddle::move() {

    //if the paddle reached one of the edges, do not let dy make it trespass the boundaries
    //only let dy exist (dy!=0) in the opposite direction

    if(getTop() <= TOP_EDGE && dy<0)
        dy = 0;

    if(getBottom() >= BOTTOM_EDGE && dy>0)
        dy = 0;

    int x = rect.x();
    int y = rect.y() + dy*speed;

    rect.moveTo(x, y);
}

void ComputerPaddle::resetState() {

    rect.moveTo(INITIAL_X, INITIAL_Y);

}

QRect ComputerPaddle::getRect() {
  return rect;
}

int ComputerPaddle::getTop() {
  return rect.top();
}

int ComputerPaddle::getBottom() {
  return rect.bottom();
}

QImage & ComputerPaddle::getImage() {
  return imagex;
}

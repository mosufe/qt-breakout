#include <iostream>
#include "resources.h"

Ball::Ball():QObject()
{
  image.load(":/images/ball.png");

  rect = image.rect();
  resetState(PLAYER_SIDE);
}

Ball::~Ball() {

  std::cout << ("Ball deleted") << std::endl;
}

void Ball::autoMove() {

  rect.translate(xdir*speed, ydir*speed);
/*
  if (rect.left() <= LEFT_EDGE) {
    xdir = -xdir;
    emit border(OTHER_COLLISION);
  }
*/
  if (rect.bottom() >= BOTTOM_EDGE) {
    ydir = -ydir;
    emit border(BOTTOM_BORDER_COLLISION);
  }

  if (rect.top() <= TOP_EDGE) {
    ydir = -ydir;
    emit border(TOP_BORDER_COLLISION);
  }
}

void Ball::resetState(int sideOf) {
    if(sideOf == COMPUTER_SIDE){
        rect.moveTo(INITIAL_COMP_X, INITIAL_Y);
        xdir = INITIAL_COMP_X_SPEED;
    }
    else{
        rect.moveTo(INITIAL_X, INITIAL_Y);
        xdir = INITIAL_X_SPEED;
    }
  ydir = 0;
}

void Ball::setXDir(int x) {

  xdir = x;
}

void Ball::setYDir(int y) {

  ydir = y;
}

int Ball::getXDir() {

  return xdir;
}

int Ball::getYDir() {

  return ydir;
}

int Ball::getSpeed(){
   return speed;
}

void Ball::setSpeed(int newSpeed){
   speed = newSpeed;
}

QRect Ball::getRect() {

  return rect;
}

QImage & Ball::getImage() {

  return image;
}

void Ball::changeSpeed(int newSpeed){
    if(newSpeed == speed) return;
    speed = newSpeed;
}

void Ball::moveBall(int x, int y){
    rect.moveTo(x,y);
}

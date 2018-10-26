#include <iostream>
#include "resources.h"

Paddle::Paddle() {
  dy = 0;
  if(owner == PLAYER_PADDLE)
      image.load("://images/RightPaddle.png");
  else
      image.load("://images/LeftPaddle.png");

  rect = image.rect();
  resetState();

  // seed the number generator for this paddle
  time = QTime::currentTime();
  qsrand(static_cast<uint>(time.msec()));

}

Paddle::~Paddle() {

 std::cout << ("Paddle deleted") << std::endl;
}

void Paddle::setDy(int y) {
  dy = y;
}

void Paddle::move() {

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

void Paddle::resetState() {
    if(owner == PLAYER_PADDLE)
        rect.moveTo(INITIAL_X, INITIAL_Y);
    else
        rect.moveTo(0,300);
}

QRect Paddle::getRect() {
  return rect;
}

int Paddle::getTop() {
  return rect.top();
}

int Paddle::getBottom() {
  return rect.bottom();
}

QImage & Paddle::getImage() {
  return image;
}

void Paddle::setOwner(int newOwner){
    owner = newOwner;
}

int Paddle::getOwner(){
    return(owner);
}

void Paddle::setBehavior(int newBehavior){
    behavior = newBehavior;
}

int Paddle::getBehavior(){
    return(behavior);
}

int Paddle::getCentralX(){
    if (getBehavior() == COMPUTER_BEHAVIOR)
        return(0 - PADDLE_RADIUS);
    else
        return(INITIAL_X + PADDLE_RADIUS);
}

void Paddle::loadImage(int owner){
    if(owner == PLAYER_PADDLE)
        image.load("://images/RightPaddle.png");
    else
        image.load("://images/LeftPaddle.png");

    rect = image.rect();
    resetState();
}

void Paddle::prepareToMove(Ball *gameBall){
    // Control the time in which the reflex will remain false. Paddle will be
    //blind to the ball.
    if (behavior == PLAYER_BEHAVIOR) return; //this function is only for computer players
    if(reflex == true){
        int delayNumber = qrand() % difficulty;
        reflex = false;
        timeToNextReflex = delayNumber;
    }
    else if(--timeToNextReflex < 1){
        reflex = true;
        yTarget = gameBall->getRect().top();
    }
    // Paddle has seen the ball. Set the target to follow it.
    if(reflex == true){
        //align the top border with the ball's top.
        if(rect.top() < yTarget)
            dy = 3;
        else if(rect.top() > yTarget)
            dy = -3;
        else
            dy = 0;
    }
}

void Paddle::movePaddle(int x, int y){
    rect.moveTo(x,y);
}

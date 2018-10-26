#include <QPainter>
#include <QApplication>
#include "resources.h"
#include <QtMath>
#include <QFileDialog>
#include <QMessageBox>

Breakout::Breakout(QWidget *parent)
    : QWidget(parent) {

  x = 0;
  gameOver = false;
  gameWon = false;
  paused = false;
  gameStarted = false;
  ball = new Ball();
  compPaddle = new Paddle();
  paddle = new Paddle();
  compPaddle->setOwner(COMPUTER_PADDLE);
  paddle->setOwner(PLAYER_PADDLE);
  compPaddle->setBehavior(COMPUTER_PADDLE);
  paddle->setBehavior(COMPUTER_PADDLE);
  setMouseTracking(true);

  int k = 0;

  for (int i=0; i<5; i++) {
    for (int j=0; j<6; j++) {
      bricks[k] = new Brick(j*40+380, i*40+130);
      k++;
    }
  }
    // load different images fot the rounded paddles.
    paddle->loadImage(PLAYER_PADDLE);
    compPaddle->loadImage(COMPUTER_PADDLE);  
}

Breakout::~Breakout() {

 delete ball;
 delete paddle;
 delete compPaddle;

 for (int i=0; i<N_OF_BRICKS; i++) {
   delete bricks[i];
 }
}

void Breakout::paintEvent(QPaintEvent *e) {

  Q_UNUSED(e);

  QPainter painter(this);

  if (gameOver || gameWon ) {
    if(computerPoints>playerPoints)
        finishGame(&painter, "Computer wins");
    else if(playerPoints>computerPoints)
        finishGame(&painter, "You win");
    else
        finishGame(&painter, "Draw");
  }
  else {
    drawObjects(&painter);
  }
}

void Breakout::finishGame(QPainter *painter, QString message) {

  QFont font("Courier", 15, QFont::DemiBold);
  QFontMetrics fm(font);
  int textWidth = fm.width(message);

  painter->setFont(font);
  int h = height();
  int w = width();

  painter->translate(QPoint(w/2, h/2));
  painter->drawText(-textWidth/2, 0, message);
}

void Breakout::drawObjects(QPainter *painter) {

  painter->drawImage(ball->getRect(), ball->getImage());
  painter->drawImage(paddle->getRect(), paddle->getImage());
  painter->drawImage(compPaddle->getRect(), compPaddle->getImage());

  for (int i=0; i<N_OF_BRICKS; i++) {
    if (!bricks[i]->isDestroyed()) {
      painter->drawImage(bricks[i]->getRect(), bricks[i]->getImage());
    }
  }
}

void Breakout::timerEvent(QTimerEvent *e) {

  Q_UNUSED(e);

  moveObjects();
  checkCollision();
  repaint();
}

void Breakout::moveObjects() {

  // move the ball according to its velocity
  ball->autoMove();

  // for paddles, verify if they have "seen the ball" (reflex) by luck.
  // if so, they will chase it.
  paddle->prepareToMove(ball);
  paddle->move();
  compPaddle->prepareToMove(ball);
  compPaddle->move();
}

void Breakout::keyReleaseEvent(QKeyEvent *e) {

    if(paddle->getBehavior() == COMPUTER_BEHAVIOR) return;
    int dy = 0;

    switch (e->key()) {
        case Qt::Key_Up:
            dy = 0;
            paddle->setDy(dy);
            break;

        case Qt::Key_Down:
            dy = 0;
            paddle->setDy(dy);
            break;
    }
}

void Breakout::keyPressEvent(QKeyEvent *e) {

    int dy = 0;

    switch (e->key()) {
    case Qt::Key_Up:

        if(paddle->getBehavior() == COMPUTER_BEHAVIOR) break;
        dy = -3;
        paddle->setDy(dy);
        break;

    case Qt::Key_Down:

        if(paddle->getBehavior() == COMPUTER_BEHAVIOR) break;
        dy = 3;
        paddle->setDy(dy);
        break;

    case Qt::Key_P:

        togglePause();
        break;

    case Qt::Key_Space:

        startGame();
        break;

    case Qt::Key_Escape:

        qApp->exit();
        break;

    default:
        QWidget::keyPressEvent(e);
    }
}

void Breakout::mouseMoveEvent(QMouseEvent *e){
    if(paddle->getBehavior() == COMPUTER_BEHAVIOR) return;
    int dy = 0;
    if(e->y() < paddle->getTop()-3)
        dy = -3;
    else if(e->y() > paddle->getBottom()+3)
        dy = 3;
    paddle->setDy(dy);
}

void Breakout::startDemoMode() {
    if (gameMode == DEMO_MODE) return;
    gameMode = DEMO_MODE;
    stopGame();
    startGame();
    emit newModeStarted("(DEMO MODE)");
}

void Breakout::startPlayMode() {
    if (gameMode == PLAY_MODE) return;
    gameMode = PLAY_MODE;
    stopGame();
    startGame();
    emit newModeStarted("(PLAY MODE)");
}

void Breakout::startGame() {

  if (!gameStarted) {
    ball->resetState(PLAYER_SIDE);
    paddle->resetState();
    compPaddle->resetState();

    // check game mode to see if paddle will have independent moves
    if(gameMode == DEMO_MODE)
        paddle->setBehavior(COMPUTER_BEHAVIOR);
    else
        paddle->setBehavior(PLAYER_BEHAVIOR);

    for (int i=0; i<N_OF_BRICKS; i++) {
      bricks[i]->setDestroyed(false);
    }

    gameOver = false;
    gameWon = false;
    if(paused == true){
        killTimer(timerId);
        paused = false;
    }
    playerPoints = 0;
    computerPoints = 0;
    remainingBalls = 6;
    emit scoreChanged(computerPoints, playerPoints, remainingBalls);
    gameStarted = true;
    timerId = startTimer(DELAY);
  }
}

void Breakout::togglePause() {
    if (gameOver == false && gameStarted == true) {
        if (paused) {

          timerId = startTimer(DELAY);
          paused = false;
        } else {
          paused = true;
          killTimer(timerId);
        }
    }
}

bool Breakout::isPaused(){
    return(paused);
}

void Breakout::stopGame() {

  killTimer(timerId);
  gameOver = true;
  gameStarted = false;

}

void Breakout::victory() {

  killTimer(timerId);
  gameWon = true;
  gameStarted = false;

}

void Breakout::checkCollision() {

  if (ball->getRect().right() > RIGHT_EDGE) {
    remainingBalls--;
    playerPoints -= 10;
    emit scoreChanged(computerPoints, playerPoints, remainingBalls);
    if(remainingBalls <= 0)
        stopGame();
    else{
        ball->resetState(PLAYER_SIDE);
        paddle->resetState();
        //compPaddle->resetState();
    }
  }

  else if (ball->getRect().left() < LEFT_EDGE) {
    remainingBalls--;
    computerPoints -= 10;
    emit scoreChanged(computerPoints, playerPoints, remainingBalls);
    if(remainingBalls <= 0)
        stopGame();
    else{
        ball->resetState(COMPUTER_SIDE);
        //paddle->resetState();
        compPaddle->resetState();
    }
  }

  for (int i=0, j=0; i<N_OF_BRICKS; i++) {

    if (bricks[i]->isDestroyed()) {
      j++;
    }

    if (j == N_OF_BRICKS) {
      victory();
    }
  }

  // first, treat collisions with paddles
  if ((ball->getRect()).intersects(paddle->getRect())) {
    lastCollision = PADDLE_COLLISION;
    collide(ball, paddle);
  }

  else if ((ball->getRect()).intersects(compPaddle->getRect())) {
    lastCollision = PADDLE_COLLISION;
    collide(ball, compPaddle);
  }

  // then, treat collision with bricks
  for (int i=0; i<N_OF_BRICKS; i++) {

    if ((ball->getRect()).intersects(bricks[i]->getRect())) {

      int ballLeft = ball->getRect().left();
      int ballHeight = ball->getRect().height();
      int ballWidth = ball->getRect().width();
      int ballTop = ball->getRect().top();

      QPoint pointRight(ballLeft + ballWidth + 1, ballTop);
      QPoint pointLeft(ballLeft - 1, ballTop);
      QPoint pointTop(ballLeft, ballTop -1);
      QPoint pointBottom(ballLeft, ballTop + ballHeight + 1);

      if (!bricks[i]->isDestroyed()) {
        if(bricks[i]->getRect().contains(pointRight) ||
           bricks[i]->getRect().contains(pointLeft)) {
           ball->setXDir(-ball->getXDir());
        }

        if(bricks[i]->getRect().contains(pointTop) ||
           bricks[i]->getRect().contains(pointBottom)   ) {
           ball->setYDir(-ball->getYDir());
        }

        bricks[i]->setDestroyed(true);
        // add points to whoever destroyed a brick
        lastCollision = BRICK_COLLISION;
        if(lastCollider == PLAYER_PADDLE)
            playerPoints += 2;
        else
            computerPoints += 2;
        emit scoreChanged(computerPoints, playerPoints, remainingBalls);
      }
    }
  }
}

void Breakout::touchedBorder(int collisionType){
    // avoid a bug which happens when the ball is moving near the
    // border with 0 y-speed adding infinite points.
    // all collisions of the same type as the last collision will be ignored.
    if(lastCollision == BRICK_COLLISION ||
            lastCollision != collisionType){
        if(lastCollider == PLAYER_PADDLE)
            playerPoints += 1;
        else
            computerPoints += 1;
        lastCollision = collisionType;
        emit scoreChanged(computerPoints, playerPoints, remainingBalls);
    }
}

/*
------------------------------------------------------------------
   void Breakout::collide(Ball *gameBall, Paddle *collidingPaddle)
------------------------------------------------------------------
   This function has pointers to the entities that collided.
   The vector algebra here results in an output vector that
   has the same reflection angle as the incidence one. This
   vector is passed to the ball's velocity variables.
------------------------------------------------------------------
*/

void Breakout::collide(Ball *gameBall, Paddle *collidingPaddle){
    lastCollider = collidingPaddle->getOwner();
    //int paddleLPos = collidingPaddle->getRect().left();
    int ballLPos = gameBall->getRect().left();
    int paddleBPos = collidingPaddle->getRect().bottom();
    int paddleTPos = collidingPaddle->getRect().top();
    //int ballTPos = gameBall->getRect().top();
    int ballBPos = gameBall->getRect().bottom();
    //int ballY = gameBall->getYDir();
    //int ballX = gameBall->getXDir();

    //find the center of the circle in the paddle
    int yc = (paddleTPos + paddleBPos)/2;
    int xc = collidingPaddle->getCentralX(); //considering paddle initial position & radius
    //approximate ball's position to its bottom left
    int xpos = ballLPos;
    int ypos = ballBPos;

    //store an incident vector
    int viX = (xc-xpos);
    int viY = (yc-ypos);

    //calculate normal vector
    qreal vecMod = qSqrt(qPow(viX,2)+qPow(viY,2));
    qreal xNorm = (viX)/vecMod;
    qreal yNorm = (viY)/vecMod;

    // Vo' = Vi' - 2(Vi'*N')*N'
    qreal voX = gameBall->getXDir() - 2 * (gameBall->getXDir()*xNorm + gameBall->getYDir()*yNorm) * xNorm;
    qreal voY = gameBall->getYDir() - 2 * (gameBall->getXDir()*xNorm + gameBall->getYDir()*yNorm) * yNorm;

    gameBall->setXDir(qRound(voX));
    gameBall->setYDir(qRound(voY));
}

int Breakout::getGameMode(){
    return gameMode;
}

/*
----------------------------------------------------------------------------
   void Breakout::loadData()
   void Breakout::saveData()
----------------------------------------------------------------------------
   Stores all game information in a GameData struct and then save
   it into the computer.

   Code inspired in the "qt.io" website
   http://doc.qt.io/qt-5/qtwidgets-tutorials-addressbook-part6-example.html
----------------------------------------------------------------------------
*/

void Breakout::loadData(){
    // If the game is in demo mode, just start play mode
    if(gameMode == DEMO_MODE)
        startPlayMode();

    // else, see if user wishes to save the current state
    else{
        if(paused == false)
            togglePause();
        QMessageBox::StandardButton answer;
        answer = QMessageBox::question(this,"Save current game",
                 "Do you wish to save your game first?",QMessageBox::Yes|
                                       QMessageBox::No|QMessageBox::Cancel);
        if(answer == QMessageBox::Yes){
            saveData();
        }
        else if(answer == QMessageBox::Cancel){
            return;
        }
    }
    if(paused == false)
        togglePause();
    QString breakoutFileName = QFileDialog::getOpenFileName(this,
                               tr("Open Breakout game"), "",
                               tr("Breakout save file (*.bkt);;All Files (*)"));
    if(breakoutFileName.isEmpty()) return;
    else{

        QFile loadFile(breakoutFileName);

        if(!loadFile.open(QIODevice::ReadOnly)){
            QMessageBox::information(this, tr("Could not open requested file."),
                                     loadFile.errorString());
            return;
        }

        QDataStream in(&loadFile);
        in.setVersion(QDataStream::Qt_4_5);

        //Load each piece of information from save struct of type GameData
        in >> toBeSaved.playerPoints;
        in >> toBeSaved.computerPoints;
        in >> toBeSaved.remainingBalls;
        for(int i=0;i<30;i++){
            in >> toBeSaved.activeBrick[i];
        }
        in >> toBeSaved.ballXPos;
        in >> toBeSaved.ballYPos;
        in >> toBeSaved.ballXDir;
        in >> toBeSaved.ballYDir;
        in >> toBeSaved.paddleXPos;
        in >> toBeSaved.paddleYPos;
        in >> toBeSaved.compPaddleXPos;
        in >> toBeSaved.compPaddleYPos;
        in >> toBeSaved.speed;
    }

    // Load all fetched information to the game
    playerPoints = toBeSaved.playerPoints;
    computerPoints = toBeSaved.computerPoints;
    remainingBalls = toBeSaved.remainingBalls;
    for(int i=0; i<30; i++){
        bricks[i]->setDestroyed(!(toBeSaved.activeBrick[i]));
    }
    ball->moveBall(toBeSaved.ballXPos, toBeSaved.ballYPos);
    ball->setXDir(toBeSaved.ballXDir);
    ball->setYDir(toBeSaved.ballYDir);
    paddle->movePaddle(toBeSaved.paddleXPos, toBeSaved.paddleYPos);
    compPaddle->movePaddle(toBeSaved.compPaddleXPos, toBeSaved.compPaddleYPos);
    ball->setSpeed(toBeSaved.speed);
    emit speedChanged(ball->getSpeed());
    emit scoreChanged(computerPoints, playerPoints, remainingBalls);
    emit newModeStarted("(PLAY MODE)");
    repaint();
}

void Breakout::saveData(){
    if(gameMode == DEMO_MODE) return;
    if(paused == false)
        togglePause();
    toBeSaved.playerPoints = playerPoints;
    toBeSaved.computerPoints = computerPoints;
    toBeSaved.remainingBalls = remainingBalls;
    for(int i=0; i<30; i++){
        toBeSaved.activeBrick[i] = !(bricks[i]->isDestroyed());
    }
    toBeSaved.ballXPos = ball->getRect().left(); //top left position will be stored
    toBeSaved.ballYPos = ball->getRect().top();
    toBeSaved.ballXDir = ball->getXDir();
    toBeSaved.ballYDir = ball->getYDir();
    toBeSaved.paddleXPos = paddle->getRect().left();
    toBeSaved.paddleYPos = paddle->getRect().top();
    toBeSaved.compPaddleXPos = compPaddle->getRect().left();
    toBeSaved.compPaddleYPos = compPaddle->getRect().top();
    toBeSaved.speed = ball->getSpeed();

    QString breakoutFileName = QFileDialog::getSaveFileName(this,
                       tr("Save Breakout game"), "",
                       tr("Breakout save file (*.bkt);;All Files (*)"));

    if(breakoutFileName.isEmpty()) return;
    else{
        QFile saveFile(breakoutFileName); //chosen by user
        if(!saveFile.open(QIODevice::WriteOnly)) {
            QMessageBox::information(this, tr("Could not open requested file."),
                    saveFile.errorString());
            return;
        }
        QDataStream out(&saveFile);
        out.setVersion(QDataStream::Qt_4_5);

        //Write each piece of information from save struct of type GameData
        out << toBeSaved.playerPoints;
        out << toBeSaved.computerPoints;
        out << toBeSaved.remainingBalls;
        for(int i=0;i<30;i++){
            out << toBeSaved.activeBrick[i];
        }
        out << toBeSaved.ballXPos;
        out << toBeSaved.ballYPos;
        out << toBeSaved.ballXDir;
        out << toBeSaved.ballYDir;
        out << toBeSaved.paddleXPos;
        out << toBeSaved.paddleYPos;
        out << toBeSaved.compPaddleXPos;
        out << toBeSaved.compPaddleYPos;
        out << toBeSaved.speed;
    }

}



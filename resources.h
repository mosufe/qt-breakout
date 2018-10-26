#include <QImage>
#include <QRect>
#include <QObject>
#include <QWidget>
#include <QKeyEvent>
#include <QSlider>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QWidget>
#include <QMainWindow>
#include "ui_mainwindow.h"
#include <QTime>

#ifndef RESOURCES_H
#define RESOURCES_H

    static const int TOP_EDGE = 0;
    static const int BOTTOM_EDGE = 400;
    static const int LEFT_EDGE = 0;
    static const int RIGHT_EDGE = 1000;
    static const int COMPUTER_PADDLE = 0;
    static const int PLAYER_PADDLE = 1;
    static const int COMPUTER_BEHAVIOR = 0;
    static const int PLAYER_BEHAVIOR = 1;
    static const int COMPUTER_SIDE = 0;
    static const int PLAYER_SIDE = 1;
    static const int DEMO_MODE = 0;
    static const int PLAY_MODE = 1;


/*
********************************************************************************
 class Ball
********************************************************************************
*/


class Ball : public QObject
{

    Q_OBJECT

  public:
    Ball();
    virtual ~Ball();

  public:
    void resetState(int sideOf);
    void autoMove();
    void moveBall(int x, int y);
    void setXDir(int);
    void setYDir(int);
    int getXDir();
    int getYDir();
    QRect getRect();
    QImage & getImage();
    int getSpeed();
    void setSpeed(int newSpeed);

  private:
    int xdir;
    int ydir;
    QImage image;
    QRect rect;
    static const int INITIAL_X = 900;
    static const int INITIAL_COMP_X = 50;
    static const int INITIAL_Y = 330;
    static const int INITIAL_X_SPEED = -3;
    static const int INITIAL_COMP_X_SPEED = 3;
    static const int RIGHT_EDGE = 1000;
    static const int LEFT_EDGE = 0;
    static const int TOP_EDGE = 0;
    static const int BOTTOM_EDGE = 400;
    static const int TOP_BORDER_COLLISION = 0;
    static const int BOTTOM_BORDER_COLLISION = 1;
    static const int PADDLE_COLLISION = 2;
    static const int BRICK_COLLISION = 3;
    static const int OTHER_COLLISION = 4;
    int speed = 1;

signals:
    void border(int collisionType);

public slots:
    void changeSpeed(int newSpeed);

};



/*
********************************************************************************
 class Brick
********************************************************************************
*/

class Brick {

  public:
    Brick(int, int);
    ~Brick();

  public:
    bool isDestroyed();
    void setDestroyed(bool);
    QRect getRect();
    void setRect(QRect);
    QImage & getImage();

  private:
    QImage image;
    QRect rect;
    bool destroyed;
};



/*
********************************************************************************
 class MainWindow
********************************************************************************
*/

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    virtual ~MainWindow();
    void OnStartDemoMode();
    void OnStartPlayMode();
    void OnLoadFile();
    void OnSaveFile();

private:
    Ui::MainWindow *ui;
    void keyPressEvent(QKeyEvent *e);
    void keyReleaseEvent(QKeyEvent *e);

public slots:
    void menuSelected(); //pause when menu selected
    void menuDeselected(); //unpause when back to game


};

/*
********************************************************************************
 class Paddle
********************************************************************************
*/

class Paddle {

  public:
    Paddle();
    ~Paddle();

  public:
    void resetState();
    void move();
    void movePaddle(int x, int y);
    void setDy(int);
    QRect getRect();
    int getTop();
    int getBottom();
    QImage & getImage();
    void setOwner(int newOwner);
    int getOwner();
    void setBehavior(int newBehavior);
    int getBehavior();
    void loadImage(int owner);
    static const int PADDLE_RADIUS = 63;
    int getCentralX();

    void setReflex(bool newReflex);
    bool getReflex();
    void setYTarget(int newYTarget);
    int getYTarget();

    void prepareToMove(Ball *gameBall);

  private:
    QImage image;
    QRect rect;
    int dy;
    static const int INITIAL_X = 950;
    static const int INITIAL_Y = 300;
    int speed = 1;
    int owner = 0;
    int behavior = 0;
    bool reflex = true; //for computer paddles, they have "seen" the ball
    int yTarget = 0;
    QTime time;
    int timeToNextReflex = 0;
    int difficulty = 30;
};

/*
********************************************************************************
 class Breakout
********************************************************************************
*/

class Breakout : public QWidget{

    Q_OBJECT

  public:
    Breakout(QWidget *parent = 0);
    ~Breakout();
    bool isPaused();
    int getGameMode();
    void loadData();
    void saveData();

  protected:
    void paintEvent(QPaintEvent *);
    void timerEvent(QTimerEvent *);
    void keyPressEvent(QKeyEvent *);
    void keyReleaseEvent(QKeyEvent *);
    void mouseMoveEvent(QMouseEvent *e);
    void drawObjects(QPainter *);
    void finishGame(QPainter *, QString);
    void moveObjects();

    void startGame();
    void togglePause();
    void stopGame();
    void victory();
    void checkCollision();

  private:
    int x;
    int timerId;
    static const int N_OF_BRICKS = 30;
    static const int DELAY = 10;
    static const int TOP_BORDER_COLLISION = 0;
    static const int BOTTOM_BORDER_COLLISION = 1;
    static const int PADDLE_COLLISION = 2;
    static const int BRICK_COLLISION = 3;

    int lastCollision = 3;
    int lastCollider = 1; //last player who hit the ball
    void collide(Ball *ball, Paddle *paddle);

    Ball *ball;
    Paddle *paddle;
    Paddle *compPaddle;
    Brick *bricks[N_OF_BRICKS];
    bool gameOver;
    bool gameWon;
    bool gameStarted;
    bool paused;
    // score memory
    int playerPoints = 0;
    int computerPoints = 0;
    int remainingBalls = 6;
    int gameMode = DEMO_MODE; // game begins in the demo mode

    struct GameData{
        int playerPoints;
        int computerPoints;
        int remainingBalls;
        bool activeBrick[30];
        int ballXPos;
        int ballYPos;
        int ballXDir;
        int ballYDir;
        int paddleXPos;
        int paddleYPos;
        int compPaddleXPos;
        int compPaddleYPos;
        int speed;
    };

    GameData toBeSaved;

public slots:
    void touchedBorder(int collisionType);
    void startDemoMode();
    void startPlayMode();

signals:
    void scoreChanged(int computerPoints, int playerPoints, int remainingBalls);
    void speedChanged(int newSpeed);
    void newModeStarted(QString message);
    // let MainWindow access all methods from the game,
    // since it will use numerous protected functions

    friend class MainWindow;
    friend class Display;
};

/*
********************************************************************************
 class Display
********************************************************************************
*/

class Display : public QWidget
{
    Q_OBJECT
public:
    Display(QWidget *parent = 0);
    ~Display();

    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout;
    bool eventFilter(QObject *object, QEvent *event);
    void getGameArea(Breakout *theGameArea);
    QSlider *velSlider;

private:
    QLabel *playerScore;
    QLabel *playerScoreTag;
    QLabel *computerScore;
    QLabel *computerScoreTag;
    QLabel *remainingBalls;
    QLabel *remainingBallsTag;
    QLabel *speedTag;
    QLabel *gameModeTag;
    Breakout *gameArea; //reference the game area here

public slots:
    void updateScoring(int compScore, int plScore, int remBalls);
    void setGameModeText(QString text);
};

#endif // RESOURCES_H

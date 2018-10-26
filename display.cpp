#include "resources.h"

Display::Display(QWidget *parent):QWidget(parent)
{
    velSlider = new QSlider(Qt::Horizontal);
    velSlider->setMinimum(1.0);
    velSlider->setMaximum(3.0);
    remainingBallsTag = new QLabel("Remaining balls: ");
    remainingBalls = new QLabel("6");
    playerScore = new QLabel("0");
    playerScoreTag = new QLabel("YOU");
    computerScore = new QLabel("0");
    computerScoreTag = new QLabel("COMPUTER");
    horizontalLayout = new QHBoxLayout();
    verticalLayout = new QVBoxLayout();

    //set fonts
    QFont tags( "Arial", 18, QFont::Bold);
    QFont scores( "Courier New", 20, QFont::Bold);
    remainingBallsTag->setFont(tags);
    playerScoreTag->setFont(tags);
    computerScoreTag->setFont(tags);
    remainingBalls->setFont(scores);
    playerScore->setFont(scores);
    computerScore->setFont(scores);

    //set background colors

    playerScoreTag->setStyleSheet("QLabel { background-color : rgb(80,0,80); color : rgb(255,255,255) }");
    computerScoreTag->setStyleSheet("QLabel { background-color : rgb(80,0,80); color : rgb(255,255,255) }");
    playerScore->setStyleSheet("QLabel { background-color : rgb(100,0,100); color : rgb(200,200,200) }");
    computerScore->setStyleSheet("QLabel { background-color : rgb(100,0,100); color : rgb(200,200,200) }");

    //set alignment
    remainingBalls->setAlignment(Qt::AlignLeft);
    remainingBallsTag->setAlignment(Qt::AlignRight);
    playerScore->setAlignment(Qt::AlignCenter);
    playerScoreTag->setAlignment(Qt::AlignCenter);
    computerScore->setAlignment(Qt::AlignCenter);
    computerScoreTag->setAlignment(Qt::AlignCenter);

    //create layout of a group points-tag
    QHBoxLayout *ballLayout = new QHBoxLayout();
    QVBoxLayout *computerLayout = new QVBoxLayout();
    QVBoxLayout *playerLayout = new QVBoxLayout();

    ballLayout ->addWidget(remainingBallsTag);
    ballLayout ->addWidget(remainingBalls);
    ballLayout->setAlignment(Qt::AlignCenter);
    computerLayout->addWidget(computerScoreTag);
    computerLayout->addWidget(computerScore);
    playerLayout->addWidget(playerScoreTag);
    playerLayout->addWidget(playerScore);

    //points layout
    horizontalLayout->addLayout(computerLayout);
    horizontalLayout->addLayout(playerLayout);

    //speed and game mode tag
    QHBoxLayout *speedAndGameModeLayout = new QHBoxLayout();
    QVBoxLayout *speedyLayout = new QVBoxLayout();
    speedTag = new QLabel("Ball speed");
    speedTag->setAlignment(Qt::AlignCenter);
    speedTag->setFont(tags);
    speedyLayout->addWidget(speedTag);
    speedyLayout->addWidget(velSlider);
    velSlider->setMaximumWidth(400);
    gameModeTag = new QLabel("(DEMO MODE)");
    gameModeTag->setAlignment(Qt::AlignCenter);
    gameModeTag->setFont(tags);
    speedAndGameModeLayout->addLayout(speedyLayout);
    speedAndGameModeLayout->addWidget(gameModeTag);

    //master layout
    verticalLayout->addLayout(horizontalLayout);
    verticalLayout->addLayout(ballLayout);
    verticalLayout->addLayout(speedAndGameModeLayout);
    this->setLayout(verticalLayout);

    //install filter
    velSlider->installEventFilter(this);

}

Display::~Display() {


}

void Display::updateScoring (int compScore, int plScore, int remBalls){
    computerScore->setText(QString::number(compScore));
    playerScore->setText(QString::number(plScore));
    remainingBalls->setText(QString::number(remBalls));
}

bool Display::eventFilter(QObject *object, QEvent *e){
      if(e->type() == QKeyEvent::KeyPress){
          QKeyEvent *theEvent = static_cast<QKeyEvent*>(e);
          gameArea->keyPressEvent(theEvent);
          if(theEvent->key() == Qt::Key_Up
             || theEvent->key() == Qt::Key_Down
             || theEvent->key() == Qt::Key_P)
          return true;
    }
    else if(e->type() == QKeyEvent::KeyRelease){
          QKeyEvent *theEvent = static_cast<QKeyEvent*>(e);
          gameArea->keyReleaseEvent(theEvent);
          if(theEvent->key() == Qt::Key_Up
             || theEvent->key() == Qt::Key_Down
             || theEvent->key() == Qt::Key_P)
          return true;
    }
    return false;
}

void Display::getGameArea(Breakout *theGameArea){
    gameArea = theGameArea; //get reference for calling key press event
}

void Display::setGameModeText(QString newText){
    gameModeTag->setText(newText);
}

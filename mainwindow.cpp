//#include "mainwindow.h"
//#include "breakout.h"
//#include "display.h"
//#include "keyfilter.h"
#include <QGridLayout>
#include <QKeyEvent>
#include "resources.h"

static Breakout *gameArea;
static Display *scoreArea;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->resize(1000,700);
    this->setWindowTitle("Breakout");

/*
------------------------------------------------------------
   The Main Window
------------------------------------------------------------
   The main window will contain all the menu items in the
   user interface. The central widget will contain one
   instance of the game itself present in the Breakout
   class followed by one instance of the score board.
------------------------------------------------------------
*/

    // create the game menu

    // part 1: file options
    QMenu * menuFile = menuBar()->addMenu("&File");
    QAction * openFile = new QAction("&Open",this);
    QAction * saveFile = new QAction("&Save",this);
    QAction * quit = new QAction("&Quit",this);
    menuFile->addAction(openFile);
    menuFile->addAction(saveFile);
    menuFile->addAction(quit);

    // part 2: game mode options
    QMenu * menuGameMode = menuBar()->addMenu("&Game Mode");
    QAction * demoMode = new QAction("&Demo",this);
    QAction * playMode = new QAction("&Play", this);
    menuGameMode->addAction(demoMode);
    menuGameMode->addAction(playMode);

    // create the tool bar
    QToolBar *toolBar = addToolBar("File");
    toolBar->addAction(openFile);
    toolBar->addAction(saveFile);
    toolBar->addAction(demoMode);
    toolBar->addAction(playMode);

    /*
    ------------------------------------------------------------
       The Slots
    ------------------------------------------------------------
       Each menu function will have its appropriate slot
       performing the requested action. Quit will default to
       qApp's quit function.
    ------------------------------------------------------------
    */

    connect(quit, &QAction::triggered, qApp, QApplication::quit);

    //pause game when menu is clicked
    connect(menuFile, SIGNAL(aboutToShow()), this, SLOT(menuSelected()));
    connect(menuGameMode, SIGNAL(aboutToShow()), this, SLOT(menuSelected()));

    //unpause game when control is back to game
    connect(menuFile, SIGNAL(aboutToHide()), this, SLOT(menuDeselected()));
    connect(menuGameMode, SIGNAL(aboutToHide()), this, SLOT(menuDeselected()));

    //set up the different fields found in the widget
    gameArea = new Breakout();
    scoreArea = new Display();
    QGridLayout * masterLayout = new QGridLayout();
    QWidget * masterWidget = new QWidget();
    masterLayout->addWidget(gameArea,0,0);
    masterLayout->addWidget(scoreArea,1,0);
    masterLayout->setRowStretch(0,1);
    masterLayout->setRowStretch(1,0);
    masterWidget->setLayout(masterLayout);

    MainWindow::setCentralWidget(masterWidget);
    connect(gameArea, SIGNAL(scoreChanged(int,int,int)), scoreArea, SLOT(updateScoring(int,int,int)));
    connect(gameArea->ball, SIGNAL(border(int)), gameArea, SLOT(touchedBorder(int)));
    connect(scoreArea->velSlider, SIGNAL(valueChanged(int)), gameArea->ball, SLOT(changeSpeed(int)));
    connect(gameArea, SIGNAL(speedChanged(int)), scoreArea->velSlider, SLOT(setValue(int)));
    connect(gameArea, SIGNAL(newModeStarted(QString)), scoreArea, SLOT(setGameModeText(QString)));

    //connect the game area to the display
    scoreArea->getGameArea(gameArea);

    //connect the other actions to their respective slots.
    connect(playMode, &QAction::triggered, this, &MainWindow::OnStartPlayMode);
    connect(demoMode, &QAction::triggered, this, &MainWindow::OnStartDemoMode);
    connect(openFile, &QAction::triggered, this, &MainWindow::OnLoadFile);
    connect(saveFile, &QAction::triggered, this, &MainWindow::OnSaveFile);
}
MainWindow::~MainWindow()
{
    delete ui;
}

/*
------------------------------------------------------------
   The Events
------------------------------------------------------------
   Since the events are only visible for the main window,
   not the Breakout class, they will be forwarded to the
   game's instance and then be treated there.
------------------------------------------------------------
*/

void MainWindow::keyPressEvent(QKeyEvent *e) {
    gameArea->keyPressEvent(e);
}

void MainWindow::keyReleaseEvent(QKeyEvent *e) {
    gameArea->keyReleaseEvent(e);
}

void MainWindow::menuSelected(){
    if (gameArea->isPaused() == false){
        gameArea->togglePause();
    }
}

void MainWindow::menuDeselected(){
    if (gameArea->isPaused() == true){
        gameArea->togglePause();
    }
}

void MainWindow::OnStartDemoMode(){
    if(gameArea->getGameMode() == DEMO_MODE) return;
    gameArea->startDemoMode();
}

void MainWindow::OnStartPlayMode(){
    if(gameArea->getGameMode() == PLAY_MODE) return;
    gameArea->startPlayMode();
}

void MainWindow::OnLoadFile(){
    gameArea->loadData();
}

void MainWindow::OnSaveFile(){
    gameArea->saveData();
}



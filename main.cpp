#include <QApplication>
//#include "mainwindow.h"
//#include "breakout.h"
//#include "display.h"
#include "resources.h"


int main(int argc, char *argv[]) {

  QApplication app(argc, argv);

  MainWindow mainWindow;
  mainWindow.show();

/*
  Display *disp = new Display();
  QWidget window;
  window.setLayout(disp->verticalLayout);
  window.show();
*/

  return app.exec();
}

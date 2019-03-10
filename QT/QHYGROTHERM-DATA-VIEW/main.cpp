#include "mainwindow.h"
#include "serialDialog.h"
#include <QApplication>
//#include <QTextCodec>

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  MainWindow w;
  //Dialog serialDialog;
  //serialDialog.show();
  w.show();
  //QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
  return a.exec();
}

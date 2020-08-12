#include "widget.h"

#include <QApplication>
#include <QDebug>
#include <QDir>

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  qDebug() << a.applicationDirPath() << QDir::currentPath();
  Widget w;
  w.show();
  return a.exec();
}

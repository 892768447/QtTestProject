#include "widget.h"

#include <QApplication>

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  MaskWidget w(0);
  w.show();
  return a.exec();
}

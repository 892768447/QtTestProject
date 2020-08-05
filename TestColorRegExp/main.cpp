#include <QCoreApplication>
#include "testcolorregexp.h"

int main(int argc, char *argv[]) {
  QCoreApplication a(argc, argv);
  TestColorRegExp::test();
  return a.exec();
}

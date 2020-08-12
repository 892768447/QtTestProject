#include "testcolorregexp.h"
#include <QDebug>
#include <QFile>
#include <QTime>

TestColorRegExp::TestColorRegExp() {}

void TestColorRegExp::test() {
  QString text =
      "background:rgb(100,200,255);\n"
      "background: RGB(100,200 ,255);\n"
      "background: rGb(   100  ,  200 , 255);\n"
      "background: rgb(100,   200 , 255);\n"
      "background:rGba(100,200,255, 10);  \n"
      "background:rGba(100,200,255, 0.2)   ;\n"
      "background: rgba( 100, 200 , 255  , 23);\n"
      "background: RGBA(100 ,200 ,255, 0.2);\n"
      "border:1px solid   red ; \n"
      "border:1px solid   #FF00ff ; \n"
      "border:1px solid rgb(23,32,45) ; \n"
      "color:   red;\n"
      "color: White ;\n"
      "color: BLACK;\n"
      "color: #fff;\n"
      "color: #fff000;\n"
      "color: #00fff000;\n";

  QRegExp exp1("#([0-9a-fA-F]{3}|[0-9a-fA-F]{6}|[0-9a-fA-F]{8})\\s*;");
  QRegExp exp2("\\s*[^#][a-zA-Z]+\\s*;");
  QRegExp exp(
      "[rR][gG][Bb][Aa]?\\((\\s*(2[0-4][0-9]|25[0-5]|[01]?[0-9][0-9]?)\\s*,\\s*"
      "){2}\\s*(2[0-4][0-9]|25[0-5]|[01]?[0-9][0-9]?)\\s*,?\\s*(0\\.\\d{1,2}|2["
      "0-4][0-9]|25[0-5]|[01]?[0-9][0-9])?\\)\\s*;");
  exp1.setMinimal(false);
  exp2.setMinimal(false);
  exp.setMinimal(false);

  int pos1 = 0;
  int pos2 = 0;
  int pos = 0;

  while ((pos1 = exp1.indexIn(text, pos1)) != -1) {
    qDebug() << pos1 << pos1 + exp1.matchedLength() << exp1.capturedTexts()
             << exp1.pos(0);
    pos1 += exp1.matchedLength();
  }

  qDebug() << "\n\n";

  while ((pos2 = exp2.indexIn(text, pos2)) != -1) {
    qDebug() << pos2 << pos2 + exp2.matchedLength() << exp2.capturedTexts()
             << exp2.pos(0);
    pos2 += exp2.matchedLength();
  }

  qDebug() << "\n\n";

  while ((pos = exp.indexIn(text, pos)) != -1) {
    qDebug() << pos << pos + exp.matchedLength() << exp.capturedTexts()
             << exp.pos(0) << exp.cap(0).length();
    qDebug() << "text: " << text.mid(exp.pos(0), exp.cap(0).length());
    pos += exp.matchedLength();
    //    qDebug() << exp.capturedTexts();
    //    qDebug() << exp.cap(0).remove(QRegExp("[;: ]")).toLower();
  }

  /*
  // 模拟大量文本
  QString css;
  for (int i = 0; i < 2000; i++) css.append(text);
  pos = 0;
  QTime time;
  time.start();
  while ((pos = exp.indexIn(css, pos)) != -1) {
    //      qDebug() << pos << pos + exp.matchedLength() <<
    //      exp.capturedTexts()
    //               << exp.pos(0) << exp.cap(0).length();
    //      qDebug() << "text: " << css.mid(exp.pos(0), exp.cap(0).length());
    css.mid(exp.pos(0), exp.cap(0).length());
    pos += exp.matchedLength();
  }
  //  qDebug() << "use time:" << time.elapsed() / 1000.0 << "s";
  */
}

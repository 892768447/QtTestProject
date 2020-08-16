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
      "border: none;\n"
      "color:   red;\n"
      "color: White ;\n"
      "color: BLACK;\n"
      "color: #fff;\n"
      "color: #fff000;\n"
      "color: #00fff000;\n"
      "color: qlineargradient(spread:pad, x1:0.298, y1:0.289773, x2:0.745192, "
      "y2:0.756, stop:0 rgba(0, 165, 255, 232), stop:1 rgba(255, 255, 255, "
      "255));\n\n\nQWidget { color: qlineargradient(spread:pad, x1:0.298, "
      "y1:0.289773, x2:0.745192, y2:0.756, stop:0 rgba(0, 165, 255, 232), "
      "stop:1 rgba(255, 255, 255, 255)); }\nQWidget { color: "
      "qlineargradient(spread:repeat, x1:0.298, y1:0.289773, x2:0.745192, "
      "y2:0.756, stop:0 rgba(0, 165, 255, 232), stop:1 rgba(255, 255, 255, "
      "255)); }\nQWidget { color: qlineargradient(spread:reflect, x1:0.298, "
      "y1:0.289773, x2:0.745192, y2:0.756, stop:0 rgba(0, 165, 255, 232), "
      "stop:1 rgba(255, 255, 255, 255)); }\nQWidget { color: "
      "qradialgradient(spread:pad, cx:0.5, cy:0.5, radius:0.5, fx:0.5, fy:0.5, "
      "stop:0 rgba(0, 165, 255, 232), stop:1 rgba(255, 255, 255, 255)); "
      "}\nQWidget { color: qradialgradient(spread:repeat, cx:0.5, cy:0.5, "
      "radius:0.5, fx:0.5, fy:0.5, stop:0 rgba(0, 165, 255, 232), stop:1 "
      "rgba(255, 255, 255, 255)); }\nQWidget { color: "
      "qradialgradient(spread:reflect, cx:0.5, cy:0.5, radius:0.5, fx:0.5, "
      "fy:0.5, stop:0 rgba(0, 165, 255, 232), stop:1 rgba(255, 255, 255, "
      "255)); }\nQWidget { color: qconicalgradient(cx:0.5, cy:0.5, angle:0, "
      "stop:0 rgba(0, 165, 255, 232), stop:1 rgba(255, 255, 255, 255)); }\n";

  QRegExp exp1("#([0-9a-fA-F]{3}|[0-9a-fA-F]{6}|[0-9a-fA-F]{8})\\s*;");
  QRegExp exp2("\\s*[^#][a-zA-Z]+\\s*;");
  QRegExp exp3(
      "[rR][gG][Bb][Aa]?\\((\\s*(2[0-4][0-9]|25[0-5]|[01]?[0-9][0-9]?)\\s*,\\s*"
      "){2}\\s*(2[0-4][0-9]|25[0-5]|[01]?[0-9][0-9]?)\\s*,?\\s*(0\\.\\d{1,2}|2["
      "0-4][0-9]|25[0-5]|[01]?[0-9][0-9])?\\)\\s*;");
  QRegExp exp4(
      "([rR][gG][Bb][Aa]?\\((\\s*(2[0-4][0-9]|25[0-5]|[01]?[0-9][0-9]?)\\s*,"
      "\\s*){2}\\s*(2[0-4][0-9]|25[0-5]|[01]?[0-9][0-9]?)\\s*,?\\s*(0\\.\\d{1,"
      "2}|2[0-4][0-9]|25[0-5]|[01]?[0-9][0-9])?\\)\\s*;)|(#([0-9a-fA-F]{3}|[0-"
      "9a-fA-F]{6}|[0-9a-fA-F]{8})\\s*;)|(\\s*[^#][a-zA-Z]+\\s*;)");
  QRegExp exp5(
      "(qlineargradient|qradialgradient|qconicalgradient)\\((.*)\\)\\s*;");
  exp1.setMinimal(false);
  exp2.setMinimal(false);
  exp3.setMinimal(false);
  exp4.setMinimal(false);
  exp5.setMinimal(true);

  int pos1 = 0;
  int pos2 = 0;
  int pos3 = 0;
  int pos4 = 0;
  int pos5 = 0;

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

  while ((pos3 = exp3.indexIn(text, pos3)) != -1) {
    qDebug() << pos3 << pos3 + exp3.matchedLength() << exp3.capturedTexts()
             << exp3.pos(0) << exp3.cap(0).length();
    qDebug() << "text: " << text.mid(exp3.pos(0), exp3.cap(0).length());
    pos3 += exp3.matchedLength();
    //    qDebug() << exp.capturedTexts();
    //    qDebug() << exp.cap(0).remove(QRegExp("[;: ]")).toLower();
  }

  qDebug() << "\n\nexp4\n\n";

  while ((pos4 = exp4.indexIn(text, pos4)) != -1) {
    qDebug() << exp4.capturedTexts();
    pos4 += exp4.matchedLength();
  }

  qDebug() << "\n\nexp5\n\n";

  while ((pos5 = exp5.indexIn(text, pos5)) != -1) {
    qDebug() << exp5.capturedTexts().length() << exp5.capturedTexts();
    pos5 += exp5.matchedLength();
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

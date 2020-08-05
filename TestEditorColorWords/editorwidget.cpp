#include "editorwidget.h"
#include <QCryptographicHash>
#include <QDateTime>
#include <QDebug>
#include <QtWidgets/QVBoxLayout>

EditorWidget::EditorWidget(QWidget *parent) : QWidget(parent) {
  QVBoxLayout *layout = new QVBoxLayout(this);
  setLayout(layout);
  editor = new QsciScintilla(this);
  layout->addWidget(editor);

  //语法分析器
  lexer = new QsciLexerCSS(editor);
  editor->setLexer(lexer);
  // 显示行号
  editor->setMarginLineNumbers(0, true);
  editor->setMarginWidth(0, 50);

  //循环生成测试数据
  for (int i = 0; i < 300; i++) generateText();

  QRegExp exp(
      "(:\\s*([a-zA-Z]+|#([0-9a-fA-F]{3}|[0-9a-fA-F]{6}|[0-9a-fA-F]{8}))\\s*;"
      "|["
      "rR][gG][Bb][Aa]?\\((\\s*(2[0-4][0-9]|25[0-5]|[01]?[0-9][0-9]?)\\s*,"
      "\\s*)"
      "{2}\\s*(2[0-4][0-9]|25[0-5]|[01]?[0-9][0-9]?)\\s*,?\\s*(0\\.\\d{1,2}|"
      "2["
      "0-4][0-9]|25[0-5]|[01]?[0-9][0-9])?\\)\\s*;)");
  exp.setMinimal(false);
  //绑定内容改变信号
  connect(editor, &QsciScintilla::textChanged, [=] {
    int pos = 0;
    QString text = editor->text();
    while ((pos = exp.indexIn(text, pos)) != -1) {
      //      qDebug() << pos << pos + exp.matchedLength() <<
      //      exp.capturedTexts()
      //               << exp.pos(0) << exp.cap(0).length();
      //      qDebug() << "text: " << text.mid(exp.pos(0), exp.cap(0).length());
      QString mstr = exp.cap(0);
      if (mstr.startsWith(":")) {
        int start = exp.pos(0) + mstr.indexOf(exp.cap(2));
        int len = exp.cap(2).length();
        QString value = exp.cap(2);
        //        qDebug() << exp.pos(0) << mstr.indexOf(exp.cap(2))
        //                 << exp.cap(2).length() << exp.capturedTexts();
        qDebug() << "text:" << editor->text(start, start + len) << start << len;
        QColor color(value);
        if (color.isValid()) {
          editor->SendScintilla(QsciScintilla::SCI_STARTSTYLING, start);
          QsciStyle style(-1, value, color, editor->palette().base().color(),
                          editor->font());
          qDebug() << "style:" << style.style() << color;
          editor->SendScintilla(QsciScintilla::SCI_SETSTYLING, len,
                                style.style());
        }
      }
      pos += exp.matchedLength();
    }
  });
}

void EditorWidget::generateText() {
  // 模拟生成css颜色样式
  // background:rgb(255,255,255);
  editor->append(QString("#%1 {    background:rgb(%2,%3,%4);}\n")
                     .arg("QWidget")
                     .arg(generateRgbInt())
                     .arg(generateRgbInt())
                     .arg(generateRgbInt()));
  // background: RGB(255,255 ,255);
  editor->append(QString("#%1{    background: RGB(%2,%3 ,%4);}\n")
                     .arg("QWidget")
                     .arg(generateRgbInt())
                     .arg(generateRgbInt())
                     .arg(generateRgbInt()));
  // background: rGb(   255  ,  255 , 255);
  editor->append(QString("#%1 {    background: rGb(   %2  ,  %3 , %4);}\n")
                     .arg("QWidget")
                     .arg(generateRgbInt())
                     .arg(generateRgbInt())
                     .arg(generateRgbInt()));
  // background: rgb(255,   255 , 255);
  editor->append(QString("#%1{    background: rgb(%2,   %3 , %4);}\n")
                     .arg("QWidget")
                     .arg(generateRgbInt())
                     .arg(generateRgbInt())
                     .arg(generateRgbInt()));
  // background:rGba(255,255,255, 10);
  editor->append(QString("#%1 {    background:rGba(%2,%3,%4, %5);}  \n")
                     .arg("QWidget")
                     .arg(generateRgbInt())
                     .arg(generateRgbInt())
                     .arg(generateRgbInt())
                     .arg(generateRgbInt()));
  // background:rGba(255,255,255, 0.2)   ;
  editor->append(QString("#%1{    background:rGba(%2,%3,%4, %5)   ; }\n")
                     .arg("QWidget")
                     .arg(generateRgbInt())
                     .arg(generateRgbInt())
                     .arg(generateRgbInt())
                     .arg(generateRgbFloat()));
  // background: rgba( 255, 255 , 255  , 23);
  editor->append(QString("#%1 {    background: rgba( %2, %3 , %4  , %5); }\n")
                     .arg("QWidget")
                     .arg(generateRgbInt())
                     .arg(generateRgbInt())
                     .arg(generateRgbInt())
                     .arg(generateRgbInt()));
  // background: RGBA(255 ,255 ,255, 0.2);
  editor->append(QString("#%1 {    background: RGBA(%2 ,%3 ,%4, %5);}\n")
                     .arg("QWidget")
                     .arg(generateRgbInt())
                     .arg(generateRgbInt())
                     .arg(generateRgbInt())
                     .arg(generateRgbFloat()));
  // color:   red;
  editor->append(
      QString("#%1{    color:   %2;}\n").arg("QWidget").arg(generateName()));
  // color: BLACK;
  editor->append(
      QString("#%1{    color :   %2 ;}\n").arg("QWidget").arg(generateName()));
  // color: #fff;
  editor->append(
      QString("#%1{    color: %2;  }\n").arg("QWidget").arg(generateHex()));
  // color: #fff000;
  editor->append(
      QString("#%1{    color: %2 ;}\n").arg("QWidget").arg(generateHex()));
  // color: #00fff000;;
  editor->append(
      QString("#%1{    color:   %2;}\n").arg("QWidget").arg(generateHex()));
}

QString EditorWidget::generateName() {
  // 随机颜色值
  QStringList colors = QColor::colorNames();
  QString name = colors.at(qrand() % colors.length());
  if (qrand() % 2) return name.toUpper();
  // 小写颜色值
  return name;
}

QString EditorWidget::generateHex() {
  // 返回 fff 或者 fff000 或者 ff00ff00
  QStringList colors = QColor::colorNames();
  QString name = colors.at(qrand() % colors.length());
  QColor color(name);

  if (qrand() % 3) return color.name(QColor::HexRgb).mid(0, 4);
  if (qrand() % 5) return color.name(QColor::HexArgb);
  return color.name(QColor::HexRgb);
}

int EditorWidget::generateRgbInt() {
  // 返回 0-255之间的数字
  return qrand() % 255;
}

float EditorWidget::generateRgbFloat() { return (qrand() % 10) / 10.0; }

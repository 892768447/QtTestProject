#ifndef EDITORWIDGET_H
#define EDITORWIDGET_H
#include <Qsci/qsciscintilla.h>
#include <QColor>
#include <QEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QRect>
#include <QToolTip>

struct ColorEntry {
  QRect rect;
  QColor color;
  ColorEntry(QRect rect, QColor color) {
    this->rect = rect;
    this->color = color;
  }
};

class EditorWidget : public QsciScintilla {
 public:
  EditorWidget(QWidget *parent = nullptr);
  int offsetAsPosition(int start, int offset);
  QFont fontForStyle(int style) const;
  QRect characterRect(int start, int offset, QString text);

 private:
  QRegExp imgExp = QRegExp("url\\((\\s*.*\\s*)\\)\\s*");
  QRegExp colorExp = QRegExp(
      "([rR][gG][Bb][Aa]?\\((\\s*(2[0-4][0-9]|25[0-5]|[01]?[0-9][0-9]?)\\s*,"
      "\\s*){2}\\s*(2[0-4][0-9]|25[0-5]|[01]?[0-9][0-9]?)\\s*,?\\s*(0\\.\\d{1,"
      "2}|2[0-4][0-9]|25[0-5]|[01]?[0-9][0-9])?\\)\\s*;)|(#([0-9a-fA-F]{3}|[0-"
      "9a-fA-F]{6}|[0-9a-fA-F]{8})\\s*;)|(\\s*[^#][a-zA-Z]+\\s*;)");
  QList<ColorEntry> colorEntries;
  void parseColors();  //解析文本得到坐标也颜色值
  void initEditor();

 protected:
  void paintEvent(QPaintEvent *event) override;
  void mouseMoveEvent(QMouseEvent *event) override;
};

#endif  // EDITORWIDGET_H

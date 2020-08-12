#ifndef EDITORWIDGET_H
#define EDITORWIDGET_H
#include <Qsci/qsciscintilla.h>
#include <QColor>
#include <QEvent>
#include <QHash>
#include <QMouseEvent>
#include <QPainter>
#include <QRect>
#include <QToolTip>

class EditorWidget : public QsciScintilla {
 public:
  EditorWidget(QWidget *parent = nullptr);
  void mySetViewportMargins(int left, int top, int right, int bottom);
  QHash<QString, QColor> getColorRects() const;
  int offsetAsPosition(int start, int offset);
  QFont fontForStyle(int style) const;
  QRect characterRect(int start, int offset, QString text);

 private:
  // 匹配诸如 border: 1px solid red; 这种颜色值
  QRegExp nameExp = QRegExp("\\s*[^#][a-zA-Z]+\\s*;");
  // 匹配诸如 color: #ffffff; 这种颜色值
  QRegExp wellNoExp =
      QRegExp("#([0-9a-fA-F]{3}|[0-9a-fA-F]{6}|[0-9a-fA-F]{8})\\s*;");
  // 匹配诸如 background: rgba(255,255,255,255); 这种颜色值
  QRegExp rgbExp = QRegExp(
      "[rR][gG][Bb][Aa]?\\((\\s*(2[0-4][0-9]|25[0-5]|[01]?[0-9][0-9]?)\\s*,\\s*"
      "){2}\\s*(2[0-4][0-9]|25[0-5]|[01]?[0-9][0-9]?)\\s*,?\\s*(0\\.\\d{1,2}|2["
      "0-4][0-9]|25[0-5]|[01]?[0-9][0-9])?\\)\\s*;");
  QHash<QString, QColor> colorRects;
  QColor m_color;  //鼠标悬停时获取的颜色

 protected:
  void paintEvent(QPaintEvent *event) override;
  void mouseMoveEvent(QMouseEvent *event) override;
};

#endif  // EDITORWIDGET_H

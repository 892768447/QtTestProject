#ifndef MAPWIDGET_H
#define MAPWIDGET_H

#include <QColor>
#include <QHash>
#include <QHashIterator>
#include <QPainter>
#include <QRect>
#include <QScrollBar>
#include <QWidget>
#include "editorwidget.h"

class MapWidget : public QWidget {
  Q_OBJECT
 public:
  explicit MapWidget(EditorWidget *editor = nullptr, QWidget *parent = nullptr);

  bool getRightSide() const;
  void setRightSide(bool value);

  int getWidth() const;
  void setWidth(int value);

  int getLineBorder() const;
  void setLineBorder(int value);

  int getLineHeight() const;
  void setLineHeight(int value);

  QColor getBackgroundColor() const;
  void setBackgroundColor(const QColor &value);

  QSize sizeHint() const override;
  void calculateGeometry();

 private:
  bool rightSide;
  int width;
  int lineBorder;
  int lineHeight;
  QColor backgroundColor;
  EditorWidget *m_editor;

 protected:
  void paintEvent(QPaintEvent *event) override;
};

#endif  // MAPWIDGET_H

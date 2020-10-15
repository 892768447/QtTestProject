#ifndef WIDGET_H
#define WIDGET_H

#include <QElapsedTimer>
#include <QPainter>
#include <QPen>
#include <QVariantAnimation>
#include <QWidget>

class Widget : public QWidget {
  Q_OBJECT
 public:
  explicit Widget(QWidget *parent = nullptr);

 private:
  QElapsedTimer m_timer;
  struct Animation : public QVariantAnimation {
    void updateCurrentValue(const QVariant &) {}
  } m_anim;
  QPolygonF m_polygon;
  qreal m_fps;

 protected:
  void paintEvent(QPaintEvent *event) override;
};

#endif  // WIDGET_H

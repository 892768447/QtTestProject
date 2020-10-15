#include "widget.h"

Widget::Widget(QWidget *parent) : QWidget(parent), m_fps(0.0) {
  m_anim.setDuration(2000);
  m_anim.setStartValue(0);
  m_anim.setEndValue(360);
  m_anim.setEasingCurve(QEasingCurve::InBounce);
  m_anim.setLoopCount(-1);
  m_anim.start();
  m_polygon.resize(4);
  m_polygon[0] = QPointF(-0.3, 0);
  m_polygon[1] = QPointF(-0.5, 0.3);
  m_polygon[2] = QPointF(0.5, 0);
  m_polygon[3] = QPointF(-0.5, -0.3);
  setAutoFillBackground(true);
  connect(&m_anim, SIGNAL(valueChanged(QVariant)), SLOT(update()));
}

void Widget::paintEvent(QPaintEvent *event) {
  const qreal t = 0.05;
  qreal iFps = 1E9 / m_timer.nsecsElapsed();
  m_fps = (1.0 - t) * m_fps + t * iFps;
  int len = qMin(height(), width());
  QPainter p(this);
  p.drawText(rect(),
             QString("%1,%2 FPS").arg(m_fps, 0, 'f', 0).arg(iFps, 0, 'f', 0));
  p.translate(width() / 2.0, height() / 2.0);
  p.scale(len * .8, len * .8);
  p.rotate(m_anim.currentValue().toReal());
  p.setPen(QPen(Qt::darkBlue, 0.1));
  p.drawPolygon(m_polygon);
  p.end();
  m_timer.restart();
}

#ifndef WIDGET_H
#define WIDGET_H

#include <QFont>
#include <QPaintEvent>
#include <QPainter>
#include <QWidget>

class MaskWidget : public QWidget {
  Q_OBJECT

 public:
  MaskWidget(HWND handle, QWidget *parent = nullptr);
  ~MaskWidget();

 protected:
  void paintEvent(QPaintEvent *event) override;
};
#endif  // WIDGET_H

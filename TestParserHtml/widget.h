﻿#ifndef WIDGET_H
#define WIDGET_H

#include <QDebug>
#include <QTextBlock>
#include <QTextFragment>
#include <QTimer>
#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget {
  Q_OBJECT

 public:
  Widget(QWidget *parent = nullptr);
  ~Widget();

 private:
  Ui::Widget *ui;
};
#endif  // WIDGET_H

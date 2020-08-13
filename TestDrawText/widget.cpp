#include "widget.h"
#include <windows.h>

MaskWidget::MaskWidget(HWND handle, QWidget *parent) : QWidget(parent) {
  setWindowFlags(Qt::Tool | Qt::FramelessWindowHint);
  setAttribute(Qt::WA_TransparentForMouseEvents, true);
  setAttribute(Qt::WA_InputMethodTransparent, true);
  setAttribute(Qt::WA_TranslucentBackground, true);
  showMaximized();

  if (handle) {
    SetParent((HWND)winId(), handle);
    SetWindowLongPtr((HWND)winId(), GWLP_HWNDPARENT, (LONG)handle);
  }
}

MaskWidget::~MaskWidget() {}

void MaskWidget::paintEvent(QPaintEvent *event) {
  Q_UNUSED(event)
  //  QWidget::paintEvent(event);
  QPainter painter(this);
  QFont font;
  font.setPixelSize(30);
  painter.setPen(Qt::red);
  painter.setFont(font);
  painter.drawText(100, 100, "hello qt from test");
}

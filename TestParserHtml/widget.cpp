#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) : QWidget(parent), ui(new Ui::Widget) {
  ui->setupUi(this);
  ui->textBrowser->setSource(QUrl("qrc:/qwidget.html"));

  QTextBlock block = ui->textBrowser->document()->firstBlock();
  while (block.isValid()) {
    for (QTextBlock::iterator it = block.begin(); !it.atEnd(); it++) {
      QTextFragment fragment = it.fragment();
      qDebug() << "text:" << fragment.text();
    }
    block = block.next();
  }
}

Widget::~Widget() { delete ui; }

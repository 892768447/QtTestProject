#include "widget.h"

#include <QDebug>

#include "qscilexerqss.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) : QWidget(parent), ui(new Ui::Widget) {
  ui->setupUi(this);
  ui->textEdit->setLexer(new QsciLexerCSS(ui->textEdit));
}

Widget::~Widget() { delete ui; }

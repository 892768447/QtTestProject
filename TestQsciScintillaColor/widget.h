#ifndef WIDGET_H
#define WIDGET_H

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
  int offsetAsPosition(int start, int offset);
  QFont fontForStyle(int style) const;
  QRect characterRect(int start, int offset, QString text);

 private:
  Ui::Widget *ui;
  QRegExp exp;

 protected:
  bool eventFilter(QObject *obj, QEvent *event) override;
};
#endif  // WIDGET_H

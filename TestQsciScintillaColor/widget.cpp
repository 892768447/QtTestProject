#include "widget.h"

#include <QDebug>

#include <Qsci/qscilexercss.h>
#include <QMouseEvent>
#include <QPainter>
#include <QToolTip>
#include "ui_widget.h"

Widget::Widget(QWidget *parent) : QWidget(parent), ui(new Ui::Widget) {
  ui->setupUi(this);
  ui->textEdit->setText(QStringLiteral(
      "/*通用样式*/\n* {\n    font-family: \"SF Pro SC\", \"SF Pro Display\", "
      "\"SF Pro Icons\", \"PingFang SC\", \"Helvetica Neue\", \"Helvetica\", "
      "\"Segoe UI\", \"Segoe UI Midlevel\", \"Segoe WP\", \"Arial\", "
      "\"Sans-Serif\";\n}\n\nQMessageBox, QErrorMessage, QInputDialog {\n    "
      "background-color: rgb(80, 80, 80);\n    border: 1px solid "
      "black;\n}\n\nQDialogButtonBox QPushButton[text=\"OK\"] {\n    "
      "qproperty-text: \"好的\";\n}\n\nQDialogButtonBox "
      "QPushButton[text=\"&Yes\"] {\n    qproperty-text: "
      "\"是\";\n}\n\nQDialogButtonBox QPushButton[text=\"&No\"] {\n    "
      "qproperty-text: \"否\";\n}\n\nQDialogButtonBox "
      "QPushButton[text=\"Cancel\"] {\n    qproperty-text: "
      "\"取消\";\n}\n\nQDialogButtonBox QPushButton[text=\"Save\"] {\n    "
      "qproperty-text: \"保存\";\n}\n\nQDialogButtonBox "
      "QPushButton[text=\"Discard\"] {\n    qproperty-text: "
      "\"放弃\";\n}\n\nQTextEdit {\n    color: white;\n    background-color: "
      "rgb(69, 69, 69);\n    border: 1px solid rgb(102, 102, "
      "102);\n}\n\nQInputDialog QLineEdit, QInputDialog QComboBox {\n    "
      "color: white;\n    min-height: 27px;\n    border: 1px solid rgb(102, "
      "102, 102);\n    background-color: rgb(69, 69, 69);\n}\n\nQPushButton, "
      "QLabel, QSpinBox, QCheckBox, QDoubleSpinBox {\n    color: "
      "white;\n}\n\nQPushButton {\n    min-width: 70px;\n    min-height: "
      "25px;\n    border: 1px solid rgb(125, 125, 125);\n    border-radius: "
      "5px;\n}\n\nQPushButton:hover, QPushButton:checked, QPushButton:default, "
      "QTextEdit:focus {\n    border: 1px solid rgb(20, 115, "
      "231);\n}\n\nQCheckBox::indicator {\n    width: 28px;\n    height: "
      "15px;\n}\n\nQCheckBox::indicator::unchecked {\n    image: "
      "url(Resources/Unchecked.png);\n}\n\nQCheckBox::indicator::checked {\n   "
      " image: url(Resources/Checked.png);\n}\n\nQCheckBox[text=\"&Show this "
      "message again\"] {\n    qproperty-text: "
      "\"再次显示此消息\";\n}\n\nQSpinBox, QDoubleSpinBox {\n    border: 1px "
      "solid rgb(102, 102, 102);\n    border-radius: 2px;\n    min-width: "
      "46px;\n    min-height: 20px;\n    max-width: 46px;\n    max-height: "
      "20px;\n    background-color: rgb(83, 83, 83);\n}\n\nQSpinBox:focus, "
      "QDoubleSpinBox:focus {\n    border: 1px solid rgb(20, 115, "
      "231);\n}\n\nQListView {\n    outline: 0px;\n    background-color: "
      "transparent;\n}\n\nQListView::item {\n    color: "
      "white;\n}\n\nQListView::item:focus {\n    border: 1px solid "
      "transparent;\n}\n\nQListView::item:alternate {\n    /*border: none;*/\n "
      "   border: 1px solid transparent;\n}\n\nQListView::item:selected {\n    "
      "/*border: none;*/\n    border-radius: 3px;\n    border: 1px solid "
      "white;\n}\n\n\nQListView::item:selected:active {\n    /*border: "
      "none;*/\n    border-radius: 3px;\n    border: 1px solid "
      "white;\n}\n\nQListView::item:hover {\n    /*border: none;*/\n    "
      "border-radius: 3px;\n    border: 1px solid rgb(180, 180, "
      "180);\n}\n\nQSlider {\n    max-height: "
      "15px;\n}\n\nQSlider::groove:horizontal {\n    height: 3px;\n    "
      "padding-left: -6px;\n    padding-right: -6px;\n    background-color: "
      "rgb(155, 155, 155);\n}\n\nQSlider::handle:Horizontal {\n    height: "
      "6px;\n    width: 12px;\n    /*background-color: white;*/\n    margin: "
      "-6px 0px;\n    /*border-radius: 3px;*/\n    border-image: "
      "url(Resources/Slider.png);\n}\n\n\nQScrollBar:vertical {\n    border: "
      "none;\n    background: rgb(88, 88, 88);\n    min-width: 8px;\n    "
      "max-width: 8px;\n    margin: 0px 0px 0px "
      "0px;\n}\n\nQScrollBar::add-page:vertical, QScrollBar::sub-page:vertical "
      "{\n    border: none;\n    background: "
      "none;\n}\n\nQScrollBar::handle:vertical {\n    background: rgb(155, "
      "155, 155);\n    min-width: 8px;\n    max-width: 8px;\n    border: "
      "none;\n    border-radius: 4px;\n}\n\nQScrollBar::add-line:vertical {\n  "
      "  border: none;\n    background: transparent;\n    height: 0px;\n    "
      "subcontrol-position: bottom;\n    subcontrol-origin: "
      "margin;\n}\n\nQScrollBar::sub-line:vertical {\n    border: none;\n    "
      "background: transparent;\n    height: 0px;\n    subcontrol-position: "
      "top;\n    subcontrol-origin: margin;\n}\n\nQScrollBar:horizontal {\n    "
      "border: none;\n    background: rgb(88, 88, 88);\n    min-height: 8px;\n "
      "   max-height: 8px;\n    margin: 0px 0px 0px "
      "0px;\n}\n\nQScrollBar::add-page:horizontal, "
      "QScrollBar::sub-page:horizontal {\n    border: none;\n    background: "
      "none;\n}\n\nQScrollBar::handle:horizontal {\n    background: rgb(155, "
      "155, 155);\n    min-height: 8px;\n    max-height: 8px;\n    border: "
      "none;\n    border-radius: 4px;\n}\n\nQScrollBar::add-line:horizontal "
      "{\n    border: none;\n    background: transparent;\n    width: 0px;\n   "
      " subcontrol-position: right;\n    subcontrol-origin: "
      "margin;\n}\n\nQScrollBar::sub-line:horizontal {\n    border: none;\n    "
      "background: transparent;\n    width: 0px;\n    subcontrol-position: "
      "left;\n    subcontrol-origin: margin;\n}\n\n\nQMenu {\n    padding: 6px "
      "0px;\n    border-radius: 4px;\n    background-color: rgb(93, 93, "
      "93);\n}\n\nQMenu::separator {\n    height: 1px;\n    background-color: "
      "rgb(66, 66, 66);\n}\n\nQMenu::item {\n    color: rgb(255, 255, 255);\n  "
      "  padding: 10px 20px;\n}\n\nQMenu::item:selected {\n    "
      "background-color: rgb(107, 107, 107);\n}\n\n\nQLineEdit {\n    "
      "min-height: 30px;\n    max-height: 30px;\n    color: white;\n    "
      "border: none;\n    border-bottom: 1px solid rgb(133, 133, 133);\n    "
      "background-color: transparent;\n}\n\nQLineEdit:focus {\n    "
      "border-bottom: 1px solid rgb(20, 115, 231);\n}\n\n#labelLogo {\n    "
      "min-width: 30px;\n    min-height: 30px;\n    max-width: 30px;\n    "
      "max-height: 30px;\n    /*border-image: "
      "url(Resources/Logo.png)*/\n}\n\n#buttonMinimum, #buttonClose {\n    "
      "font-family: \"Webdings\";\n    border: none;\n    color: white;\n    "
      "min-width: 30px;\n    min-height: 30px;\n    max-width: 30px;\n    "
      "max-height: 30px;\n}\n\n#buttonMinimum:hover {\n    background-color: "
      "rgba(125, 125, 125, 200);\n}\n\n#buttonClose:hover {\n    "
      "background-color: rgba(255, 84, 57, 200);\n}\n\n#buttonLogin {\n    "
      "min-height: 30px;\n    max-height: 30px;\n    color: white;\n    "
      "border-radius: 4px;\n    background-color: rgba(20, 115, 231, "
      "200);\n}\n\n#buttonLogin:hover {\n    background-color: rgba(20, 115, "
      "231, 180);\n}\n\n/*登录界面*/\n#FormLogin {\n    background-color: "
      "rgb(38, 38, 38);\n}\n\n#FormLogin #labelMessage {\n    color: rgb(255, "
      "0, 0);\n}\n\n/*进度框*/\n#widgetLoading {\n    background-color: "
      "rgb(38, 38, 38);\n}\n\n#labelLoadingMsg {\n    color: rgb(255, 255, "
      "255);\n}\n\n/*启动PS*/\n#FormChoosePs {\n    background-color: rgb(80, "
      "80, 80);\n    border: 1px solid black;\n}\n\n#FormChoosePs #labelText "
      "{\n    color: white;\n}\n\n#FormChoosePs QToolButton {\n    color: "
      "white;\n    border-radius: 10px;\n    border: 1px solid rgb(174, 174, "
      "174);\n}\n\n#FormChoosePs QToolButton:hover {\n    color: rgb(174, 174, "
      "174);\n    border: 1px solid rgb(20, 115, 231);\n}\n\n#FormChoosePs "
      "QToolButton:pressed {\n    color: "
      "white;\n}\n\n/*批量导出*/\n#FormExport {\n    background-color: rgb(80, "
      "80, 80);\n    border: 1px solid black;\n}\n\n#buttonBrowser {\n    "
      "color: white;\n    min-width: 70px;\n    min-height: 25px;\n    border: "
      "1px solid rgb(125, 125, 125);\n    border-radius: "
      "5px;\n}\n\n#buttonBrowser:hover, #buttonBrowser:checked, "
      "#buttonBrowser:default {\n    border: 1px solid rgb(20, 115, "
      "231);\n}\n\n#buttonBrowser:disabled {\n    background: rgb(125, 125, "
      "125);\n}\n\n#FormExport QDialogButtonBox QPushButton[text=\"OK\"] {\n   "
      " qproperty-text: \"输出\";\n}\n\n/*背景色*/\n#labelWelcome, "
      "#widgetWelcome, #widgetFormulas, #widgetBottom, #widgetTitle, "
      "#widgetRight, #widgetTools,\n#treeViewFormula, QTabWidget::pane, "
      "QTabBar::tab:selected, QTabBar::tab:hover, #qt_tabwidget_stackedwidget "
      "{\n    background-color: rgb(83, 83, 83);\n}\n\nQTabWidget::pane {\n    "
      "border-top: 1px solid rgb(83, 83, 83);\n}\n\nQTabBar::tab:selected {\n  "
      "  border-bottom-color: rgb(83, 83, 83);\n}\n\nQTabBar::tab {\n    "
      "color: rgb(157, 157, 157);\n    background-color: rgb(66, 66, 66);\n    "
      "border: 1px solid rgb(56, 56, 56);\n    min-width: 24px;\n    "
      "min-height: 28px;\n    padding: 0px 10px;\n}\n\n#widgetTools QLabel {\n "
      "   qproperty-alignment: \"AlignLeft | "
      "AlignBottom\";\n}\n\n\n#widgetTool > QPushButton {\n    min-width: "
      "30px;\n    min-height: 25px;\n    max-width: 30px;\n    max-height: "
      "25px;\n    border: none;\n    border-radius: 2px;\n}\n\n#widgetTool > "
      "QPushButton:hover {\n    background-color: rgb(69, 69, 69);\n    "
      "border: 1px solid rgb(99, 99, 99);\n}\n\n#widgetTool > "
      "QPushButton:checked {\n    background-color: rgb(54, 54, 54);\n    "
      "border: 1px solid rgb(99, 99, 99);\n}\n\n#widgetAutoTonePanel, "
      "#widgetXiJiePanel, #widgetJuBuPanel, #widgetPiFuPanel {\n    "
      "border-radius: 6px;\n    background-color: rgb(155, 155, "
      "155);\n}\n\n#widgetAutoTonePanel > QPushButton, #widgetXiJiePanel > "
      "QPushButton, #widgetJuBuPanel > QPushButton, #widgetPiFuPanel > "
      "QPushButton {\n    border: none;\n}\n\n#buttonDelAutoTone, "
      "#buttonDelXiJie, #buttonDelJuBu, #buttonDelPiFu {\n    min-width: "
      "35px;\n    max-width: 35px;\n}\n\n#buttonDelAutoTone, #buttonDelJuBu, "
      "#buttonDelPiFu {\n    qproperty-icon: "
      "url(Resources/Delete.png);\n}\n\n#buttonXiJieIcon, #buttonJuBuIcon, "
      "#buttonPiFuIcon {\n    min-width: 35px;\n    max-width: 35px;\n}\n\n"));
}

Widget::~Widget() { delete ui; }

/**
bool Widget::eventFilter(QObject *obj, QEvent *event) {
  Q_UNUSED(obj);
  if (event->type() == QEvent::Paint) {
    // 得到当前可视范围内的文字
    int first_visible_line =
        ui->textEdit->SendScintilla(QsciScintilla::SCI_GETFIRSTVISIBLELINE);
    int last_visible_line =
        first_visible_line +
        ui->textEdit->SendScintilla(QsciScintilla::SCI_LINESONSCREEN);
    int start_pos = ui->textEdit->SendScintilla(
        QsciScintilla::SCI_POSITIONFROMLINE, first_visible_line);
    int end_pos = ui->textEdit->SendScintilla(
        QsciScintilla::SCI_GETLINEENDPOSITION, last_visible_line);
    QString text = ui->textEdit->text(start_pos, end_pos);
    // 匹配文字区域并计算所在坐标
    int pos = 0;
    QPainter painter;
    painter.begin(static_cast<QWidget *>(obj));
    while ((pos = exp.indexIn(text, pos)) != -1) {
      if (exp.matchedLength()) {
        QString m_text = exp.cap(0).toLower();
        if (m_text.startsWith("rgb")) {
          QRect rect = characterRect(start_pos, pos, m_text);
          painter.setBrush(Qt::red);
          painter.fillRect(rect, Qt::red);
          painter.setPen(Qt::yellow);
          painter.drawRect(rect);
          painter.drawText(0, 0, "hello");
        }
      }
      pos += exp.matchedLength();
    }
    painter.end();
  }

  /*
  if (event->type() == QEvent::MouseMove) {
    QMouseEvent *mouse_event = static_cast<QMouseEvent *>(event);
    QPoint pos = mouse_event->pos();
    int line = ui->textEdit->lineAt(pos);
    QString text = ui->textEdit->text(line);
    if (line > -1 && exp.indexIn(text, 0) != -1) {
      QString color_str = exp.cap(0).toLower();
      QColor color;
      if (color_str.startsWith("rgb")) {
        color_str = color_str.remove("rgba")
                        .remove("rgb")
                        .remove("(")
                        .remove(")")
                        .remove(";")
                        .remove(" ")
                        .trimmed();
        QStringList color_list = color_str.split(",");
        //        qDebug() << color_list.length() << color_list;
        if (color_list.length() >= 3) {
          color.setRed(color_list.at(0).toInt());
          color.setGreen(color_list.at(1).toInt());
          color.setBlue(color_list.at(2).toInt());
          if (color_list.length() >= 4) {
            if (color_list.at(3).length() > 0) {
              if (color_list.at(3).toInt() > 1)
                color.setAlpha(color_list.at(3).toInt());
              else
                color.setAlphaF(color_list.at(3).toFloat());
            }
          }
        }
      } else {
        color_str = color_str.remove(":").remove(";").trimmed();
        color.setNamedColor(color_str);
      }
      //      qDebug() << pos << line << color_str << color;
      if (color.isValid())
        QToolTip::showText(
            QCursor::pos(),
            QStringLiteral("<b "
                           "style=\"font-size:18px;font-weight:"
                           "bold;background:%1;\">&nbsp;　&nbsp;　&nbsp;</b>")
                .arg(color.name(QColor::HexArgb)));
    }
  }
  /
  return false;
}
*/
#include "widget.h"
#include <QMainWindow>
#include <QScriptEngineDebugger>
#include <QTimer>
#include <QtScript>
#include "ui_widget.h"

Widget::Widget(QWidget *parent) : QWidget(parent), ui(new Ui::Widget) {
  ui->setupUi(this);

  // js引擎
  QScriptEngine *engine = new QScriptEngine(this);
  QScriptValue w_object = engine->newQObject(this);
  //添加实例对象
  w_object.setProperty("testBtn", engine->newQObject(ui->testBtn));
  w_object.setProperty("retView", engine->newQObject(ui->retView));
  //注册对象
  engine->globalObject().setProperty("w_object", w_object);
  //调试器
  QScriptEngineDebugger *debugger = new QScriptEngineDebugger(this);
  debugger->attachTo(engine);
  debugger->setAutoShowStandardWindow(true);
  debugger->standardWindow()->show();

  connect(ui->testBtn, &QPushButton::clicked, [=] {
    engine->evaluate(
        "w_object.testBtn.clicked.connect(function() {\n"
        "    print('hello');\n"
        "    w_object.retView.append('from js');\n"
        "});");
  });
}

Widget::~Widget() { delete ui; }

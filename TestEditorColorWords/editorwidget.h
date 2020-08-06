#ifndef EDITORWIDGET_H
#define EDITORWIDGET_H

#include <Qsci/qscilexercss.h>
#include <Qsci/qsciscintilla.h>
#include <Qsci/qscistyle.h>
#include <Qsci/qscistyledtext.h>
#include <QWidget>
#include "qscilexerqss.h"

class EditorWidget : public QWidget {
  Q_OBJECT
 public:
  explicit EditorWidget(QWidget *parent = nullptr);
  void generateText();
  QString generateName();
  QString generateHex();
  int generateRgbInt();
  float generateRgbFloat();

 private:
  QsciScintilla *editor;
  QsciLexerQSS *lexer;
};

#endif  // EDITORWIDGET_H

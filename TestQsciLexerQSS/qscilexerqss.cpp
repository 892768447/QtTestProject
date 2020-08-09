#include "qscilexerqss.h"

#include <QDebug>

// The ctor.
QsciLexerQSS::QsciLexerQSS(QObject *parent) : QsciLexerCSS(parent) {
  connect(static_cast<QsciScintilla *>(parent), &QsciScintilla::SCN_STYLENEEDED,
          this, &QsciLexerQSS::handleStyleNeeded);
}

// The dtor.
QsciLexerQSS::~QsciLexerQSS() {}

// Set the style for a number of characters.
void QsciLexerQSS::setStyling(int length, int style) {
  if (!editor()) return;

  editor()->SendScintilla(QsciScintillaBase::SCI_SETSTYLING, length, style);
}

// Set the style for a number of characters.
void QsciLexerQSS::setStyling(int length, const QsciStyle &style) {
  setStyling(length, style.style());
}

// Start styling.
void QsciLexerQSS::startStyling(int start, int) {
  if (!editor()) return;

  editor()->SendScintilla(QsciScintillaBase::SCI_STARTSTYLING, start);
}

void QsciLexerQSS::styleText(int start, int end) { qDebug() << start << end; }

// Set the attached editor.
// void QsciLexerQSS::setEditor(QsciScintilla *new_editor) {
//  qDebug() << "has editor:" << editor();
//  if (editor())
//    disconnect(editor(), &QsciScintilla::SCN_STYLENEEDED, this,
//               &QsciLexerQSS::handleStyleNeeded);

//  QsciLexerCSS::setEditor(new_editor);
//  qDebug() << new_editor << editor();

//  if (editor())
//    connect(editor(), &QsciScintilla::SCN_STYLENEEDED, this,
//            &QsciLexerQSS::handleStyleNeeded);
//}

// Return the number of style bits needed by the lexer.
int QsciLexerQSS::styleBitsNeeded() const { return 5; }

// Handle a request to style some text.
void QsciLexerQSS::handleStyleNeeded(int pos) {
  int start = editor()->SendScintilla(QsciScintillaBase::SCI_GETENDSTYLED);
  int line =
      editor()->SendScintilla(QsciScintillaBase::SCI_LINEFROMPOSITION, start);
  start =
      editor()->SendScintilla(QsciScintillaBase::SCI_POSITIONFROMLINE, line);

  qDebug() << start << line << pos;

  if (start != pos) styleText(start, pos);
}

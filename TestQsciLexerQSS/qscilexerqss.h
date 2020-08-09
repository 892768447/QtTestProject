#ifndef QSCILEXERQSS_H
#define QSCILEXERQSS_H

#include <Qsci/qscilexercss.h>
#include <Qsci/qsciscintilla.h>
#include <Qsci/qsciscintillabase.h>
#include <Qsci/qscistyle.h>

#include <QObject>

class QsciLexerQSS : public QsciLexerCSS {
  Q_OBJECT
 public:
  //! Construct a QsciLexerQSS with parent \a parent.  \a parent is typically
  //! the QsciScintilla instance.
  QsciLexerQSS(QObject *parent = 0);

  //! Destroys the QsciLexerQSS instance.
  virtual ~QsciLexerQSS();

  //! The next \a length characters starting from the current styling
  //! position have their style set to style number \a style.  The current
  //! styling position is moved.  The styling position is initially set by
  //! calling startStyling().
  //!
  //! \sa startStyling(), styleText()
  void setStyling(int length, int style);

  //! The next \a length characters starting from the current styling
  //! position have their style set to style \a style.  The current styling
  //! position is moved.  The styling position is initially set by calling
  //! startStyling().
  //!
  //! \sa startStyling(), styleText()
  void setStyling(int length, const QsciStyle &style);

  //! The styling position is set to \a start.  \a styleBits is unused.
  //!
  //! \sa setStyling(), styleBitsNeeded(), styleText()
  void startStyling(int pos, int styleBits = 0);

  //! This is called when the section of text beginning at position \a start
  //! and up to position \a end needs to be styled.  \a start will always be
  //! at the start of a line.  The text is styled by calling startStyling()
  //! followed by one or more calls to setStyling().  It must be
  //! re-implemented by a sub-class.
  //!
  //! \sa setStyling(), startStyling(), QsciScintilla::bytes(),
  //! QsciScintilla::text()
  virtual void styleText(int start, int end);

  //! \reimp
  //  virtual void setEditor(QsciScintilla *editor);

  //! \reimp This re-implementation returns 5 as the number of style bits
  //! needed.
  virtual int styleBitsNeeded() const;

 private slots:
  void handleStyleNeeded(int pos);

 private:
  QsciLexerQSS(const QsciLexerQSS &);
  QsciLexerQSS &operator=(const QsciLexerQSS &);
};

#endif  // QSCILEXERQSS_H

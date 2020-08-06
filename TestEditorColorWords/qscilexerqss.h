#ifndef QSCILEXERQSS_H
#define QSCILEXERQSS_H

#include <QDebug>
#include <QObject>
#include "Qsci/qscilexercustom.h"

#include <QRegExp>
#include "Qsci/qsciscintilla.h"
#include "Qsci/qsciscintillabase.h"
#include "Qsci/qscistyle.h"

class QsciLexerQSS : public QsciLexerCustom {
  Q_OBJECT
 private:
  QRegExp rgbExp;

 public:
  enum {
    //! The default.
    Default = 0,

    //! A tag.
    Tag = 1,

    //! A class selector.
    ClassSelector = 2,

    //! A pseudo class.  The list of pseudo classes is defined by keyword
    //! set 2.
    PseudoClass = 3,

    //! An unknown pseudo class.
    UnknownPseudoClass = 4,

    //! An operator.
    Operator = 5,

    //! A CSS1 property.  The list of CSS1 properties is defined by keyword
    //! set 1.
    CSS1Property = 6,

    //! An unknown property.
    UnknownProperty = 7,

    //! A value.
    Value = 8,

    //! A comment.
    Comment = 9,

    //! An ID selector.
    IDSelector = 10,

    //! An important value.
    Important = 11,

    //! An @-rule.
    AtRule = 12,

    //! A double-quoted string.
    DoubleQuotedString = 13,

    //! A single-quoted string.
    SingleQuotedString = 14,

    //! A CSS2 property.  The list of CSS2 properties is defined by keyword
    //! set 3.
    CSS2Property = 15,

    //! An attribute.
    Attribute = 16,

    //! A CSS3 property.  The list of CSS3 properties is defined by keyword
    //! set 4.
    CSS3Property = 17,

    //! A pseudo element.  The list of pseudo elements is defined by
    //! keyword set 5.
    PseudoElement = 18,

    //! An extended (browser specific) CSS property.  The list of extended
    //! CSS properties is defined by keyword set 6.
    ExtendedCSSProperty = 19,

    //! An extended (browser specific) pseudo class.  The list of extended
    //! pseudo classes is defined by keyword set 7.
    ExtendedPseudoClass = 20,

    //! An extended (browser specific) pseudo element.  The list of
    //! extended pseudo elements is defined by keyword set 8.
    ExtendedPseudoElement = 21,

    //! A media rule.
    MediaRule = 22,

    //! A variable.
    Variable = 23,
  };
  QsciLexerQSS(QObject *parent = 0);

  //! Returns the name of the language.
  const char *language() const;
  QString description(int style) const;

  virtual void styleText(int start, int end);
};

#endif  // QSCILEXERQSS_H

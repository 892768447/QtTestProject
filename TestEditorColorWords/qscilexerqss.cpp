#include "qscilexerqss.h"
#include <QtDebug>

QsciLexerQSS::QsciLexerQSS(QObject *parent) : QsciLexerCustom(parent) {
  rgbExp = QRegExp(
      "[rR][gG][Bb][Aa]?\\((\\s*(2[0-4][0-9]|25[0-5]|[01]?[0-9][0-9]?)\\s*,\\s*"
      "){2}\\s*(2[0-4][0-9]|25[0-5]|[01]?[0-9][0-9]?)\\s*,?\\s*(0\\.\\d{1,2}|2["
      "0-4][0-9]|25[0-5]|[01]?[0-9][0-9])?\\)\\s*;");
  rgbExp.setMinimal(false);
}

const char *QsciLexerQSS::language() const { return "QSS"; }

QString QsciLexerQSS::description(int style) const {
  switch (style) {
    case Default:
      return tr("Default");

    case Tag:
      return tr("Tag");

    case ClassSelector:
      return tr("Class selector");

    case PseudoClass:
      return tr("Pseudo-class");

    case UnknownPseudoClass:
      return tr("Unknown pseudo-class");

    case Operator:
      return tr("Operator");

    case CSS1Property:
      return tr("CSS1 property");

    case UnknownProperty:
      return tr("Unknown property");

    case Value:
      return tr("Value");

    case Comment:
      return tr("Comment");

    case IDSelector:
      return tr("ID selector");

    case Important:
      return tr("Important");

    case AtRule:
      return tr("@-rule");

    case DoubleQuotedString:
      return tr("Double-quoted string");

    case SingleQuotedString:
      return tr("Single-quoted string");

    case CSS2Property:
      return tr("CSS2 property");

    case Attribute:
      return tr("Attribute");

    case CSS3Property:
      return tr("CSS3 property");

    case PseudoElement:
      return tr("Pseudo-element");

    case ExtendedCSSProperty:
      return tr("Extended CSS property");

    case ExtendedPseudoClass:
      return tr("Extended pseudo-class");

    case ExtendedPseudoElement:
      return tr("Extended pseudo-element");

    case MediaRule:
      return tr("Media rule");

    case Variable:
      return tr("Variable");
  }

  return QString();
}

void QsciLexerQSS::styleText(int start, int end) {
  if (!editor()) return;
  end = qMin(end, editor()->length());
  QString text;
  if (end > start) text = editor()->text(start, end);
  if (text.length() == 0) return;

  for (QString line : text.split("\n")) {
    if (line.toLower().indexOf("rgb")) {
      int pos = 0;
      while ((pos = rgbExp.indexIn(line, pos)) != -1) {
        int r = rgbExp.cap(1).remove(",").trimmed().toUInt();
        int g = rgbExp.cap(2).remove(",").trimmed().toUInt();
        int b = rgbExp.cap(3).remove(",").trimmed().toUInt();
        QString alpha = rgbExp.cap(4).remove(",").trimmed();
        QColor color;
        if (alpha.length() > 0)
          color = QColor(r, g, b, alpha.toFloat());
        else
          color = QColor(r, g, b);
        QString value = rgbExp.cap(0).remove(";");

        startStyling(start + pos, 0xFF);

        QsciStyle style(0, value, QColor("red"), color, editor()->font(), true);
        setStyling(value.length(), style);
        //        editor()->SendScintilla(QsciScintilla::SCI_SETSTYLING,
        //        value.length(),
        //                                style.style());
        qDebug() << start << end << pos << value.length()
                 << "style:" << style.style() << color << value;
        pos += rgbExp.matchedLength();
      }
    }
  }
}

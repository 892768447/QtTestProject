#include "editorwidget.h"
#include <Qsci/qscilexercss.h>
#include <QDebug>

EditorWidget::EditorWidget(QWidget *parent) : QsciScintilla(parent) {}

void EditorWidget::mySetViewportMargins(int left, int top, int right,
                                        int bottom) {
  nameExp.setMinimal(false);
  wellNoExp.setMinimal(false);
  rgbExp.setMinimal(false);
  setUtf8(true);
  setLexer(new QsciLexerCSS(this));
  setViewportMargins(left, top, right, bottom);
  viewport()->setMouseTracking(true);
  SendScintilla(QsciScintilla::SCI_SETCODEPAGE, QsciScintilla::SC_CP_UTF8);
  SendScintilla(QsciScintilla::SCI_SETWORDCHARS, lexer()->wordCharacters());
}

QHash<QString, QColor> EditorWidget::getColorRects() const {
  return colorRects;
}

int EditorWidget::offsetAsPosition(int start, int offset) {
  return SendScintilla(QsciScintilla::SCI_POSITIONRELATIVE, start, offset);
}

QFont EditorWidget::fontForStyle(int style) const {
  char fontName[64];
  int len = SendScintilla(QsciScintilla::SCI_STYLEGETFONT, style, fontName);
  int size = SendScintilla(QsciScintilla::SCI_STYLEGETSIZE, style);
  bool italic = SendScintilla(QsciScintilla::SCI_STYLEGETITALIC, style);
  int weight = SendScintilla(QsciScintilla::SCI_STYLEGETWEIGHT, style);
  return QFont(QString::fromUtf8(fontName, len), size, weight, italic);
}

QRect EditorWidget::characterRect(int start, int offset, QString text) {
  int position = offsetAsPosition(start, offset);
  int x = SendScintilla(QsciScintillaBase::SCI_POINTXFROMPOSITION, 0, position);
  int y = SendScintilla(QsciScintillaBase::SCI_POINTYFROMPOSITION, 0, position);

  int style = SendScintilla(QsciScintillaBase::SCI_GETSTYLEAT, position);
  QFontMetrics metrics(fontForStyle(style));
  QRect rect(x, y, metrics.width(text), metrics.height());
  return rect;
}

void EditorWidget::paintEvent(QPaintEvent *event) {
  QsciScintilla::paintEvent(event);
  // 得到当前可视范围内的文字
  int first_visible_line =
      SendScintilla(QsciScintilla::SCI_GETFIRSTVISIBLELINE);
  int last_visible_line =
      first_visible_line + SendScintilla(QsciScintilla::SCI_LINESONSCREEN);
  int start_pos =
      SendScintilla(QsciScintilla::SCI_POSITIONFROMLINE, first_visible_line);
  int end_pos =
      SendScintilla(QsciScintilla::SCI_GETLINEENDPOSITION, last_visible_line);
  QString code = text(start_pos, end_pos);
  colorRects.clear();
  // 匹配文字区域并计算所在坐标
  int pos = 0;
  while ((pos = rgbExp.indexIn(code, pos)) != -1) {
    if (rgbExp.matchedLength()) {
      QString m_text = rgbExp.cap(0).toLower();
      if (m_text.startsWith("rgb")) {
        QRect rect = characterRect(start_pos, pos, m_text);
        colorRects.insert(QString("%1,%2,%3,%4")
                              .arg(rect.x())
                              .arg(rect.y())
                              .arg(rect.width())
                              .arg(rect.height()),
                          QColor(Qt::red));
      }
    }
    pos += rgbExp.matchedLength();
  }
}

/**
 * @brief EditorWidget::mouseMoveEvent
 * @param event
 * 鼠标移动时获取当前文字并提取可能的颜色表达
 */
void EditorWidget::mouseMoveEvent(QMouseEvent *event) {
  QsciScintilla::mouseMoveEvent(event);
  QPoint pos = event->pos();
  int line = lineAt(pos);
  QString code = text(line);
  if (line > -1 && rgbExp.indexIn(code, 0) != -1) {
    QString color_str = rgbExp.cap(0).toLower();
    color_str = color_str.remove("rgba")
                    .remove("rgb")
                    .remove("(")
                    .remove(")")
                    .remove(";")
                    .remove(" ")
                    .trimmed();
    QStringList color_list = color_str.split(",");
    if (color_list.length() >= 3) {
      m_color.setRed(color_list.at(0).toInt());
      m_color.setGreen(color_list.at(1).toInt());
      m_color.setBlue(color_list.at(2).toInt());
      if (color_list.length() >= 4) {
        if (color_list.at(3).length() > 0) {
          if (color_list.at(3).toInt() > 1)
            m_color.setAlpha(color_list.at(3).toInt());
          else
            m_color.setAlphaF(color_list.at(3).toFloat());
        }
      }
    } else {
      m_color.setNamedColor("");
    }
    if (m_color.isValid()) {
      QToolTip::showText(
          QCursor::pos(),
          QStringLiteral("<b "
                         "style=\"font-size:18px;font-weight:"
                         "bold;background:%1;\">&nbsp;　&nbsp;　&nbsp;</b>")
              .arg(m_color.name(QColor::HexArgb)));
    }
  }
}

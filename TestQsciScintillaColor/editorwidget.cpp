#include "editorwidget.h"
#include <Qsci/qscilexercss.h>
#include <QDebug>

EditorWidget::EditorWidget(QWidget *parent) : QsciScintilla(parent) {
  colorExp.setMinimal(false);
  imgExp.setMinimal(false);
  setUtf8(true);
  setLexer(new QsciLexerCSS(this));
  viewport()->setMouseTracking(true);
  SendScintilla(QsciScintilla::SCI_SETCODEPAGE, QsciScintilla::SC_CP_UTF8);
  SendScintilla(QsciScintilla::SCI_SETWORDCHARS, lexer()->wordCharacters());
}

int EditorWidget::offsetAsPosition(int start, int offset) {
  return SendScintilla(QsciScintilla::SCI_POSITIONRELATIVE, start, offset);
}

/**
 * @brief EditorWidget::fontForStyle
 * @param style
 * @return QFont
 * 获取样式对应的字体
 */
QFont EditorWidget::fontForStyle(int style) const {
  char fontName[64];
  int len = SendScintilla(QsciScintilla::SCI_STYLEGETFONT, style, fontName);
  int size = SendScintilla(QsciScintilla::SCI_STYLEGETSIZE, style);
  bool italic = SendScintilla(QsciScintilla::SCI_STYLEGETITALIC, style);
  int weight = SendScintilla(QsciScintilla::SCI_STYLEGETWEIGHT, style);
  return QFont(QString::fromUtf8(fontName, len), size, weight, italic);
}

/**
 * @brief EditorWidget::characterRect
 * @param start
 * @param offset
 * @param text
 * @return QRect
 * 获取某一行最后末尾对应界面中的坐标矩形框
 */
QRect EditorWidget::characterRect(int start, int offset, QString text) {
  Q_UNUSED(text)
  int line = SendScintilla(QsciScintilla::SCI_LINEFROMPOSITION, start + offset);
  int position = SendScintilla(QsciScintilla::SCI_GETLINEENDPOSITION, line);
  //  int position = offsetAsPosition(start, offset + text.length());
  int x = SendScintilla(QsciScintilla::SCI_POINTXFROMPOSITION, 0, position);
  int y = SendScintilla(QsciScintilla::SCI_POINTYFROMPOSITION, 0, position);

  int style = SendScintilla(QsciScintilla::SCI_GETSTYLEAT, position);
  QFontMetrics metrics(fontForStyle(style));
  int height = metrics.height();
  int factor = SendScintilla(QsciScintilla::SCI_GETZOOM);
  height += factor;
  y += factor;
  QRect rect(x + 2, y, height, height);
  return rect;
}

/**
 * @brief EditorWidget::parseColors
 * 解析字符串中的可能出现的颜色值
 */
void EditorWidget::parseColors() {
  // 提取当前可视范围内的文字中包含的颜色
  // 获取可见的第一行行号
  int first_visible_line =
      SendScintilla(QsciScintilla::SCI_GETFIRSTVISIBLELINE);
  // 获取可见的最后一行行号
  int last_visible_line =
      first_visible_line + SendScintilla(QsciScintilla::SCI_LINESONSCREEN);
  // 得到可见的第一行的position
  int start_pos =
      SendScintilla(QsciScintilla::SCI_POSITIONFROMLINE, first_visible_line);
  // 得到可见的最后一行的position
  int end_pos =
      SendScintilla(QsciScintilla::SCI_GETLINEENDPOSITION, last_visible_line);
  //得到当前可见的文字
  QString code = text(start_pos, end_pos);
  // 清空上一次提取的所有数据
  colorEntries.clear();
  // 通过3种正则匹配文字区域并计算所在坐标
  int pos = 0;
  QColor color;
  QRect rect;
  while ((pos = colorExp.indexIn(code, pos)) != -1) {
    if (colorExp.matchedLength()) {
      QString m_text = colorExp.cap(0).toLower();
      rect = characterRect(start_pos, pos, m_text);
      color.setAlpha(255);
      if (m_text.startsWith("rgb")) {
        //去掉字符串中的rgba ();等其它字符
        m_text.remove(QRegExp("[^\\d,.]"));
        //        qDebug() << m_text;
        QStringList color_list = m_text.split(",");
        if (color_list.length() >= 3) {
          color.setRed(color_list.at(0).toInt());    // Red
          color.setGreen(color_list.at(1).toInt());  // Green
          color.setBlue(color_list.at(2).toInt());   // Blue
          if (color_list.length() >= 4) {
            if (color_list.at(3).length() > 0) {
              // Alpha
              if (color_list.at(3).toInt() > 1)
                color.setAlpha(color_list.at(3).toInt());
              else
                color.setAlphaF(color_list.at(3).toFloat());
            }
          }
        }
      } else if (m_text.startsWith("#")) {
        // 去掉字符串其它字符
        m_text.remove(QRegExp("[^#0-9a-fA-F]"));
        //        qDebug() << m_text;
        color.setNamedColor(m_text);
      } else {
        // 去掉字符串其它字符
        m_text.remove(QRegExp("[^a-zA-Z]"));
        //        qDebug() << m_text;
        color.setNamedColor(m_text);
      }
      if (color.isValid()) colorEntries.append(ColorEntry(rect, color));
    }
    pos += colorExp.matchedLength();
  }
}

void EditorWidget::paintEvent(QPaintEvent *event) {
  QsciScintilla::paintEvent(event);
  // 绘制提取的颜色方框
  if (colorEntries.size()) {
    QPainter painter(viewport());
    for (ColorEntry entry : colorEntries) {
      painter.setPen(Qt::NoPen);
      painter.setBrush(entry.color);
      painter.drawRect(entry.rect);
    }
    viewport()->update();
  }
  parseColors();
}

/**
 * @brief EditorWidget::mouseMoveEvent
 * @param event
 * 鼠标移动时获取当前文字并提取可能的图片并预览
 */
void EditorWidget::mouseMoveEvent(QMouseEvent *event) {
  QsciScintilla::mouseMoveEvent(event);
  QPoint pos = event->pos();
  int line = lineAt(pos);
  QString code = text(line);
  if (line > -1 && imgExp.indexIn(code, 0) != -1) {
    if (imgExp.capturedTexts().size() > 1)
      QToolTip::showText(
          QCursor::pos(),
          QStringLiteral(
              "<html><head/><body><p><img src=\"%1\"/></p></body></html>")
              .arg(imgExp.cap(1)));
  }
}

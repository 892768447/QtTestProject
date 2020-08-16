#include "editorwidget.h"

#include <Qsci/qscilexercss.h>
#include <qcssparser.h>

#include <QDebug>

EditorWidget::EditorWidget(QWidget *parent) : QsciScintilla(parent) {
  colorExp.setMinimal(false);
  imgExp.setMinimal(false);
  gradientExp.setMinimal(true);
  viewport()->setMouseTracking(true);
  initEditor();
  testColor();
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
  // 如果当前行不可见则跳过
  if (!SendScintilla(QsciScintilla::SCI_GETLINEVISIBLE, line)) return QRect();
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
      if (color.isValid() && rect.isValid())
        colorEntries.append(ColorEntry(rect, QBrush(color)));
    }
    pos += colorExp.matchedLength();
  }

  //匹配渐变色
  pos = 0;
  while ((pos = gradientExp.indexIn(code, pos)) != -1) {
    QStringList texts = gradientExp.capturedTexts();
    if (texts.length() == 3) {
      rect = characterRect(start_pos, pos, texts.takeFirst());
      QCss::Value value;
      value.type = QCss::Value::Function;
      value.variant = texts;
      QCss::BrushData data = QCss::Parser::parseBrushValue(value, palette());
      if (data.type == QCss::BrushData::Brush)
        colorEntries.append(ColorEntry(rect, data.brush));
    }
    pos += gradientExp.matchedLength();
  }

  viewport()->update();
}

/**
 * @brief EditorWidget::initEditor
 * 设置编辑器的一些选项
 */
void EditorWidget::initEditor() {
  // 设置字体
  setFont(QFont("Consolas", 12));  // 字体
  setMarginsFont(font());

  // 自动换行
  setEolMode(QsciScintilla::EolMode::EolUnix);  // \n换行
  setWrapMode(QsciScintilla::WrapWord);         // 自动换行
  setWrapVisualFlags(QsciScintilla::WrapFlagNone);
  setWrapIndentMode(QsciScintilla::WrapIndentFixed);

  // 编码
  setUtf8(true);
  SendScintilla(QsciScintilla::SCI_SETCODEPAGE, QsciScintilla::SC_CP_UTF8);

  // 自动提示
  setAnnotationDisplay(QsciScintilla::AnnotationBoxed);  // 提示显示方式
  setAutoCompletionSource(
      QsciScintilla::AcsAll);  // 自动补全。对于所有Ascii字符
  setAutoCompletionReplaceWord(true);
  setAutoCompletionCaseSensitivity(false);  // 忽略大小写
  // setAutoCompletionUseSingle(QsciScintilla::AcusAlways);
  setAutoCompletionThreshold(1);  // 输入多少个字符才弹出补全提示
  setCallTipsPosition(QsciScintilla::CallTipsBelowText);  // 设置提示位置
  setCallTipsStyle(QsciScintilla::CallTipsNoContext);     // 设置提示样式

  // 设置折叠样式
  setFolding(QsciScintilla::FoldStyle::BoxedTreeFoldStyle);  // 代码折叠
  setFoldMarginColors(QColor(233, 233, 233), Qt::white);
  // 折叠标签颜色
  SendScintilla(QsciScintilla::SCI_MARKERSETBACK,
                QsciScintilla::SC_MARKNUM_FOLDERSUB, QColor("0xa0a0a0"));
  SendScintilla(QsciScintilla::SCI_MARKERSETBACK,
                QsciScintilla::SC_MARKNUM_FOLDERMIDTAIL, QColor("0xa0a0a0"));
  SendScintilla(QsciScintilla::SCI_MARKERSETBACK,
                QsciScintilla::SC_MARKNUM_FOLDERTAIL, QColor("0xa0a0a0"));

  // 设置报错信息提示
  //  setCallTipsForegroundColor(Qt::red);
  //  errorIndicatorNumber = indicatorDefine(QsciScintilla::SquiggleIndicator);
  //  setIndicatorForegroundColor(Qt::red, errorIndicatorNumber);

  // 设置当前行背景
  setCaretLineVisible(true);
  setCaretLineBackgroundColor(QColor(232, 232, 255));
  // 设置选中文本颜色

  //  setSelectionForegroundColor(QColor(192, 192, 192));
  //  setSelectionBackgroundColor(QColor(192, 192, 192,));

  // 括号匹配
  setBraceMatching(QsciScintilla::StrictBraceMatch);  // 大括号严格匹配
  setMatchedBraceBackgroundColor(Qt::blue);
  setMatchedBraceForegroundColor(Qt::white);
  setUnmatchedBraceBackgroundColor(Qt::red);
  setUnmatchedBraceForegroundColor(Qt::white);

  // 启用活动热点区域的下划线
  setHotspotUnderline(true);
  setHotspotWrap(true);

  // 缩进
  setAutoIndent(true);  //换行后自动缩进
  setTabWidth(4);
  setIndentationWidth(4);
  setTabIndents(true);
  // 缩进指南
  setIndentationGuides(true);
  setIndentationsUseTabs(true);  // 使用Tab
  setBackspaceUnindents(true);  // 当一行没有其它字符时删除前面的缩进
  setIndentationGuidesForegroundColor(QColor(192, 192, 192));
  setIndentationGuidesBackgroundColor(Qt::white);

  // 显示行号
  setMarginLineNumbers(0, true);
  setMarginWidth(0, 50);
  setMarginWidth(1, 0);  // 行号
  //  setMarginWidth(2, 0);  // 折叠
  setMarginWidth(3, 0);
  setMarginWidth(4, 0);
  //  // 折叠区域
  //  setMarginType(3, QsciScintilla::SymbolMargin);
  //  setMarginLineNumbers(3, false);
  //  setMarginWidth(3, 15);
  //  setMarginSensitivity(3, true);

  // 设置空白字符显示
  setWhitespaceSize(1);  // 可见的空白点的尺寸
  setWhitespaceVisibility(
      QsciScintilla::WsVisible);  // 空白的可见性。默认的是空格是无形的
  setWhitespaceForegroundColor(QColor(255, 181, 106));

  // 设置为CSS语法
  QsciLexerCSS *lexer = new QsciLexerCSS(this);
  // Default text settings
  lexer->setDefaultFont(font());
  lexer->setDefaultColor(QColor("#ff000000"));
  lexer->setDefaultPaper(QColor("#ffffffff"));
  // Initialize colors per style
  lexer->setColor(QColor("#000"), QsciLexerCSS::Default);
  lexer->setColor(QColor("#099"), QsciLexerCSS::Tag);         //青色
  lexer->setColor(QColor("#a11"), QsciLexerCSS::IDSelector);  //紫红969
  lexer->setColor(QColor("#a11"), QsciLexerCSS::ClassSelector);
  lexer->setColor(QColor("#a11"), QsciLexerCSS::PseudoElement);
  lexer->setColor(QColor("#a11"), QsciLexerCSS::PseudoClass);
  lexer->setColor(QColor("#aaa"), QsciLexerCSS::Attribute);
  lexer->setColor(QColor("gray"), QsciLexerCSS::Operator);
  lexer->setColor(QColor("#04e"), QsciLexerCSS::CSS1Property);  //浅蓝03c
  lexer->setColor(QColor("#04e"), QsciLexerCSS::CSS2Property);  //浅蓝03c
  lexer->setColor(QColor("#04e"), QsciLexerCSS::CSS3Property);  //浅蓝03c
  lexer->setColor(QColor("#808"), QsciLexerCSS::Value);         //深红 a11
  lexer->setColor(QColor("gray"), QsciLexerCSS::Comment);
  lexer->setColor(QColor("#690"), QsciLexerCSS::DoubleQuotedString);  //# 浅绿
  lexer->setColor(QColor("#690"), QsciLexerCSS::SingleQuotedString);
  lexer->setColor(QColor("#a60"), QsciLexerCSS::Variable);  // 05a深蓝
  setLexer(lexer);
  SendScintilla(QsciScintilla::SCI_SETWORDCHARS, lexer->wordCharacters());
}

void EditorWidget::testColor() {
  QCss::Value value;
  QCss::ColorData cdata;
  QCss::BrushData bdata;
  // test red
  value.type = QCss::Value::String;
  value.variant = QVariant("red");
  cdata = QCss::Parser::parseColorValue(value);
  qDebug() << cdata.color;

  // test #ff0000db  格式是RGBA
  value.type = QCss::Value::String;
  value.variant = "#ff0000db";
  cdata = QCss::Parser::parseColorValue(value);
  qDebug() << cdata.color;

  // test rgb(25,23,64)
  value.type = QCss::Value::Function;
  value.variant = (QStringList() << "rgb"
                                 << "25,   23,  64  ");
  cdata = QCss::Parser::parseColorValue(value);
  qDebug() << cdata.color << cdata.color.alpha();

  // test rgba(25, 23, 64, 0.2)
  value.type = QCss::Value::Function;
  value.variant = (QStringList() << "rgba"
                                 << "25,   23,  64  ,0.2");
  cdata = QCss::Parser::parseColorValue(value);
  qDebug() << cdata.color << cdata.color.alpha();

  // test rgba(25, 23, 64, 200)
  value.type = QCss::Value::Function;
  value.variant = (QStringList() << "rgba"
                                 << "25,   23,  64  ,200");
  cdata = QCss::Parser::parseColorValue(value);
  qDebug() << cdata.color << cdata.color.alpha();

  // test qlineargradient(spread:pad, x1:0.298, y1:0.289773, x2:0.745192,
  // y2:0.756, stop:0 rgba(0, 165, 255, 232), stop:1 rgba(255, 255, 255, 255))
  value.type = QCss::Value::Function;
  value.variant =
      (QStringList()
       << "qlineargradient"
       << "spread:pad, x1:0.298, y1:0.289773, x2:0.745192, y2:0.756, stop:0 "
          "rgba(0, 165, 255, 232), stop:1 rgba(255, 255, 255, 255)");
  bdata = QCss::Parser::parseBrushValue(value, palette());
  qDebug() << bdata.brush << bdata.type;

  // test qlineargradient(spread:repeat, x1:0.298, y1:0.289773, x2:0.745192,
  // y2:0.756, stop:0 rgba(0, 165, 255, 232), stop:1 rgba(255, 255, 255, 255))
  value.type = QCss::Value::Function;
  value.variant =
      (QStringList()
       << "qlineargradient"
       << "spread:repeat, x1:0.298, y1:0.289773, x2:0.745192, y2:0.756, stop:0 "
          "rgba(0, 165, 255, 232), stop:1 rgba(255, 255, 255, 255)");
  bdata = QCss::Parser::parseBrushValue(value, palette());
  qDebug() << bdata.brush << bdata.type;

  // test qlineargradient(spread:reflect, x1:0.298, y1:0.289773, x2:0.745192,
  // y2:0.756, stop:0 rgba(0, 165, 255, 232), stop:1 rgba(255, 255, 255, 255))
  value.type = QCss::Value::Function;
  value.variant =
      (QStringList()
       << "qlineargradient"
       << "spread:reflect, x1:0.298, y1:0.289773, x2:0.745192, y2:0.756, "
          "stop:0 rgba(0, 165, 255, 232), stop:1 rgba(255, 255, 255, 255)");
  bdata = QCss::Parser::parseBrushValue(value, palette());
  qDebug() << bdata.brush << bdata.type;

  // test qradialgradient(spread:pad, cx:0.5, cy:0.5, radius:0.5, fx:0.5,
  // fy:0.5, stop:0 rgba(0, 165, 255, 232), stop:1 rgba(255, 255, 255, 255))
  value.type = QCss::Value::Function;
  value.variant =
      (QStringList()
       << "qradialgradient"
       << "spread:pad, cx:0.5, cy:0.5, radius:0.5, fx:0.5, fy:0.5, stop:0 "
          "rgba(0, 165, 255, 232), stop:1 rgba(255, 255, 255, 255)");
  bdata = QCss::Parser::parseBrushValue(value, palette());
  qDebug() << bdata.brush << bdata.type;

  // test qradialgradient(spread:repeat, cx:0.5, cy:0.5, radius:0.5, fx:0.5,
  // fy:0.5, stop:0 rgba(0, 165, 255, 232), stop:1 rgba(255, 255, 255, 255))
  value.type = QCss::Value::Function;
  value.variant =
      (QStringList()
       << "qradialgradient"
       << "spread:repeat, cx:0.5, cy:0.5, radius:0.5, fx:0.5, fy:0.5, stop:0 "
          "rgba(0, 165, 255, 232), stop:1 rgba(255, 255, 255, 255)");
  bdata = QCss::Parser::parseBrushValue(value, palette());
  qDebug() << bdata.brush << bdata.type;

  // test qradialgradient(spread:reflect, cx:0.5, cy:0.5, radius:0.5, fx:0.5,
  // fy:0.5, stop:0 rgba(0, 165, 255, 232), stop:1 rgba(255, 255, 255, 255))
  value.type = QCss::Value::Function;
  value.variant =
      (QStringList()
       << "qradialgradient"
       << "spread:reflect, cx:0.5, cy:0.5, radius:0.5, fx:0.5, fy:0.5, stop:0 "
          "rgba(0, 165, 255, 232), stop:1 rgba(255, 255, 255, 255)");
  bdata = QCss::Parser::parseBrushValue(value, palette());
  qDebug() << bdata.brush << bdata.type;

  // test qconicalgradient(cx:0.5, cy:0.5, angle:0, stop:0 rgba(0, 165, 255,
  // 232), stop:1 rgba(255, 255, 255, 255))
  value.type = QCss::Value::Function;
  value.variant =
      (QStringList() << "qconicalgradient"
                     << "cx:0.5, cy:0.5, angle:0, stop:0 rgba(0, 165, 255, "
                        "232), stop:1 rgba(255, 255, 255, 255)");
  bdata = QCss::Parser::parseBrushValue(value, palette());
  qDebug() << bdata.brush << bdata.type;
}

void EditorWidget::paintEvent(QPaintEvent *event) {
  QsciScintilla::paintEvent(event);
  // 绘制提取的颜色方框
  if (colorEntries.size()) {
    const int pixSize = 5;
    QPainter painter(viewport());
    // 绘制模拟白黑透明块
    if (alphaBg.isNull()) {
      alphaBg = QPixmap(2 * pixSize, 2 * pixSize);
      QPainter pmp;
      pmp.begin(&alphaBg);
      pmp.fillRect(0, 0, pixSize, pixSize, Qt::white);
      pmp.fillRect(pixSize, pixSize, pixSize, pixSize, Qt::white);
      pmp.fillRect(0, pixSize, pixSize, pixSize, Qt::black);
      pmp.fillRect(pixSize, 0, pixSize, pixSize, Qt::black);
      pmp.end();
    }
    for (ColorEntry entry : colorEntries) {
      painter.setPen(Qt::NoPen);
      painter.fillRect(entry.rect, QBrush(alphaBg));
      painter.fillRect(entry.rect, entry.brush);
    }
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

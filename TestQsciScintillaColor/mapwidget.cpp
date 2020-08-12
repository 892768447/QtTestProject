#include "mapwidget.h"

MapWidget::MapWidget(EditorWidget *editor, QWidget *parent)
    : QWidget(parent),
      rightSide(false),
      width(14),
      lineBorder(1),
      lineHeight(parent->fontMetrics().height()),
      backgroundColor(QColor("#e7e7e7")),
      m_editor(editor) {
  setAttribute(Qt::WA_OpaquePaintEvent, true);
  editor->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
  if (rightSide)
    editor->mySetViewportMargins(0, 0, width, 0);
  else
    editor->mySetViewportMargins(width, 0, 0, 0);
}

bool MapWidget::getRightSide() const { return rightSide; }

void MapWidget::setRightSide(bool value) { rightSide = value; }

int MapWidget::getWidth() const { return width; }

void MapWidget::setWidth(int value) {
  if (width != value) {
    width = qMax(6, value);
    update();
  }
}

int MapWidget::getLineBorder() const { return lineBorder; }

void MapWidget::setLineBorder(int value) { lineBorder = value; }

int MapWidget::getLineHeight() const { return lineHeight; }

void MapWidget::setLineHeight(int value) { lineHeight = value; }

QColor MapWidget::getBackgroundColor() const { return backgroundColor; }

void MapWidget::setBackgroundColor(const QColor &value) {
  backgroundColor = value;
}

QSize MapWidget::sizeHint() const { return QSize(width, 0); }

/**
 * @brief MapWidget::calculateGeometry
 * 设置该控件位置
 */
void MapWidget::calculateGeometry() {
  if (m_editor) {
    QRect cr = m_editor->contentsRect();
    QScrollBar *vsb = m_editor->verticalScrollBar();
    int vsbw = vsb->isVisible() ? vsb->contentsRect().width() : 0;
    int left, top, right, bottom;
    m_editor->getContentsMargins(&left, &top, &right, &bottom);
    if (right > vsbw) vsbw = 0;
    if (rightSide)
      setGeometry(cr.right() - width - vsbw, cr.top(), width, cr.height());
    else
      setGeometry(0, cr.top(), width, cr.height());
    update();
  }
}

/**
 * @brief MapWidget::paintEvent
 * @param event
 * 绘制颜色矩形框
 */
void MapWidget::paintEvent(QPaintEvent *event) {
  Q_UNUSED(event)
  QPainter painter(this);
  painter.fillRect(rect(), backgroundColor);
  const QHash<QString, QColor> colorRects = m_editor->getColorRects();
  if (colorRects.size() == 0) return;
  QHashIterator<QString, QColor> item(colorRects);
  while (item.hasNext()) {
    item.next();
    painter.setPen(item.value());
    painter.setBrush(item.value());
    QStringList rect = item.key().split(",");
    painter.drawRect(rect.at(0).toInt(), rect.at(1).toInt(), rect.at(2).toInt(),
                     rect.at(3).toInt());
  }
}

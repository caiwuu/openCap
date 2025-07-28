#include "ScreenshotRenderer.h"

#include <QApplication>
#include <QFont>
#include <QScreen>

// 构造函数
ScreenshotRenderer::ScreenshotRenderer(const QPixmap& screenshot)
  : m_screenshot(screenshot),
    m_imageCacheValid(false),
    m_cachedMagnifierPos(QPoint(-1, -1)),
    m_cachedColorPos(QPoint(-1, -1))
{
}

// 析构函数
ScreenshotRenderer::~ScreenshotRenderer()
{
  // Qt会自动清理QPixmap和QImage对象
}

// 绘制背景截图
void ScreenshotRenderer::drawBackground(QPainter& painter)
{
  // 绘制原始截图作为背景，截图已经设置了正确的设备像素比
  // Qt会自动处理Retina屏幕的缩放，确保显示比例正确
  painter.drawPixmap(0, 0, m_screenshot);
}

// 绘制半透明遮罩
void ScreenshotRenderer::drawOverlay(QPainter& painter,
                                     const QRect& selectionRect,
                                     bool hasSelection)
{
  // 绘制半透明黑色遮罩，覆盖整个截图区域
  painter.fillRect(m_screenshot.rect(), QColor(0, 0, 0, 127));

  // 如果有选择区域，清除选择区域的遮罩
  if (hasSelection && selectionRect.isValid())
  {
    painter.setCompositionMode(QPainter::CompositionMode_Clear);
    painter.fillRect(selectionRect, Qt::transparent);
    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);

    // 在选择区域重新绘制原始截图，使用正确的设备像素比
    QScreen* screen = QApplication::primaryScreen();
    qreal devicePixelRatio = screen ? screen->devicePixelRatio() : 1.0;

    // 计算源区域，考虑设备像素比
    QRect sourceRect(selectionRect.x() * devicePixelRatio,
                     selectionRect.y() * devicePixelRatio,
                     selectionRect.width() * devicePixelRatio,
                     selectionRect.height() * devicePixelRatio);

    painter.drawPixmap(selectionRect, m_screenshot, sourceRect);
  }
}

// 绘制选择框
void ScreenshotRenderer::drawSelectionBox(QPainter& painter, const QRect& selectionRect)
{
  if (!selectionRect.isValid())
    return;

  // 绘制选择框边框
  QPen pen(QColor(0, 170, 255), 2); // QQ蓝色画笔，宽度为2
  painter.setPen(pen);
  painter.setBrush(Qt::NoBrush);
  painter.drawRect(selectionRect);
}

// 绘制放大镜
void ScreenshotRenderer::drawMagnifier(QPainter& painter,
                                       const QPoint& mousePos,
                                       int widgetWidth,
                                       int widgetHeight)
{
  // 获取设备像素比
  QScreen* screen = QApplication::primaryScreen();
  qreal devicePixelRatio = screen ? screen->devicePixelRatio() : 1.0;

  // 计算放大镜位置，避免超出屏幕边界
  int magnifierX = mousePos.x() + MAGNIFIER_OFFSET_X;
  int magnifierY = mousePos.y() + MAGNIFIER_OFFSET_Y;

  // 如果放大镜会超出右边界，放到鼠标左边
  if (magnifierX + MAGNIFIER_SIZE > widgetWidth)
  {
    magnifierX = mousePos.x() - MAGNIFIER_SIZE - MAGNIFIER_OFFSET_X;
  }

  // 如果放大镜会超出下边界，放到鼠标上方
  if (magnifierY + MAGNIFIER_SIZE + 60 > widgetHeight) // 60是下方文字区域的高度
  {
    magnifierY = mousePos.y() - MAGNIFIER_SIZE - 60 - MAGNIFIER_OFFSET_Y;
  }

  // 确保放大镜完全在屏幕内
  magnifierX = qMax(5, qMin(magnifierX, widgetWidth - MAGNIFIER_SIZE - 5));
  magnifierY = qMax(5, qMin(magnifierY, widgetHeight - MAGNIFIER_SIZE - 60 - 5));

  QRect magnifierRect(magnifierX, magnifierY, MAGNIFIER_SIZE, MAGNIFIER_SIZE);

  // 保存painter状态
  painter.save();

  // 绘制放大镜背景
  painter.fillRect(magnifierRect, QColor(255, 255, 255, 240));

  // 绘制放大镜边框
  QPen borderPen(QColor(0, 0, 0, 200), 2);
  painter.setPen(borderPen);
  painter.drawRect(magnifierRect);

  // 检查是否可以使用缓存的放大镜内容
  QPixmap magnifiedPixmap;
  if (isMagnifierCacheValid(mousePos))
  {
    magnifiedPixmap = m_cachedMagnifierSource;
  }
  else
  {
    // 重新生成放大镜内容
    magnifiedPixmap = getMagnifierSourcePixmap(mousePos);

    // 更新缓存
    m_cachedMagnifierSource = magnifiedPixmap;
    m_cachedMagnifierPos = mousePos;
  }

  // 绘制放大的图像
  painter.drawPixmap(magnifierRect, magnifiedPixmap);

  // 绘制中心十字线
  QPen crossPen(QColor(255, 0, 0), 1);
  painter.setPen(crossPen);
  int centerX = magnifierRect.x() + MAGNIFIER_SIZE / 2;
  int centerY = magnifierRect.y() + MAGNIFIER_SIZE / 2;

  // 水平线
  painter.drawLine(centerX - 5, centerY, centerX + 5, centerY);
  // 垂直线
  painter.drawLine(centerX, centerY - 5, centerX, centerY + 5);

  // 获取当前像素颜色（使用缓存）
  QColor pixelColor = getPixelColor(mousePos);

  // 绘制信息文字背景
  QRect textRect(magnifierX, magnifierY + MAGNIFIER_SIZE + 2, MAGNIFIER_SIZE, 55);
  painter.fillRect(textRect, QColor(0, 0, 0, 200));

  // 设置文字样式
  QFont font;
  font.setFamily("Arial");
  font.setPixelSize(11);
  painter.setFont(font);
  painter.setPen(QColor(160, 160, 160));

  // 绘制颜色信息文字
  QString coordText = QString("坐标: (%1, %2)").arg(mousePos.x()).arg(mousePos.y());
  QString hexText = QString("色值: %1").arg(colorToHex(pixelColor));
  QString copyHintText = QString("按c复制色值");

  // 绘制文字信息
  painter.drawText(magnifierX + 5, magnifierY + MAGNIFIER_SIZE + 15, coordText);
  painter.drawText(magnifierX + 5, magnifierY + MAGNIFIER_SIZE + 30, hexText);
  painter.drawText(magnifierX + 5, magnifierY + MAGNIFIER_SIZE + 45, copyHintText);

  // 恢复painter状态
  painter.restore();
}

// 绘制调整锚点
void ScreenshotRenderer::drawResizeHandles(QPainter& painter, const QRect& selectionRect)
{
  if (!selectionRect.isValid())
    return;

  painter.save();

  // 设置锚点样式
  painter.setPen(QPen(QColor(0, 170, 255), 2));
  painter.setBrush(QColor(255, 255, 255));

  // 绘制8个锚点
  QPoint handles[] = {
      QPoint(selectionRect.left(), selectionRect.top()),          // 左上
      QPoint(selectionRect.center().x(), selectionRect.top()),    // 上中
      QPoint(selectionRect.right(), selectionRect.top()),         // 右上
      QPoint(selectionRect.left(), selectionRect.center().y()),   // 左中
      QPoint(selectionRect.right(), selectionRect.center().y()),  // 右中
      QPoint(selectionRect.left(), selectionRect.bottom()),       // 左下
      QPoint(selectionRect.center().x(), selectionRect.bottom()), // 下中
      QPoint(selectionRect.right(), selectionRect.bottom())       // 右下
  };

  for (const QPoint& handle : handles)
  {
    QRect handleRect(
        handle.x() - HANDLE_SIZE / 2, handle.y() - HANDLE_SIZE / 2, HANDLE_SIZE, HANDLE_SIZE);
    painter.drawRect(handleRect);
  }

  painter.restore();
}

// 获取指定位置的像素颜色
QColor ScreenshotRenderer::getPixelColor(const QPoint& pos) const
{
  // 检查缓存是否有效
  if (pos == m_cachedColorPos && m_cachedPixelColor.isValid())
  {
    return m_cachedPixelColor;
  }

  // 获取设备像素比
  QScreen* screen = QApplication::primaryScreen();
  qreal devicePixelRatio = screen ? screen->devicePixelRatio() : 1.0;

  // 计算在原始截图中的像素位置
  int pixelX = pos.x() * devicePixelRatio;
  int pixelY = pos.y() * devicePixelRatio;

  // 确保坐标在截图范围内
  pixelX = qMax(0, qMin(pixelX, m_screenshot.width() - 1));
  pixelY = qMax(0, qMin(pixelY, m_screenshot.height() - 1));

  // 确保Image缓存有效
  ensureImageCache();

  QColor color(0, 0, 0); // 默认黑色
  if (!m_cachedScreenshotImage.isNull() && pixelX < m_cachedScreenshotImage.width() &&
      pixelY < m_cachedScreenshotImage.height())
  {
    color = QColor(m_cachedScreenshotImage.pixel(pixelX, pixelY));
  }

  // 更新缓存
  m_cachedColorPos = pos;
  m_cachedPixelColor = color;

  return color;
}

// 颜色转换为十六进制字符串
QString ScreenshotRenderer::colorToHex(const QColor& color) const
{
  return QString("#%1%2%3")
      .arg(color.red(), 2, 16, QChar('0'))
      .arg(color.green(), 2, 16, QChar('0'))
      .arg(color.blue(), 2, 16, QChar('0'))
      .toUpper();
}

// 性能优化：清除所有缓存
void ScreenshotRenderer::clearCache()
{
  m_cachedScreenshotImage = QImage();
  m_imageCacheValid = false;
  m_cachedMagnifierSource = QPixmap();
  m_cachedMagnifierPos = QPoint(-1, -1);
  m_cachedPixelColor = QColor();
  m_cachedColorPos = QPoint(-1, -1);
}

// 性能优化：检查放大镜缓存是否有效
bool ScreenshotRenderer::isMagnifierCacheValid(const QPoint& mousePos) const
{
  // 如果鼠标位置没有变化，且缓存的像素图有效，则可以重用
  return !m_cachedMagnifierSource.isNull() && m_cachedMagnifierPos == mousePos;
}

// 性能优化：确保Image缓存有效
void ScreenshotRenderer::ensureImageCache() const
{
  if (!m_imageCacheValid || m_cachedScreenshotImage.isNull())
  {
    m_cachedScreenshotImage = m_screenshot.toImage();
    m_imageCacheValid = true;
  }
}

// 性能优化：获取放大镜源像素图
QPixmap ScreenshotRenderer::getMagnifierSourcePixmap(const QPoint& mousePos) const
{
  QScreen* screen = QApplication::primaryScreen();
  qreal devicePixelRatio = screen ? screen->devicePixelRatio() : 1.0;

  // 计算要放大的源区域，以鼠标位置为中心
  int sourceSize = MAGNIFIER_SIZE / MAGNIFIER_ZOOM;

  // 计算在屏幕坐标系中的源区域中心
  int sourceCenterX = mousePos.x() * devicePixelRatio;
  int sourceCenterY = mousePos.y() * devicePixelRatio;

  // 计算源区域的左上角坐标，以中心点为基准
  int sourceX = sourceCenterX - (sourceSize * devicePixelRatio) / 2;
  int sourceY = sourceCenterY - (sourceSize * devicePixelRatio) / 2;

  // 调整源区域尺寸以匹配设备像素比
  int adjustedSourceSize = sourceSize * devicePixelRatio;

  // 确保源区域在截图范围内
  sourceX = qMax(0, qMin(sourceX, m_screenshot.width() - adjustedSourceSize));
  sourceY = qMax(0, qMin(sourceY, m_screenshot.height() - adjustedSourceSize));

  QRect sourceRect(sourceX, sourceY, adjustedSourceSize, adjustedSourceSize);

  // 拷贝并缩放图像
  QPixmap sourcePixmap = m_screenshot.copy(sourceRect);
  return sourcePixmap.scaled(
      MAGNIFIER_SIZE, MAGNIFIER_SIZE, Qt::KeepAspectRatio, Qt::SmoothTransformation);
}

// 性能优化：更新背景缓存
void ScreenshotRenderer::updateBackgroundCache()
{
  // 预先转换为Image以提高getPixelColor性能
  ensureImageCache();
}
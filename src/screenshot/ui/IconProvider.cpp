#include "IconProvider.h"

#include <QFileInfo>
#include <QPainter>
#include <QSvgRenderer>

// 创建工具图标
QIcon IconProvider::createToolIcon(const QString& iconFileName)
{
  // 构建图标文件路径
  QString iconPath = QString("icons/%1").arg(iconFileName);

  // 检查SVG文件是否存在
  if (fileExists(iconPath))
  {
    return createWhiteSvgIcon(iconPath);
  }
  return QIcon();
}

// 创建白色SVG图标
QIcon IconProvider::createWhiteSvgIcon(const QString& svgPath)
{
  // 使用QPainter着色模式（最安全，保持原始结构）
  QSvgRenderer svgRenderer(svgPath);
  if (!svgRenderer.isValid())
  {
    return QIcon();
  }

  // 渲染为不同尺寸的图标（支持高DPI）
  QIcon icon;
  for (int size : {16, 24, 32, 48})
  {
    QPixmap pixmap(size, size);
    pixmap.fill(Qt::transparent);

    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);

    // 先正常渲染SVG
    svgRenderer.render(&painter);

    // 使用CompositionMode将图标着色为白色，保持形状不变
    painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
    painter.fillRect(pixmap.rect(), Qt::white);

    icon.addPixmap(pixmap);
  }

  return icon;
}

// 检查文件是否存在
bool IconProvider::fileExists(const QString& filePath)
{
  return QFileInfo::exists(filePath);
}
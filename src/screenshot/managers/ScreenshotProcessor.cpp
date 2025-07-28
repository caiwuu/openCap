#include "ScreenshotProcessor.h"

#include <QApplication>
#include <QClipboard>
#include <QDateTime>
#include <QDebug>
#include <QDir>
#include <QScreen>
#include <QStandardPaths>

// 构造函数
ScreenshotProcessor::ScreenshotProcessor(const QPixmap& screenshot, QObject* parent)
  : QObject(parent), m_screenshot(screenshot)
{
}

// 析构函数
ScreenshotProcessor::~ScreenshotProcessor() {}

// 复制到剪切板
void ScreenshotProcessor::copyToClipboard(const QRect& selectionRect)
{
  qDebug() << "复制截图到剪切板，逻辑区域:" << selectionRect;

  if (!selectionRect.isValid() || selectionRect.isEmpty())
  {
    qWarning() << "无效的选择区域，无法复制到剪切板";
    emit processingError("选择区域无效");
    return;
  }

  if (m_screenshot.isNull())
  {
    qWarning() << "截图数据为空，无法复制到剪切板";
    emit processingError("截图数据为空");
    return;
  }

  // 裁剪截图
  QPixmap croppedPixmap = cropScreenshot(selectionRect);
  if (croppedPixmap.isNull())
  {
    qWarning() << "裁剪截图失败";
    emit processingError("裁剪截图失败");
    return;
  }

  // 获取系统剪切板
  QClipboard* clipboard = QApplication::clipboard();
  if (!clipboard)
  {
    qWarning() << "无法获取系统剪切板";
    emit processingError("无法获取系统剪切板");
    return;
  }

  // 将裁剪后的图片保存到剪切板
  clipboard->setPixmap(croppedPixmap);

  qDebug() << "截图已保存到剪切板，最终尺寸:" << croppedPixmap.size();
  emit processingFinished();
}

// 保存到文件
void ScreenshotProcessor::saveToFile(const QRect& selectionRect)
{
  qDebug() << "保存截图到文件，区域:" << selectionRect;

  if (!selectionRect.isValid() || selectionRect.isEmpty())
  {
    qWarning() << "无效的选择区域，无法保存文件";
    emit processingError("选择区域无效");
    return;
  }

  if (m_screenshot.isNull())
  {
    qWarning() << "截图数据为空，无法保存文件";
    emit processingError("截图数据为空");
    return;
  }

  // 裁剪截图
  QPixmap croppedPixmap = cropScreenshot(selectionRect);
  if (croppedPixmap.isNull())
  {
    qWarning() << "裁剪截图失败";
    emit processingError("裁剪截图失败");
    return;
  }

  // 生成文件名
  QString timestamp = QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss");
  QString fileName = QString("screenshot_%1.png").arg(timestamp);

  // 获取桌面路径
  QString desktopPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
  QString filePath = QDir(desktopPath).absoluteFilePath(fileName);

  // 保存截图
  if (croppedPixmap.save(filePath, "PNG"))
  {
    qDebug() << "截图已保存到:" << filePath;
    emit processingFinished();
  }
  else
  {
    qWarning() << "保存截图失败:" << filePath;
    emit processingError(QString("保存文件失败: %1").arg(filePath));
  }
}

// 设置截图数据
void ScreenshotProcessor::setScreenshot(const QPixmap& screenshot)
{
  m_screenshot = screenshot;
}

// 裁剪截图
QPixmap ScreenshotProcessor::cropScreenshot(const QRect& selectionRect) const
{
  // 调整区域坐标到实际像素
  QRect actualRect = adjustRectForDevicePixelRatio(selectionRect);

  qDebug() << "调整后的物理像素区域:" << actualRect;

  // 确保裁剪区域不超出截图边界
  QRect screenshotRect = m_screenshot.rect();
  actualRect = actualRect.intersected(screenshotRect);

  qDebug() << "最终裁剪区域:" << actualRect;

  // 从原始截图中裁剪选择区域
  return m_screenshot.copy(actualRect);
}

// 调整区域坐标到实际像素
QRect ScreenshotProcessor::adjustRectForDevicePixelRatio(const QRect& logicalRect) const
{
  // 获取设备像素比
  QScreen* screen = QApplication::primaryScreen();
  qreal devicePixelRatio = screen ? screen->devicePixelRatio() : 1.0;

  qDebug() << "设备像素比:" << devicePixelRatio;
  qDebug() << "原始截图尺寸:" << m_screenshot.size();

  // 如果设备像素比不是1，需要调整选择区域坐标
  if (devicePixelRatio != 1.0)
  {
    // 将逻辑坐标转换为物理像素坐标
    return QRect(static_cast<int>(logicalRect.x() * devicePixelRatio),
                 static_cast<int>(logicalRect.y() * devicePixelRatio),
                 static_cast<int>(logicalRect.width() * devicePixelRatio),
                 static_cast<int>(logicalRect.height() * devicePixelRatio));
  }

  return logicalRect;
}
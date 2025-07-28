#ifndef SCREENSHOTPROCESSOR_H
#define SCREENSHOTPROCESSOR_H

#include <QObject>
#include <QPixmap>
#include <QRect>

// 截图处理器类 - 负责截图的保存和剪切板操作
class ScreenshotProcessor : public QObject
{
  Q_OBJECT

public:
  // 构造函数和析构函数
  explicit ScreenshotProcessor(const QPixmap& screenshot, QObject* parent = nullptr);
  ~ScreenshotProcessor();

  // 截图处理方法
  void copyToClipboard(const QRect& selectionRect);
  void saveToFile(const QRect& selectionRect);

  // 设置截图数据
  void setScreenshot(const QPixmap& screenshot);

signals:
  // 处理完成信号
  void processingFinished();
  void processingError(const QString& errorMessage);

private:
  // 私有辅助方法
  QPixmap cropScreenshot(const QRect& selectionRect) const;
  QRect adjustRectForDevicePixelRatio(const QRect& logicalRect) const;

  QPixmap m_screenshot; // 原始截图数据
};

#endif // SCREENSHOTPROCESSOR_H
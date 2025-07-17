#ifndef SCREENSHOTRENDERER_H
#define SCREENSHOTRENDERER_H

#include <QPixmap>
#include <QPainter>
#include <QRect>
#include <QPoint>
#include <QColor>

// 截图渲染器类，负责处理所有绘制功能
class ScreenshotRenderer
{
public:
  // 构造函数
  explicit ScreenshotRenderer(const QPixmap &screenshot);
  ~ScreenshotRenderer(); // 析构函数

  // 绘制背景截图
  void drawBackground(QPainter &painter);

  // 绘制半透明遮罩
  void drawOverlay(QPainter &painter, const QRect &selectionRect, bool hasSelection);

  // 绘制选择框
  void drawSelectionBox(QPainter &painter, const QRect &selectionRect);

  // 绘制放大镜（优化版本）
  void drawMagnifier(QPainter &painter, const QPoint &mousePos, int widgetWidth, int widgetHeight);

  // 绘制调整锚点
  void drawResizeHandles(QPainter &painter, const QRect &selectionRect);

  // 获取指定位置的像素颜色
  QColor getPixelColor(const QPoint &pos) const;

  // 颜色转换为十六进制字符串
  QString colorToHex(const QColor &color) const;

  // 性能优化相关函数
  void clearCache();                                        // 清除所有缓存
  bool isMagnifierCacheValid(const QPoint &mousePos) const; // 检查放大镜缓存是否有效
  void updateBackgroundCache();                             // 更新背景缓存

private:
  // 常量定义
  static constexpr int MAGNIFIER_SIZE = 120;    // 放大镜尺寸
  static constexpr int MAGNIFIER_ZOOM = 3;      // 放大倍数
  static constexpr int MAGNIFIER_OFFSET_X = 20; // 放大镜X偏移
  static constexpr int MAGNIFIER_OFFSET_Y = 20; // 放大镜Y偏移
  static constexpr int HANDLE_SIZE = 8;         // 锚点尺寸

  QPixmap m_screenshot; // 截图数据

  // 性能优化缓存
  mutable QImage m_cachedScreenshotImage;  // 缓存的截图Image（避免重复转换）
  mutable bool m_imageCacheValid;          // Image缓存是否有效
  mutable QPixmap m_cachedMagnifierSource; // 缓存的放大镜源像素图
  mutable QPoint m_cachedMagnifierPos;     // 缓存的放大镜位置
  mutable QColor m_cachedPixelColor;       // 缓存的像素颜色
  mutable QPoint m_cachedColorPos;         // 缓存的颜色位置

  // 私有辅助函数
  void ensureImageCache() const;                                  // 确保Image缓存有效
  QPixmap getMagnifierSourcePixmap(const QPoint &mousePos) const; // 获取放大镜源像素图
};

#endif // SCREENSHOTRENDERER_H
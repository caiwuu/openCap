#ifndef PERFORMANCEMANAGER_H
#define PERFORMANCEMANAGER_H

#include <QElapsedTimer>
#include <QPixmap>
#include <QPoint>
#include <QWidget>

// 性能管理器类 - 负责性能优化和缓存管理
class PerformanceManager
{
public:
  // 构造函数和析构函数
  explicit PerformanceManager(QWidget* widget);
  ~PerformanceManager();

  // 更新控制方法
  bool shouldUpdate();           // 检查是否应该更新（节流控制）
  void triggerOptimizedUpdate(); // 触发优化的更新
  void updateMagnifierRegion();  // 更新放大镜区域
  void forceFullRedraw();        // 强制完整重绘

  // 缓存管理
  void invalidateMagnifierCache(); // 清除放大镜缓存
  void clearAllCaches();           // 清除所有缓存

  // 缓存状态检查
  bool isMagnifierCacheValid(const QPoint& mousePos) const;

  // 缓存访问
  QPixmap getCachedMagnifierPixmap() const;
  void setCachedMagnifierPixmap(const QPixmap& pixmap, const QPoint& pos);

  // 性能配置
  void setThrottleTime(int milliseconds); // 设置节流时间
  int getThrottleTime() const;            // 获取节流时间

private:
  QWidget* m_widget; // 关联的窗口部件

  // 性能优化相关
  QElapsedTimer m_updateTimer; // 高性能更新计时器
  qint64 m_lastUpdateTime;     // 上次更新时间
  int m_throttleTime;          // 更新节流时间
  bool m_needsFullRedraw;      // 是否需要完整重绘

  // 缓存管理
  QPixmap m_cachedMagnifierPixmap; // 缓存的放大镜像素图
  QPoint m_cachedMagnifierPos;     // 缓存的放大镜位置

  // 常量定义
  static constexpr int DEFAULT_THROTTLE_MS = 16; // 默认节流时间（约60FPS）
};

#endif // PERFORMANCEMANAGER_H
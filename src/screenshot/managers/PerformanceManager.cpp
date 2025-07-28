#include "PerformanceManager.h"

#include <QDebug>
#include <QWidget>

// 构造函数
PerformanceManager::PerformanceManager(QWidget* widget)
  : m_widget(widget),
    m_lastUpdateTime(0),
    m_throttleTime(DEFAULT_THROTTLE_MS),
    m_needsFullRedraw(true),
    m_cachedMagnifierPixmap(),
    m_cachedMagnifierPos(QPoint(-1, -1))
{
  if (!m_widget)
  {
    qWarning() << "PerformanceManager: widget不能为空";
  }

  // 启动高精度计时器
  m_updateTimer.start();
}

// 析构函数
PerformanceManager::~PerformanceManager() {}

// 检查是否应该更新（节流控制）
bool PerformanceManager::shouldUpdate()
{
  // 使用高精度计时器，性能更优且不受系统时间影响
  qint64 elapsed = m_updateTimer.elapsed();

  // 检查自上次更新以来是否已过足够时间
  if (elapsed - m_lastUpdateTime < m_throttleTime)
  {
    return false;
  }

  m_lastUpdateTime = elapsed;
  return true;
}

// 触发优化的更新
void PerformanceManager::triggerOptimizedUpdate()
{
  if (!m_widget)
    return;

  // 检查是否应该更新
  if (!shouldUpdate())
  {
    return;
  }

  // 触发重绘
  m_widget->update();
}

// 更新放大镜区域
void PerformanceManager::updateMagnifierRegion()
{
  // 使用优化的更新函数，它包含节流机制
  triggerOptimizedUpdate();
}

// 强制完整重绘
void PerformanceManager::forceFullRedraw()
{
  m_needsFullRedraw = true;

  if (m_widget)
  {
    m_widget->update();
  }
}

// 清除放大镜缓存
void PerformanceManager::invalidateMagnifierCache()
{
  m_cachedMagnifierPixmap = QPixmap();
  m_cachedMagnifierPos = QPoint(-1, -1);

  qDebug() << "放大镜缓存已清除";
}

// 清除所有缓存
void PerformanceManager::clearAllCaches()
{
  invalidateMagnifierCache();
  m_needsFullRedraw = true;

  qDebug() << "所有缓存已清除";
}

// 检查放大镜缓存是否有效
bool PerformanceManager::isMagnifierCacheValid(const QPoint& mousePos) const
{
  // 如果鼠标位置没有变化，且缓存的像素图有效，则可以重用
  return !m_cachedMagnifierPixmap.isNull() && m_cachedMagnifierPos == mousePos;
}

// 获取缓存的放大镜像素图
QPixmap PerformanceManager::getCachedMagnifierPixmap() const
{
  return m_cachedMagnifierPixmap;
}

// 设置缓存的放大镜像素图
void PerformanceManager::setCachedMagnifierPixmap(const QPixmap& pixmap, const QPoint& pos)
{
  m_cachedMagnifierPixmap = pixmap;
  m_cachedMagnifierPos = pos;
}

// 设置节流时间
void PerformanceManager::setThrottleTime(int milliseconds)
{
  if (milliseconds > 0)
  {
    m_throttleTime = milliseconds;
    qDebug() << "节流时间已设置为:" << milliseconds << "ms";
  }
  else
  {
    qWarning() << "无效的节流时间:" << milliseconds;
  }
}

// 获取节流时间
int PerformanceManager::getThrottleTime() const
{
  return m_throttleTime;
}
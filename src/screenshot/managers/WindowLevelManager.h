#ifndef WINDOWLEVELMANAGER_H
#define WINDOWLEVELMANAGER_H

#include <QTimer>
#include <QWidget>

// 窗口层级管理器类 - 负责窗口层级和焦点管理
class WindowLevelManager : public QObject
{
  Q_OBJECT

public:
  // 构造函数和析构函数
  explicit WindowLevelManager(QWidget* widget, QObject* parent = nullptr);
  ~WindowLevelManager();

  // 窗口层级管理
  void setWindowLevel();     // 设置窗口层级为最高
  void ensureWindowActive(); // 确保窗口保持活跃状态
  void setupWindow();        // 设置窗口属性和层级

  // 焦点管理
  void startFocusMonitoring();                  // 开始焦点监控
  void stopFocusMonitoring();                   // 停止焦点监控
  void setFocusCheckInterval(int milliseconds); // 设置焦点检查间隔

private slots:
  void onFocusCheckTimer(); // 焦点检查定时器槽函数

private:
  QWidget* m_widget;        // 关联的窗口部件
  QTimer* m_focusTimer;     // 焦点检查定时器
  int m_focusCheckInterval; // 焦点检查间隔

  // 私有辅助方法
  void setupWindowFlags();        // 设置窗口标志
  void setupWindowAttributes();   // 设置窗口属性
  void setGeometryToFullScreen(); // 设置窗口几何为全屏

  // 常量定义
  static constexpr int DEFAULT_FOCUS_CHECK_INTERVAL = 250; // 默认焦点检查间隔（毫秒）
};

#endif // WINDOWLEVELMANAGER_H
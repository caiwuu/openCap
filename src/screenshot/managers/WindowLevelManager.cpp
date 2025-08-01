#include "WindowLevelManager.h"

#include <QApplication>
#include <QCursor>
#include <QDebug>
#include <QScreen>

#ifdef Q_OS_MACOS
  #include <QWindow>

  #include "../platform/mac/MacWindowLevel.h"
#else
// 非macOS平台的空实现
class MacWindowLevel
{
public:
  static int getWindowLevel(WId) { return 0; }
  static void setWindowAboveStatusBar(WId) {}
  static void disableSystemGestures(WId) {}
};
#endif

// 构造函数
WindowLevelManager::WindowLevelManager(QWidget* widget, QObject* parent)
  : QObject(parent),
    m_widget(widget),
    m_focusTimer(nullptr),
    m_focusCheckInterval(DEFAULT_FOCUS_CHECK_INTERVAL)
{
  if (!m_widget)
  {
    qWarning() << "WindowLevelManager: widget不能为空";
    return;
  }

  // 创建焦点检查定时器
  m_focusTimer = new QTimer(this);
  connect(m_focusTimer, &QTimer::timeout, this, &WindowLevelManager::onFocusCheckTimer);
}

// 析构函数
WindowLevelManager::~WindowLevelManager()
{
  stopFocusMonitoring();
}

// 设置窗口层级为最高
void WindowLevelManager::setWindowLevel()
{
  if (!m_widget)
    return;

  qDebug() << "提升窗口层级";

  // 简单而稳定的窗口层级提升
  m_widget->raise();          // 提升窗口
  m_widget->activateWindow(); // 激活窗口
  m_widget->setFocus();       // 设置焦点

  // 在macOS上使用原生API设置窗口层级高于状态栏
#ifdef Q_OS_MACOS
  if (m_widget->windowHandle() && m_widget->windowHandle()->winId())
  {
    qDebug() << "使用原生API设置窗口层级高于状态栏";

    // 首先获取当前窗口层级用于调试
    int currentLevel = MacWindowLevel::getWindowLevel(m_widget->windowHandle()->winId());
    qDebug() << "当前窗口层级:" << currentLevel;

    // 设置窗口层级高于状态栏
    MacWindowLevel::setWindowAboveStatusBar(m_widget->windowHandle()->winId());

    // 禁用系统手势（防止状态栏下拉等）
    // MacWindowLevel::disableSystemGestures(m_widget->windowHandle()->winId());

    // 验证设置结果
    int newLevel = MacWindowLevel::getWindowLevel(m_widget->windowHandle()->winId());
    qDebug() << "新的窗口层级:" << newLevel;
  }
  else
  {
    qWarning() << "无法获取窗口句柄，回退到Qt方法";
  }
#else
  qDebug() << "非macOS平台，使用Qt标准方法";
#endif

  qDebug() << "窗口状态:";
  qDebug() << "  - 几何:" << m_widget->geometry();
  qDebug() << "  - 大小:" << m_widget->size();
  qDebug() << "  - 是否可见:" << m_widget->isVisible();
  qDebug() << "  - 是否激活:" << m_widget->isActiveWindow();
}

// 确保窗口保持活跃状态
void WindowLevelManager::ensureWindowActive()
{
  if (!m_widget)
    return;

  // 检查窗口是否仍然是活动窗口
  if (!m_widget->isActiveWindow() || !m_widget->hasFocus())
  {
    qDebug() << "窗口失去焦点，正在恢复...";

    // 重新激活窗口
    m_widget->raise();          // 提升窗口
    m_widget->activateWindow(); // 激活窗口
    m_widget->setFocus();       // 设置焦点

    // 确保光标属性正确
    m_widget->setCursor(Qt::ArrowCursor);

    // 在macOS上重新设置窗口层级
#ifdef Q_OS_MACOS
    if (m_widget->windowHandle() && m_widget->windowHandle()->winId())
    {
      MacWindowLevel::setWindowAboveStatusBar(m_widget->windowHandle()->winId());
      // MacWindowLevel::disableSystemGestures(m_widget->windowHandle()->winId());
    }
#endif
  }
}

// 设置窗口属性和层级
void WindowLevelManager::setupWindow()
{
  if (!m_widget)
    return;

  setupWindowFlags();
  setupWindowAttributes();
  setGeometryToFullScreen();
}

// 开始焦点监控
void WindowLevelManager::startFocusMonitoring()
{
  if (m_focusTimer && !m_focusTimer->isActive())
  {
    m_focusTimer->start(m_focusCheckInterval);
    qDebug() << "焦点监控已启动，检查间隔:" << m_focusCheckInterval << "ms";
  }
}

// 停止焦点监控
void WindowLevelManager::stopFocusMonitoring()
{
  if (m_focusTimer && m_focusTimer->isActive())
  {
    m_focusTimer->stop();
    qDebug() << "焦点监控已停止";
  }
}

// 设置焦点检查间隔
void WindowLevelManager::setFocusCheckInterval(int milliseconds)
{
  if (milliseconds > 0)
  {
    m_focusCheckInterval = milliseconds;

    // 如果定时器正在运行，重新启动以使用新的间隔
    if (m_focusTimer && m_focusTimer->isActive())
    {
      m_focusTimer->stop();
      m_focusTimer->start(m_focusCheckInterval);
    }

    qDebug() << "焦点检查间隔已设置为:" << milliseconds << "ms";
  }
  else
  {
    qWarning() << "无效的焦点检查间隔:" << milliseconds;
  }
}

// 焦点检查定时器槽函数
void WindowLevelManager::onFocusCheckTimer()
{
  ensureWindowActive();
}

// 设置窗口标志
void WindowLevelManager::setupWindowFlags()
{
  if (!m_widget)
    return;

  // 设置窗口属性 - 使用稳定的标志组合，避免Popup的异常行为
  m_widget->setWindowFlags(Qt::FramelessWindowHint |     // 无边框窗口
                           Qt::WindowStaysOnTopHint |    // 窗口置顶
                           Qt::BypassWindowManagerHint | // 绕过窗口管理器
                           Qt::Tool);                    // 工具窗口类型

  qDebug() << "窗口标志已设置";
}

// 设置窗口属性
void WindowLevelManager::setupWindowAttributes()
{
  if (!m_widget)
    return;

  m_widget->setAttribute(Qt::WA_DeleteOnClose);      // 关闭时自动删除
  m_widget->setAttribute(Qt::WA_NoSystemBackground); // 不使用系统背景
  m_widget->setAttribute(Qt::WA_OpaquePaintEvent);   // 不透明绘制事件
  m_widget->setAttribute(Qt::WA_AlwaysShowToolTips); // 始终显示工具提示
  m_widget->setAttribute(Qt::WA_MouseTracking);      // 启用鼠标追踪属性

  // 设置鼠标追踪和焦点
  m_widget->setMouseTracking(true);          // 启用鼠标追踪
  m_widget->setFocusPolicy(Qt::StrongFocus); // 设置强焦点策略

  qDebug() << "窗口属性已设置";
}

// 设置窗口几何为全屏
void WindowLevelManager::setGeometryToFullScreen()
{
  if (!m_widget)
    return;

  // 手动设置窗口几何覆盖整个屏幕，包括状态栏
  QScreen* screen = QApplication::primaryScreen();
  if (screen)
  {
    // 获取完整屏幕区域（包含状态栏），但要考虑设备像素比
    QRect fullGeometry = screen->geometry();
    qreal devicePixelRatio = screen->devicePixelRatio();

    qDebug() << "屏幕几何信息:";
    qDebug() << "  - screen->geometry():" << fullGeometry;
    qDebug() << "  - screen->availableGeometry():" << screen->availableGeometry();
    qDebug() << "  - devicePixelRatio:" << devicePixelRatio;

    // 设置窗口覆盖完整屏幕，从(0,0)开始
    m_widget->setGeometry(fullGeometry);

    qDebug() << "设置窗口几何:" << fullGeometry;
  }
  else
  {
    qWarning() << "无法获取主屏幕信息";
  }
}
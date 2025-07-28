#ifndef MACGLOBALSHORTCUT_H
#define MACGLOBALSHORTCUT_H

#include <QObject>
#include <QKeySequence>

#ifdef Q_OS_MACOS

/**
 * macOS 全局快捷键实现
 * 使用 Carbon Event Manager API 注册系统级快捷键
 */
class MacGlobalShortcut : public QObject
{
  Q_OBJECT

public:
  explicit MacGlobalShortcut(QObject *parent = nullptr);
  ~MacGlobalShortcut();

  // 注册全局快捷键
  bool registerShortcut(const QKeySequence &shortcut);

  // 注销快捷键
  void unregisterShortcut();

signals:
  // 快捷键被激活时发射的信号
  void activated();

private:
  static MacGlobalShortcut *s_instance; // 静态实例指针，用于回调
  void *m_eventHandler;                 // Carbon 事件处理器
  void *m_hotKeyRef;                    // 热键引用
  bool m_registered;                    // 是否已注册

  // Carbon 回调函数
  static int hotKeyHandler(void *nextHandler, void *theEvent, void *userData);
};

#else

// 非 macOS 平台的空实现
class MacGlobalShortcut : public QObject
{
  Q_OBJECT

public:
  explicit MacGlobalShortcut(QObject *parent = nullptr) : QObject(parent) {}
  bool registerShortcut(const QKeySequence &) { return false; }
  void unregisterShortcut() {}

signals:
  void activated();
};

#endif // Q_OS_MACOS

#endif // MACGLOBALSHORTCUT_H
#include "SystemTray.h" // 包含系统托盘头文件

#include <QApplication> // 包含Qt应用程序框架
#include <QDebug>       // 包含Qt调试输出功能
#include <QStyle>       // 包含Qt样式系统

// 系统托盘构造函数
SystemTray::SystemTray(QObject* parent)
  : QSystemTrayIcon(parent) // 调用基类构造函数
    ,
    m_trayMenu(nullptr) // 初始化托盘菜单指针为空
    ,
    m_screenshotAction(nullptr) // 初始化截图动作指针为空
    ,
    m_exitAction(nullptr) // 初始化退出动作指针为空
{
  createTrayIcon(); // 创建托盘图标
  createMenu();     // 创建右键菜单

  // 连接信号
  connect(this,
          &QSystemTrayIcon::activated, // 连接托盘图标激活信号
          this,
          &SystemTray::onTrayActivated); // 到托盘激活处理函数

  // 显示托盘图标
  show(); // 显示系统托盘图标

  qDebug() << "系统托盘已创建"; // 输出调试信息
}

// 创建托盘图标
void SystemTray::createTrayIcon()
{
  // 使用系统默认图标（实际项目中应该使用自定义图标）
  QIcon icon = QApplication::style()->standardIcon(QStyle::SP_ComputerIcon); // 获取系统计算机图标
  setIcon(icon);                                                             // 设置托盘图标
  setToolTip("截图工具");                                                    // 设置鼠标悬停提示文字
}

// 创建右键菜单
void SystemTray::createMenu()
{
  m_trayMenu = new QMenu(); // 创建菜单对象

  // 创建截图动作
  m_screenshotAction = new QAction("截图", this); // 创建截图动作，显示文字为"截图"
  connect(m_screenshotAction,
          &QAction::triggered, // 连接动作触发信号
          this,
          &SystemTray::onScreenshotAction); // 到截图动作处理函数

  // 创建退出动作
  m_exitAction = new QAction("退出", this); // 创建退出动作，显示文字为"退出"
  connect(m_exitAction,
          &QAction::triggered, // 连接动作触发信号
          this,
          &SystemTray::onExitAction); // 到退出动作处理函数

  // 添加到菜单
  m_trayMenu->addAction(m_screenshotAction); // 添加截图动作到菜单
  m_trayMenu->addSeparator();                // 添加分隔线
  m_trayMenu->addAction(m_exitAction);       // 添加退出动作到菜单

  // 设置上下文菜单
  setContextMenu(m_trayMenu); // 设置右键菜单
}

// 处理托盘图标激活事件
void SystemTray::onTrayActivated(QSystemTrayIcon::ActivationReason reason)
{
  switch (reason)
  {                                    // 根据激活原因进行不同处理
    case QSystemTrayIcon::DoubleClick: // 双击事件
      // 双击开始截图
      onScreenshotAction();        // 调用截图动作处理函数
      break;                       // 跳出switch语句
    case QSystemTrayIcon::Context: // 右键事件
      // 右键显示菜单（自动处理）
      break; // 跳出switch语句
    default: // 其他情况
      break; // 跳出switch语句
  }
}

// 处理截图动作触发
void SystemTray::onScreenshotAction()
{
  qDebug() << "托盘：请求截图"; // 输出调试信息
  emit screenshotRequested();   // 发射截图请求信号
}

// 处理退出动作触发
void SystemTray::onExitAction()
{
  qDebug() << "托盘：请求退出"; // 输出调试信息
  emit exitRequested();         // 发射退出请求信号
}
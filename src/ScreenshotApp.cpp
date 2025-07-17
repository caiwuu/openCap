#include "openCap/ScreenshotApp.h" // 包含截图应用程序头文件

#include <QApplication>   // 包含Qt应用程序框架
#include <QDateTime>      // 包含Qt日期时间功能
#include <QDebug>         // 包含Qt调试输出功能
#include <QDir>           // 包含Qt目录操作功能
#include <QPixmap>        // 包含Qt像素图类
#include <QScreen>        // 包含Qt屏幕相关功能
#include <QStandardPaths> // 包含Qt标准路径功能
#include <QTimer>         // 包含Qt定时器功能

#include "openCap/MacGlobalShortcut.h" // 包含全局快捷键头文件
#include "openCap/ScreenshotOverlay.h" // 包含截图覆盖层头文件
#include "openCap/SystemTray.h"        // 包含系统托盘头文件

// 截图应用程序构造函数
ScreenshotApp::ScreenshotApp(QObject* parent) : QObject(parent) // 调用基类构造函数
{
  // 创建系统托盘
  m_systemTray = std::make_unique<SystemTray>(this); // 使用智能指针创建系统托盘对象

  // 连接信号
  connect(m_systemTray.get(),
          &SystemTray::screenshotRequested, // 连接截图请求信号
          this,
          &ScreenshotApp::startScreenshot); // 到开始截图槽函数
  connect(m_systemTray.get(),
          &SystemTray::exitRequested, // 连接退出请求信号
          this,
          &ScreenshotApp::exitApplication); // 到退出应用程序槽函数

  // 创建全局快捷键 Cmd+Shift+A
  m_globalShortcut = new MacGlobalShortcut(this);

  // 连接快捷键到截图功能
  connect(m_globalShortcut, &MacGlobalShortcut::activated, this, &ScreenshotApp::startScreenshot);

  // 注册快捷键 Meta+Shift+A (Command+Shift+A)
  bool success = m_globalShortcut->registerShortcut(QKeySequence("Meta+Shift+A"));
  if (success)
  {
    qDebug() << "全局快捷键注册成功: Command+Shift+A";
  }
  else
  {
    qWarning() << "全局快捷键注册失败";
  }
}

// 析构函数，清理资源
ScreenshotApp::~ScreenshotApp()
{
  // 清理快捷键
  if (m_globalShortcut)
  {
    m_globalShortcut->unregisterShortcut();
    delete m_globalShortcut;
    m_globalShortcut = nullptr;
  }
}

// 开始截图操作
void ScreenshotApp::startScreenshot()
{
  qDebug() << "开始截图模式"; // 输出调试信息

  // 如果已经有覆盖窗口在运行，取消当前截图而不是启动新的
  if (m_screenshotOverlay) // 检查是否存在截图覆盖窗口
  {
    return;
  }

  // 先捕获全屏
  m_fullScreenCapture = captureFullScreen(); // 调用捕获全屏方法

  if (m_fullScreenCapture.isNull()) // 检查截图是否成功
  {
    qWarning() << "无法捕获屏幕"; // 输出警告信息
    return;                       // 直接返回
  }

  qDebug() << "屏幕捕获完成，开始创建覆盖窗口"; // 输出调试信息

  // 创建截图覆盖窗口
  m_screenshotOverlay =
      std::make_unique<ScreenshotOverlay>(m_fullScreenCapture); // 创建覆盖窗口对象

  // 连接信号
  connect(m_screenshotOverlay.get(),
          &ScreenshotOverlay::screenshotFinished, // 连接截图完成信号
          this,
          &ScreenshotApp::onScreenshotFinished); // 到截图完成处理函数
  connect(m_screenshotOverlay.get(),
          &ScreenshotOverlay::screenshotCancelled, // 连接截图取消信号
          this,
          &ScreenshotApp::onScreenshotCancelled); // 到截图取消处理函数

  qDebug() << "显示覆盖窗口（不创建新桌面）"; // 输出调试信息

  // 显示覆盖窗口 - 使用show()而不是showFullScreen()避免创建新桌面
  m_screenshotOverlay->show(); // 显示覆盖窗口

  // 立即提升窗口层级
  m_screenshotOverlay->raise();          // 提升窗口到最前
  m_screenshotOverlay->activateWindow(); // 激活窗口

  qDebug() << "窗口显示完成，延迟提升层级"; // 输出调试信息

  // 延迟提升窗口层级以确保覆盖状态栏
  QTimer::singleShot(50,
                     m_screenshotOverlay.get(),
                     [this]() // 延迟50毫秒执行
                     {
                       if (m_screenshotOverlay) // 检查窗口是否仍然存在
                       {
                         qDebug() << "执行延迟窗口层级提升";    // 输出调试信息
                         m_screenshotOverlay->setWindowLevel(); // 设置窗口层级
                       }
                     });
}

// 退出应用程序
void ScreenshotApp::exitApplication()
{
  qDebug() << "退出应用程序"; // 输出调试信息
  QApplication::quit();       // 退出Qt应用程序
}

// 处理截图完成事件
void ScreenshotApp::onScreenshotFinished(const QRect& region)
{
  qDebug() << "截图完成，区域:" << region; // 输出截图区域信息

  // 保存截图
  saveScreenshot(region); // 调用保存截图方法

  // 安全关闭覆盖窗口
  if (m_screenshotOverlay) // 检查窗口是否存在
  {
    qDebug() << "正在关闭截图窗口"; // 输出调试信息
    m_screenshotOverlay->close();   // 关闭窗口
    m_screenshotOverlay.reset();    // 重置智能指针，释放资源
  }

  // 释放全屏截图内存（截图完成后不再需要）
  m_fullScreenCapture = QPixmap();
  qDebug() << "已释放全屏截图内存"; // 输出调试信息
}

// 处理截图取消事件
void ScreenshotApp::onScreenshotCancelled()
{
  qDebug() << "截图已取消"; // 输出调试信息

  // 安全关闭覆盖窗口
  if (m_screenshotOverlay) // 检查窗口是否存在
  {
    qDebug() << "正在关闭截图窗口"; // 输出调试信息
    m_screenshotOverlay->close();   // 关闭窗口
    m_screenshotOverlay.reset();    // 重置智能指针，释放资源
  }

  // 释放全屏截图内存（截图取消后不再需要）
  m_fullScreenCapture = QPixmap();
  qDebug() << "已释放全屏截图内存"; // 输出调试信息
}

// 捕获全屏截图
QPixmap ScreenshotApp::captureFullScreen()
{
  // 获取主屏幕
  QScreen* primaryScreen = QApplication::primaryScreen(); // 获取主屏幕对象
  if (!primaryScreen)                                     // 检查屏幕是否存在
  {
    qWarning() << "无法获取主屏幕"; // 输出警告信息
    return QPixmap();               // 返回空像素图
  }

  // 捕获整个屏幕
  QPixmap screenshot = primaryScreen->grabWindow(0); // 捕获整个屏幕内容

  if (screenshot.isNull()) // 检查截图是否成功
  {
    qWarning() << "屏幕捕获失败！这可能是权限问题。"; // 输出警告信息
    qWarning() << "请在 系统偏好设置 > 安全性与隐私 > 隐私 > 屏幕录制 中添加此应用"; // 输出权限提示
  }
  else // 截图成功
  {
    qDebug() << "屏幕捕获成功，尺寸:" << screenshot.size(); // 输出截图尺寸信息
    // 设置正确的设备像素比，确保显示时不会被放大
    screenshot.setDevicePixelRatio(primaryScreen->devicePixelRatio()); // 设置设备像素比
  }

  return screenshot; // 返回截图
}

// 保存截图到文件
void ScreenshotApp::saveScreenshot(const QRect& region)
{
  if (m_fullScreenCapture.isNull() || region.isEmpty()) // 检查截图数据和区域是否有效
  {
    qWarning() << "无效的截图数据或区域"; // 输出警告信息
    return;                               // 直接返回
  }

  // 获取设备像素比，处理Retina屏幕
  QScreen* screen = QApplication::primaryScreen();                    // 获取主屏幕
  qreal devicePixelRatio = screen ? screen->devicePixelRatio() : 1.0; // 获取设备像素比

  // 调整区域坐标到实际像素
  QRect actualRegion(region.x() * devicePixelRatio,       // 调整X坐标
                     region.y() * devicePixelRatio,       // 调整Y坐标
                     region.width() * devicePixelRatio,   // 调整宽度
                     region.height() * devicePixelRatio); // 调整高度

  // 从全屏截图中裁剪选定区域
  QPixmap croppedPixmap = m_fullScreenCapture.copy(actualRegion); // 裁剪截图

  // 生成文件名
  QString timestamp = QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss"); // 生成时间戳
  QString fileName = QString("screenshot_%1.png").arg(timestamp);               // 生成文件名

  // 获取桌面路径
  QString desktopPath =
      QStandardPaths::writableLocation(QStandardPaths::DesktopLocation); // 获取桌面路径
  QString filePath = QDir(desktopPath).absoluteFilePath(fileName);       // 组合完整文件路径

  // 保存截图
  if (croppedPixmap.save(filePath, "PNG")) // 保存为PNG格式
  {
    qDebug() << "截图已保存到:" << filePath; // 输出成功信息
  }
  else // 保存失败
  {
    qWarning() << "保存截图失败:" << filePath; // 输出失败信息
  }
}
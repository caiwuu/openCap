#ifndef SCREENSHOTAPP_H // 防止头文件重复包含的宏定义开始
#define SCREENSHOTAPP_H // 定义头文件标识符

#include <QObject> // 包含Qt对象基类
#include <QPixmap> // 包含Qt像素图类
#include <memory>  // 包含C++智能指针

class SystemTray;        // 前向声明系统托盘类
class ScreenshotOverlay; // 前向声明截图覆盖层类
class MacGlobalShortcut; // 前向声明全局快捷键类

// 截图应用程序主类，继承自QObject
class ScreenshotApp : public QObject
{
  // Qt元对象系统宏，支持信号槽机制
  Q_OBJECT

public:
  // 构造函数：创建截图应用程序，接收父对象参数
  explicit ScreenshotApp(QObject* parent = nullptr);
  ~ScreenshotApp(); // 析构函数：清理资源

public slots:
  // 公共槽函数：开始截图操作
  void startScreenshot();
  // 公共槽函数：退出应用程序
  void exitApplication();

private slots:
  // 私有槽函数：处理截图完成事件，接收选中的区域
  void onScreenshotFinished(const QRect& region);
  // 私有槽函数：处理截图取消事件
  void onScreenshotCancelled();

private:
  // 私有成员函数：保存截图到文件，接收选中的区域
  void saveScreenshot(const QRect& region);
  // 私有成员函数：裁剪截图并返回裁剪后的像素图
  QPixmap cropScreenshot(const QRect& region);
  // 私有成员函数：显示文件对话框保存截图
  void saveScreenshotWithDialog(const QPixmap& croppedPixmap);
  // 私有成员函数：捕获全屏截图并返回像素图
  QPixmap captureFullScreen();
  // 私有成员函数：检查屏幕录制权限
  void checkScreenRecordingPermission();

  // 私有成员变量
  std::unique_ptr<SystemTray> m_systemTray;               // 系统托盘管理器（智能指针）
  std::unique_ptr<ScreenshotOverlay> m_screenshotOverlay; // 截图覆盖层（智能指针）
  QPixmap m_fullScreenCapture;                            // 存储全屏截图数据
  MacGlobalShortcut* m_globalShortcut;                    // 全局快捷键（Cmd+Shift+A）
};

#endif // SCREENSHOTAPP_H      // 防止头文件重复包含的宏定义结束